/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TLSClientIOHandler.h"

#include "asiopal/tls/TLSStreamChannel.h"

#include "opendnp3/LogLevels.h"

#include <utility>

using namespace asiopal;

namespace asiodnp3
{

TLSClientIOHandler::TLSClientIOHandler(const openpal::Logger& logger,
                                       const std::shared_ptr<IChannelListener>& listener,
                                       const std::shared_ptr<asiopal::Executor>& executor,
                                       asiopal::TLSConfig config,
                                       const asiopal::ChannelRetry& retry,
                                       const asiodnp3::IPEndpointsList& remotes,
                                       std::string adapter)
    : IOHandler(logger, false, listener),
      executor(executor),
      config(std::move(config)),
      retry(retry),
      remotes(remotes),
      adapter(std::move(adapter)),
      retrytimer(*executor)
{
}

void TLSClientIOHandler::ShutdownImpl()
{
    this->ResetState();
}

void TLSClientIOHandler::BeginChannelAccept()
{
    std::error_code ec;

    this->client = TLSClient::Create(logger, executor, adapter, config, ec);

    if (ec)
    {
        this->client.reset();
    }
    else
    {
        this->StartConnect(this->retry.minOpenRetry);
    }
}

void TLSClientIOHandler::SuspendChannelAccept()
{
    this->ResetState();
}

void TLSClientIOHandler::OnChannelShutdown()
{
    if (!client) return;

    this->retrytimer.Start(this->retry.reconnectDelay, [this, self = shared_from_this()]() {
        if (!client) return;
        this->BeginChannelAccept();
    });
}

void TLSClientIOHandler::StartConnect(const openpal::TimeDuration& delay)
{
    if (!this->client) false;

    auto cb = [=, self = shared_from_this()](const std::shared_ptr<Executor>& executor,
                                             const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>>& stream,
                                             const std::error_code& ec) -> void {
        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, openpal::logflags::WARN, "Error Connecting: %s", ec.message().c_str());

            ++this->statistics.numOpenFail;

            const auto newDelay = this->retry.NextDelay(delay);

            auto cb = [self, newDelay, this]() {
                this->remotes.Next();
                this->StartConnect(newDelay);
            };

            this->retrytimer.Start(delay, cb);
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, openpal::logflags::INFO, "Connected to: %s, port %u",
                             this->remotes.GetCurrentEndpoint().address.c_str(),
                             this->remotes.GetCurrentEndpoint().port);

            this->OnNewChannel(TLSStreamChannel::Create(executor, stream));
        }
    };

    FORMAT_LOG_BLOCK(this->logger, openpal::logflags::INFO, "Connecting to: %s, port %u",
                     this->remotes.GetCurrentEndpoint().address.c_str(), this->remotes.GetCurrentEndpoint().port);

    this->client->BeginConnect(this->remotes.GetCurrentEndpoint(), cb);
}

void TLSClientIOHandler::ResetState()
{
    if (this->client)
    {
        this->client->Cancel();
        this->client.reset();
    }

    this->remotes.Reset();

    retrytimer.Cancel();
}

} // namespace asiodnp3

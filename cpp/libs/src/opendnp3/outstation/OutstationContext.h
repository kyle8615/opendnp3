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
#ifndef OPENDNP3_OUTSTATIONCONTEXT_H
#define OPENDNP3_OUTSTATIONCONTEXT_H

#include <openpal/container/Pair.h>
#include <openpal/executor/TimerRef.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LayerInterfaces.h"
#include "opendnp3/gen/SecurityStatIndex.h"
#include "opendnp3/link/Addresses.h"
#include "opendnp3/outstation/ControlState.h"
#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/DeferredRequest.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/OutstationChannelStates.h"
#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/outstation/OutstationSeqNum.h"
#include "opendnp3/outstation/OutstationStates.h"
#include "opendnp3/outstation/ParsedRequest.h"
#include "opendnp3/outstation/RequestHistory.h"
#include "opendnp3/outstation/ResponseContext.h"
#include "opendnp3/outstation/TimeSyncState.h"
#include "opendnp3/outstation/event/EventBuffer.h"

namespace opendnp3
{

///
/// Represent all of the mutable state in an outstation
///
class OContext : public IUpperLayer
{
    friend class StateIdle;
    friend class StateSolicitedConfirmWait;
    friend class StateUnsolicitedConfirmWait;

public:
    OContext(const Addresses& addresses,
             const OutstationConfig& config,
             const DatabaseSizes& dbSizes,
             const openpal::Logger& logger,
             const std::shared_ptr<openpal::IExecutor>& executor,
             std::shared_ptr<ILowerLayer> lower,
             std::shared_ptr<ICommandHandler> commandHandler,
             std::shared_ptr<IOutstationApplication> application);

    /// ----- Implement IUpperLayer ------

    virtual bool OnLowerLayerUp() override;

    virtual bool OnLowerLayerDown() override;

    virtual bool OnTxReady() override final;

    virtual bool OnReceive(const Message& message) override final;

    /// --- Other public members ----

    void CheckForTaskStart();

    IUpdateHandler& GetUpdateHandler();

    DatabaseConfigView GetConfigView();

    void SetRestartIIN();

private:
    /// ---- Helper functions that operate on the current state, and may return a new state ----

    OutstationState& ContinueMultiFragResponse(const Addresses& addresses, const AppSeqNum& seq);

    OutstationState& RespondToReadRequest(const ParsedRequest& request);

    OutstationState& ProcessNewRequest(const ParsedRequest& request);

    OutstationState& OnReceiveSolRequest(const ParsedRequest& request);

    void RespondToNonReadRequest(const ParsedRequest& request);

    // ---- Processing functions --------

    bool ProcessMessage(const Message& message);

    bool ProcessObjects(const ParsedRequest& request);

    bool ProcessRequest(const ParsedRequest& request);

    bool ProcessRequestNoAck(const ParsedRequest& request);

    bool ProcessConfirm(const ParsedRequest& request);

    // ---- common helper methods ----

    void BeginResponseTx(uint16_t destination, const openpal::RSlice& data, const AppControlField& control);

    void BeginUnsolTx(const AppControlField& control, const openpal::RSlice& response);

    void BeginTx(uint16_t destination, const openpal::RSlice& message);

    void CheckForDeferredRequest();

    bool ProcessDeferredRequest(const ParsedRequest& request);

    void RestartConfirmTimer();

    void CheckForUnsolicited();

    bool CanTransmit() const;

    IINField GetResponseIIN();

    IINField GetDynamicIIN();

    /// --- methods for handling app-layer functions ---

    /// Handles non-read function codes that require a response. builds the response using the supplied writer.
    /// @return An IIN field indicating the validity of the request, and to be returned in the response.
    IINField HandleNonReadResponse(const APDUHeader& header, const openpal::RSlice& objects, HeaderWriter& writer);

    /// Handles read function codes. May trigger an unsolicited response
    /// @return an IIN field and a partial AppControlField (missing sequence info)
    openpal::Pair<IINField, AppControlField> HandleRead(const openpal::RSlice& objects, HeaderWriter& writer);

    // ------ Function Handlers ------

    IINField HandleWrite(const openpal::RSlice& objects);
    IINField HandleSelect(const openpal::RSlice& objects, HeaderWriter& writer);
    IINField HandleOperate(const openpal::RSlice& objects, HeaderWriter& writer);
    IINField HandleDirectOperate(const openpal::RSlice& objects, OperateType opType, HeaderWriter* pWriter);
    IINField HandleDelayMeasure(const openpal::RSlice& objects, HeaderWriter& writer);
    IINField HandleRecordCurrentTime();
    IINField HandleRestart(const openpal::RSlice& objects, bool isWarmRestart, HeaderWriter* pWriter);
    IINField HandleAssignClass(const openpal::RSlice& objects);
    IINField HandleDisableUnsolicited(const openpal::RSlice& objects, HeaderWriter& writer);
    IINField HandleEnableUnsolicited(const openpal::RSlice& objects, HeaderWriter& writer);
    IINField HandleCommandWithConstant(const openpal::RSlice& objects, HeaderWriter& writer, CommandStatus status);

    // ------ resources --------
    const Addresses addresses;
    openpal::Logger logger;
    const std::shared_ptr<openpal::IExecutor> executor;
    const std::shared_ptr<ILowerLayer> lower;
    const std::shared_ptr<ICommandHandler> commandHandler;
    const std::shared_ptr<IOutstationApplication> application;

    // ------ Database, event buffer, and response tracking
    EventBuffer eventBuffer;
    Database database;
    ResponseContext rspContext;

    // ------ Static configuration -------
    OutstationParams params;

    // ------ Shared dynamic state --------
    bool isOnline;
    bool isTransmitting;
    IINField staticIIN;
    openpal::TimerRef confirmTimer;
    RequestHistory history;
    DeferredRequest deferred;

    // ------ Dynamic state related to controls ------
    ControlState control;

    // ------ Dynamic state related to time synchronization ------
    TimeSyncState time;

    // ------ Dynamic state related to solicited and unsolicited modes ------
    OutstationSolState sol;
    OutstationUnsolState unsol;
    OutstationState* state = &StateIdle::Inst();
};

} // namespace opendnp3

#endif

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
#ifndef ASIOPAL_SERIALTYPES_H
#define ASIOPAL_SERIALTYPES_H

#include "openpal/executor/TimeDuration.h"

#include "opendnp3/gen/FlowControl.h"
#include "opendnp3/gen/Parity.h"
#include "opendnp3/gen/StopBits.h"

#include <string>

namespace asiopal
{

/// Settings structure for the serial port
struct SerialSettings
{

    /// Defaults to the familiar 9600 8/N/1, no flow control
    SerialSettings()
        : baud(9600),
          dataBits(8),
          stopBits(opendnp3::StopBits::One),
          parity(opendnp3::Parity::None),
          flowType(opendnp3::FlowControl::None),
          asyncOpenDelay(openpal::TimeDuration::Milliseconds(500))
    {
    }

    /// name of the port, i.e. "COM1" or "/dev/tty0"
    std::string deviceName;

    /// Baud rate of the port, i.e. 9600 or 57600
    int baud;

    /// Data bits, usually 8
    int dataBits;

    /// Stop bits, usually set to 1
    opendnp3::StopBits stopBits;

    /// Parity setting for the port, usually PAR_NONE
    opendnp3::Parity parity;

    /// Flow control setting, usually FLOW_NONE
    opendnp3::FlowControl flowType;

    /// Some physical layers need time to "settle" so that the first tx isn't lost
    openpal::TimeDuration asyncOpenDelay;
};

} // namespace asiopal

#endif

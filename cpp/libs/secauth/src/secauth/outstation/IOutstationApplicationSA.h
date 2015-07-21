/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef SECAUTH_IOUTSTATIONAPPLICATIONSA_H
#define SECAUTH_IOUTSTATIONAPPLICATIONSA_H

#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/app/User.h>

#include "secauth/UpdateKey.h"
#include "secauth/outstation/Permissions.h"

namespace secauth
{	

class IOutstationUserSink
{
public:

	virtual void Load(opendnp3::User, const UpdateKey&, Permissions) = 0;
};

/** 
	Extends the outstation application interface for vanilla DNP3 with additional methods required for secure authentication
*/
class IOutstationApplicationSA : public opendnp3::IOutstationApplication
{
	public:	
		

		/**
		*	Called once during initialization	
		*
		*	The sink is invoked for every user that the outstation has persisted in non-volatile memory
		*/
		virtual void LoadUsers(IOutstationUserSink& sink) = 0;
				
};

}

#endif

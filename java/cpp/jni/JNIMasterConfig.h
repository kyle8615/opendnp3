//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3JAVA_JNIMASTERCONFIG_H
#define OPENDNP3JAVA_JNIMASTERCONFIG_H

#include <jni.h>

#include "../adapters/LocalRef.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class MasterConfig
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // field getter methods
            LocalRef<jobject> getcontrolQualifierMode(JNIEnv* env, jobject instance);
            jboolean getdisableUnsolOnStartup(JNIEnv* env, jobject instance);
            LocalRef<jobject> geteventScanOnEventsAvailableClassMask(JNIEnv* env, jobject instance);
            jboolean getignoreRestartIIN(JNIEnv* env, jobject instance);
            jboolean getintegrityOnEventOverflowIIN(JNIEnv* env, jobject instance);
            jint getmaxRxFragSize(JNIEnv* env, jobject instance);
            jint getmaxTxFragSize(JNIEnv* env, jobject instance);
            LocalRef<jobject> getresponseTimeout(JNIEnv* env, jobject instance);
            LocalRef<jobject> getstartupIntegrityClassMask(JNIEnv* env, jobject instance);
            LocalRef<jobject> gettaskRetryPeriod(JNIEnv* env, jobject instance);
            LocalRef<jobject> gettaskStartTimeout(JNIEnv* env, jobject instance);
            LocalRef<jobject> gettimeSyncMode(JNIEnv* env, jobject instance);
            LocalRef<jobject> getunsolClassMask(JNIEnv* env, jobject instance);

            private:

            jclass clazz = nullptr;

            // field ids
            jfieldID responseTimeoutField = nullptr;
            jfieldID timeSyncModeField = nullptr;
            jfieldID disableUnsolOnStartupField = nullptr;
            jfieldID ignoreRestartIINField = nullptr;
            jfieldID unsolClassMaskField = nullptr;
            jfieldID startupIntegrityClassMaskField = nullptr;
            jfieldID integrityOnEventOverflowIINField = nullptr;
            jfieldID eventScanOnEventsAvailableClassMaskField = nullptr;
            jfieldID taskRetryPeriodField = nullptr;
            jfieldID taskStartTimeoutField = nullptr;
            jfieldID maxTxFragSizeField = nullptr;
            jfieldID maxRxFragSizeField = nullptr;
            jfieldID controlQualifierModeField = nullptr;
        };
    }
}

#endif

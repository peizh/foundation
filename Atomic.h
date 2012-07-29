/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_UTILS_ATOMIC_H
#define ANDROID_UTILS_ATOMIC_H

#include "AndroidConfig.h"
//#include <cutils/atomic.h>
#ifdef HAVE_IOS_OS
#define android_atomic_inc(a) 1
#define android_atomic_dec(a) 1
#define android_atomic_add(a, b) 1
#define android_atomic_or(a, b) 1
#define android_atomic_cmpxchg(a, b, c) 1
#else
#endif

#endif // ANDROID_UTILS_ATOMIC_H

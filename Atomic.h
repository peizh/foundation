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
#ifdef HAVE_IOS_OS
#include <libkern/OSAtomic.h>
#define android_atomic_inc(a) OSAtomicIncrement32((int32_t*)a)
#define android_atomic_dec(a) OSAtomicDecrement32((int32_t*)a)
#define android_atomic_add(a, b) OSAtomicAdd32((a), (int32_t*)(b))
#define android_atomic_and(a, b) OSAtomicAnd32((a), (int32_t*)(b))
#define android_atomic_or(a, b) OSAtomicOr32((a), (uint32_t*)(b))
#define android_atomic_cmpxchg(a, b, c) !OSAtomicCompareAndSwap32((a), (b), (c))
#else
//#include <cutils/atomic.h>
#include <sys/atomics.h>
#define android_atomic_inc(a) __atomic_inc(a)
#define android_atomic_dec(a) __atomic_dec(a)
#define android_atomic_add(a, b) __atomic_add((a), (b))
#define android_atomic_or(a, b) __atomic_or((a), (b))
#define android_atomic_cmpxchg(a, b, c) __atomic_cmpxchg((a), (b), (c))
#endif

#endif // ANDROID_UTILS_ATOMIC_H

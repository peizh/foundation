/*
 * Copyright (C) 2007 The Android Open Source Project
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
#include "AndroidConfig.h"
#include <time.h>
#include <stdio.h>
#ifdef HAVE_PTHREADS
#include <pthread.h>
#endif
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//#include <cutils/logger.h>
//#include <cutils/logd.h>
//#include <cutils/log.h>
#include "Log.h"

#define LOG_BUF_SIZE	1024

#if FAKE_LOG_DEVICE
// This will be defined when building for the host.
#define log_open(pathname, flags) fakeLogOpen(pathname, flags)
#define log_writev(filedes, vector, count) fakeLogWritev(filedes, vector, count)
#define log_close(filedes) fakeLogClose(filedes)
#else
#define log_open(pathname, flags) open(pathname, flags)
#define log_writev(filedes, vector, count) writev(filedes, vector, count)
#define log_close(filedes) close(filedes)
#endif

static int __write_to_log_stderr(int fd, struct iovec *vec, size_t nr);
static int (*write_to_log)(int fd, struct iovec *vec, size_t nr) = __write_to_log_stderr;
#ifdef HAVE_PTHREADS
static pthread_mutex_t log_init_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

static char filterPriToChar (android_LogPriority pri)
{
    switch (pri) {
        case ANDROID_LOG_VERBOSE:       return 'V';
        case ANDROID_LOG_DEBUG:         return 'D';
        case ANDROID_LOG_INFO:          return 'I';
        case ANDROID_LOG_WARN:          return 'W';
        case ANDROID_LOG_ERROR:         return 'E';
        case ANDROID_LOG_FATAL:         return 'F';
        case ANDROID_LOG_SILENT:        return 'S';

        case ANDROID_LOG_DEFAULT:
        case ANDROID_LOG_UNKNOWN:
        default:                        return '?';
    }
}

static int __write_to_log_stderr(int fd, struct iovec *vec, size_t nr)
{
    ssize_t ret;
    fd = STDERR_FILENO;
    do {
        ret = log_writev(fd, vec, nr);
    } while (ret < 0 && errno == EINTR);

    return ret;
}

int __android_log_write(int prio, const char *tag, const char *msg)
{
    struct iovec vec[5];
    if (!tag)
        tag = "";

    char prios[3];
    prios[0] = filterPriToChar(prio);
    prios[1] = '|';
    prios[2] = '\n';
    vec[0].iov_base   = (unsigned char *) &prios[0];
    vec[0].iov_len    = 2;
    vec[1].iov_base   = (void *) tag;
    vec[1].iov_len    = strlen(tag);
    vec[2].iov_base   = (unsigned char *) &prios[1];
    vec[2].iov_len    = 1;
    vec[3].iov_base   = (void *) msg;
    vec[3].iov_len    = strlen(msg);
    vec[4].iov_base   = (unsigned char *) &prios[2];
    vec[4].iov_len    = 1;

    return write_to_log(0, vec, 5);
}

int __android_log_print(int prio, const char *tag, const char *fmt, ...)
{
    va_list ap;
    char buf[LOG_BUF_SIZE];

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    return __android_log_write(prio, tag, buf);
}

void __android_log_assert(const char *cond, const char *tag,
			  const char *fmt, ...)
{
    char buf[LOG_BUF_SIZE];

    if (fmt) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
        va_end(ap);
    } else {
        /* Msg not provided, log condition.  N.B. Do not use cond directly as
         * format string as it could contain spurious '%' syntax (e.g.
         * "%d" in "blocks%devs == 0").
         */
        if (cond)
            snprintf(buf, LOG_BUF_SIZE, "Assertion failed: %s", cond);
        else
            strcpy(buf, "Unspecified assertion failed");
    }

    __android_log_write(ANDROID_LOG_FATAL, tag, buf);

    __builtin_trap(); /* trap so we have a chance to debug the situation */
}


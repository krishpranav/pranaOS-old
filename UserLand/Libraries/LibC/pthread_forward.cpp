/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <LibC/bits/pthread_forward.h>

static PthreadFunctions s_pthread_functions;

void __init_pthread_forward(PthreadFunctions funcs)
{
  s_pthread_functions = funcs;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex)
{
    VERIFY(s_pthread_functions.pthread_mutex_trylock);
    return s_pthread_functions.pthread_mutex_trylock(mutex);
}

int pthread_mutex_destroy(pthread_mutex_t* mutex)
{
    VERIFY(s_pthread_functions.pthread_mutex_destroy);
    return s_pthread_functions.pthread_mutex_destroy(mutex);
}

int pthread_mutexattr_init(pthread_mutexattr_t* attr)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_init);
    return s_pthread_functions.pthread_mutexattr_init(attr);
}

int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_settype);
    return s_pthread_functions.pthread_mutexattr_settype(attr, type);
}

int pthread_mutexattr_destroy(pthread_mutexattr_t* attr)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_destroy);
    return s_pthread_functions.pthread_mutexattr_destroy(attr);
}

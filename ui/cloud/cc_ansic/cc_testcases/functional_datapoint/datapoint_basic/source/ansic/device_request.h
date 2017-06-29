/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */

/* Internal structure to pass the arguments to the test threads */
typedef struct test_thread_arguments {
    unsigned int numberOfLoops;
    unsigned int numberPointsPerStream;
    unsigned int numberStreams;
    char * streamIdentifier;
    char * valueType;
    unsigned int length_bytes;
} test_thread_arguments_t;

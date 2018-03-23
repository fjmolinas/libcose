/*
 * Copyright (C) 2018 Freie Universität Berlin
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef COSE_TEST_H
#define COSE_TEST_H


typedef void (*test_func)(void);
typedef struct test {
    const test_func f;
    const char *n;
} test_t;

#endif
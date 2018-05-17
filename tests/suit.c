/*
 * Copyright (C) 2018 Freie Universitat Berlin
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cose/crypto.h"
#include "cose.h"
#include "cose_defines.h"

#include "cose/test.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CUnit/Automated.h"

#ifdef HAVE_ALGO_EDDSA
static uint8_t buf[2048];

static const unsigned char cose_suite[] = {
  0xd8, 0x62, 0x84, 0x44, 0xa1, 0x03, 0x18, 0x2a, 0xa0, 0x58, 0xcd, 0x8a,
  0x02, 0xa4, 0x01, 0x6e, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20,
  0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x02, 0x6e, 0x41, 0x20, 0x74, 0x65,
  0x73, 0x74, 0x20, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x03, 0x6f,
  0x41, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x76, 0x65, 0x6e,
  0x64, 0x6f, 0x72, 0x04, 0x75, 0x41, 0x6e, 0x20, 0x65, 0x78, 0x70, 0x65,
  0x72, 0x69, 0x6d, 0x65, 0x6e, 0x74, 0x61, 0x6c, 0x20, 0x6d, 0x6f, 0x64,
  0x65, 0x6c, 0x50, 0xaa, 0xbc, 0x3a, 0xed, 0x65, 0xe1, 0xe3, 0x20, 0x26,
  0x1f, 0xbc, 0x50, 0x25, 0xd4, 0x99, 0xfc, 0x1a, 0x5a, 0xbb, 0x97, 0xf7,
  0x82, 0x82, 0x02, 0x50, 0x14, 0x92, 0xaf, 0x14, 0x25, 0x69, 0x5e, 0x48,
  0xbf, 0x42, 0x9b, 0x2d, 0x51, 0xf2, 0xab, 0x45, 0x82, 0x01, 0x50, 0xfa,
  0x6b, 0x4a, 0x53, 0xd5, 0xad, 0x5f, 0xdf, 0xbe, 0x9d, 0xe6, 0x63, 0xe4,
  0xd4, 0x1f, 0xfe, 0xf6, 0xf6, 0xf6, 0xf6, 0x87, 0x81, 0x01, 0x10, 0x43,
  0x66, 0x6f, 0x6f, 0x81, 0x82, 0x6e, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f,
  0x2f, 0x66, 0x6f, 0x6f, 0x2e, 0x63, 0x6f, 0x6d, 0x01, 0x81, 0x01, 0xa1,
  0x01, 0x58, 0x20, 0xc3, 0x12, 0x11, 0xd1, 0xff, 0x88, 0xf7, 0x7a, 0x5a,
  0xaf, 0x65, 0x36, 0x77, 0x89, 0x5b, 0xfc, 0xa7, 0x69, 0xf0, 0x6d, 0xa1,
  0x98, 0xa8, 0xfa, 0x71, 0x15, 0x6a, 0xa6, 0x4a, 0xcd, 0x69, 0x5d, 0xf6,
  0x81, 0x83, 0x58, 0x18, 0xa2, 0x01, 0x27, 0x04, 0x53, 0x53, 0x6f, 0x6d,
  0x65, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x40, 0x73, 0x6f, 0x6d, 0x65, 0x77,
  0x68, 0x65, 0x72, 0x65, 0xa0, 0x58, 0x40, 0xc5, 0x5f, 0x03, 0x49, 0x6c,
  0xb1, 0x12, 0x29, 0x5c, 0x93, 0x0f, 0xa6, 0x41, 0xf5, 0xdb, 0xbe, 0x63,
  0x1a, 0x06, 0x0f, 0xda, 0xa2, 0xd6, 0x00, 0xd0, 0x6d, 0x46, 0x66, 0xfc,
  0x45, 0xf9, 0x10, 0x47, 0x48, 0x2a, 0x97, 0x1c, 0x96, 0x9f, 0x09, 0xd4,
  0xf4, 0x01, 0x05, 0x77, 0x05, 0x0b, 0xd8, 0xdb, 0x00, 0x14, 0x8a, 0x7d,
  0xaa, 0x91, 0xf1, 0xb4, 0xbb, 0x27, 0x2d, 0x5b, 0x6f, 0xf8, 0x0c
};
unsigned int test_out_signed_cose_len = 311;

static const uint8_t keyid[] = "Something@somewhere";

static unsigned char pk[COSE_CRYPTO_SIGN_ED25519_PUBLICKEYBYTES] = {
    0x73, 0xc1, 0xc7, 0x59, 0x8f, 0x86, 0x58, 0x3a, 0xcc, 0x84, 0xb4, 0xbd,
    0xf4, 0xb3, 0x7a, 0x79, 0xc0, 0xb9, 0xf5, 0x9b, 0xd3, 0x4d, 0x2f, 0xe7,
    0xe7, 0x04, 0x71, 0x06, 0xa2,0x5c, 0xe3, 0x5a
};


static void print_bytestr(const uint8_t *bytes, size_t len)
{
    for(unsigned int idx=0; idx < len; idx++)
    {
        printf("%02X", bytes[idx]);
    }
}

void test_suit1(void)
{
    cose_sign_t verify;
    cose_key_t signer;
    const uint8_t *kid = NULL;
    /* Initialize struct */
    cose_sign_init(&verify, 0);

    /* First signer */
    cose_key_init(&signer);
    cose_key_set_keys(&signer, COSE_EC_CURVE_ED25519, COSE_ALGO_EDDSA, pk, NULL, NULL);

    printf("COSE bytestream: \n");
    print_bytestr(cose_suite, 311);
    printf("\n");
    /* Decode again */
    int decode_success = cose_sign_decode(&verify, cose_suite, 311);
    printf("Decoding: %d\n", decode_success);
    /* Verify with signature slot 0 */
    CU_ASSERT_EQUAL_FATAL(decode_success, 0);
    int verification = cose_sign_verify(&verify, &signer, 0, buf, sizeof(buf));
    printf("Verification: %d\n", verification);
    CU_ASSERT_EQUAL(verification, 0);
    cose_hdr_t hdr;
    CU_ASSERT(cose_sign_get_protected(&verify, &hdr, COSE_HDR_CONTENT_TYPE));
    CU_ASSERT_EQUAL(hdr.v.value, 42);
    ssize_t res = cose_sign_get_kid(&verify, 0, &kid);
    CU_ASSERT(res);
    CU_ASSERT_EQUAL(memcmp(kid, keyid, sizeof(keyid) - 1), 0);
}
#endif

const test_t tests_suit[] = {
#ifdef HAVE_ALGO_EDDSA
    {
        .f = test_suit1,
        .n = "Verify with known signed",
    },
#endif
    {
        .f = NULL,
        .n = NULL,
    }
};

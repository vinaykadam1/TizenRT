/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file tc_net_core.c
/// @brief Test Case Example for lwip_htonl() API
#include <arpa/inet.h>
#include "tc_internal.h"

#define	NTOHS_VAL		0x1012
#define	NTOHS_CONVAL	0x1210
#define	NTOHL_VAL		0x112A380
#define NTOHL_CONVAL	0X80A31201

/**
* @testcase            : tc_net_core_ntohs_p
* @brief               : The ntohs API converts network to host byte order.
* @scenario            : Function converts the unsigned short integer from host byte order to network byte order.
* @apicovered          : ntohs()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
void tc_net_core_ntohs_p(void)
{
	int ret = ntohs(NTOHS_VAL);	/* Observe value in network byte order */
	TC_ASSERT_NEQ("tc_net_core_ntohs_p", ret, NTOHS_CONVAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_core_ntohl_p
* @brief               : The ntohl API converts network to host byte order.
* @scenario            : Function converts the unsigned integer netlong from network byte order to host byte order.
* @apicovered          : ntohl()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
void tc_net_core_ntohl_p(void)
{
	int ret = ntohl(NTOHL_VAL);	/* Observe value in network byte order */
	TC_ASSERT_NEQ("tc_net_core_ntohl_p", ret, NTOHL_CONVAL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: ntohs() and ntohl()
 ****************************************************************************/
int net_core_ntohs_main(void)
{
	tc_net_core_ntohs_p();
	tc_net_core_ntohl_p();
	return 0;
}

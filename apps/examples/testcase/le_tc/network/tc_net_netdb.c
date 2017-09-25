/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_net_netdb.c
/// @brief Test Case Example for getaddrinfo(), freeaddrinfo() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>

#include <sys/socket.h>
#ifdef CONFIG_LIBC_NETDB
#include <netdb.h>
#endif

#include "tc_internal.h"

#ifdef CONFIG_LIBC_NETDB
/**
* @testcase            :tc_net_netdb_p
* @brief               :frees the memory.
* @scenario            :it frees the memory that was allocated for
*                       the dynamically allocated linked list res.
* @apicovered          :getaddrinfo, freeaddrinfo
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
static void tc_net_netdb_p(void)
{
	int ret;
	struct addrinfo hints;
	struct addrinfo *res;
	char *port = "9099";

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(NULL, port, &hints, &res);
	TC_ASSERT_EQ("getaddrinfo", ret, ZERO)

	/* This API has no way to check errors. */
	freeaddrinfo(res);
	TC_SUCCESS_RESULT()
}
#endif

/****************************************************************************
 * Name:net_netdb_main
 ****************************************************************************/
int net_netdb_main(void)
{
#ifdef CONFIG_LIBC_NETDB
	tc_net_netdb_p();
#endif
	return 0;
}

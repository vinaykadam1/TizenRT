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

/// @file tc_net_api.c
/// @brief Test Case Example for api() API
#include "tc_internal.h"
#include <net/lwip/api.h>
#include <net/lwip/sockets.h>

#define PORT_NUM        5000
#define MAXBUF          80
#define COUNT           10
#define BACKLOG         3

static int count_wait;

/**
* @fn                    :sig_wait
* @brief                 :function to wait on semaphore
* @scenario              :use wait function to decrement count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void sig_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    :sig_call
* @brief                 :function to signal semaphore.
* @scenario              :use to increase the count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void sig_call(void)
{
	count_wait++;
}

/**
* @fn                    :tc_net_server
* @brief                 :create the server.
* @scenario              :implementation of server's functionality for client-server architect.
* @API's covered         :socket,bind,listen,accept,recv
* @Preconditions         :socket file descriptor.
* @Postconditions        :none
* @return                :void
*/
static void tc_net_server(void)
{
	int i = 0, rc = 0, flags, ret;
	int new_sd;
	char buffer[MAXBUF];
	struct sockaddr_in addr;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	flags = fcntl(sock, F_GETFL, 0);
	TC_ASSERT_NEQ("fcntl", flags, NEG_VAL);

	flags = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	TC_ASSERT_NEQ("fcntl", flags, NEG_VAL);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_LOOPBACK;
	addr.sin_port = htons(PORT_NUM);

	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);

	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ("listen", ret, NEG_VAL);

	sig_call();
	new_sd = accept(sock, NULL, NULL);
	TC_ASSERT_NEQ("accept", new_sd, NEG_VAL);

	while (i++ < COUNT && rc < ZERO) {
		rc = recv(new_sd, buffer, sizeof(buffer), 0);
	}
	TC_ASSERT_NEQ("recv", rc, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                    :tc_net_client
* @brief                 :client thread.
* @scenario              :implementation of client's functionality for client-server architect.
* @API's covered         :socket,connect,close
* @Preconditions         :socket file descriptor.
* @Postconditions        :none
* @return                :void
*/
static void tc_net_client(void)
{
	int ret, len;
	char buffer[MAXBUF] = "HELLOWORLD\n";
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORT_NUM);

	sig_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_NEQ("connect", ret, NEG_VAL);

	len = write(sock, buffer, strlen(buffer));
	TC_ASSERT_NEQ("write", len, NEG_VAL);
}

/**
* @fn                    :net_api_server
* @brief                 :server thread.
* @scenario              :server thread for creating server's socket program.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void*
*/
void* net_api_server(void *args)
{
	tc_net_server();
	return NULL;
}

/**
* @fn                    :net_api_client
* @brief                 :client thread.
* @scenario              :client thread for creating client's socket program.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void*
*/
void* net_api_client(void *args)
{
	tc_net_client();
	return NULL;
}

/****************************************************************************
 * Name: net_api_main
 ****************************************************************************/
int net_api_main(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, net_api_server, NULL);
	pthread_create(&Client, NULL, net_api_client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
	return 0;
}

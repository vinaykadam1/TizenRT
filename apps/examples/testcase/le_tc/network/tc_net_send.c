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

// @file tc_net_send.c
// @brief Test Case Example for send() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netutils/netlib.h>
#include "tc_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORTNUM 1110
#define MAXRCVLEN 20
int s1 = 0;
/**
* @fn                   : wait1
* @brief                : function to wait on semaphore
* @scenario             : To aquire the resource
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void wait1(void)
{
	while (s1 <= 0) {

		printf("");
	}
	s1--;
}

/**
* @fn                   : signal1
* @brief                : function to signal semaphore
* @scenario             : To release the resource
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void signal1(void)
{
	s1++;
}

/**
* @testcase            : tc_net_send_p
* @brief               : +ve test case for send api
* @scenario            : used for tcp connection
* @apicovered          : accept(),send()
* @precondition        : Valid file descriptor
* @postcondition       : none
*/
void tc_net_send_p(int fd)
{
	char *msg = "Hello World !\n";
	int ConnectFD = accept(fd, NULL, NULL);
	int ret = send(ConnectFD, msg, strlen(msg), 0);

	TC_ASSERT_NEQ("send", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

	close(ConnectFD);
}

/**
* @fn                   : server
* @brief                : Server thread
* @scenario             : used for tcp connection
* API's covered         : socket,bind,listen,close
* Preconditions         : Valid file descriptor
* Postconditions        : none
* @return               : void *
*/
void *server(void *args)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));

	listen(sock, 2);

	signal1();
	tc_net_send_p(sock);

	close(sock);
	return NULL;
}

/**
* @fn                   : client
* @brief                : Client thread
* @scenario             : used for tcp connection
* API's covered         : socket,connect,recv,close
* Preconditions         : Valid file descriptor
* Postconditions        : none
* @return               : void *
*/
void *client(void *args)
{

	char buffer[MAXRCVLEN];
	int len, ret;
	struct sockaddr_in dest;
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	wait1();

	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	len = recv(sock, buffer, MAXRCVLEN, 0);
	buffer[len] = '\0';

	close(sock);
	return NULL;
}

/**
* @fn                  : tc_net_send
* @brief               :
* @scenario            :
* API's covered        :
* Preconditions        :
* Postconditions       :
* @return              : void
*/
void tc_net_send(void)
{
	int ret;
	pthread_t Server, Client;

	ret = pthread_create(&Server, NULL, server, NULL);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);
	ret = pthread_create(&Client, NULL, client, NULL);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);
	ret = pthread_join(Server, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
	ret = pthread_join(Client, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
}

/****************************************************************************
 * Name: send()
 ****************************************************************************/
int net_send_main(void)
{

	tc_net_send();
	return 0;
}

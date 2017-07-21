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

#include <net/lwip/api.h>

static void tc_net_netconn_close_n(void)
{
    struct netconn *conn = NULL;
    netconn_close(conn);
}


int net_api_main(void)
{
    tc_net_netconn_close_n();
    return 0;
}

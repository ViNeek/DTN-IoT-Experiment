/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Testing the broadcast layer in Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include <iot/common.h>

#include <iot/mjson.h>

#include "sys/ctimer.h"

#include <stdio.h>

static const char *mesg = "{ \"type\": \"traffic\", \"desc\": \"polu\"}";

static const char *json_str3 = "[\"foo\",\"bar\",\"baz\"]";


static char *stringptrs[5];
static char stringstore[256];
static int stringcount;

static const struct json_array_t json_array_3 = {
    .element_type = t_string,
    .arr.strings.ptrs = stringptrs,
    .arr.strings.store = stringstore,
    .arr.strings.storelen = sizeof(stringstore),
    .count = &stringcount,
    .maxlen = sizeof(stringptrs) / sizeof(stringptrs[0]),
};


#ifdef DTN_SERVER
  static struct iotDataMule g_DataMule;
#endif

#ifdef DTN_CLIENT
  static struct iotClient g_Client;
#endif

/*---------------------------------------------------------------------------*/
PROCESS(dtn_process, "dtn process");
AUTOSTART_PROCESSES(&dtn_process);

PROCESS_THREAD(dtn_process, ev, data) {

  // static struct etimer e;

  // PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  int status;

  status = json_read_array(json_str3, &json_array_3, NULL);
  printf("Got string %d", stringcount);
  printf("Got string %s", stringptrs[0]);
  printf("Got string %s", stringptrs[1]);
  printf("Got string %s\n", stringptrs[2]);
 
  iot_init();
  iot_log_level(INFO);

#ifdef DTN_SERVER
  struct iotDataMule data_mule;

  iot_mule_create(&g_DataMule);
#endif

#ifdef DTN_CLIENT
  //iot_producer_create();
#endif

  //ctimer_set(iot_ticker(), 1 * CLOCK_SECOND, iot_clock_tick, (void *)NULL);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

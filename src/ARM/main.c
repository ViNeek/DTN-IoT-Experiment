#include <iot/common.h>

#include <stdio.h>

// Client or Mule
#if ROLE==IOT_SERVER
  static struct iotDataMule g_DataMule;
  struct iotClient* iot_client_ref() { return NULL; }
  struct iotDataMule* iot_mule_ref() { return &g_DataMule; }
#endif

#if ROLE==IOT_CLIENT
  static struct iotClient g_Client;
  struct iotClient* iot_client_ref() { return &g_Client; }
  struct iotDataMule* iot_mule_ref() { return NULL; }
#endif

/*---------------------------------------------------------------------------*/
PROCESS(dtn_process, "dtn process");
AUTOSTART_PROCESSES(&dtn_process);

PROCESS_THREAD(dtn_process, ev, data) {

// Set up exit handlers
#if ROLE==IOT_SERVER
  PROCESS_EXITHANDLER(iot_mule_close(&g_DataMule);)
#endif
  
#if ROLE==IOT_CLIENT
  PROCESS_EXITHANDLER(iot_client_close(&g_Client);)
#endif

  PROCESS_BEGIN();

  /*
  int status;

  status = json_read_array(json_str3, &json_array_3, NULL);
  printf("Got string %d\n", stringcount);
  printf("Got string %s\n", stringptrs[0]);
  printf("Got string %s\n", stringptrs[1]);
  printf("Got string %s\n", stringptrs[2]);
  */

  iot_init();

#if ROLE==IOT_SERVER
  IOT_LOG_INFO("Starting Mule at address %s", iot_node_address());

  iot_mule_create(&g_DataMule);
#endif
  
#if ROLE==IOT_CLIENT
  IOT_LOG_INFO("Starting Client at address %s", iot_node_address());

  iot_client_create(&g_Client);
#endif

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

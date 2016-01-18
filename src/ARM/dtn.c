#include <iot/dtn.h>

#include <iot/networking.h>

#include <stdio.h>

#include <string.h>

static struct ctimer g_AnnounceCallback;
static void _iot_mule_announce(void *dm) {
  iotChar buff[256];
  buff[0] = 0;
  iotInt32 len = 0;

  struct iotDataMule *mule = (struct iotDataMule *)dm;

  printf("Description %s\n",
       iot_cache_json_desc(&mule->m_PacketCache, buff, &len));
  printf("Size %d", len);

  ctimer_reset(&g_AnnounceCallback);
  //packetbuf_copyfrom("Hello", 6);
  //broadcast_send(&mule->m_BC);
}

iotInt32 iot_mule_create(struct iotDataMule *dm) {
  memset(dm, 0, sizeof(struct iotDataMule));
  //printf("Mule %d", sizeof(struct iotDataMule));
  broadcast_open(&dm->m_BC, 129, iot_mule_broadcast_callbacks());
  unicast_open(&dm->m_UC, 129, iot_mule_unicast_callbacks());

  iot_cache_init(&dm->m_PacketCache);
  iot_cache_random_populate(&dm->m_PacketCache);
  
  ctimer_set(&g_AnnounceCallback, CLOCK_SECOND, _iot_mule_announce, dm);

  // char buff[256];
  // printf("Description %s\n", iot_cache_json_desc(&dm->m_PacketCache, buff));

  return 0;
}

iotInt32 iot_client_create(struct iotClient *p) { return 0; }

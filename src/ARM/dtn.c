#include <iot/dtn.h>

#include <iot/networking.h>

#include <stdio.h>

static struct ctimer g_AnnounceCallback;
static void _iot_mule_announce(void *dm) {
  struct iotDataMule *mule = (struct iotDataMule *)dm;

  printf("Hello from inside the mule\n");

  ctimer_reset(&g_AnnounceCallback);
}

iotInt32 iot_mule_create(struct iotDataMule *dm) {
  memset(dm, 0, sizeof(struct iotDataMule));

  broadcast_open(&dm->m_BC, 129, iot_mule_broadcast_callbacks());
  unicast_open(&dm->m_UC, 129, iot_mule_unicast_callbacks());

  ctimer_set(&g_AnnounceCallback, CLOCK_SECOND, _iot_mule_announce, NULL);

  return 0;
}

iotInt32 iot_client_create(struct iotClient* p) { return 0; }

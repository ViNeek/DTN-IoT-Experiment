#include <iot/common.h>

#include <stdio.h>

#include <string.h>

static struct ctimer g_AnnounceCallback;
static void _iot_mule_announce(void *dm) {
  iotChar buff[IOT_PACKET_SIZE];
  buff[0] = 0;
  iotInt32 len = 0;

  struct iotDataMule *mule = (struct iotDataMule *)dm;

  iot_cache_json_desc(&mule->m_PacketCache, buff, &len);

  // Reset Timer
  ctimer_reset(&g_AnnounceCallback);

  iot_broadcast(IOT_NETWORK_ENTITY(mule), buff, len);
}

static struct ctimer g_ClientAnnounceCallback;
static void _iot_client_announce(void *c) {
  iotChar buff[IOT_PACKET_SIZE];
  buff[0] = 0;
  iotInt32 len = 0;

  struct iotClient *client = (struct iotClient *)c;

  // Reset Timer
  ctimer_reset(&g_ClientAnnounceCallback);

}

iotInt32 iot_mule_create(struct iotDataMule *dm) {
  memset(dm, 0, sizeof(struct iotDataMule));

  broadcast_open(&dm->m_BC, IOT_RIME_BROADCAST_CHANNEL, iot_mule_broadcast_callbacks());
  unicast_open(&dm->m_UC, IOT_RIME_UNICAST_CHANNEL, iot_mule_unicast_callbacks());

  iot_cache_init(&dm->m_PacketCache);
  iot_cache_random_populate(&dm->m_PacketCache);

  ctimer_set(&g_AnnounceCallback, IOT_BROADCAST_INTERVAL, _iot_mule_announce, dm);

  return 0;
}

iotInt32 iot_client_create(struct iotClient *client) { 
  iotChar buff[IOT_PACKET_SIZE];
  buff[0] = 0;
  iotInt32 length;

  broadcast_open(&client->m_BC, IOT_RIME_BROADCAST_CHANNEL, iot_client_broadcast_callbacks());
  unicast_open(&client->m_UC, IOT_RIME_UNICAST_CHANNEL, iot_client_unicast_callbacks());

  client->m_InRange = iot_flip_coin();
  for (iotInt32 i = 0; i < MAX_TYPE; ++i) {
    client->m_Interests[i] = iot_flip_coin();
  }

  iot_client_interest_json_desc(client, buff, &length);

  IOT_LOG_INFO("Client interest %s of size %d", buff, length);

  return 0;
}

iotChar *iot_client_interest_json_desc(struct iotClient *c, iotChar *buffer,
                             iotInt32 *len) {
  iotInt32 length = 2;
  buffer[0] = 0;
  iotBool empty = IOT_TRUE;

  strcat(buffer, "[");
  for (iotInt32 i = 0; i < MAX_TYPE; ++i) {
    if (c->m_Interests[i] == 1) {
      empty = IOT_FALSE;
      strcat(buffer, "\"");
      strcat(buffer, iot_packet_type(i));
      strcat(buffer, "\"");
      strcat(buffer, ",");
    }
  }

  if ( !empty ) {
    // Erase dangling 'comma'
    length = strlen(buffer);
    buffer[length-1] = 0;
  }

  strcat(buffer, "]");

  *len = length;
  
  return buffer;

}

// Mule Node RIME address
static iotChar g_MuleRimeAddrDesc[IOT_RIME_ADDRESS_LENGTH] = { 0 };
static rimeaddr_t g_MuleRimeAddr;
iotBool iot_mule_discovered() {
  return g_MuleRimeAddrDesc[0] != 0;
}

const rimeaddr_t *iot_mule_address() {
  return &g_MuleRimeAddr;
}

const iotChar* iot_mule_address_desc() {
  return g_MuleRimeAddrDesc;
}

iotInt32 iot_set_mule_address(const rimeaddr_t *address) {
  g_MuleRimeAddr = *address;
  g_MuleRimeAddrDesc[0] = 0;
  sprintf(g_MuleRimeAddrDesc, "%d.%d", g_MuleRimeAddr.u8[0],g_MuleRimeAddr.u8[1]);
  
  return 0;
}

iotInt32 iot_mule_close(struct iotDataMule *dm) {
  broadcast_close(&dm->m_BC);
  unicast_close(&dm->m_UC);

  return 0;
}

iotInt32 iot_client_close(struct iotClient* client) {
  broadcast_close(&client->m_BC);
  unicast_close(&client->m_UC);

  return 0;
}

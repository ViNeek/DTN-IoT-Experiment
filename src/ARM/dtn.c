#include <iot/common.h>

#include <stdio.h>

#include <string.h>

#if TARGET!=IOT_PLATFORM_NATIVE
#include "dev/light-sensor.h"
#include "dev/leds.h"
#endif

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

static struct ctimer g_ClientCreateInterestCallback;
static void _iot_client_announce(void *c) {
  iotChar buff[IOT_PACKET_SIZE];
  buff[0] = 0;
  iotInt32 len = 0;

  struct iotClient *client = (struct iotClient *)c;
  client->m_PendingAnnounce = IOT_TRUE;
  if ( iot_mule_discovered() ) {
    //iot_client_interest_json_desc(client, buff, &len);
    iot_packet_generate_forward(client, buff, &len);
    iot_send(IOT_NETWORK_ENTITY(client), buff, len, iot_mule_address() );
    client->m_PendingAnnounce = IOT_FALSE;
  } else {
    IOT_LOG_INFO("No mule around");
  }
  // Reset Timer
  //ctimer_reset(&g_ClientAnnounceCallback);

}

static struct ctimer g_StateSwapCallback;
static void _iot_state_swap(void *c) {
  iotInt32 nextEpochDuration;
  iotInt32 nextInterestPoint;
  struct iotClient *client = (struct iotClient *)c;

  client->m_InRange = iot_flip_coin();

  nextEpochDuration = iot_random_in_range(IOT_EPOCH_MIN, IOT_EPOCH_MAX);
  //IOT_LOG_INFO("Next Epoch %d", nextEpochDuration);
  if ( client->m_InRange ) {
    #if TARGET==IOT_PLATFORM_SKY
      leds_toggle(LEDS_ALL);
      _iot_client_announce(client);
    #else
      nextInterestPoint = iot_random_in_range(IOT_INTEREST_MIN, nextEpochDuration / 2);
      //IOT_LOG_INFO("Next Interest %d %d", nextInterestPoint, nextInterestPoint - CLOCK_SECOND);
      ctimer_set(&g_ClientCreateInterestCallback, nextInterestPoint, _iot_client_announce, c);
    #endif
    //IOT_LOG_INFO("ALIVE");
  } else {
    //IOT_LOG_INFO("NOT ALIVE");
  }
  // New Timer
  //ctimer_reset(&g_StateSwapCallback);
  ctimer_set(&g_StateSwapCallback, nextEpochDuration, _iot_state_swap, c);
}

iotInt32 iot_mule_create(struct iotDataMule *dm) {
  memset(dm, 0, sizeof(struct iotDataMule));

  broadcast_open(&dm->m_BC, IOT_RIME_BROADCAST_CHANNEL, iot_mule_broadcast_callbacks());
  unicast_open(&dm->m_UC, IOT_RIME_UNICAST_CHANNEL, iot_mule_unicast_callbacks());

  iot_cache_init(&dm->m_PacketCache);
  //iot_cache_random_populate(&dm->m_PacketCache);

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
    client->m_Interests[i] = IOT_FALSE;
    IOT_LOG_INFO("Client interest %d", iot_flip_coin());
  }

  //iot_client_interest_json_desc(client, buff, &length);
  iot_packet_generate_forward(client, buff, &length);

  IOT_LOG_INFO("Client interest %s of size %d", buff, length);

  ctimer_set(&g_StateSwapCallback, IOT_EPOCH_MID, _iot_state_swap, client);
  if ( client->m_InRange ) {
    #if TARGET==IOT_PLATFORM_SKY
      leds_toggle(LEDS_ALL);
      _iot_client_announce(client);
    #else
      ctimer_set(&g_ClientCreateInterestCallback, IOT_EPOCH_MID / 2, _iot_client_announce, client);
    #endif
  }
  
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
  return &g_MuleRimeAddrDesc[0];
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

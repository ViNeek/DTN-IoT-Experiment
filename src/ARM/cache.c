#include <string.h>
#include <iot/cache.h>
#include <iot/util.h>

#include <iot/mjson.h>

#include <string.h>
#include <stdio.h>

iotInt32 iot_cache_clear(struct iotCache *c) {
  c->m_NextIdx = 0;

  return 0;
}

iotInt32 iot_cache_init(struct iotCache *c) {
  c->m_NextIdx = 0;
  for (iotInt32 i = 0; i < IOT_CACHE_SIZE; ++i) {
    c->m_Cache[i].m_Buff[IOT_PACKET_HEADER_OFFSET] = 0;
    c->m_Cache[i].m_Buff[IOT_PACKET_PAYLOAD_OFFSET] = 0;
    c->m_Cache[i].m_Type = UNKNOWN_TYPE;
  }

  return 0;
}

iotInt32 iot_cache_random_populate(struct iotCache *c) {
  iot_cache_init(c);
  
  for (iotInt32 i = 0; i < IOT_CACHE_SIZE; ++i) {
    iotInt32 randType = iot_random_in_range(0, MAX_TYPE);
   
    c->m_Cache[i].m_Type = randType;
    iotChar *header = &c->m_Cache[i].m_Buff[IOT_PACKET_HEADER_OFFSET];
    iotChar *payload = &c->m_Cache[i].m_Buff[IOT_PACKET_PAYLOAD_OFFSET];

    *header = 0;
    *payload = 0;
    
    strcpy(header, iot_packet_type(randType));
    sprintf(payload, "https://www.google.com/?id=%d", i);
    //strcpy(payload, "https://www.google.com/?id=%d");

    //IOT_LOG_INFO("Header %s", header);
    //IOT_LOG_INFO("Payload %s", payload);
  }

  return 0;
}

struct iotCachedPacket *iot_cache_next(struct iotCache *c) {
  // Packet cache is a simple FIFO queue
  if (c->m_NextIdx == IOT_CACHE_SIZE)
    c->m_NextIdx = 0;

  // Get open slot
  struct iotCachedPacket *slot = &c->m_Cache[c->m_NextIdx];

  c->m_NextIdx++;

  return slot;
}

iotChar *iot_cache_json_desc(struct iotCache *c, iotChar *buffer,
                             iotInt32 *len) {
  iotInt32 length = 2;
  buffer[0] = 0;
  iotBool cacheHits[MAX_TYPE+1];
  iotBool empty = IOT_TRUE;
  
  memset(cacheHits, 0, MAX_TYPE+1);

  strcat(buffer, "[");
  for (iotInt32 i = 0; i < IOT_CACHE_SIZE; ++i) {
      cacheHits[c->m_Cache[i].m_Type] = 1;
  }

  for (iotInt32 i = 0; i < MAX_TYPE; ++i) {
    if (cacheHits[i] == 1) {
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

static iotChar *g_StringPtrs[MAX_TYPE];
static iotChar g_StringBuffer[IOT_PACKET_SIZE];
static iotInt32 g_StringCount;
static const struct json_attr_t g_ForwardType[] = {
    {"F", t_array, .addr.array.element_type = t_string,
                       .addr.array.arr.strings.ptrs = g_StringPtrs,
                       .addr.array.arr.strings.store = g_StringBuffer,
                       .addr.array.arr.strings.storelen = sizeof(g_StringBuffer),
                       .addr.array.count = &g_StringCount,
                       .addr.array.maxlen = sizeof(g_StringPtrs) / sizeof(g_StringPtrs[0])},
    {NULL},
};

iotInt32 iot_cache_add(struct iotCache *c, iotChar* buff, iotInt32 len) {
  // Packet cache is a simple FIFO queue
  iotInt32 status;

  status = json_read_object(buff, g_ForwardType, NULL);
  //IOT_LOG_INFO("Error %d", status);
  if ( status != 0 ) {
    return -1;
  }
  //IOT_LOG_INFO("Cache add %d %s", status, buff);
  //IOT_LOG_INFO("Count %d", g_StringCount);
  //IOT_LOG_INFO("String %s", g_StringPtrs[0]);
  //IOT_LOG_INFO("String %s", g_StringPtrs[1]);

  if (c->m_NextIdx == IOT_CACHE_SIZE)
    c->m_NextIdx = 0;

  // Get open slot
  struct iotCachedPacket *slot = &c->m_Cache[c->m_NextIdx];
  iotChar *header = &slot->m_Buff[IOT_PACKET_HEADER_OFFSET];
  iotChar *payload = &slot->m_Buff[IOT_PACKET_PAYLOAD_OFFSET];

  // Update slot with incoming packet
  strcpy(header, g_StringPtrs[0]);
  strcpy(payload, g_StringPtrs[1]);
  slot->m_Len = strlen(g_StringPtrs[1]);
  slot->m_Type = iot_packet_type_by_string(header);
  
  //IOT_LOG_INFO("Count %d", slot->m_Len);
  //IOT_LOG_INFO("Payload %s", payload);
  //IOT_LOG_INFO("Header %s", header);

  c->m_NextIdx++;

  return 0;
}

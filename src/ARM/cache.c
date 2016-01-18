#include <string.h>
#include <iot/cache.h>
#include <iot/util.h>

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
    c->m_Cache[i].m_Type = 0;
  }

  return 0;
}

iotInt32 iot_cache_random_populate(struct iotCache *c) {
  iot_cache_init(c);

  for (iotInt32 i = 0; i < IOT_CACHE_SIZE; ++i) {
    int randType = iot_random_in_range(0, MAX_TYPE);
    //printf("Max Type %d", MAX_TYPE);
    if (randType > MAX_TYPE) {
        printf("Rand type : %d", randType);
    }
    c->m_Cache[i].m_Type = randType;
    iotChar *header = &c->m_Cache[i].m_Buff[IOT_PACKET_HEADER_OFFSET];
    iotChar *payload = &c->m_Cache[i].m_Buff[IOT_PACKET_PAYLOAD_OFFSET];

    *header = 0;
    *payload = 0;

    strcpy(header, iot_packet_type(randType));
    strcpy(payload, "Random text");
    
    // printf("Key %s\n", header );
    // printf("Pay %s\n", payload );
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
  iotInt32 length = 0;
  buffer[0] = 0;
  iotInt32 cacheHits[MAX_TYPE];

  strcat(buffer, "[");
  for (iotInt32 i = 0; i < IOT_CACHE_SIZE; ++i) {
    //printf("Type %d", c->m_Cache[i].m_Type);
      cacheHits[c->m_Cache[i].m_Type] = 1;
    //cacheHits[0] = 1;
    //cacheHits[1] = 1;
    //cacheHits[2] = 1;
    //cacheHits[3] = 1;
    //cacheHits[4] = 1;
  }

  for (iotInt32 i = 0; i < MAX_TYPE; ++i) {
    if (cacheHits[i] == 1) {
      strcat(buffer, "\"");
      strcat(buffer, iot_packet_type(i));
      strcat(buffer, "\"");
      strcat(buffer, ",");
    }
  }

  // Erase last 'comma'
  length = strlen(buffer);
  buffer[length-1] = 0;
  strcat(buffer, "]");

  *len = length + 1;
  
  return buffer;
}

iotInt32 iot_cache_add(struct iotCache *c, struct iotCachedPacket *p) {
  // Packet cache is a simple FIFO queue
  if (c->m_NextIdx == IOT_CACHE_SIZE)
    c->m_NextIdx = 0;

  // Get open slot
  struct iotCachedPacket *slot = &c->m_Cache[c->m_NextIdx];

  // Update slot with incoming packet
  slot->m_Len = p->m_Len;
  memcpy(&slot->m_Buff[0], &p->m_Buff[0], p->m_Len);
  slot->m_Buff[slot->m_Len] = 0;

  c->m_NextIdx++;

  return 0;
}

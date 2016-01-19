#ifndef __IOT_PACKET__H
#define __IOT_PACKET__H

#include <iot/types.h>

#define IOT_PACKET_SIZE 128
#define IOT_PACKET_HEADER_OFFSET 0
#define IOT_PACKET_PAYLOAD_OFFSET 32

// Available packet types

enum iotPacketTypes {
  EVENT,
  TRAFFIC,
  DISCOUNT,
  PARKING,
  FOOD,
  MAX_TYPE,
  UNKNOWN_TYPE
};

struct iotCachedPacket {
  iotInt32 m_Len;
  enum iotPacketTypes m_Type;
  char m_Buff[IOT_PACKET_SIZE];
} iotCachedPacket;

const char *iot_packet_type(enum iotPacketTypes t);

#endif

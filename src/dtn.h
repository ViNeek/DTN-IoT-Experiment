#ifndef __IOT_DTN__H
#define __IOT_DTN__H

struct iotProducer {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotProducer;

struct iotConsumer {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotProducer;

struct iotDataMule {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotProducer;

int 	iot_data_mule_create(struct iotPacket* p, char* msg);
int 	iot_packet_create(struct iotPacket* p, char* msg);


struct iotPacket {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotPacket;

int 	iot_packet_create(struct iotPacket* p, char* msg);
int 	iot_packet_create_json(char* json);
void* 	iot_packet_create_serialized_json(char* json);

#endif  // __IOT_DTN__H
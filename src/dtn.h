#ifndef __IOT_DTN__H
#define __IOT_DTN__H

#include "common.h"

struct iotProducer {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotProducer;

struct iotConsumer {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotConsumer;

struct iotDataMule {
	int 	m_Len;
	int 	m_EndFlag;
	char	m_Buff[256];
} iotDataMule;

int 	iot_data_mule_create(struct iotDataMule* dm);
int 	iot_producer_create(struct iotProducer* p);
int 	iot_consumer_create(struct iotConsumer* c);


#endif  // __IOT_DTN__H
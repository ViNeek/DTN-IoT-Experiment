#include <stdio.h>

#include "util.h"
#include "networking.h"

#include "dtn.h"

int iot_listener_create(struct iotListener* b) {
	int iResult = 0;
	int broadcastFlag = 1;
	char str[256];
#ifdef NATIVE

	b->m_Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (b->m_Sock < 0) {
        iot_error("ERROR opening listener socket");
        return -1;
    }

	setsockopt(b->m_Sock, SOL_SOCKET, SO_BROADCAST,
            (const char *)&broadcastFlag, sizeof broadcastFlag);

	memset(&b->m_ClientAddr, 0, sizeof(b->m_ClientAddr));

	printf("Luda\n");

	b->m_ClientAddr.sin_family = AF_INET;
	b->m_ClientAddr.sin_port = htons(BROADCAST_PORT);
	b->m_ClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);;

	//inet_pton(AF_INET, "127.0.0.1", &(b->m_ClientAddr.sin_addr));

	// now get it back and print it
	//inet_ntop(AF_INET, &(b->m_ClientAddr.sin_addr), str, 256);

	//printf("%s\n", str); // prints "192.0.2.33"

	// When receiving we need to bind
	if ( bind(b->m_Sock, (struct sockaddr *)&b->m_ClientAddr, sizeof(struct sockaddr)) < 0 )  {
		iot_error("ERROR binding listener socket");
        return -1;
	}

#endif //NATIVE	

	return iResult;
}

int iot_broadcaster_create(struct iotBroadcaster* b) {
	int iResult = 0;
	int broadcastFlag = 1;
	char str[256];
#ifdef NATIVE

	b->m_Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (b->m_Sock < 0) {
        iot_error("ERROR opening broadcast socket");
        return -1;
    }

	setsockopt(b->m_Sock, SOL_SOCKET, SO_BROADCAST,
            (const char *)&broadcastFlag, sizeof broadcastFlag);

	memset(&b->m_ClientAddr, 0, sizeof(b->m_ClientAddr));

	b->m_ClientAddr.sin_family = AF_INET;
	b->m_ClientAddr.sin_port = htons(BROADCAST_PORT);
	//b->m_ClientAddr.sin_addr.s_addr = inet_pton(BROADCAST_ADDRESS);
	inet_pton(AF_INET, BROADCAST_ADDRESS, &(b->m_ClientAddr.sin_addr));

	// now get it back and print it
	inet_ntop(AF_INET, &(b->m_ClientAddr.sin_addr), str, 256);

	printf("%s\n", str); // prints "192.0.2.33"
	// When sending there is no need to bind

#endif //NATIVE	

	return iResult;
}

int iot_client_create(struct iotClient* c) {
	int iResult = 0;

#ifdef NATIVE

	c->m_Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (c->m_Sock < 0) {
        iot_error("ERROR opening unicast socket");
        return -1;
    }

	memset(&c->m_ClientAddr, 0, sizeof(c->m_ClientAddr));
	c->m_ClientAddr.sin_family = AF_INET;
	c->m_ClientAddr.sin_port = htons(BROADCAST_PORT+1);
	c->m_ClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);;

	if ( bind(c->m_Sock, (struct sockaddr *)&c->m_ClientAddr, sizeof(struct sockaddr)) < 0 )  {
		iot_error("ERROR binding unicast socket");
        return -1;
	}

#endif //NATIVE	

	return iResult;
}

static const char* DEBUG_MESSAGE = "Please be received!";

int iot_client_sendto(struct iotClient* s, struct iotServer* r,struct iotPacket* p) {
	int iResult = 0;

#ifdef NATIVE
	//char b[SND_BUFFER_MAX];
	int sendSize = strlen(DEBUG_MESSAGE);
	int actualSendSize;

	if ( NULL == p ) {
    	if ( ( actualSendSize = sendto(s->m_Sock, DEBUG_MESSAGE, sendSize, 0, (struct sockaddr *) 
         	 &r->m_ClientAddr, sizeof(r->m_ClientAddr)) ) < 0 ) {
        	iot_error("sendto() sent a different number of bytes than expected");
    	}

    	printf("In flight %d\n", actualSendSize);
	}
#endif //NATIVE

	return iResult;
}

int iot_client_recvfrom(struct iotServer* r, struct iotClient* s) {
	int iResult = 0;
	
#ifdef NATIVE
	char b[RCV_BUFFER_MAX];
	int rcvdSize;
	struct sockaddr_in 		ClientAddr;
	memset(&ClientAddr, 0, sizeof(ClientAddr));

	//int actualSendSize;

	while(1)
	{
    	int slen=sizeof(struct sockaddr);
    	if ( ( rcvdSize = recvfrom(r->m_Sock, b, sizeof(b)-1, 0, (struct sockaddr *)&ClientAddr, &slen) ) < 0 ) {
        	iot_error("recvfrom() rcved a different number of bytes than expected");
    	}

    	printf("recv: %s : %d\n", b, rcvdSize);
	}

	/*
    if ( actualSendSize = sendto(s->m_Sock, DEBUG_MESSAGE, sendSize, 0, (struct sockaddr *) 
         &r->m_ClientAddr, sizeof(r->m_ClientAddr)) ) {
        iot_error("sendto() sent a different number of bytes than expected");
    }
	*/
#endif //NATIVE

	return iResult;
}

int iot_init_networking() {
	int iResult = 0;

#ifdef NATIVE

#ifdef WIN32	

	WSADATA wsaData;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
    	printf("WSAStartup failed with code: %d\n", iResult);
    	return 1;
	}

#endif //WIN32

#endif //NATIVE

	return iResult;
}

int iot_client_close(struct iotClient* c) {
	int iResult = 0;

#ifdef NATIVE

	close(c->m_Sock);

#endif //NATIVE

	return iResult;
}

int iot_uninit_networking() {
	int iResult = 0;

#ifdef NATIVE

#ifdef WIN32	

	// DeInitialize Winsock
	iResult = WSACleanup();
	if (iResult != 0) {
    	printf("WSACleanup failed with code: %d\n", iResult);
    	return 1;
	}

#endif //WIN32

#endif //NATIVE

	return iResult;
}
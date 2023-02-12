#ifndef FILE_net
#ifndef FILE_nss
#include "ns/s.h"
#endif
// Для корректной работы freeaddrinfo в MinGW
// Подробнее: http://stackoverflow.com/a/20306451
#define _WIN32_WINNT 0x502
#include <WinSock2.h>
#include <WS2tcpip.h>

class Server {
private:
	STRING error;
	WSADATA wsa;
	addrinfo hints;
	SOCKET socket;
	addrinfo* addr=NULL;
public:
	Server(LETTER(addr),LETTER(port)){
		INT_M res;
		ZeroMemory(&this->hints,sizeof(this->hints));
		hints.ai_family=AF_INET;
		hints.ai_flags=AI_PASSIVE;
		hints.ai_protocol=IPPROTO_TCP;
		hints.ai_socktype=SOCK_STREAM;
		res=getaddrinfo(addr,port,&this->hints,&this->addr);
		if(!res){
			this->socket=socket(this->addr->ai_family,
			this->addr->ai_socktype,this->addr->ai_protocol);
			res=bind(this->socket,this->addr->ai_addr,(INT_L)this->addr->ai_addrlen);
			res=listen(this->socket,SOMAXCONN);
		}else{
			this->error="getaddrinfo failed: ";
			this->error+=s::tome(res);
		}
	}
	~Server(){
		closesocket(listen_socket);
		freeaddrinfo(this->addr);
		WSACleanup();
	}
	STRING Error(){
		return this->error;
	}
}

#define FILE_net
#endif

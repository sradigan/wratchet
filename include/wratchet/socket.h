#ifndef WRATCHET_SOCKET_H
#define WRATCHET_SOCKET_H

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <netdb.h>
	#include <errno.h>
	#include <string.h>
#endif

#define WRATCHET_STREAM SOCK_STREAM
#define WRATCHET_DGRAM SOCK_DGRAM

#ifdef _WIN32
	typedef SOCKET wratchet_socket;
#else
	typedef int wratchet_socket;
#endif

typedef struct sockaddr_in wratchet_sockaddr_in;
typedef struct sockaddr wratchet_sockaddr;

void wratchet_display_error(const char* str);

int wratchet_init();

void wratchet_cleanup();

wratchet_socket wratchet_socket_create(const int domain,
                                       const int type,
                                       const int proto);

int wratchet_socket_destroy(wratchet_socket s);

int wratchet_socket_connect(wratchet_socket s,
                            const wratchet_sock_addr* addr,
                            int addrlen);

int wratchet_socket_bind(wratchet_socket s,
                         const wratchet_sockaddr* addr,
                         int addrlen);

wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       int* addrlen);

int wratchet_socket_send(wratchet_socket s,
                         const void* msg,
                         int msglen);

int wratchet_socket_sendto(wratchet_socket s,
                           const void* msg,
                           int msglen
                           const wratchet_sockaddr* addr,
                           int addrlen);

int wratchet_socket_recv(wratchet_socket s,
                         void* msg,
                         int msglen);

int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             int msglen
                             const wratchet_sockaddr* addr,
                             int addrlen);

int wratchet_socket_listen(wratchet_socket s,
                           int backlog);

int wratchet_socket_shutdown(wratchet_socket s,
                             int how);

#endif //WRATCHET_SOCKET_H

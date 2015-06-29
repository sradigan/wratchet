#ifndef WRATCHET_SOCKET_H
#define WRATCHET_SOCKET_H

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <netdb.h>
	#include <errno.h>
#endif

#ifdef _WIN32
	typedef SOCKET wratchet_socket;
#else
	typedef int wratchet_socket;
#endif

typedef struct sockaddr_in wratchet_sockaddr_in;
typedef struct sockaddr wratchet_sockaddr;

int wratchet_init();

void wratchet_cleanup();

wratchet_socket wratchet_socket_create(const int domain,
                                       const int type,
                                       const int proto);

int wratchet_socket_destroy(wratchet_socket s);

int wratchet_socket_connect(wratchet_socket s,
                            const wratchet_sockaddr* addr,
                            size_t addrlen);

int wratchet_socket_bind(wratchet_socket s,
                         const wratchet_sockaddr* addr,
                         size_t addrlen);

#ifdef _WIN32
wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       int* addrlen);
#else
wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       socklen_t* addrlen);
#endif

int wratchet_socket_send(wratchet_socket s,
                         const void* msg,
                         size_t msglen);

int wratchet_socket_sendto(wratchet_socket s,
                           const void* msg,
                           size_t msglen,
                           const wratchet_sockaddr* addr,
                           size_t addrlen);

int wratchet_socket_recv(wratchet_socket s,
                         void* msg,
                         size_t msglen);

#ifdef _WIN32
int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             size_t msglen,
                             wratchet_sockaddr* addr,
                             int* addrlen);
#else
int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             size_t msglen,
                             wratchet_sockaddr* addr,
                             socklen_t* addrlen);
#endif

int wratchet_socket_listen(wratchet_socket s,
                           int backlog);

int wratchet_socket_shutdown(wratchet_socket s,
                             int how);

#endif //WRATCHET_SOCKET_H

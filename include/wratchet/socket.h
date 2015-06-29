#ifndef WRATCHET_SOCKET_H
#define WRATCHET_SOCKET_H

#ifdef _WIN32
	//block warnings from system headers on windows
	#pragma warning(push, 0)
	#include <winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
	#pragma warning(pop)
#else
	#include <sys/socket.h>
	#include <netdb.h>
	#include <errno.h>
#endif

#ifdef _WIN32
	typedef SOCKET wratchet_socket;
	typedef int wratchet_addrlen_t;
	typedef int wratchet_msglen_t;
#else
	typedef int wratchet_socket;
	typedef socklen_t wratchet_addrlen_t;
	typedef size_t wratchet_msglen_t;
#endif

typedef struct sockaddr_in wratchet_sockaddr_in;
typedef struct sockaddr wratchet_sockaddr;

int wratchet_init(void);

void wratchet_cleanup(void);

wratchet_socket wratchet_socket_create(const int domain,
                                       const int type,
                                       const int proto);

int wratchet_socket_destroy(wratchet_socket s);

int wratchet_socket_connect(wratchet_socket s,
                            const wratchet_sockaddr* addr,
                            wratchet_addrlen_t addrlen);

int wratchet_socket_bind(wratchet_socket s,
                         const wratchet_sockaddr* addr,
                         wratchet_addrlen_t addrlen);

wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       wratchet_addrlen_t* addrlen);

int wratchet_socket_send(wratchet_socket s,
                         const void* msg,
                         wratchet_msglen_t msglen);

int wratchet_socket_sendto(wratchet_socket s,
                           const void* msg,
                           wratchet_msglen_t msglen,
                           const wratchet_sockaddr* addr,
                           wratchet_addrlen_t addrlen);

int wratchet_socket_recv(wratchet_socket s,
                         void* msg,
                         wratchet_msglen_t msglen);

int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             wratchet_msglen_t msglen,
                             wratchet_sockaddr* addr,
                             wratchet_addrlen_t* addrlen);

int wratchet_socket_listen(wratchet_socket s,
                           int backlog);

int wratchet_socket_shutdown(wratchet_socket s,
                             int how);

#endif //WRATCHET_SOCKET_H

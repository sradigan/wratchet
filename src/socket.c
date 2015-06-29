#include "wratchet/socket.h"

void wratchet_display_error(const char* str)
{
	#ifdef _WIN32
		LPSTR err_str = NULL;  // will be allocated and filled by FormatMessage
		int err = WSAGetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		              FORMAT_MESSAGE_FROM_SYSTEM,
		              0, err, 0, (LPSTR)&err_str, 0, 0);
		printf("%s.\nError Code: %d. Error String: %s\n",
		       str, err, err_str);
		LocalFree(err_str);
	#else
		int err = errno;
		printf("%s.\nError Code: %d. Error String: %s\n",
		       str, err, strerror(err));
	#endif
}

/// \brief Initialize wratchet
///
/// \detail Currently only required on Windows
int wratchet_init()
{
	#ifdef _WIN32
		WSADATA wsa;
		if (0 != WSAStartup(MAKEWORD(2,2),&wsa))
		{
			printf("Winsock Failed to initilize. Error Code : %d\n",
			       WSAGetLastError());
			return -1;
		}
	#endif

	return 0;
}

/// \brief Clean up wratchet
///
/// \detail Currently only required on Windows
void wratchet_cleanup()
{
	#ifdef _WIN32
		WSACleanup();
	#endif
}

/// \brief Create a new socket
///
/// \param domain The Protocol family of the socket.
///  Generally PF_INET or AF_INET
/// \param type
/// \return
wratchet_socket wratchet_socket_create(const int domain,
                                       const int type,
                                       const int proto)
{
	return socket(domain, type, proto);
}

/// \brief Destroy/Close the socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_destroy(wratchet_socket s)
{
	#ifdef _WIN32
		return closesocket(s);
	#else
		return close(s);
	#endif
}

/// \brief Connect a socket to a remote address
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_connect(wratchet_socket s,
                            const wratchet_sock_addr* addr,
                            int addrlen)
{
	return connect(s, addr, addrlen);
}

/// \brief Bind a socket to an address and port on the local machine
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_bind(wratchet_socket s,
                         const wratchet_sockaddr* addr,
                         int addrlen)
{
	return bind(s, addr, addrlen);
}

/// \brief Accept a connection from a socket
///
/// \param s Socket for operation
///
/// \return
wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       int* addrlen)
{
	return accept(s, addr, addrlen);
}

/// \brief Send data from a socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_send(wratchet_socket s,
                         const void* msg,
                         int msglen)
{
	return send(s, msg, msglen, 0);
}

/// \brief Send data from a DGRAM socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_sendto(wratchet_socket s,
                           const void* msg,
                           int msglen
                           const wratchet_sockaddr* addr,
                           int addrlen)
{
	return sendto(s, msg, msglen, addr, addrlen);
}

/// \brief Receive data from a socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_recv(wratchet_socket s,
                         void* msg,
                         int msglen)
{
	return recv(s, msg, msglen, 0);
}

/// \brief Receive data from a DGRAM socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             int msglen
                             const wratchet_sockaddr* addr,
                             int addrlen)
{
	return recvfrom(s, msg, msglen, addr, addrlen);
}

/// \brief Listen for connections on a socket
///
/// \param s Socket for operation
///
/// \return
int wratchet_socket_listen(wratchet_socket s,
                           int backlog)
{
	return listen(s, backlog);
}

/// \brief Shutdown communication on a socket
///
/// \param s Socket for operation
/// \param how What communication to stop. 0 = Receive, 1 = Send, 2 = Both.
///
/// \return
int wratchet_socket_shutdown(wratchet_socket s,
                             int how)
{
	return shutdown(s, how);
}
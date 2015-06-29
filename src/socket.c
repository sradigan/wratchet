#include "wratchet/socket.h"
#ifdef _WIN32
	//block warnings from system headers on windows
	#pragma warning(push, 0)
#endif

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
	//block warnings from system headers on windows
	#pragma warning(pop)
#endif

#ifndef _WIN32
	#include <unistd.h>
#endif

static void wratchet_display_error(const char* str)
{
	#ifdef _WIN32
		LPSTR err_str = NULL; // will be allocated and filled by FormatMessage
		int err = WSAGetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		              FORMAT_MESSAGE_FROM_SYSTEM,
		              0, err, 0, (LPSTR)&err_str, 0, 0);
		printf("Wratchet: %s.\nError Code: %d. Error String: %s\n",
		       str, err, err_str);
		LocalFree(err_str);
	#else
		int err = errno;
		printf("Wratchet: %s.\nError Code: %d. Error String: %s\n",
		       str, err, strerror(err));
	#endif
}

static void wratchet_display_message(const char* str)
{
	printf("Wratchet: %s.\n", str);
}

/// \brief Initialize wratchet
///
/// \detail Currently only required on Windows
///
/// \return Returns 0 on success and -1 on failure
int wratchet_init(void)
{
	#ifdef _WIN32
		WSADATA wsa;
		if (0 != WSAStartup(MAKEWORD(2,2),&wsa)) {
			wratchet_display_error("Winsock failed to initilize");
			return -1;
		}
	#endif

	return 0;
}

/// \brief Clean up wratchet
///
/// \detail Currently only required on Windows
void wratchet_cleanup(void)
{
	#ifdef _WIN32
		WSACleanup();
	#endif
}

/// \brief Create a new socket
///
/// \param domain The Protocol family of the socket.
///  Generally PF_INET or AF_INET
/// \param type Type of socket. Generally SOCK_STREAM or SOCK_DGRAM
/// \param proto Protocol to use.  Set to 0 for automatic selection.
///
/// \return Returns 0 on success, and non-zero on failure
wratchet_socket wratchet_socket_create(const int domain,
                                       const int type,
                                       const int proto)
{
	wratchet_socket ret = socket(domain, type, proto);
	#ifdef _WIN32
		if (INVALID_SOCKET != ret)
	#else
		if (0 != ret)
	#endif
	{
		wratchet_display_error("Error creating socket");
	}
	return ret;
}

/// \brief Destroy/Close the socket
///
/// \param s Socket for operation
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_destroy(wratchet_socket s)
{
	int ret = 0;
	#ifdef _WIN32
		ret = closesocket(s);
	#else
		ret = close(s);
	#endif

	if (0 != ret) {
		wratchet_display_error("Error destroying socket");
	}

	return ret;
}

/// \brief Connect a socket to a remote address
///
/// \param s Socket for operation
/// \param addr Address of the destination connecting to
/// \param addrlen Length of the type in addr.
/// Can set to sizeof(wratchet_sockaddr)
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_connect(wratchet_socket s,
                            const wratchet_sockaddr* addr,
                            wratchet_addrlen_t addrlen)
{
	int ret = connect(s, addr, addrlen);
	if (0 != ret) {
		wratchet_display_error("Error connecting to socket");
	}
	return ret;
}

/// \brief Bind a socket to an address and port on the local machine
///
/// \param s Socket for operation
/// \param addr Address on local machine to associate with
/// \param addrlen Length of the type in addr.
/// Can set to sizeof(wratchet_sockaddr)
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_bind(wratchet_socket s,
                         const wratchet_sockaddr* addr,
                         wratchet_addrlen_t addrlen)
{
	int ret = bind(s, addr, addrlen);
	if (0 != ret) {
		wratchet_display_error("Error binding to socket");
	}
	return ret;
}

/// \brief Accept a connection from a socket
///
/// \param s Socket for operation
/// \param addr Will be set to the address of the incoming connection.
/// \param addrlen Length of the type in addr.
/// Can set to sizeof(wratchet_sockaddr).  Will be filled with size of addr
/// (if smaller) upon return.
///
/// \return Returns 0 on success, and non-zero on failure
wratchet_socket wratchet_socket_accept(wratchet_socket s,
                                       wratchet_sockaddr* addr,
                                       wratchet_addrlen_t* addrlen)
{
	wratchet_socket ret = accept(s, addr, addrlen);
	#ifdef _WIN32
		if (INVALID_SOCKET != ret)
	#else
		if (0 != ret)
	#endif
	{
		wratchet_display_error("Error accepting connection from socket");
	}
	return ret;
}

/// \brief Send data from a socket
///
/// \param s Socket for operation
/// \param msg Pointer to data to send through the socket.
/// \param msglen Length of the data to be sent in bytes.
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_send(wratchet_socket s,
                         const void* msg,
                         wratchet_msglen_t msglen)
{
	int ret = send(s, msg, msglen, 0);
	if (0 != ret) {
		wratchet_display_error("Error sending from STREAM socket");
	}
	return ret;
}

/// \brief Send data from a DGRAM socket
///
/// \param s Socket for operation
/// \param msg Pointer to data to send through the socket.
/// \param msglen Length of the data to be sent in bytes.
/// \param addr Address to send the data to
/// \param addrlen Length of the type in addr.
/// Can set to sizeof(wratchet_sockaddr)
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_sendto(wratchet_socket s,
                           const void* msg,
                           wratchet_msglen_t msglen,
                           const wratchet_sockaddr* addr,
                           wratchet_addrlen_t addrlen)
{
	int ret = sendto(s, msg, msglen, 0, addr, addrlen);
	if (0 != ret) {
		wratchet_display_error("Error sending from DGRAM socket");
	}
	return ret;
}

/// \brief Receive data from a socket
///
/// \param s Socket for operation
/// \param msg Pointer to place data received through the socket.
/// \param msglen Maximum size of data msg can hold.
///
/// \return Returns 0, -1, or SOCKET_ERROR on failure.  Returns the length of
/// the message in msg otherwise.
int wratchet_socket_recv(wratchet_socket s,
                         void* msg,
                         wratchet_msglen_t msglen)
{
	int ret = recv(s, msg, msglen, 0);
	if (0 == ret) {
		wratchet_display_message("Remote has closed connection");
	}
	#ifdef _WIN32
	else if (SOCKET_ERROR == ret)
	#else
	else if (-1 == ret)
	#endif
	{
		wratchet_display_error("Error receiving from STREAM socket");
	}
	return ret;
}

/// \brief Receive data from a DGRAM socket
///
/// \param s Socket for operation
/// \param msg Pointer to place data received through the socket.
/// \param msglen Maximum size of data msg can hold.
/// \param addr Address the data was received from
/// \param addrlen Length of the type in addr.
/// Can set to sizeof(wratchet_sockaddr)
///
/// \return Returns 0, -1, or SOCKET_ERROR on failure.  Returns the length of
/// the message in msg otherwise.
int wratchet_socket_recvfrom(wratchet_socket s,
                             void* msg,
                             wratchet_msglen_t msglen,
                             wratchet_sockaddr* addr,
                             wratchet_addrlen_t* addrlen)
{
	int ret = recvfrom(s, msg, msglen, 0, addr, addrlen);
	if (0 == ret) {
		wratchet_display_message("Remote has closed connection");
	}
	#ifdef _WIN32
	else if (SOCKET_ERROR == ret) {
	#else
	else if (-1 == ret) {
	#endif
		wratchet_display_error("Error receiving from DGRAM socket");
	}
	return ret;
}

/// \brief Listen for connections on a socket
///
/// \param s Socket for operation
/// \param backlog Maximum number of outstanding connections.
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_listen(wratchet_socket s,
                           int backlog)
{
	int ret = listen(s, backlog);
	if (0 != ret) {
		wratchet_display_error("Error listening on socket");
	}
	return ret;
}

/// \brief Shutdown communication on a socket
///
/// \param s Socket for operation
/// \param how What communication to stop. 0 = Receive, 1 = Send, 2 = Both.
///
/// \return Returns 0 on success, and non-zero on failure
int wratchet_socket_shutdown(wratchet_socket s,
                             int how)
{
	int ret = shutdown(s, how);
	if (0 != ret) {
		wratchet_display_error("Error shutting down socket");
	}
	return ret;
}
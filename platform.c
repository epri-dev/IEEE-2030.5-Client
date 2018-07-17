// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <stdint.h>

/** @defgroup platform Platform
    @{
*/

/** @brief Initialize the platform layer.

    This function should be called before any other plaform layer function to
    ensure that the platform is properly initialized.
*/
void platform_init ();

/** @brief Set the time zone.

    Set the time zone portably for the localtime function, ISO C leaves the
    time zone handling implementation defined.
*/
void set_timezone (int tz_offset, int dst_offset,
		   time_t dst_start, time_t dst_end);

/** @defgroup event Event
    @{
 */

enum EventType {
  EVENT_NONE, ///< Indicates no event.
  SYSTEM_EVENT, //< A place holder for system events
  UDP_PORT=16, ///< A UdpPort with data to be read.
  TCP_PORT, ///< A TcpPort with data to be read.
  TCP_CONNECT, ///< A newly connected TcpPort
  TCP_ACCEPT, ///< A newly accepted TcpPort
  TCP_CLOSED, ///< A TcpPort that was closed.
  TCP_TIMEOUT, ///< A TcpPort that timed out
  TIMER_EVENT, ///< A timer that expired.
  POLL_TIMEOUT, ///< The event_poll function timed out waiting for an event.
  EVENT_NEW=32 ///< A place holder for higher level events.
};

/** @brief Poll an event from the Platform layer.

    The event_poll function blocks until an event is available or the the
    timeout expires.
    @param any receives the event object pointer.
    @param timeout is the polling timeout in milliseconds, or -1 to indicate
    an infitie timeout.
    @returns the @ref EventType and the associated object in the any parameter.
*/
int event_poll (void **any, int timeout);

/** @} */

/** @defgroup file File
    @{
*/

enum FileType {FILE_NONE, FILE_REGULAR, FILE_DIR};

/** @brief Read the entire contents of a file.
    
    The buffer returned needs to be freed with @ref free.
    @param name is the name of the file to read
    @param length is a pointer to the returned length, can be NULL
    @returns a buffer with the contents of the file, also the length of the
    file if the length parameter is not NULL 
*/
char *file_read (const char *name, int *length);

/** @brief Determine the file type given its name.
    @param name is the name of the file
    @returns the @ref FileType.
*/
int file_type (const char *name);

/** @brief Process a file line by line.    
    @param name is the name of the file.
    @param func is a function that takes a line number and a buffer parameter
    that represents the contents of the line.
*/
void process_file (const char *name, void (*func) (int n, char *));

/** @brief Process all the files within a directory
    @param name is the name of the directory
    @param ctx is a pointer to a user defined context
    @param func is a function that takes a file name parameter representing
    one of the files within a directory and an context.
*/
void process_dir (const char *name, void *ctx,
		  void (*func) (const char *, void *ctx));

/** @} */

/** @defgroup timer Timer 
    @{
*/

typedef struct _Timer Timer;
typedef struct _ClockTime ClockTime;

/** @brief Set the timeout for a Timer in seconds.
    @param timer is a pointer to the Timer
    @param timeout is the timeout in seconds
 */
void set_timer (Timer *timer, int timeout);

void set_timer_ct (Timer *timer, ClockTime *ct);

/** @brief Create a new timer.

    When the timer expires the event type returned is the value passed to this
    function. This way different timers can be distinguished.
    @param type is the type to be returned by event_poll, type should be a
    unique event type, it can be obtained by adding to the value of EVENT_NEW,
    e.g. add_timer (EVENT_NEW+1)
    @returns a new unarmed timer
*/
Timer *add_timer (int type);

/** @break Create a new timer, and set the timeout.
    
    This is the equivalent of calling set_timer (add_timer (type), timeout).
    @param type is the event type to be returned by event_poll
    @param timeout is the timeout in seconds
    @returns the new armed timer
*/
Timer *new_timer (int type, int timeout);

/** @} */

/** @defgroup network Network
    @{
 */

/** @defgroup address Address
    @{
*/

/** @brief The Address struture represents an IPv4 or IPv6 address with
    an associated port. */
typedef struct _Address Address;

/** @brief Allocate an Address.
    @returns a new Address
*/
Address *address_new ();

/** @brief Contruct an IPv4 Address given the address number and port.
    @param ipv4 is an IPv4 address in host format
    @param port is the port to associated with the IPv4 address
*/
Address *ipv4_address (Address *addr, uint32_t ipv4, int port);

/** @brief Contruct an IPv6 Address given the address number and port.
    @param ipv6 is a 16 byte (128 bit) IPv6 address
    @param port is the port to associated with the IPv6 address
*/
Address *ipv6_address (Address *addr, const char *ipv6, int port);

/** @} */

/** @defgroup interface Interface
    @{
*/

/** @brief Select the network interface. */
void net_select (int index);
int interface_index (char *name);
int interface_address (Address *host, char *name, int type);
void print_interfaces (int ipv4);

/** @} */

/** @defgroup tcp TcpPort
    @{
*/

/** @brief The status of a TcpPort */
enum TcpStatus {Closed, /**< TcpPort is closed */
		InProgress, /**< Connection initiated with @ref tcp_connect */
		Connected /**< TcpPort is connected */
};

/** @brief A TcpPort represents a TCP communication channel. */
typedef struct _TcpPort TcpPort;

TcpPort *new_tcp_port ();

/** @brief Return the Address of the connected TCP host.
    @param addr is a pointer to an Address
    @param port is a pointer to a TcpPort
    @returns the Address of connected TCP host.
*/
Address *net_remote (Address *addr, void *port);

/** @brief Return the local Address of the TcpPort.
    @param addr is a pointer to an Address
    @param port is a pointer to a TcpPort
    @returns the local Address of the TcpPort.
*/
Address *net_local (Address *addr, void *port);

/** @brief Return the status of the TcpPort.
    @param port is a pointer to a TcpPort
    @returns the status of the TcpPort (@ref TcpStatus)
*/
int net_status (void *port);

/** @brief An Acceptor represents a queue of clients waiting to
    establish a TCP connection on a specific address/port. */
typedef struct _Acceptor Acceptor;

/** @brief Listen for connection requests on a specifc address/port.
    @param address is the address/port on which to listen for connection
    requests
    @returns an Acceptor ready to accept connections
*/
Acceptor *net_listen (Address *address);

/** @brief Accept a connection request and assign to a TcpPort.

    If there are no active requests, queue the port to receive a connection
    at a later time.
    @param port is a pointer to a TcpPort
    @param a is pointer to an Acceptor
*/
void net_accept (void *port, Acceptor *a);

/** @brief Establish a TCP connection with a host.

    @param port is a pointer to a TcpPort
    @param server is a pointer to an host Address
*/
void net_connect (void *port, Address *server);

/** @brief Read data from a TcpPort.
    @param port is a pointer to a TcpPort
    @param buffer is a container for the data to be read
    @param length is the length of the buffer
    @returns the length of the data read from the TcpPort or -1 to indicate
    that no data is available.
*/
int net_read (void *port, char *buffer, int length);

/** @brief Write data to a TcpPort.
    @param port is a pointer to a TcpPort
    @param buffer is a container for the data to write
    @param length is the length of the data to write
*/
int net_write (void *port, const char *buffer, int length);

/** @brief Close a TCP connection.
    @param port is a pointer to a TcpPort
*/
void net_close (void *port);

/** @} */

/** @defgroup udp UdpPort
    @{
*/

/** @brief UdpPort represent a port on which to send and receive UDP datagrams.
 */
typedef struct _UdpPort UdpPort;

/** @brief Receive a UDP datagram from a host on a UdpPort.
    @param p is a pointer to a UDP port.
    @param length is updated to indicate the length of the datagram.
    @returns a pointer to a buffer containing the datagram and the length of the
    datagram or NULL if no datagram is available 
*/
char *net_receive (UdpPort *p, int *length);

/** @brief Send a UDP datagram to a host Address from a UdpPort. 
    @param p is a pointer to a UdpPort
    @param data is a pointer to a buffer containing the datagram
    @param length is the length of the datagram
    @param address is a pointer to the Address of the host to receive the
    datagram
    @returns the length of the datagram sent or -1 if 
*/
int net_send (UdpPort *p, char *data, int length, Address *address);

/** @brief Allocate a new UdpPort.
    @param size is the size of the buffer to receive datagrams
    @returns a pointer to a UdpPort with buffer of the requested size
*/
UdpPort *new_udp_port (int size);

/** @brief Open a UdpPort.
    @param p is a pointer to a UdpPort
    @param address is a pointer to an Address at which to receive datagrams 
*/
void net_open (UdpPort *p, Address *address);

/** @brief Join a multicast group (IPv6).
    @param p is a pointer to a UdpPort
    @param addr is an IPv6 multicast address
    @param loop selects whether to receive our own multicasts
*/
void net_join_group (UdpPort *p, const char *addr, int loop);

/** @} */

/** @} */

#if defined _WIN32 || defined _WIN64

#include "win32/platform.c"

#else

#include "linux/platform.c"

#endif


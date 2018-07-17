Platform
========

The EPRI 2030.5 client library defines a portable platform API (`platform.c`)
to perform platform dependent functions such as networking, file IO, timers,
and event handling. In C these functions are not standard with each OS or
system defining its own interface.

The platform API is based upon simple event model which is also compatible with
the most efficient means processing OS events in systems like Linux (`epoll`)
and Windows (`IOCP`). In this model the application polls the system for events
defined upon objects such as UdpPorts, TcpPorts, and Timers. The platform layer
returns both the object and the event code in one function call (`event_poll`).
The application can then perform operations on the object such as reading or
writing for TcpPorts. These operations are non-blocking so that a single
threaded application can process events upon multiple objects in a timely
manner within a single event loop.

Examples of events are timers expiring, UdpPorts receiving packets, TcpPorts
establishing a connection, receiving data, or being closed.

Event Polling
-------------

Events are recevied by the application with the `event_poll` function:

    int event_poll (void **any, int timeout);

`event_poll` returns an integer event code and updates a pointer to point to
the object associated with the event (if any).

The base set of events that must be supported by a system are defined as
follows (platform.c):

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

The event `SYSTEM_EVENT` (event code 1) is a place holder for platform
dependent events, up to 15 platform dependent events can be defined
(1 though 15). These events may be returned by `event_poll` or they might only
be used internally to help implement the platform layer.

The event `EVENT_NEW` (event code 32) is a place holder for higher level events.
Higher level events can be defined by creating a function to process events
returned `event_poll`. An example of defining a higher level event is the
`SERVICE_FOUND` event returned by `client_poll`. The `client_poll` function
handles the packet level processing of service discovery only notifying the
application of the higher level event when a service is newly discovered.

The application programmer is free to define new events, the only caution is
make sure the event code is unique (a unique offset from `EVENT_NEW`) so that
it does not clash with an existing platform event or client library event.

Porting
-------

The only system supported at the time of release is Linux, however the Linux
port can be used as a guide for porting to other systems. The implementation
of the Linux platform layer can be found in the directory `linux`.

Using the Linux port as a guide, some tasks that need to be completed are:

- define the Address type and related operations (bsd.c)
- define the TcpPort type and related operations (linux/tcp.c)
- define the UdpPort type and related operations (linux/udp.c)
- define the Timer type and related operations (linux/timer.c)
- define platform dependent file operations (linux/file.c)
- define `set_timezone` to for correct localtime (linux/time.c)
- define `event_poll` in terms of event model described above (linux/event.c)
- define functions to query the network interfaces (linux/interface.c)
- define `platform_init` to initialize the plaform layer (linux/platform.c)
- define any supporting functions and data structures as needed

For the Linux port it was useful to define a `PollEvent` type as a base type for
TcpPort, UdpPort, and Timer. The `PollEvent` type includes a link so the events
can be placed in a queue and also includes storage for the event code, file
descriptor, and other common attributes.

The Linux port includes a queue because of the way events are reported by
`epoll`. In edge triggered mode an event that indicates that a TCP socket has
data is only reported once by `epoll`. The application is then expected to read
all the data from the socket before `epoll` will return a new event. In such
cases where the application does not read all the data, these objects are
placed in queue by `event_poll` so that a new `TCP_PORT` event is returned by
`event_poll` even though no new events may be returned by `epoll`.

Because of the peculiarities of system interfaces the techniques used for the
Linux port may or may not apply in porting to other systems. For example, in
Windows the IOCP (IO Completion Port) event model means that events are
reported by the OS only when operations complete such as reading data from TCP
socket. To implement the platform layer (and event model) means that operations
such as reading must be buffered by performing the Windows API call before the
data is actually requested by the application.  


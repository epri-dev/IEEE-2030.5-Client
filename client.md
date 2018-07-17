Client API
==========

The IEEE 2030.5 client API provides a means for developers to integrate IEEE
2030.5 functionality into their applications. At the base is a portable
platform layer that includes support for files, TCP connections, UDP ports,
timers, and events. The event polling mechanism provides a portable and unified
way of processing operating system events that is both responsive
(non-blocking) and balanced (includes round robin scheduling).

After platform initialization and interface selection are performed, the client
will typically perform the following:

-   DNS service discovery (DNS-SD)
-   TCP/TLS connection establishment
-   %Resource retrieval
-   Event scheduling

These and other functions are divided up into the following modules:

Platform
--------

The platform layer provides portable functions for performing operating system
I/O. The platform layer consists of the following modules:

-   @ref platform
  *  @ref event
  *  @ref file
  *  @ref timer
  *  @ref network

Applications must call @ref platform_init before any other I/O based function
call to properly initialize the platform layer.

### Events

Applications call @ref event_poll to retrieve events from the platform layer.
The set of events that can be retrieved from the platform layer are fixed,
relating to network operations, expiring timers, and polling timeouts. This
base set of events can be extended by adding timers and defining higher level
event retrieval functions. For example @ref client_poll processes UDP level
packets from the service discovery port and returns a `SERVICE_FOUND` event to
report a new service to the client.

For the purpose of defining new events the plaform layer includes the constant
`EVENT_NEW`. Higher level event retrieval functions can extend the set of events
by returning unique offsets from `EVENT_NEW`.

### File I/O

Developers can use the standard C functions to perform file I/O portably,
however there are no standard functions that can determine the file type or
process directories. For these purposes the platform layer provides
@ref file_type to determine whether a file name represents a regular file or a
directory, and also @ref process_dir to process the files within a directory.

### Timers

Timers are another platform dependent feature. The platform layer provides
@ref add_timer and @ref new_timer to create timers. The value passed to these
functions are returned by @ref event_poll at the time indicated by
@ref set_timer. They should be defined as a unique offset from `EVENT_NEW` in
order to distinguish timer events from other event types.

### Networking

The network layer provides a base for the IEEE 2030.5 functionality. Service
discovery is performed by exchanging UDP packets, the HTTP RESTful operations
that form the core of the IEEE 2030.5 protocol are performed on top of a TCP
connection. In addition to providing this base, the network layer gives the
developer a means of performing network operations portably and efficiently.

Security/TLS Initialization
---------------------------

The security layer provides the means for loading device and root certifcates,
initializing the TLS library, and computing the device credentials (LFDI and
SFDI).

-   @ref security

Schema
------

IEEE 2030.5 resources are defined by an XML schema (sep.xsd). This schema is
processed to extract C type definitions (se_types.h) and a table (se_schema.c)
which can be used to transform 2030.5 resources from their XML or EXI
representation into a C object representation. The process can also be
reversed, transforming a C object into an XML or EXI representation. The
@ref schema module provides basic types for representing schemas in table form,
and methods for querying the schema table and performing operations on schema
based objects.

-   @ref schema
-   @ref se_object
-   @ref se_types

Parsing / Output
----------------

The @ref parse and @ref output modules provide the means to transform a
resource from its XML or EXI representation into its C object representation
and vice versa. These modules are general purpose however support is currently
limited to the features required by the IEEE 2030.5 XML schema.

-   @ref parse
-   @ref output

Connections
-----------

The client API provides three types of connections that build upon each other
to provide higher level functionality.

The @ref connection module extends the platform `TcpPort` to provide either a
TCP or a TCP+TLS connection with a server.  The @ref http_connection module
extends the @ref connection module to provide support for HTTP client and
server connections. Finally, the @ref se_connection module extends the
@ref http_connection module to provide support for IEEE 2030.5 media types,
"application/sep+xml" and "application/sep-exi".

-   @ref connection
-   @ref http_connection
-   @ref se_connection

Service Discovery
-----------------

The @ref dnssd_client module provide functions for DNS service discovery
(DNS-SD). The @ref se_discover module extends the @ref dnssd_client module to
provide support for IEEE 2030.5 subtype queries and connecting with IEEE 2030.5
service providers.

-   @ref dnssd_client
-   @ref se_discover

Event Queuing
-------------

The @ref event_queue module provides a means to add time based events without
the need to create a new system timer for each type of event. The
@ref event_queue uses only a single timer which set to expire for the time of
the next event.

-   @ref event_queue

Retrieval
---------

The @ref retrieve module provides high level functions for IEEE 2030.5 resource
retrieval, including support for creating placeholders called @ref Stub
resources, tracking requirements and dependencies, and signaling retrieval
completion.

-   @ref retrieve

Scheduling
----------
For event based function sets such as Distributed Energy Resources (DER) the
@ref schedule module provides functions and data structures to organize the
events into schedules (@ref Schedule) corresponding to particular EndDevices.
Scheduling is done first when retrieval is complete for a given function set
and is subsequently updated as events become active, are completed, superseded,
or canceled. These events are recorded internally as the @ref event_queue
events `EVENT_START` and `EVENT_END` so that the client application can respond
appropriately to them.

-   @ref schedule

DER Client
----------

The @ref der module organizes the resources of a DER EndDevice providing a
method of scheduling DERControls based upon the FunctionSetAssignments for an
EndDevice.

-   @ref der

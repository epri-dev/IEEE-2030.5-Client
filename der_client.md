Writing a 2030.5 DER Client Application
=======================================

A 2030.5 DER client application can manage a single inverter or manage a group of
inverters in the case of an aggregator, both types of clients are supported by
the EPRI 2030.5 library. This document gives general guidiance in using the
client library not considering the whether the client is an aggregator or
represents a single inverter. For the aggregator case there may additional
functions performed such as managing the server's EndDeviceList or performing
other managment functions on behalf of the managed inverters.

DER Client Model
----------------

In writing a DER client it's helpful to understand the overall process of the
client's interaction with a 2030.5 server, even though many of these steps are
done on behalf of the application by the client library. This process can be
broken down into a series of sub-tasks:

### 1. Using service discovery (DNS-SD) to discover a 2030.5 server or using an already known address and port number.

While DNS service discovery (DNS-SD) is possible over the internet, using a
known IP address and port number is expected to be a more common method of
configuration in connection to utilities over the internet.

### 2. Connecting to the server using TCP/TLS.

In order to be authenticated by a server the client needs to provide a
certificate chain linking to a known CA (Certificate Authority), some servers
may also accept self-signed certificates. Certificate exchange between the
client and server also establishes the parameters for secure communication via
the TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8 cipher suite. 

### 3. Retrieval of the DeviceCapability and EndDeviceList resources.

The DeviceCapability provides an index to the function sets available on a
server. For DER servers this index should include a link to an EndDeviceList
resource. The server can populate the EndDeviceList based upon an out-of-band
registration process, some servers may also support in-band registration where
the client can POST new EndDevice instances to a server. For aggregators the
EndDevice list should contain not only an EndDevice instance corresponding to the
aggregator, but also a set of EndDevice instances for each inverter that the
aggregator manages.

### 4. Validating EndDevice registration, POSTing an EndDevice instance if necessary.

According to the 2030.5 standard the registration procedure is completed by the
client retrieving its own EndDevice instance and verifying that the Registration
resource linked from the EndDevice contains a matching PIN number. In some cases
the client may be registered with a server but the server's EndDeviceList does
contain the corresponding EndDevice instance. In such cases the client is
required to POST its EndDevice instance to the server.

### 5. Performing retrieval of subordinate resources.

Information on 2030.5 servers is represented as a set of linked resources with
the DeviceCapability being the root resource, in the general case the resources
together with the links may form a directed acyclic graph (DAG). Resource
retrieval generally begins with the root resource (DeviceCapability) and
proceeds to the subordinate or linked resources using the HTTP GET method to
perform the retrieval. If service discovery is used then client can also begin
with the root resource for a particular function set (the "path" variable
contained in a DNS-SD response)

Clients may not require every resource provided by a 2030.5 server. Every
resource that is requested takes bandwidth, storage, time, and energy from
the client's host system. The EPRI client library therefore defines a retrieval
module that allow a client to customize the retrieval process. For each
resource the client can determine exactly what subordinate resources are
requested.

### 6. Assigning a completion routine

Retrieval is a asynchronous process which may involve many exchanges with a
server (request/response pairs). One feature of the retrieval module is to track
the dependencies between resources and the retrieval status of individual
resources and their subordinates. When a resource and all of its subordinates
have been completely retrieved, the retrieval module can optional call a
completion routine.

Completion routines can be assigned to any resource and can be used for any
purpose. One use of the completion routine is to signal that retrieval for an
EndDevice and its subordinate resources is complete and therefore that it can
perform local event scheduling.

### 7. Local event scheduling.

According to the Common Smart Inverter Profile (CSIP) inverters can belong to one
or more groups. CSIP defines groups according to a 7-layer topological model,
with each layer or group defining a set DER Programs and DER Controls (events).
In addition to having a set of controls, each DER Program has a primacy, and may
have a default control that is applied when there are no active controls. The
primacy of a DER program determines the priority of events when there are
overlapping DER controls.

For each inverter, given the group assignments, the set of DER Programs and DER
Controls the client must create a local event schedule taking into account that
there may be overlapping events, and that these instances are resolved by the
associated primacy of the event along with the general rules involving events
(IEEE 2030.5, section 12.1.3). If the server indicates that the client must
respond to events, then the client must also send response messages to indicate
that for example the event was received, that the event has started, completed,
etc.

The function of local scheduling is to create a timed sequence of events that can
interpreted locally and also to generate event responses sent to the server at
the appropriate times, taking into account event randomization (if any). The EPRI
client scheduler takes the information described above and generates a sequence
of `EVENT_START`, and `EVENT_END` event pairs to indicate that a DER Control has
become active and then inactive. An event becomes active at the beginning of the
effective start period for that event, an event can become inactive due to local
scheduling (superseded by an overlapping event), the event being canceled, or the
effective time period for an event expiring.

### 8.   Interpreting events.

When a DER Control becomes active the client can then decide how to respond to
the event. Applying a DER Control can mean sending a series of MODBUS commands
to an inverter. The client may also decide to opt-out of an event and if so must
send an appropriate response message to the server.

### 9.   Model maintenance.

Over time the server may change group assignments, add or remove DER Programs,
cancel events or replace an event with a superseding event. In order for the
local event scheduling to be synchronized with a server, the client needs to
periodically poll (retrieve) the resources involved in scheduling. 2030.5
provides another mechanism called subscription/notification whereby the server
connects to the client in order to push updates, this mechanism is not yet
supported by the EPRI client.

The EPRI client library can seamlessly handle to changes to the model, including
resource and event removal, event cancellation, superseding events, adding or
removing DER programs and changing the associated primacy of events. All that
needs to be done on behalf of the application is to setup an appropriate polling
rate for resources, and selecting which resources to poll. The polling rate can
be based upon the `pollRate` element present in some resources or some other
suitable interval. The application can also request updates in a custom way if
a fixed polling interval is not appropriate.

When an update is requested either by polling or manually updating the request
will temporarily invalidate the completion property of the resource and all the
resources that depend upon the resource. When the completion property is
reestablished (by successful retrieval) the retrieval module will again
signal completion for those resources with an associated completion routine.

This point should be kept in mind if scheduling or some other operation is
performed as a result of completion. A strategy must devised to either group
together updates (to control completion signaling) or to perform scheduling
or other operations on a separate interval so that they don't depend on such
signaling. Technically scheduling only needs to performed when there is an
update to the model, so it makes sense to link scheduling to the completion
property.

DER Client reference implementation (`der_client.c`)
---------------------------------------------------

The EPRI 2030.5 client library provides a reference implementation for DER
client applications (`der_client.c`) that:

-   assembles the required client library modules

-   provides an event polling function `der_poll` that handles events such as
    schedule updates, and resource polling updates.
    
-   provides functions for displaying the contents of events and the event
    schedules on `stdout`.

`der_client.c` can be used as is or modified to suit the needs of an application.

Included with `der_client.c` is the module `der.c` that provides:

-   a DerDevice structure that stores information associated with a DER EndDevice
    instance.
    
-   a hash table container for DerDevices that uses the EndDevice SFDI as a
    retrieval key
    
-   a function `scheduler_der` that creates a local event schedule for an
    EndDevice based upon the group assignments for that EndDevice.

DER Client Examples
-------------------

Two example applications are included with EPRI client library, `csip_test.c` and
`client_test.c`. Both applications use `der_client.c` as a basis, the EPRI DER
client reference implementation. Of the two, `client_test.c` is far more
complete in terms of demonstrating the EPRI client library API, it is also more
complex due to its usage as a tool for demonstrating compliance with the SunSpec
test procedure. The command line arguments of `client_test.c` provide a way to
test different configurations and demonstrate various test procedures that are
useful for certification purposes, but might not be needed for typical DER
applications.

The application `csip_test.c` provides a better starting point for a DER
client applications because, with the exception of registration, it performs
minimally all the functions described in the DER Client Model section. The
function `main` shows the basic structure for a client application:

    int main (int argc, char **argv) {
      void *any; int index; Service *s;
      // platform initialization
      platform_init ();
      // process command line arguments
      if (argc < 2) {
        print_interfaces (0); exit (0);
      }
      if ((index = interface_index (argv[1])) < 0) {
        printf ("interface %s not found\n", argv[1]); exit (0); 
      }
      // load certificates and initialize client
      client_init (argv[1], "pti_dev.x509"); der_init ();
      load_cert_dir ("certs");
      // perform service discovery
      discover_device ();
      while (1) {
        // process events
        switch (der_poll (&any, -1)) {
        case SERVICE_FOUND: s = any;
          print_service (s); get_dcap (s, 1); break;
        case TCP_PORT:
          if (conn_session (any))
	    process_http (any, csip_dep); break;
        case DEVICE_SCHEDULE:
          print_event_schedule (any); break;
        case EVENT_START:
          print_event_start (any); break;
        case EVENT_END:
          print_event_end (any); break;
        }
      }
    }

The `main` function can be abbreviated as:

    int main (int argc, char **argv) {
      // declare local variables ...
      // platform initialization
      platform_init ();
      // process command line arguments
      ...
      // load certificates and initialize client
      ...
      // perform service discovery
      discover_device ();
      while (1) {
        // process events
        ...
      }
    }

The steps of the application can be described as follows:

1.   Platform initialization must occur first since other features of the client
library may depend on platform initialization, this is accomplished by the call
to `platform_init`.

2.   The client can then process any command line arguments, for `csip_test` the
only argument is name of the network interface for which the client should
use to perfom service discovery. If none is provided the application will print
a list of interfaces.

3.   If the network device is valid then load the device certificate, the CA
certificates, and initialize the client library. These certificate will
be used when the client attempts a secure connection to a server.

4.   Call `discover_device` to perform DNS service discovery (DNS-SD), this will
send a MDNS packet on the local network with a subtype query to discover a
server that contains an EndDevice instance with an SFDI that matches the client's
SFDI (computed from the device certificate). Since only servers that contain
such an instance will respond the registration step is omitted. A real world
application will want to retrieve the EndDevice instance along with the
Registration resource to verify the PIN for the device. Other subtype queries
are possible (see the function `se_discover` (`se_discover.c`)), alternatively
the application may want to connect directly to a server with a known
address/port number.

5.   Process events. Events may include services being discovered
(`SERVICE_FOUND`), receiving data from a TCP connection (`TCP_PORT`), DER
Controls becoming active and inactive (`EVENT_START`, `EVENT_END`), also
application defined events (timers expiring, ect).

Resource retrieval example
--------------------------

Besides providing a model for structuring client applications, `csip_test.c` also
shows how the retrieval process can be customized. In the event processing of
`TCP_PORT` data, the client performs:

          if (conn_session (any))
            process_http (any, csip_dep); break;

`conn_session` will return True (non-zero) when a TCP/TLS session has been
established. For a TLS session multiple calls may required as several messages
are exchanged as part the TLS handshake.

If the session has been established, the application calls `process_http` to
handle responses from a 2030.5 server. The function `process_http` takes as
arguments an `SeConnection` (provided by the `TCP_PORT` event) and also a
dependency function which is called on every successfully retrieved resource.
`csip_test.c` defines the dependency function as follows:

    void csip_dep (Stub *r) {
      switch (resource_type (r)) {
      case SE_Time: set_time (resource_data (r)); break;
      case SE_DERProgram: der_program (r); break;
      case SE_DeviceCapability: dcap (r); break;
      case SE_EndDevice: edev (r); break;
      case SE_FunctionSetAssignments: fsa (r);
      }
    }

`csip_dep` takes a pointer to a `Stub` (the local representation of a resource)
and calls an appropriate function based on type of resource. When type is
`SE_DeviceCapability`, `csip_dep` calls the function `dcap`:

    void dcap (Stub *r) {
      SE_DeviceCapability_t *dcap = resource_data (r);
      if (!se_exists (dcap, EndDeviceListLink)) return;
      get_root (r->conn, dcap, Time);
      get_list_root (r->conn, dcap, EndDeviceList);
      get_list_root (r->conn, dcap, MirrorUsagePointList);
    }

The `dcap` function demonstrates several key features of the EPRI library:

-   `SE_DeviceCapability_t` is a C struct type that contains all the possible
    elements of the `DeviceCapability` document as defined by 2030.5 XML schema.
    XML/EXI documents are automatically parsed and converted into their C
    object form by the function `se_recieve` called by `process_http`.
    
-   `resource_data` is a macro to access the C object representation of a
    resource with the corresponding type (in this case `SE_DeviceCapability_t`).

-   `se_exists` is a macro to determine the presence of optional elements within
    an object/document. `se_exists` works by checking the presence of flag bits
    within the C object (see `se_types.h` for a listing of possible flags for
    each document type).

-   The line `if (!se_exists (dcap, EndDeviceListLink)) return;` says that if
    the `DeviceCapability` resource does not contain an `EndDeviceListLink`
    element then return (no further processing).

-   `get_root (r->conn, dcap, Time);` will perform a GET request for the `Time`
    subordinate resource if the `TimeLink` exists. `get_root` is actually a
    macro that calls `get_resource` the primary retrieval function. Besides
    performing the GET request, `get_resource` will also create a placeholder
    (a `Stub` object) for when resource is successfully retrieved.
    
-   The line `get_list_root (r->conn, dcap, EndDeviceList);` will perform a GET
    request for the `EndDeviceList` subordinate resource if the
    `EndDeviceListLink` element exists. List resources require a different macro,
    because the `ListLink` elements contain the `all` attribute indicating the
    number of items contained in the list. The `get_list_root` macro also
    produces a call to `get_resource` with different arguments from `get_root`.

The `dcap` function performs retrieval but does create any dependencies between
the `DeviceCapability` resource and its subordinates, in this example there is no
need to signal completion for the `DeviceCapability` resource. Other functions in
`csip_test.c` do create dependencies such as the `der_program` function:

    void der_program (Stub *d) {
      SE_DERProgram_t *dp = resource_data (d);
      get_dep (d, dp, DefaultDERControl);
      get_list_dep (d, dp, DERControlList);
      get_list_dep (d, dp, DERCurveList);
   }

The `der_program` function retrieves the `DefaultDERControl`, `DERControlList`,
and `DERCurveList` resources if the `DERProgram` contains links to those
resources by calling `get_dep` and `get_list_dep`. The macros `get_dep` and
`get_list_dep` also create dependencies between the `Stub` representing the
`DERProgram` and the newly created `Stubs` representing the subordinate
resources.

The function `edev` gives an example of assigning a completion routine and
setting up a polling interval for the `FunctionSetAssignmentsList` subordinate
resource:

    void edev (Stub *d) { Stub *r;
      SE_EndDevice_t *edev = resource_data (d);
      d->completion = schedule_der;
      get_list_dep (d, edev, DERList);
      if (r = get_list_dep (d, edev, FunctionSetAssignmentsList)) {
        r->poll_rate = 10; poll_resource (r);
      }
    }

In this case the completion routine is `schedule_der` which has same signature
as the `completion` field of the `Stub` type:

    typedef struct _Stub {
      ...
      void (*completion) (struct _Stub *);
      ...
     } Stub;

    void schedule_der (Stub *edev);

The following lines perform retrieval for the `FunctionSetAssignmentsList`
subordinate resource and the set up polling for the resource:

      if (r = get_list_dep (d, edev, FunctionSetAssignmentsList)) {
        r->poll_rate = 10; poll_resource (r);
      }

The polling rate is set to 10 seconds, the function `poll_resource` creates a
timer based event to signal to the client to poll the resource after the poll
interval has expired.

Conclusion
----------

`csip_test.c` and `client_test.c` provide two example client applications that
show how to implement a DER client according to the model presented here. While
only the `csip_test.c` example was covered in detail here, the `client_test.c`
application can provide a useful reference to performing a other functions
required of a DER client such a Metering, updating the DER status, and posting
alarm messages.






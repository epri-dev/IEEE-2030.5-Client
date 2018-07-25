EPRI 2030.5 Client {#mainpage}
==================

The EPRI 2030.5 Client is a C library and application framework for creating
IEEE 2030.5 compliant applications. The framework is lightweight, portable, and
fast (based on state machines and asynchronous events).

Features
--------

-   portable networking layer (UDP, TCP, IPv6/IPv4)
-   support for the Linux platform
-   TLS 1.2 currently supported through the OpenSSL library
-   DNS-SD client for IEEE 2030.5 service discovery
-   HTTP 1.1 client/server
-   XML/EXI schema based parser/serializer
-   IEEE 2030.5 client API
-   IEEE 2030.5 client examples (DER function set)
-   small modular source code

Dependencies
------------

-   GCC compiler version 4.6 or greater
-   OpenSSL version 1.1.0 or greater
-   GNU Bash -- for building

Building
--------

To build the framework library and applications run:

    ./build.sh

By default the build script uses the compiler `gcc`. To compile for a specific
host compiler you can edit the variable `linux_host` in `build.sh`, this
variable describes the system target. If you are unsure of the target, run:

    gcc -v|&grep -e Target

You can also change the prefix used to search for the C target libraries, by
default it is `/usr`, this prefix concatenated the gcc sysroot (the value
produced by `gcc -sysroot`)

With no arguments given `build.sh` will build the library and applications
using `-Os` (optimized for size), however debug builds and cross compiled are
also possible. Running `build.sh` with the `debug` option will pass the
argument `-g` to gcc:

    ./build.sh debug

To build cross compiled applications edit the variables `linux_cross_host`
and `linux_cross_prefix` appropriately and run:

    ./build.sh cross

Version History
---------------

### Version 0.2.11

Changes:

-   Change build.sh to use `gcc` by default.
-   Added the `inverter` command to `client_test` to perform tests in
    "inverter" mode.

### Version 0.2.10

Changes:

-   Added `pin` command to PUT the PIN for in-bound registration.
-   Added `delete` command to delete a managed EndDevice instance.

### Version 0.2.9

Changes:

-   Added documentation on the platform event model and porting.

Fixes:

-   Fix crash involving paged retrieval of list resources.
-   Fix command line subtype identification for client_test application.

### Version 0.2.8

Changes:

-   Device Certificate can be in PEM or ASN1 format, if the format is ASN the
    client loads the private key from a correspondingly name .pem file,
    otherwise us the same PEM file to load the private key.

-   Added a separate HashPointer structure to iterate over a hash table so that
    the hash table can be searched while iteration is taking place.

### Version 0.2.6/0.2.7

Changes:

-   Stack allocation for parsing URIs instead of static allocation.
-   Add LFDI to EndDevice for the DERControlResponse when computed from a
    certificate, but not provided by the server.
-   Print IEEE 2030.5 objects sent in POST/PUT requests.

Fixes:

-   Minor fixes to HTTP code (functions converted to macros, functions made
    static, clarify parsing logic).
-   Bug fixes for retrieval and scheduler algorithms.

### Version 0.2.5

Changes:

-   Add the HTTP status to the Stub, can indicate an update in progress as well
    as the status returned for a successful request.
-   Print the HTTP headers enabled with `http_debug`.
-   Add `cleanup_http` to remove the Stubs of requests that have failed or
    timed out.
-   Print DefaultDERControl when there is no active DERControl.

### Version 0.2.4

Changes:

-   Add `subquery/path` syntax for command line resource retrieval.
-   Minor changes to the platform API.
-   Minor updates to the retrieval/scheduler algorithms.

Fixes:

-   Fix compiler warnings, add check for OpenSSL 1.1.0.
-   Add Time synchronization for event based tests.
-   Make xmDNS multicast address the source address when no site local address
    exists for the client.
-   Add polling for DERProgramList and DERControlList.
-   Cap list retrieval at 255 items (l=255)

### Version 0.2.3

Changes:

-   Event scheduler rewrite, support for program removal and addition, primacy
    changes and overlapping DER controls.
-   added commands `edev`, `time`, `self`, `metering` `meter`, `alarm`, `device`


### Version 0.2.2

Changes:

-   New event scheduler, only schedule one future event per EndDevice.

Fixes:

-   Divide by zero exception when randomization bound is 0.
-   remove "level=-S1" from Content-Type in PUT/POST

### Version 0.2.1

Changes:

-   Added `sfdi SFDI` command to specify the SFDI of the client
    EndDevice when no certificate is used.

Fixes:

-   Safely ignore responses with resources that do not contain an href.
-   Fixes to the EXI parsing and output based upon more
    extensive testing.

### Version 0.2.0

Changes:

-   Added responses for IEEE 2030.5 events
-   Added polling interval for active events
-   Added polling for event status changes from scheduled to active
-   Added support to load device settings from a file and PUT to the
    server
-   Added device registration sequence
-   Added `primary` and `all` commands and associated
    retrieval patterns.
-   Added user manual and version information

### Version 0.1.0

Initial version

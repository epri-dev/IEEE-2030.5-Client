Client Test Application
=======================

The purpose of the client_test application is demonstrate usage of the EPRI
2030.5 client API and demonstrate compliance with the SunSpec test procedure.

Usage
-----

The `client_test` application is a command line tool with the following
arguments:

    client_test interface [device_cert ca_certs..]
                           <subtype[:1][/path] | URI> [commands]

The `interface` argument is the the name of the network interface,
running the `client_test` application with no arguments will list the
available network interfaces.

The `device_cert` argument names the device certificate file. If the
certificate ends in `.x509` then the `client_test` application looks for the
corresponding private key in similarly named file with the `.pem` extension. If
certificate end in `.pem`, then both the certificate and private key are loaded
from the same file.

The `ca_certs` argument is a list of CA certificates or certificate
directories to be loaded. If a directory is specified, then all the
certificates within the directory are loaded. If no certificates or
directories then the certificates from `./certs` in the local directory
are loaded.

The `<subtype[:1][/path] | URI>` argument delimits the list of certificates and
specifies either to perform xmDNS service discovery, or to retrieve the
document at the specified URI.

Subtype DNS-SD queries
----------------------

The `subtype` can be one of:

-   `bill` - to discover Billing servers
-   `dr` - to discover Demand Response / Load Control servers
-   `derp` - to discover Distributed Energy Resources servers
-   `file` - to discover File Download servers
-   `msg` - to discover Messaging servers
-   `ppy` - to discover Prepayment servers
-   `rsps` - to discover Response servers
-   `tp` - to discover Pricing servers
-   `tm` - to discover Time servers
-   `upt` - to discover Metering servers
-   `edev` - to discover End Device servers
-   `mup` - to discover Meter Mirroring servers
-   `sdev` - to discover Self Device servers
-   `smartenergy` - to discover IEEE 2030.5 servers of any type

When the number `1` is affixed to the `subtype`, the `client_test` sets
the `QU` bit to `1` in the service discovery request indicating that a
unicast response is desired.

Since the `subtype` argument can delimit the list of certificates it
follows that there can not be a certificate or certificate directory
with the same name as the `subtype` argument.

After the service discovery is performed the `client_test` application
will retrieve the associated resource for the subtype query or the
DeviceCapability resource for a general query. Optionally a path can be
specified on the command line. When specified, the client will retrieve the
resource identified by the path from the server location returned by the DNS-SD
response.

URI retrieval
-------------

If a `subtype` is not specified in the list of arguments, the
`client_test` application attempts to parse a URI. The first argument
that matches an absolute form URI (e.g <https:://>\[::1\]/dcap ) will
delimit the list of certificates and attempt to retrieve the URI. The
URI scheme specified determines the type of connection used to retrieve
the resource either `http` for an unencrypted connection or `https` for
a secure TLS connection. If no port is specified, the default ports for
HTTP and HTTPS are used, 80 and 443 respectively.

Commands
--------

The last set of arguments are a list of commands to be interpreted.
These can be any of the following:

-   `sfdi SFDI` - Use the specified sfdi in performing device registration.
This command should be the first command in the list of commands.

-   `register` - Register the client EndDevice with the server. The
`client_test` application will first check the available EndDevices in
the EndDeviceList resource. If an EndDevice with a matching sFDI is not
found, the client POSTs an EndDevice instance that contains the sFDI
computed from the device certificate. If the POST succeeds or the
EndDevice with a matching sFDI was found, the client will retrieve the
Registration resource linked from the EndDevice instance, the pIN value
should be 111115, if not the registration fails. If registration
succeeds and the EndDevice has an associated DER instance, then PUT the
device settings to the links provided in the DER instance.

-   `pin xxxxxc` - When registering the device do a PUT request to send the
    server the PIN of the client EndDevice.

-   `delete SFDI` - Perform retrieval on the EndDeviceList and perform a DELETE
    request on the EndDevice instance with a matching SFDI.

-   `primary` - Perform all the functions of `register`, then retrieve the
FunctionSetAssignmentsList associated with each EndDevice. Determine the
FunctionSetAssignments with the highest priority according to the list
ordering and retrieve only the DERProgramList associated with that FSA,
then retrieve the DERCurveList and DERControlList associated with the
highest priority DERProgram in the DERProgramList. Finally schedule the
events when retrieval is complete.

-   `all` - Perform the same functions as `primary`, except all DERPrograms
and the associated DERCurveLists and DERControlLists are retrieved.

-   `time` - Perform the time test.

-   `self` - Get the SelfDevice and poll the server LogEventList if present.

-   `metering` - Perform the mirrored metering test.

-   `meter` - Perform the inverter meter reading test.

-   `alarm` - Perfrom the alarms test.

-   `device directory` - Load the device certificates from the directory and
    perform the DER Identification Test.

-   `poll interval` - Set the poll rate for active events in seconds, the
    default is 300 seconds or 5 minutes.

-   `load sfdi directory` - Load the device settings located in `directory`
    for the EndDevice with the `sfdi` specified.

-   `inverter` - Perform the test as an inverter client rather than an
    aggregator client (the default). An inverter client will only retrieve
    subordinate resources for the EndDevice instance with an SFDI matching the
    client's computed SFDI. An aggregator client will retrieve the subordinate
    resources and perform scheduling for every EndDevice managed by the
    aggregator client.


Client Test Application
=======================
基于之前文档中的API(在client.md文档中有说明)构建的一个测试应用，用于展示于SunSpec 测试过程的顺应性问题。
The purpose of the client_test application is demonstrate usage of the EPRI
2030.5 client API and demonstrate compliance with the SunSpec test procedure.


Usage
-----

The `client_test` application is a command line tool with the following
arguments:

    client_test interface [device_cert ca_certs..] <subtype[:1][/path] | URI> [commands]

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

The `<subtype[:1][/path] | URI>` argument delimits(vt. 划界；定界限) the list of certificates and
specifies either to perform xmDNS service discovery, or to retrieve the document at the specified URI.
要么使用xmDNS方式来发现服务Service，要么通过遍历的方式找到在URI指定下的文档

Subtype DNS-SD queries 注意，下面的指令只是针对DNS-SD服务
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

When the number `1` is affixed（v. 粘上，附上；（使）附于；可固定（affix 的过去式及过去分词）） to the `subtype`, 
the `client_test` sets the `QU` bit to `1` in the service discovery request indicating that a
unicast response is desired.

Since the `subtype` argument can delimit（vt. 划界；定界限） the list of certificates it
follows that there can not be a certificate or certificate directory
with the same name as the `subtype` argument.

After the service discovery is performed the `client_test` application
will retrieve the associated resource for the subtype query or the
DeviceCapability resource for a general query. Optionally a path can be
specified on the command line. When specified, the client will retrieve the
resource identified by the path from the server location returned by the DNS-SD
response.

URI retrieval 这个是另外一种方式，前面指令中的“|”符号表示的是“或”的意思。
-------------

如果前面的Subtype参数没有给出，则将使用这个URI路径的方式来

If a `subtype` is not specified in the list of arguments, the
`client_test` application attempts to parse a URI. The first argument
that matches an absolute form URI (e.g <https:://>\[::1\]/dcap ) will
delimit（意思就是截断的意思，指如果出现了一个URI，则前面的certificates参数就算是截止了） 
the list of certificates and attempt to retrieve the URI. The
URI scheme specified determines the type of connection used to retrieve
the resource either `http` for an unencrypted connection or `https` for
a secure TLS connection. If no port is specified, the default ports for
HTTP and HTTPS are used, 80 and 443 respectively.

通过URI路径中是HTTPS还是HTTP来判断是加密的还是没有加密的


Commands

--------

The last set of arguments are a list of commands to be interpreted.
These can be any of the following:

这个参数应当是第一个参数
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

执行一个“注册”的任务，然后 retrieve 跟这个设备相关的FunctionSetAssignmentsList。
决定最高优先级，执行与前面的FSA相关联的DERProgramList，然后获取到DERCurveList和DERControlList。
最后执行各个events。

-   `primary` - Perform all the functions of `register`, then retrieve the
FunctionSetAssignmentsList associated with each EndDevice. Determine the
FunctionSetAssignments with the highest priority according to the list
ordering and retrieve only the DERProgramList associated with that FSA,
then retrieve the DERCurveList and DERControlList associated with the
highest priority DERProgram in the DERProgramList. Finally schedule the
events when retrieval is complete.

跟前面的primary类似，除了所有的DERPrograms和与之相关联的DERCurveLists和DERControlLists。

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


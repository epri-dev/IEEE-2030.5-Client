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


检索 DeviceCapability 和 EndDeviceList 资源
### 3. Retrieval of the DeviceCapability and EndDeviceList resources.

DeviceCapability 提供了一个索引，该索引指向服务器上可用的 function sets。

The DeviceCapability provides an index to the function sets available on a
server. For DER servers this index should include a link to an EndDeviceList
resource. The server can populate the EndDeviceList based upon an out-of-band
registration process, some servers may also support in-band registration where
the client can POST new EndDevice instances to a server. For aggregators the
EndDevice list should contain not only an EndDevice instance corresponding to the
aggregator, but also a set of EndDevice instances for each inverter that the
aggregator manages.

作为一个aggregator，EndDevice list不仅仅包含了代表自身的一个EndDevice实例，同时也包含了其下所管理的各个DER设备。

### 4. Validating EndDevice registration, POSTing an EndDevice instance if necessary.

According to the 2030.5 standard the registration procedure is completed by the
client retrieving its own EndDevice instance and verifying that the Registration
resource linked from the EndDevice contains a matching PIN number包含了一个匹配的PIN数字. 
In some cases the client may be registered with a server but the server's EndDeviceList does
contain the corresponding EndDevice instance. In such cases the client is
required to POST its EndDevice instance to the server.

### 5. Performing retrieval of subordinate resources.

Information on 2030.5 servers is represented as a set of linked resources with
the DeviceCapability being the root resource, in the general case the resources
together with the links may form a directed acyclic graph (DAG) 有向无环图 . Resource
retrieval generally begins with the root resource (DeviceCapability) and
proceeds to(proceed to： 继续下去；进入…；) the subordinate or linked resources using the HTTP GET method to
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

Retrieval is a asynchronous(adj. [电] 异步的；不同时的；不同期的) process which may involve many exchanges with a
server (request/response pairs). “检索”是一个异步的过程，伴随着很多跟服务器之间的数据交换 

One feature of the retrieval module(检索模块) is to track
the dependencies between resources and the retrieval status of individual
resources and their subordinates. 一个检索模块的特性是跟踪某一个资源及其所属资源的相互依存关系，
When a resource and all of its subordinates
have been completely retrieved, the retrieval module can optional call a
completion routine. 当某一个资源和其下的从属的资源被完整的检索到了之后，这个检索模块可以可选的调用一个“完成”流程。

"完成"流程可以分配给任何资源以及可以用于任何目的（什么意思？）。
Completion routines can be assigned to any resource and can be used for any
purpose. One use of the completion routine is to signal that retrieval for an
EndDevice and its subordinate resources is complete and therefore that it can
perform local event scheduling.
“完成模”流程的一个用处是用于“指示”对一个EndDevice资源以及从属资源的检索过程已经完成，因此接下去可以执行本地的event计划执行流程了。


### 7. Local event scheduling.

根据CSIP，逆变器可以从属于一个或者多个group。
According to the Common Smart Inverter Profile (CSIP) inverters can belong to one
or more groups. CSIP defines groups according to a 7-layer topological model,
with each layer or group defining a set DER Programs and DER Controls (events).CSIP以7层逻辑模型定义了“组”，而每一个层级都定义了一系列的DER Program和DER Control(Events)。
In addition to having a set of controls, each DER Program has a primacy, and may
have a default control that is applied when there are no active controls.  除了有一系列的control之外，不同的DER Program有一个优先等级(primacy)，以及可以有一个
默认控制（default control），这个默认控制用于在没有当前活跃控制（active control）的时候发挥作用。
The primacy of a DER program determines the priority of events when there are
overlapping DER controls.

一个DER program的primacy定义了存在不同的DER control相互叠加（overlapping）的时候，各个event（控制流程）的执行优先过程。

对于各个逆变器，在分配了组（group）之后，DER Program 系列和 DER Controls
For each inverter, given the group assignments, the set of DER Programs and DER
Controls the client must create a local event schedule taking into account that
there may be overlapping events, and that these instances are resolved by the
associated primacy of the event along with the general rules involving events
(IEEE 2030.5, section 12.1.3). If the server indicates that the client must
respond to events, then the client must also send response messages to indicate
that for example the event was received, that the event has started, completed,
etc.
在事件接收到、发生了，完成了等不同的状态下，要报告给服务器。

The function of local scheduling is to create a timed sequence of events that can
interpreted locally and also to generate event responses sent to the server at
the appropriate times, taking into account event randomization (if any)考虑到随机化处理. 
The EPRI client scheduler takes the information described above and generates a sequence
of `EVENT_START`, and `EVENT_END` event pairs to indicate that a DER Control has
become active and then inactive. An event becomes active at the beginning of the
effective start period for that event, an event can become inactive due to local
scheduling (superseded by an overlapping event), the event being canceled, or the
effective time period for an event expiring.

### 8.   Interpreting events. 解释事件（events）

当一个DER Control变成了活跃状态，客户端将决定如何响应这个event。
When a DER Control becomes active the client can then decide how to respond to
the event. Applying a DER Control can mean sending a series of MODBUS commands
to an inverter. 执行一个DER Control意味着发送一串MODBUS 指令到逆变器。
The client may also decide to opt-out（n. 选择退出；不参加） of an event and if so must
send an appropriate response message to the server.

客户端（指的是我们现在的FG设备）同样也可以决定退出一个event，如果确实如此的话，就要求发送一个合适的响应消息到服务器。

### 9.   Model maintenance.  模型维护

随着时间推移，服务器可能改变“组”分配，即增加或者移除DER Program，取消event或者替代event。
Over time the server may change group assignments, add or remove DER Programs,
cancel events or replace an event with a superseding（接替） event. In order for the
local event scheduling to be synchronized with a server, the client needs to
periodically poll (retrieve) the resources involved in scheduling.需要定期的去查询事件变化情况。
2030.5 provides another mechanism called subscription/notification whereby the server
connects to the client in order to push updates, this mechanism is not yet
supported by the EPRI client. 
订阅功能目前还没有在（这个？）客户端上实现。


The EPRI client library can seamlessly（adv. 无缝地） handle to changes to the model, including
resource and event removal, event cancellation, superseding events, adding or
removing DER programs and changing the associated primacy of events. All that
needs to be done on behalf of the application is to setup an appropriate polling
rate for resources, and selecting which resources to poll. The polling rate can
be based upon the `pollRate` element present in some resources or some other
suitable interval. The application can also request updates in a custom way if
a fixed polling interval is not appropriate.

When an update is requested either by polling or manually updating the request
will temporarily invalidate(v. 使无效，使作废) the completion property of the resource and all the
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

DER Client reference implementation 参考实现 (`der_client.c`) 
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

两个实例应用包含了EPRI客户端库（EPRI client library）。

Two example applications are included with EPRI client library, `csip_test.c` and
`client_test.c`. Both applications use `der_client.c` as a basis, the EPRI DER
client reference implementation. Of the two, `client_test.c` is far more
complete in terms of demonstrating the EPRI client library API, it is also more
complex due to its usage as a tool for demonstrating compliance with the SunSpec
test procedure. The command line arguments of `client_test.c` provide a way to
test different configurations and demonstrate various test procedures that are
useful for certification purposes, but might not be needed for typical DER
applications.

client_test.c程序提供了更加典型的应用，但是有些功能在平时正常的应用中是用不到的。

csip_test.c是一个更好的开端。
The application `csip_test.c` provides a better starting point for a DER
client applications because, with the exception(n. 例外，除外) of registration, it performs
minimally(adv. 最低限度地；最低程度地) all the functions described in the DER Client Model section. 
The function `main` shows the basic structure for a client application:

    int main (int argc, char **argv) {
      void *any; int index; Service *s;
      // platform initialization 平台初始化
      platform_init ();
      // process command line arguments	
      if (argc < 2) {
        print_interfaces (0); exit (0);
      }
      if ((index = interface_index (argv[1])) < 0) {
        printf ("interface %s not found\n", argv[1]); exit (0); 
      }
      // load certificates and initialize client 导入证书和初始化客户端
      client_init (argv[1], "pti_dev.x509"); der_init ();
      load_cert_dir ("certs");
      // perform service discovery 执行 “设备发现”
      discover_device ();
      while (1) {
        // process events	处理事件（events）
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

The `main` function can be abbreviated (adj. 简短的；小型的；服装超短的) as:

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

platform_init要首先调用，否则其他很多函数无法执行


对于 csip_test 任务来说，只有一个参数就是网络端口名

2.   The client can then process any command line arguments, for `csip_test` the
only argument is name of the network interface for which the client should
use to perfom service discovery. If none is provided the application will print
a list of interfaces.


3.   If the network device is valid then load the device certificate, the CA
certificates, and initialize the client library. These certificate will
be used when the client attempts a secure connection to a server.

调用 discover_device 函数来发现DNS服务发现功能（DNS-SD）。
4.   Call `discover_device` to perform DNS service discovery (DNS-SD), this will
send a MDNS packet on the local network with a subtype query to discover a
server that contains an EndDevice instance with an SFDI that matches the client's
SFDI (computed from the device certificate). 通过DNSSD服务来发现本测试程序中导入的证书文件中对应的设备
Since only servers that contain
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

Besides providing a model(n. 模型；模式；) for structuring client applications, `csip_test.c` also
shows how the retrieval process can be customized. In the event processing of
`TCP_PORT` data, the client performs:

          if (conn_session (any))
            process_http (any, csip_dep); break;

`conn_session` will return True (non-zero) when a TCP/TLS session has been
established. For a TLS session multiple calls may required as several messages
are exchanged as part the TLS handshake.

If the session has been established, the application calls `process_http` to
handle responses from a 2030.5 server. The function `process_http` takes as
arguments an `SeConnection` (provided by the `TCP_PORT` event)（就是前面函数中的any参数） and also a
dependency function（就是前面函数中的csip_dep函数） which is called on every successfully retrieved resource.
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
    
    SE_DeviceCapability_t是一个C结构体对象，包含了所有可能的DeviceCapability这个对象的元素（在2030.5 XML schema中定义）。
    XML/EXI文档将被自动解析并且转换成C对象形式

    se_recieve将自动解析和转换到C对象形式。
    
-   `resource_data` is a macro to access the C object representation of a
    resource with the corresponding type (in this case `SE_DeviceCapability_t`).
	resource_data 是一个宏定义，用于访问C对象中的某一个特定的类型。
	

-   `se_exists` is a macro to determine the presence of optional elements within
    an object/document. `se_exists` works by checking the presence of flag bits
    within the C object (see `se_types.h` for a listing of possible flags for
    each document type).
    
    se_exists是一个宏定义，用于检测某一个选配元素是否存在于对象中（可能实际的某一个对象数据不存在某一个元素）。


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

跟前面的get_root和get_list_root不同，这里加入了建议依赖关系的功能。


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
下面这个函数执行了一个检索，并且构建起一个轮询过程。
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






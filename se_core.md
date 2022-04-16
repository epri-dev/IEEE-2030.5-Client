Core Modules and Building
=========================

Modules can be integrated into an application by including their source code
(e.g. `#include ...`) or at a later stage by linking.

为了减少编译时间并且提供一个公用的基础给应用开发者，这个被用于开发IEEE2030.5应用的core模块已经被包含在文件 se_core.c 中。
In order to cut down the compile times and also provide a common base for
application developers the core modules needed to develop IEEE 2030.5
applications have been included in the file `se_core.c`:

~~~
#include "pack.c"
#include "util.c"
#include "list.c"
#include "queue.c"
#include "platform.c"
#include "parse.c"
#include "xml_parse.c"
#include "exi_parse.c"
#include "output.c"
#include "xml_output.c"
#include "exi_output.c"
#include "se_types.h"
#include "se_object.c"
#include "sha256.c"
#include "openssl.c"
#include "security.c"
#include "connection.c"
#include "uri.c"
#include "http.c"
#include "se_connection.c"
#include "mdns.c"
#include "dnssd_client.c"
#include "se_discover.c"
~~~

被包含到文件包se_core.c的这些文件，不含main程序，所以仅仅se_core.c文件是不完整的。一个应用必须要么
The files included by `se_core.c` contain no `main` function, so by itself
`se_core.c` is not complete. An application must either include `se_core.c` or
link with a compiled version of `se_core.c`. The file `targets.sh` gives an
example linking with `se_core.o` (compiled from `se_core.c`):

~~~
se_objects=( se_core.o )
se_libs=( ${tls_libs[@]} )
se_targets=( client_test csip_test )
targets=( schema_gen se )
~~~

The following line defines two targets `schema_gen` and `se`:

~~~
targets=( schema_gen se )
~~~

The target `se` is a sublist target which includes the targets `client_test`
and `csip_test`, two applications that test the client API.

~~~
se_targets=( client_test csip_test )	这两个应用用于测试客户端API 
~~~

These targets both link with `se_core.o` (compiled from se_core.c):

这两个应用都跟se_core.o链接在一起。

~~~
se_objects=( se_core.o )
~~~

最终，应用程序必须链接到TLS库。
Finally the applications must be linked with the TLS libraries:

~~~
se_libs=( ${tls_libs[@]} )
~~~

The variable `tls_libs` is a Bash array variable that expands to a list of
libraries required to use the TLS library, for OpenSSL the list has been
defined as:

~~~
openssl_libs=( -lssl -lcrypto -lpthread -ldl )
~~~

Header Files
------------

Typically C applications that are built from seperate binary modules utilize
header files that define the interface (structures and functions prototypes)
for the functionality provided by those modules.

In order to simplify maintenance, the modules provided by this framework
include the header (or interface) in the same file as the implementation. For
all of the core modules provided the framework, the implementation can be
excluded by defining the constant `HEADER_ONLY`. For example, the header file
`se_core.h` includes the same modules as `se_core.c` only the constant
`HEADER_ONLY` has been defined thus excluding the implementation:

~~~
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_ONLY
#include "se_core.c"
#undef HEADER_ONLY
~~~



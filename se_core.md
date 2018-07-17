Core Modules and Building
=========================

Modules can be integrated into an application by including their source code
(e.g. `#include ...`) or at a later stage by linking.

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
se_targets=( client_test csip_test )
~~~

These targets both link with `se_core.o` (compiled from se_core.c):

~~~
se_objects=( se_core.o )
~~~

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



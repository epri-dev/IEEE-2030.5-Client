Build System
============

`build.sh` is a Bash based build system/script, the purpose of the script is to
simplify C based cross platform development using gcc. While the script shares
features of a build system it is not intended as a replacement for existing
build systems such as `make` and the 'auto'-tools (`automake`/`autoconf`) whose
scope and application are much broader. The scope here is limited to simply
compiling applications and libraries that are part this framework using gcc as
the compiler and Bash as the shell/scripting environment.

Bash was chosen as the language because of it's widespread use as a scripting
language and shell environment and it's use in configuring software (including
its use in build systems). The features of the Bash language (arrays,
introspection of variables) make it well suited to describe build targets, and
the flexibility of using a scripting language makes it possible to extend the
build system, for example defining custom build commands. These features are
more than adequate in addressing the current needs of the framework and future
needs as the framework is ported to other systems.

Targets
-------

Running the build script with no arguments will attempt to build the targets
specified in `targets.sh`. Within `targets.sh` the variable `targets` should
be defined, this variable is an array of targets to be built:

    targets=( ... )

Each target can be either:

-   A subdirectory with its own `targets.sh`
-   A target with it's own build command
-   A C based target (executable, object, or shared library)
-   A sublist of targets that contains any of these possibilities

Targets can also be specified on the command line, in this case the file
`targets.sh` is still read and processed but the command line arguments become
the list of targets.

Subdirectory Target
-------------------

The subdirectory target is a subdirectory off of the main project directory
with its own `targets.sh`. Building a subdirectory target is equivalent to
changing to that directory and executing the build script. This feature is
used to define subprojects, these can be worked on independently of the main
project.

Build Command Target
--------------------

The build command target is a target that is "built" by executing a Bash
function. The name of this function is the target name with name `_build`
appended to it. For example, if a function is defined within `targets.sh` as:

    hello_build () { echo "Hello World"; }

Then this target can be built by executing the command:

    ./build.sh hello

C Based Targets
---------------

C based targets can be either an executable, object, or shared library. For a
given target name `target`, the following variables can be defined:

-   `target_flags` - compiler flags used to compile the target
-   `target_libs` - libraries to be linked with the target
-   `target_objects` - objects to be linked with the target

The type of the target is determined by its name, targets ending in `.o` are
compiled objects, targets of the form `lib{target}.so` are shared library
targets, targets without either extensions are executable targets. In all cases,
the `_flags`, `_libs`, and `_objects` variables will have the target name
without the extensions, so that in compiling `libtest.so`, the variables
`test_flags`, `test_libs` and `test_objects` will be used.

Only a single source file is used as the input to compile a target, its name is
the same as the target name with the ".c" extension used instead of the target
extensions. So, to produce `target`, `target.o`, or `libtarget.so` (an
executable, object, or shared library) the file `target.c` is used as the
input file.

Sublist Targets
---------------

The final type of targets are sublist targets, these are Bash arrays that can
contain any of the prior types of targets. To create a sublist target, simply
create a Bash array and include it in the list of targets:

    sublist=( ... )
    targets=( ... sublist ... )

The `_flags, `_libs`, and `_objects` variables can be defined for a sublist
target:

    sublist_flags=( ... )
    sublist_libs=( ... )
    sublist_objects=( ... )

When this occurs the targets included in the sublist will inherit these
attributes. This provides a useful way to group together executables or other
targets that share common attributes such as the compiler flags used and
libraries linked.

Build Types
-----------

The following build types can be specified on the command line:

-   `debug` - Compile targets with debugging symbols included and no
    	      optimization.
-   `static` - Use static linking.
-   `cross` - Use the cross compiler instead of the default compiler.

With no arguments specified, the default build type is optimized code
generation. Any of these build types can be combined, for example running the
command:

    ./build.sh debug cross

Will use the cross compiler and compile the targets for debugging.

Dependencies and Rebuilding
---------------------------

Changes to the source files are frequently made in the course of development,
and as it so happens, the compiled targets (executables, objects, and shared
libraries) become out of date or unsynchronized with the source files. In such
cases a rebuild is required and it is desirable to rebuild only those files
that require it. In order to do that, we need to know what files a target
depends on. If any of these files are newer than the target then a rebuild is
required.

In compiling a C source file, there are two kinds of dependencies, source file
dependencies created by including other source files (`#include` directives),
and binary dependencies that are implied through linking. The latter
dependencies are explicit within the build system, specified by listing the
linked objects required to build a target, the former can be discovered by the
gcc compiler itself using the `-MM` option. Before compiling a source file, the
build system checks a dependency file, one generated by running gcc with the
`-MM` option. From that listing of files, the build system can decide whether to
rebuild a target or not.

The source dependency listings are only created the first time a target is
compiled. Because source files can sometimes change their source dependencies
(by adding or removing `#include` directives), the dependency files themselves
sometimes need to regenerated, this can be done with the "dep" command:

    ./build.sh dep

Finally, a change in build type will require the targets to be rebuilt, for
this purpose the build system stores the set of build arguments and compares
this list with the build arguments specified on the command line. A change in
arguments will force a rebuild for all the targets. 


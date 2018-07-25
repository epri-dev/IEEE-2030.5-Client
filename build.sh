#!/bin/bash
#
# to build targets, run the script as follows:
#
# ./build.sh [static] [debug] [cross] [targets ...]
#
#--------------------------------------------------------
#
# configuration options
# change these for your host systems

system="linux" # only linux supported in this version
linux_host=
linux_prefix="/usr"
linux_cross_host=
linux_cross_prefix=

#---------------------------------------------------------

cc=gcc
cpp=cpp
args=( "$@" )
compile_args=()
debug_flags=( -g )
optimize_flags=( -Os -ffunction-sections -Wl,--gc-sections
		 -fno-asynchronous-unwind-tables )
static_flags=( -static -Wl,-Bstatic )

contains () {
    local -n array=$1; local e
    for e in "${array[@]}"; do
	if [[ "$e" == "$2" ]]; then
	    return 0
	fi
    done
    return 1
}

intersect () {
    local -n array1=$1 array2=$2
    local -a int; local e
    for e in "${array1[@]}"; do
	if contains array2 "$e"; then
	    int+=( "$e" )
	fi
    done
    array1=( ${int[@]} )
}

subtract () {
    local -n array1=$1 array2=$2
    local -a sub=( ); local e
    for e in "${array1[@]}"; do
	contains array2 "$e" || {
	    sub+=( "$e" )
	}
    done
    array1=( ${sub[@]} )
}

remove () {
    local -n array=$1
    local -a r=( "$2" )
    subtract array r
}

if [ -e targets.sh ]; then
    source targets.sh
fi

if contains args "debug"; then
    compile_args+=( debug )
    flags+=( ${debug_flags[@]} )
else
    flags+=( ${optimize_flags[@]} )
fi

if contains args "static"; then
    compile_args+=( static )
    flags+=( $static_flags[@] )
fi

if contains args "cross"; then
    compile_args+=( cross )
    host=${system}_cross_host
    prefix=${system}_cross_prefix
else
    host=${system}_host
    prefix=${system}_prefix
fi

recompile=0
build_dep=0

if contains args "dep"; then
    build_dep=1
    remove args "dep"
fi

# compile target objects flags libs
compile () {
    local target=$1; local -n objs_=$2 flags_=$3 libs_=$4
    if [[ $target =~ \.o$ ]]; then
	# object target
	local name=${target/%.o/}
	local source="-c ${name}.c"
    elif [[ $target =~ \.so$ ]]; then
	# shared library target
	local name=${target/%.so/}
	name=${name/#lib/}
	local source="${name}.c"
	local lflags=( -shared -fPIC ) 
    else
	# application target
	local name=$target
	local source=${target}.c
    fi
    local need_compile=0
    local target_file=build/$target
    local target_file_dep=${target_file}.dep
    local -n target_flags=${name}_flags
    local -n target_libs=${name}_libs
    local -n target_objects=${name}_objects
    local c_flags="${lflags[@]} ${flags_[@]} ${target_flags[@]}"
    local c_libs="${libs_[@]} ${target_libs[@]}"
    local c_objs="${objs_[@]} ${target_objects[@]}"
    if [ ! -f $target_file_dep ] || (( $build_dep == 1 )); then
	cmd="$cpp -MM $c_flags ${name}.c"
	echo $cmd; $cmd > $target_file_dep
    fi
    local dep=`cat $target_file_dep` # get the dependency list
    local target_dep=( ${dep//$'\\'$'\n'/} ) # remove line breaks
    target_dep+=( ${objs_[@]} )
    if [ -f $target_file ]; then
	for dep in "${target_dep[@]:1}"; do
	    if [ $dep -nt $target_file ]; then
		need_compile=1; break
	    fi
	done
    else
	need_compile=1
    fi
    if (( $need_compile == 1 || $recompile == 1 )); then
	cmd="$cc $c_flags -o $target_file $source $c_objs -L${prefix}/lib $c_libs"
	echo $cmd; $cmd
    fi
}

# add build dir for linked objects and build them if necessary
linked_objects () {
    local -n objs_in=$1 flags=$2 libs=$3
    local objs_out=()
    for obj in "${objs_in[@]}"; do
	local target=build/$obj objs=()
	compile $obj objs flags libs
	objs_out+=( $target )
    done
    objs_in=( ${objs_out[@]} )
}

function_exists () {
    declare -f -F $1 > /dev/null
    return $?
}

# build a list of targets and any sub lists
build () { # list objects flags libs
    local -n list=$1 _objs=$2 _flags=$3 _libs=$4
    for target in "${list[@]}"; do
	local sub=${target}_targets
	local command=${target}_build
	if [ -d $target ]; then
	    # subdirectory target
	    script=`readlink -f $0`
	    pushd $target > /dev/null
	    $script $compile_args
	    popd > /dev/null
	elif function_exists $command; then
	    # target with build command
	    $command
	elif [ -v $sub ]; then
	    # sublist target
	    local -n target_flags=${target}_flags
	    local -n target_libs=${target}_libs
	    local -n target_objects=${target}_objects
	    local sub_flags=( ${_flags[@]} ${target_flags[@]} )
	    local sub_libs=( ${_libs[@]} ${target_libs[@]} )
	    linked_objects target_objects _flags _libs
	    local sub_objects=( ${_objs[@]} ${target_objects[@]} )
	    build $sub sub_objects sub_flags sub_libs
	else # C target
	    compile $target _objs _flags _libs
	fi
    done
}

subtract args compile_args
if [ ${#args[@]} -eq 0 ]; then
    args=( ${targets[@]} )
fi

compile_args="${compile_args[@]}"

if [ ! -d build ]; then mkdir build; fi
if (( $build_dep == 0 )); then
   if [ -f build/.args ]; then
       old_args=`cat build/.args`
   fi
   if [[ $compile_args != $old_args ]]; then
       recompile=1
   fi
   echo $compile_args > build/.args
fi
sys_flags=( ${flags[@]} )
#echo ${sys_flags[@]}
if [ -n "${!host}" ]; then
    cc="${!host}-${cc}"
    cpp="${!host}-${cpp}"
fi
if [[ $system == "win32" ]]; then
    cc="${cc}.exe"
    cpp="${cpp}.exe"
    sys_flags+=( -DWINVER=0x0602 -D_WIN32_WINNT=0x0602 )
    libs=( -lws2_32 -liphlpapi )
fi
prefix=`$cc -print-sysroot`${!prefix}
sys_objects=()
build args sys_objects sys_flags libs


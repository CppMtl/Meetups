#!/bin/bash
set -e;

function usage
{
	echo "docker run --rm -it -v <PathToSourcesDir>:/Sources -v <PathToBuildDir>:/Build -v <PathToInstallDir>:/Install -v /etc/passwd:/etc/passwd --user \$(id -u):\$(id -g) toolchain"
	exit 1
}

if [[ ! -d /Sources ]]; then
	usage
	exit 1
fi

if [[ ! -d /Build ]]; then
	usage
	exit 1
fi

if [[ ! -d /Install ]]; then
	usage
	exit 1
fi

cd /Build
cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-Release} -DCMAKE_INSTALL_PREFIX=/Install $CMAKE_OPTIONS -G Ninja /Sources
ninja
if ninja -t targets | grep ^install:; then
	ninja install
fi

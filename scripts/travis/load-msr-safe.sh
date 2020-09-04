#!/bin/bash

which g++
g++ --version

uname -a
uname -r

export CC=${COMPILER_CC}
export CXX=${COMPILER_CXX}

git clone https://github.com/llnl/msr-safe
cd msr-safe

${CC} --version
${CXX} --version

make VERBOSE=1

sudo insmod msr-safe.ko

sudo chmod o=u /dev/cpu/*/msr_safe
sudo chmod o=u /dev/cpu/msr_*

sleep 2

ls -l /dev/cpu
ls -l /dev/cpu/*/msr_safe

# The version information for msr-safe resides in /sys/module/msr_safe/version
MSR_SAFE_VERSION=`cat /sys/module/msr_safe/version`

# coreutils-7 and later have the -V option for sort.  Verify via sort --version,
#  which puts the earlier version first.
if [ `echo -e "${MSR_SAFE_VERSION}\n1.5" | sort -V | head -n1` = "1.5" ]; then 
	# MSR_SAFE_VERSION is 1.5 or greater, so we need to deal with allowlists.
	AL=$(printf 'al_%.2x%x\n' $(lscpu | grep "CPU family:" | awk -F: '{print $2}') $(lscpu | grep "Model:" | awk -F: '{print $2}'))
	echo -e "AL:" ${AL}

	cat allowlists/${WL} > /dev/cpu/msr_allowlist
	cat /dev/cpu/msr_allowlist | head
else
	# MSR_SAFE_VERSION is less than 1.5, so we need to deal with whitelists.
	WL=$(printf 'wl_%.2x%x\n' $(lscpu | grep "CPU family:" | awk -F: '{print $2}') $(lscpu | grep "Model:" | awk -F: '{print $2}'))
	echo -e "WL:" ${WL}

	cat whitelists/${WL} > /dev/cpu/msr_whitelist
	cat /dev/cpu/msr_whitelist | head
fi


#

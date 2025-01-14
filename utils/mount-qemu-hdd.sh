#! /bin/bash
if [ $# -lt 1 ]; then
	echo "Usage error. Argument expected: mount-qemu-hdd.sh hdd.qcow2"
	exit 1
fi

originalDir=$(pwd)
dir=$(dirname "$1")

cd $dir
# qemu-img convert -f qcow2 -O raw $1 hdd.raw
modprobe nbd max_part=8 
qemu-nbd --connect=/dev/nbd0 $1
mkdir -p ./mnt
mount /dev/nbd0p1 ./mnt
echo "QEMU virtual HDD mounted in ${pwd}/mnt"

cd $originalDir
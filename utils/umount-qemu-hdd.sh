#! /bin/bash
if [ $# -lt 1 ]; then
	echo "Usage error. Argument expected: mount-qemu-hdd.sh hdd.qcow2"
	exit 1
fi

originalDir=$(pwd)
dir=$(dirname "$1")

cd $dir

umount ./mnt
qemu-nbd --disconnect /dev/nbd0
rmmod nbd
echo "QEMU virtual HDD unmounted"

cd $originalDir
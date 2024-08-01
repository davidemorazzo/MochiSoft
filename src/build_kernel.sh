mkdir -p build-kernel

KERNEL_PATH="src/kernel"
BUILD_PATH="build-kernel"
BOOTLOADER_PATH="src/bootloader"

cd /workspaces/MochiSoft

echo "Assembly the bootloader ..."
/home/codespace/opt/cross/bin/i686-elf-as $BOOTLOADER_PATH/boot.s -o $BUILD_PATH/boot.o

echo "Build Kernel object file ..."
/home/codespace/opt/cross/bin/i686-elf-g++ -c $KERNEL_PATH/kernel.cpp -o $BUILD_PATH/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

echo "Link kernel image ..."
/home/codespace/opt/cross/bin/i686-elf-gcc -T $KERNEL_PATH/linker.ld -o $BUILD_PATH/MochiOS.bin -ffreestanding -O2 -nostdlib $BUILD_PATH/boot.o $BUILD_PATH/kernel.o -lgcc
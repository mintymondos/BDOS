nasm -f elf32 JMP.S -o JMP.o

gcc -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c main.c -o main.o
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c SHELL.C -o shell.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c MISL.C -o misl.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c KEY.C -o key.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c LOAD.C -o load.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c PANIC.C -o panic.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c DET.C -o det.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c ATA.C -o ata.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c FDC.C -o fdc.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c HPPKG.C -o hppkg.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c INSTALLER.C -o installer.o -x none
gcc -x c -m16 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-exceptions -fno-asynchronous-unwind-tables -Os -c ugui.c -o ugui.o -x none

objcopy -I binary -O elf32-i386 CMD.MS CMD.o
objcopy -I binary -O elf32-i386 KCI.MS KCI.o
objcopy -I binary -O elf32-i386 HumbleBumble.txt HB.o

ld -m elf_i386 -T linker.ld JMP.o main.o shell.o misl.o key.o load.o panic.o det.o ata.o fdc.o hppkg.o installer.o ugui.o CMD.o KCI.o HB.o  -o stage2.bin

STAGE2_SECTORS=$(( ($(stat -c%s stage2.bin) + 511) / 512 ))
echo "stage2.bin is $STAGE2_SECTORS sectors"

nasm -f bin -D STAGE2_SECTORS=$STAGE2_SECTORS BOOT.S -o boot.bin

cat boot.bin stage2.bin > os.img
nm CMD.o
nm KCI.o
nm HB.o

#truncate -s 1M os.img
qemu-system-i386 -drive format=raw,file=os.img

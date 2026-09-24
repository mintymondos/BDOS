# BDOS
the Bumble (D)OS is a WIP operating system with its own floppy-based package manager (HPPKG)!

just run ./run.sh to build BDOS and run it in qemu / create the .img file

every file does almost exactly what it is named,
ex: FDC.C: floppy disk controller shit, DET.C: detects ATA drive, etc etc
all except MAIN.C & ATA.C (MAIN.C controls logic and printing and ATA.C controls the ATA stuff and the filesystem.)

packages can be downloaded from (PLACEHOLDER.COM)

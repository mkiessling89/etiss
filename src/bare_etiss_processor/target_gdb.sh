#!/bin/bash

# first change working dir to 1st argument
cd $1
echo $(pwd)
echo "target file: $2"
/opt/riscv/bin/riscv32-unknown-elf-gdb -ex "tar rem :2222" $2
#gdb-multiarch -ex "tar rem :2222" $2

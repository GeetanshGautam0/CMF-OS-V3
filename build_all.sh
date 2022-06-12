clear

export OSName="cmfOS"

cd $OSName/gnu-efi
make bootloader
cd ../kernel
make kernel
make buildimg

./run.sh

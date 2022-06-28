clear

export OSName="cmfOS"

cd $OSName/gnu-efi
make bootloader
cd ../kernel
rm -r ./lib
make kernel
make buildimg

./run.sh

clear

export os_name="cmfOS"
cd $os_name/gnu-efi
make bootloader
cd ../kernel
rm -r ./lib
make kernel
make buildimg
chmod +x run.sh
./run.sh

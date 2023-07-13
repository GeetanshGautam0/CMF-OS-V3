export OSNAME="CustomOS"
export NPATH="/mnt/c/Users/geeta/Documents/CMF-OS-V3"

rm -r "$NPATH/.run"
mkdir "$NPATH/.run"
cp "./vgabios-stdvga.bin" "$NPATH"
cp "./kvmvapic.bin" "$NPATH"
cp "./bin/$OSNAME.img" "$NPATH/.run" 

cp "../OVMFbin/OVMF_CODE-pure-efi.fd" "$NPATH/.run"
cp "../OVMFbin/OVMF_VARS-pure-efi.fd" "$NPATH/.run"
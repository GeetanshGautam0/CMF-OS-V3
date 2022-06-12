# clear
# ls
export OSNAME="CustomOS"
export NPATH="/mnt/d/User Files/OneDrive/Documents/2. Electronics/00 - CMF OS/v3 (UEFI)"

mkdir "$NPATH/run"
cp "/mnt/c/Program Files/qemu/vgabios-stdvga.bin" "$NPATH"
cp "/mnt/c/Program Files/qemu/kvmvapic.bin" "$NPATH"
cp "./bin/$OSNAME.img" "$NPATH/run"
cp "../OVMFbin/OVMF_CODE-pure-efi.fd" "$NPATH/run"
cp "../OVMFbin/OVMF_VARS-pure-efi.fd" "$NPATH/run"

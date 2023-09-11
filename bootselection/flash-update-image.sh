#! /bin/bash

if [ "$EUID" -ne 0 ]
    then echo "Please run as root"
    exit 
fi

if [ -z "$1" ]
    then echo "Please provide image "
    exit 
fi

Help()
{
   # Display Help
   echo "Flash update image to inactive slot. Provide tar.gz image as first parameter."
   echo
   echo "Syntax: flash-update-image.sh <path-to-tar.gz> "
   echo "options:"
   echo "h     Print this Help."
   echo
}

while getopts ":h" option; do
   case $option in
      h) # display Help
         Help
         exit;;
   esac
done

IMAGE_PATH="$1"
SLOT_A="/dev/mmcblk1p2"
SLOT_B="/dev/mmcblk1p3"

flashRootFs()
{
    rm -r /mnt/root/*
    tar -xvzf "$IMAGE_PATH" -C /mnt/root
    umount /mnt/root
}

# check if provided image file exists
if test -f "$IMAGE_PATH"; then
    # determine if slot a or slot b currently running
    RUNNING_PARTITION=$(findmnt -no SOURCE --target /)
    if [[ "$RUNNING_PARTITION" == *"$SLOT_A"* ]]; then
        # SLOT_B needs to be flashed
        echo "slot_b is going to be flashed"
        mkdir /mnt/root
        mount "$SLOT_B" /mnt/root
        flashRootFs
    elif [[ "$RUNNING_PARTITION" == *"$SLOT_B"* ]]; then
        # SLOT_A need to be flashed
        echo "slot_a is going to be flashed"
        mkdir /mnt/root
        mount "$SLOT_A" /mnt/root
        flashRootFs
    else
        echo "No suitable partition found, running on MT310s2?"
    fi
else
    echo "Provided Image file not found"
fi
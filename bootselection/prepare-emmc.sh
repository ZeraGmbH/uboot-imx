#! /bin/bash

if [ "$EUID" -ne 0 ]
    then echo "Please run as root"
    exit 
fi

sdMMC="mmcblk1"
internalMMC="mmcblk0"

internalMMCPartitionBoot="${internalMMC}p1"
internalMMCPartitionRoot="${internalMMC}p2"

sdMMCPartitionBoot="${sdMMC}p1"
sdMMCPartitionRoot="${sdMMC}p2"

deletePartitionTable()
{
    parted -s /dev/${internalMMC} -- mklabel msdos
}

preparePartitions()
{
    # create new boot partition in emmc
    # get start and size of current boot partition
    MMC_BOOT_START=$(</sys/block/${sdMMC}/${sdMMCPartitionBoot}/start)
    MMC_BOOT_SIZE=$(</sys/block/${sdMMC}/${sdMMCPartitionBoot}/size)
    parted -s /dev/${internalMMC} -- mkpart primary ext4 1s "${MMC_BOOT_SIZE}"s

    # rootfs
    MMC_NEW_ROOT_START=$((MMC_BOOT_START + MMC_BOOT_SIZE))
    parted -s /dev/${internalMMC} -- mkpart primary ext4 "${MMC_NEW_ROOT_START}"s -1s
}

prepareFilesystems()
{
    # boot
    mkfs.fat -F 16 /dev/${internalMMCPartitionBoot}
    # rootfs
    mkfs.ext4 -F /dev/${internalMMCPartitionRoot}
}

createOSinMMC()
{
    mkdir /mnt/boot
    mkdir /mnt/root

    mount /dev/${internalMMCPartitionBoot} /mnt/boot
    mount /dev/${internalMMCPartitionRoot} /mnt/root

    cp -ax / /mnt/root/
    cp -ax /boot/* /mnt/boot

    cp boot.scr /mnt/boot
    cp uEnv.txt /mnt/boot

    cp boot.scr /boot
    cp uEnv.txt /boot

    # change /boot to internal emmc
    sed -i 's|mmcblk1p1|mmcblk0p1|g' /mnt/root/etc/fstab

    # mark partition as slot_rescue
    touch /mnt/root/opt/slot_rescue
}

deletePartitionTable
preparePartitions
prepareFilesystems
createOSinMMC

echo "Successfully finished"

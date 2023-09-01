#! /bin/bash

if [ "$EUID" -ne 0 ]
    then echo "Please run as root"
    exit 
fi

sdMMC="mmcblk1"

sdMMCPartitionBoot="${sdMMC}p1"
sdMMCPartitionRoot="${sdMMC}p2"
sdMMCPartitionNewRoot="${sdMMC}p3"
sdMMCPartitionHome="${sdMMC}p4"

preparePartitions()
{
    SD_ROOTFS_SIZE=$(</sys/block/${sdMMC}/${sdMMCPartitionRoot}/size)
    if (( SD_ROOTFS_SIZE > 3014657 )) # determine if SD already in correct shape
    then
        # resize existing Partition and filesystem
        echo "Shrinking existing rootfs"
        e2fsck -f /dev/${sdMMCPartitionRoot}
        resize2fs /dev/${sdMMCPartitionRoot} 1490M
        echo yes | parted /dev/${sdMMC} ---pretend-input-tty resizepart 2 1500MiB
        resize2fs /dev/${sdMMCPartitionRoot}

        # create slot_b partition
        echo "Create slot_b"
        SD_ROOT_START=$(</sys/block/${sdMMC}/${sdMMCPartitionRoot}/start)
        SD_ROOT_SIZE=$(</sys/block/${sdMMC}/${sdMMCPartitionRoot}/size)
        SD_ROOT_END=$((SD_ROOT_START + SD_ROOT_SIZE - 1))

        SD_NEW_ROOT_START=$((SD_ROOT_END+1))
        SD_NEW_ROOT_END=$((SD_NEW_ROOT_START + SD_ROOT_SIZE - 1))

        parted -s /dev/${sdMMC} -- mkpart primary ext4 "$SD_NEW_ROOT_START"s "$SD_NEW_ROOT_END"s
        
        # create new home partition
        echo "Create new home"
        SD_NEW_ROOT_START=$(</sys/block/${sdMMC}/${sdMMCPartitionNewRoot}/start)
        SD_NEW_ROOT_SIZE=$(</sys/block/${sdMMC}/${sdMMCPartitionNewRoot}/size)
        SD_NEW_ROOT_END=$((SD_NEW_ROOT_START + SD_NEW_ROOT_SIZE - 1))

        SD_NEW_HOME_START=$((SD_NEW_ROOT_END+2))

        parted -s -a optimal /dev/${sdMMC} -- mkpart primary ext4 "$SD_NEW_HOME_START"s -1s 
    else
        echo "SD card not suited for formating"
        exit
    fi    
}

prepareFilesystems()
{
    mkfs.ext4 -F /dev/${sdMMCPartitionNewRoot}
    mkfs.ext4 -F /dev/${sdMMCPartitionHome}
}

createOSinNewPartitions()
{
    # mount everything and copy over
    mkdir /mnt/tmp_old_root
    mkdir /mnt/tmp_new_root
    mkdir /mnt/tmp_new_home

    mount /dev/${sdMMCPartitionRoot} /mnt/tmp_old_root
    mount /dev/${sdMMCPartitionNewRoot} /mnt/tmp_new_root
    mount /dev/${sdMMCPartitionHome} /mnt/tmp_new_home

    cp -ax /mnt/tmp_old_root/* /mnt/tmp_new_root/
    cp -ax /mnt/tmp_old_root/home/* /mnt/tmp_new_home/

    # mark slot b and current root
    touch /mnt/tmp_new_root/opt/slot_b.txt
    touch /mnt/tmp_old_root/opt/slot_root.txt
    
    umount /mnt/tmp_new_home
    umount /mnt/tmp_old_root
    umount /mnt/tmp_new_root

    rm -r /mnt/tmp_*
}

preparePartitions
prepareFilesystems
createOSinNewPartitions

echo "Successfully finished"

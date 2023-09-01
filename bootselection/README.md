# Files
- boot.scr: compiled boot script for arm
- boot-script-partition-selection.sh: source for the boot.scr file, for boot selection process, when multiple slots/partitions available
- card-partition-mbr.sh: partition cards of unmounted filesystems
- prepare-emmc.sh: use internal emmc as rescue system and move running system into emmc
- uEnv.txt: environment file for u-boot to extend environment variables

 
 ## Compile boot.scr file
 use mkimage 
 ```
 mkimage -c none -A arm -T script -d resuce-boot-script-src.sh boot.scr
 ```

 ## Process of u-boot
 1. u-boot lives in "unallocated" space before /boot partition on SD card
 2. on boot up, u-boot will be loaded, reads uEnv.txt first and then executes boot.scr, both located in /boot
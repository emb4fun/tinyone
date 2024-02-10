
To debug your Bare Metal application, you must first stop the
U-Boot and Linux. This can be achieved by erasing the first
sector of the eMMC device. The first sector can be erased by
using the following commands from the u-boot prompt:

mmc dev 1
mmc erase 0 512

===============================================================================

SPI Flash bootloader
---------------------

The TinyB3 Cape is required for the SPI Flash Bootloader. Further information
about the cape can be found here:
https://github.com/emb4fun/tiny-b3-cape.

For the bootloader functionality, only the following components need to be
equipped on the cape:

C1, C2
D1
U1, U2
R1, R2, R3
JP1, JP2

Note: In the case of U1, you should only equip the pin headers that are actually
needed. This makes it easy if the cape needs to be removed.

The bootimage.srec file is used for the content in the SPI flash (U2).

===============================================================================

SD Card bootloader
-------------------

I can't recommend the SD card bootloader at the moment because I can't find
a current SD card that works. Only the SD cards that came with the BeagleBone
works here. These were 4 GB Class 4 Kingston HC cards. But current, comparable
SD cards don't work for me here.

In case of the "MLO bootloader" it must be placed on a SD card. But here the
SD card must not be greater than 4GB. It must be FAT32 formatted, and a
master boot record should be available.

The bootloader consists of the MLO file and must be located in the root of
the SD card. The actual program is stored on the SD card too and is loaded
and started by the "MLO bootloader" from the SD card.



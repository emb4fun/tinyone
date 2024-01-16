
To debug your Bare Metal application, you must first stop the
U-Boot and Linux. This can be achieved by erasing the first
sector of the eMMC device. The first sector can be erased by
using the following commands from the u-boot prompt:

mmc dev 1
mmc erase 0 512

============================================================================

In case of the "MLO bootloader" it must be placed on a SD card. But here the
SD card must not be greater than 4GB. It must be FAT32 formatted, and a
master boot record should be available.

The bootloader consists of the MLO file and must be located in the root of
the SD card. The actual program is stored on the SD card too and is loaded
and started by the "MLO bootloader" from the SD card.


This hardware/cape is not needed anymore for booting. It is possible to 
place the bootloader on a SD card. But in this case the SD card must not
be greater than 4GB. It must be FAT32 formatted, and a master boot record
should be available.

The bootloader consists of the MLO file and must be located in the root of
the SD card. The actual program is stored on the SD card too and is loaded
and started by the "MLO bootloader" from the SD card.

============================================================================

The additional cape is only needed if the bootloader is to be stored in the
SPI flash. In this case the BeagleBone Black must be configured to start from
a connected SPI-Flash. The SPI-Flash and an additional DUO-Led are located on
the corresponding cape. The actual program is stored on the SD card and is
loaded and started by the bootloader in the SPI flash. 
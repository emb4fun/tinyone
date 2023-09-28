@echo off
cls
echo.
echo Convert bootloader to SPI image

mkdir build
copy ".\prj\RAM Release\b3-boot-mmc.bin" ".\build" >NUL
cd build
..\tools\tiimage 0x402F0400 SPI b3-boot-mmc.bin b3-boot-mmc.spi
..\tools\xbin2c -i:b3-boot-mmc.spi -o:bootimage -v:_boot_image
..\tools\srec_cat b3-boot-mmc.spi -binary -o bootimage.srec
cd ..
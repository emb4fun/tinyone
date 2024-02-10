REM @echo off
cls
echo.
echo Convert bootloader to SPI image

mkdir build
copy ".\prj\RAM Release\b3-boot-mmc.bin" ".\build" >NUL
cd build

::
:: Prepare bootloader for the SPI flash
::
..\tools\tiimage 0x402F0400 SPI b3-boot-mmc.bin b3-boot-mmc.spi
..\tools\xbin2c -i:b3-boot-mmc.spi -o:bootimage -v:_boot_image
..\tools\srec_cat b3-boot-mmc.spi -binary -o bootimage.srec

::
:: Create bootloader for the SD card
::
..\tools\tiimage 0x402F0400 MLO b3-boot-mmc.bin mlo

cd ..

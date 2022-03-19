
To debug your Bare Metal application, you must first stop the
U-Boot and Linux. This can be achieved by erasing the first
sector of the eMMC device. The first sector can be erased by
using the following commands from the u-boot prompt:


mmc dev 1
mmc erase 0 512


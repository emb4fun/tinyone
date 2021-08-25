del xfile.bin
del romfs_data.h
del romfs_data.c
..\tools\xfile -i:romfs
..\tools\xbin2c -i:xfile.bin -o:romfs_data -v:_romfs_data
del xfile.bin


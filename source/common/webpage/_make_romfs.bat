del xfile.bin

del romfs_data.h
del romfs_data.c
del romfs_data_ns.h
del romfs_data_ns.c

..\tools\xfile -i romfs
..\tools\xbin2c -i xfile.bin -o romfs_data -v _romfs_data

..\tools\xfile -i romfs
..\tools\xbin2c -i xfile.bin -o romfs_data_ns -v _romfs_data_ns

del xfile.bin


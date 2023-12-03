@echo off
cls

::
:: Project name
::
set PRJ_NAME=tinyone_web

:: ------------------------------------

::
:: Delete files which does not needed anymore
::
mkdir build >NUL 2>&1
del *.bak /S >NUL 2>&1
del build\*.bin >NUL 2>&1

::
:: Create file system
::
cd build
..\tools\xfile -i:..\htdocs -c:etc\config.txt -z -a:4096

::
:: Rename file system to "project" files
::
copy xfile.bin web1.bin >NUL 2>&1
copy xfile.bin web2.bin >NUL 2>&1
rename xfile.bin %PRJ_NAME%.bin

::
:: Add version info to the filenames
::
..\tools\xname -o:%PRJ_NAME%.bin -v:..\htdocs\etc\config.txt >NUL 2>&1

cd..

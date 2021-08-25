@echo off
cls

::
:: Project name
::
set PRJ_NAME=tinyone_fw
set ORG_NAME=tiny-one

:: ------------------------------------

::
:: Delete files which does not needed anymore
::
mkdir build >NUL 2>&1
del build\*.* /Q >NUL 2>&1

::
:: Copy output file and create new image file
::
copy "prj\RAM Release\%ORG_NAME%.bin" .\build >NUL 2>&1
.\common\tools\xbin -i:.\build\%ORG_NAME%.bin -o:.\build\fw.bin -v:.\incprj\project.h

::
:: Rename image files to "project" files
::
copy .\build\fw.bin .\build\fw1.bin >NUL 2>&1
copy .\build\fw.bin .\build\fw2.bin >NUL 2>&1
copy .\build\fw.bin .\build\firmware.bin >NUL 2>&1
move .\build\fw.bin .\build\%PRJ_NAME%.bin >NUL 2>&1

::
:: Add version info to the filenames
::
.\common\tools\xname -o:.\build\%PRJ_NAME%.bin -v:.\incprj\project.h
del .\build\%ORG_NAME%.bin >NUL 2>&1

echo.
rem pause

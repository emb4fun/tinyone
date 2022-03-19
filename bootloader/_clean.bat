@echo off
del *.bak /S
rmdir "prj\ARM Debug" /S /Q 
rmdir "prj\ARM Release" /S /Q 
del .\build /Q

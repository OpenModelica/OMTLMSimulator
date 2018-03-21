cd 3rdParty\misc
echo Building 3rdParty\misc
nmake -f Makefile.msvc
cd ..\rtime
echo Building 3rdParty\rtime
nmake -f Makefile.msvc
cd ..\..\common
echo Building common
nmake -f Makefile.msvc
cd ..
EXIT /B

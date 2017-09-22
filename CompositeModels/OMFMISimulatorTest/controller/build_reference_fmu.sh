cd reference_fmu
g++ -fPIC -c reference.cpp -I"." 

gcc -fPIC -c reference_model_cs.c -I"/home/robbr48/Documents/Git/hopsan/Dependencies/FMILibrary/include"

g++ -fPIC -w -shared -static-libstdc++ -static-libgcc -Wl,--rpath,'$ORIGIN/.' reference.o reference_model_cs.o -o reference.so 

mkdir binaries
mkdir binaries/linux64

mv reference.so binaries/linux64

zip -r reference.fmu modelDescription.xml binaries/linux64/reference.so

rm -rf binaries
rm *.o

cp reference.fmu ../

cd ..

./fmuCheck.linux64 -n 5 reference.fmu

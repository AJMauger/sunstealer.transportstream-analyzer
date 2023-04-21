clear

if [ ! -L sunstealer.system ] ; then
    ln -s ../sunstealer.system sunstealer.system
fi
if [ ! -L sunstealer.framework ] ; then
    ln -s ../sunstealer.framework sunstealer.framework
fi
if [ ! -L sunstealer.logging.so ] ; then
    ln -s ../sunstealer.logging.so sunstealer.logging.so
fi

sh ./clean.sh

cd ./sunstealer.iso-13818-1
cmake -S . -B build
cd build
make
cd ../..

cd ./sunstealer.iso-13818-1-file.so
cmake -S . -B build
cd build
make
cd ../..

cd ./sunstealer.iso-13818-1-service-information.so
cmake -S . -B build
cd build
make
cd ../..

cd ./sunstealer.logging.so
cmake -S . -B build
cd build
make
cd ../..

cmake -S . -B build
cd ./build
make

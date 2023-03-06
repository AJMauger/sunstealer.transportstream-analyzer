clear

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

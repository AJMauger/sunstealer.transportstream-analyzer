sh ./build.sh

cd build

# nm -D ./sunstealer.logging.so/build/libsunstealer.logging.so 
# nm -D ./sunstealer.iso-13818-1-file.so/build/libsunstealer.iso-13818-1-file.so 
# nm -D ./sunstealer.iso-13818-1-service-information.so/build/libsunstealer.iso-13818-1-service-information.so 

cp ../sunstealer.logging.so/build/libsunstealer.logging.so . 
cp ../sunstealer.iso-13818-1-file.so/build/libsunstealer.iso-13818-1-file.so .
cp ../sunstealer.iso-13818-1-service-information.so/build/libsunstealer.iso-13818-1-service-information.so .

# valgrind --leak-check=full --show-leak-kinds=all ./sunstealer -c transportstreamanalyzer
./sunstealer -c transportstreamanalyzer

cd ..

sh ./build.sh

cd build

# ajm: nm -D ./sunstealer.logging.so/build/libsunstealer.logging.so 
# ajm: nm -D ./sunstealer.iso-13818-1-file.so/build/libsunstealer.iso-13818-1-file.so 
# ajm: nm -D ./sunstealer.iso-13818-1-service-information.so/build/libsunstealer.iso-13818-1-service-information.so 

cp ../sunstealer.logging.so/build/libsunstealer.logging.so . 
cp ../sunstealer.iso-13818-1-file.so/build/libsunstealer.iso-13818-1-file.so .
cp ../sunstealer.iso-13818-1-service-information.so/build/libsunstealer.iso-13818-1-service-information.so .


# ajm: --show-leak-kinds: all definite indirect possible reachable
# valgrind --leak-check=full --show-leak-kinds=all ./sunstealer -c transportstreamanalyzer

./sunstealer -c transportstreamanalyzer

cd ..

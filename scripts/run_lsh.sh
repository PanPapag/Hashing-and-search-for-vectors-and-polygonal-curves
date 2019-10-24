cd ../lsh
make
cd build/
./lsh -d ../../datasets/vectors/input_small_id -q ../../datasets/vectors/query_small_id -k 4 -L 5 -o ../../results/vectors/lsh_small
cd ..
make clean

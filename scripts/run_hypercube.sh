cd ../vectors/hypercube
make
cd build/
./cube -d ../../../datasets/vectors/input_small_id -q ../../../datasets/vectors/query_small_id -k 3 -M 10 --probes 2 -o ../../../results/vectors/hypercube_small
cd ..
make clean

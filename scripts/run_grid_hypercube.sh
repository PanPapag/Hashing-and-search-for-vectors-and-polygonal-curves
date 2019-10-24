cd ../curves/grid_hypercube
make
cd build/
./curve_grid_hypercube -d ../../../datasets/curves/trajectories_grid_dataset.txt -q ../../../datasets/curves/trajectories_grid_query.txt --k_hypercube 3 -M 10 --probes 2 --L_grid 4 -o ../../../results/curves/grid_hypercube
cd ..
make clean

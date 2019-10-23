make
cd build/
./curve_grid_lsh -d ../../datasets/curves/trajectories_grid_dataset.txt -q ../../datasets/curves/trajectories_grid_query.txt --k_vec 4 --L_grid 4 -o ../../results/curves/grid_lsh
cd ..
make clean

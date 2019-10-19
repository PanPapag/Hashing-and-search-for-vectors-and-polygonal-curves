make
cd build/
./curve_grid_lsh -d ../../datasets/curves/trajectories_dataset.txt -q ../../datasets/curves/trajectories_query.txt --k_vec 4 --L_grid 4 -o ../../results/curves/grid_lsh_small
cd ..
make clean

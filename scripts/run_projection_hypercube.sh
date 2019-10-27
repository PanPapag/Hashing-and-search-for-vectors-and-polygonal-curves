cd ../curves/projection_hypercube
make
cd build/
./curve_projection_hypercube -d ../../../datasets/curves/trajectories_projection_dataset.txt -q ../../../datasets/curves/trajectories_projection_query.txt --k_vec 4 --L_grid 4 -e 0.5 -o ../../../results/curves/projection
cd ..
make clean
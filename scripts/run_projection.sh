cd ../curves/projection
make
cd build/
./projection -d ../../../datasets/curves/trajectories_projection_dataset.txt -q ../../../datasets/curves/trajectories_projection_query.txt --k_vec 4 --L_grid 4 -o ../../../results/curves/projection
cd ..
make clean
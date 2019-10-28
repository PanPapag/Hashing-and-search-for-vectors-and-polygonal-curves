cd ../curves/projection_lsh
make
cd build/
./curve_projection_lsh -d ../../../datasets/curves/trajectories_projection_dataset.txt -q ../../../datasets/curves/trajectories_projection_query.txt --k_vec 4 --L_vec 4 -e 0.5 -o ../../../results/curves/projection
cd ..
make clean
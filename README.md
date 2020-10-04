# Hashing-and-search-for-vectors-and-polygonal-curves
The present project implements:
 
**A**. The LSH algorithm for vectors on d-dimensional space based on the Manhatan metric (L1), as well as, the Hypercube algorithm based on the same metric. Having as input a vector q, the program returns:
 * The nearest neighbour of q 
 * All vectors at distance R from q

**B**. The LSH and Hypercube algorithms for polygonal curves. Having as input a polygonal curve q, the program finds its nearest neighbour. It supports the Dynamic Time Wrapping for calculating the distance between two polygonal curves.

**Notes**: 

1. The LSH for curves projects the polygonal curves on grid curves, which are deduced to vectors. ΅e implement storage and search in grid curves with:
   - The LSH algorithm using Manhatan metric
   - The Hypercube algorithm using Manhatan metric
2. The Hypercube algorithm deduces curves to vectors and execute search in the vectors' space. Τhese result from possible traversals of the data with curves of as long as possible the length of the query curve.
3. The design of the code allows its easy extension for different curve spacing functions and different metrics

## Directory Structure

    ├── core                  # The core functionalities of the prohect
         ├── hash             # Hash functions used by LSH and Hypercube 
         ├── metric           # DTW, Manhatan metric (L1)
         ├── search           # BF, LSH and Hypercube algorithms
         ├── utils            # General utilities
    ├── curves                # The code structure for curves
         ├── core             # The core functionalities for curves
         ├── grid_hybercube   
         ├── grid_lsh        
         ├── project_hybecube 
         ├── project_lsh     
    ├── datasets              # Plethora of datasets for vectors and curves
    ├── results               # Output directory for the experiments
    ├── scripts               # All scripts to compile and run the experiments
    ├── vectors               # The code structure for curves
         ├── lsh              
         ├── hybercube        

## Installing

Download source code by typing:

```
https://github.com/PanPapag/Hashing-and-search-for-vectors-and-polygonal-curves.git
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

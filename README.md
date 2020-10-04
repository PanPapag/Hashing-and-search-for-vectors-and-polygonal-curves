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

    ├── core                  # The core of 
         ├── hash             # Fonts, images, and sounds for UI
         ├── metric           # Fonts, images, and sounds for UI
         ├── search           # Fonts, images, and sounds for UI
         ├── utils            # Fonts, images, and sounds for UI
    ├── curves                # The messaging spec and libs used for all logs
         ├── core             # Fonts, images, and sounds for UI
         ├── grid_hybercube   # Fonts, images, and sounds for UI
         ├── grid_lsh         # Fonts, images, and sounds for UI
         ├── project_hybecube # Fonts, images, and sounds for UI
         ├── project_lsh      # Fonts, images, and sounds for U
    ├── datasets              # Library like functionality we've developed here
    ├── results               # Manages auto-updates of openpilot
    ├── scripts               # Files showing how to interpret data from cars
    ├── vectors               # Code used to communicate on CAN
         ├── lsh              # Fonts, images, and sounds for UI
         ├── hybercube        # Fonts, images, and sounds for UI

## Installing

Download source code by typing:

```
https://github.com/PanPapag/Hashing-and-search-for-vectors-and-polygonal-curves.git
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

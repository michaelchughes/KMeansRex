KMeansRex : Fast C++ implementation of K-Means using the Eigen matrix template library, with hooks to Matlab and Python/Numpy

This toolbox is meant to be the tyrant lizard king of k-means implementations. It is at least 2x as fast as the native kmeans implementations provided by matlab and scipy. It provides smart modern kmeans++ initialization as a default. Code is fully vectorized using the excellent Eigen matrix template library.

# Repo Contents

* src/    : C++ source files and header files
* matlab/ : matlab MEX source files
* python/ : python package files

# Dependencies

* Eigen Matrix Template Library (http://eigen.tuxfamily.org)
* Python 2.7 (for python functionality)
* * Requires numpy and other standard numerical packages.
* Matlab R2011 or greater (for matlab functionality)

# Python Usage

Assuming your PYTHONPATH includes /path/to/KMeansRex/python/
```
  >> import numpy as np
  >> import KMeansRex
  >> X = np.random.rand(1000,5)
  >> Mu, Z = KMeansRex.RunKMeans(X, 3)
```

# Matlab Usage 

Assuming your current path includes /path/to/KMeansRex/matlab/
```
  >> X = rand(1000, 5)
  >> [Mu,Z] = KMeansRex( X, 3 );
```

# C++ Usage

In your source file, include KMeansRexCoreInterface.h

```
    #include "KMeansRexCoreInterface.h"
```

Then you can call public functions like RunKMeans as needed.


# Compiling

To build everything for Matlab
```
    $ cd /path/to/KMeansRex/
    $ make matlab
```

To build everything for Python

```
    $ cd /path/to/KMeansRex/
    $ make python
```

To build a custom C++ application

When compiling, modify your include and library paths as necessary.
```
    g++ main.cpp -o main \
        -I/path/to/eigen \
        -I/path/to/KMeansRex/src/ \
        -L/path/to/KMeansRex/lib/ \
        -Wl,-rpath=/path/to/KMeansRex/lib/ -lkmeansrex[32|64]
```

All Makefile builds require a correct path to the Eigen library in the environment variable EIGEN_INCLUDE, which will be detected automatically via pkg-config if possible. See Makefile for details.

Useful flags for eigen to suppress many warnings:
* -Wno-ignored-attributes 
* -Wno-misleading-indentation
* -Wno-deprecated-declarations

# Author:      
Mike Hughes (www.michaelchughes.com). Please email all comments/questions to mike AT michaelchughes.com

# License
This software is released under a BSD 3-clause license. See the LICENSE file for details.

# Acknowledgements
Thanks to Alexander Grebenyuk and Kaveh Vakili for many helpful suggestions which improved the organization and usefulness of this code.

Includes the random number generation code provided by Makoto Matsumoto and Takuji Nishimura. Source: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html.



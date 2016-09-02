.PHONY: all clean python matlab lib

# Determine architecture (32 or 64 bit)
ARCH := $(shell getconf LONG_BIT)

# Find header files for the Eigen matrix library
# Usually in a standard location like /usr/include/eigen3/
# Change manually here if needed
EIGEN_INCLUDE := $(shell pkg-config --cflags eigen3)

# Remove nasty Eigen warnings when compiling
CXX_EIGEN_FLAGS := -Wno-ignored-attributes -Wno-misleading-indentation -Wno-deprecated-declarations
CXX_FLAGS := -fPIC -shared -O3 -DNDEBUG $(CXX_EIGEN_FLAGS)

CWD = $(shell pwd)
LIBFILE := $(CWD)/lib/libkmeansrex$(ARCH).so

all: matlab python

lib: src/KMeansRexCore.cpp
	# Create lib/ directory, or be silent if already exists
	mkdir -p lib/
	# Create shared library LIBFILE
	g++ $(CXX_FLAGS) $(EIGEN_INCLUDE) src/KMeansRexCore.cpp -o $(LIBFILE)

python: lib

matlab: lib matlab/KMeansRex.cpp
	mex matlab/KMeansRex.cpp $(LIBFILE) \
		-outdir matlab/ \
		$(EIGEN_INCLUDE) \
		-Isrc/

clean:
	$(RM) $(LIBFILE)
	$(RM) matlab/KMeansRex.mex*

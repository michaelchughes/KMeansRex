# Find header files for the Eigen matrix library
# Usually in a standard location like /usr/include/eigen3/
# Change manually here if needed
EIGEN_INCLUDE := $(shell pkg-config --cflags eigen3)

CXX_EIGEN_FLAGS := -Wno-ignored-attributes -Wno-misleading-indentation -Wno-deprecated-declarations
CXX_OPT_FLAGS := -O3 -DNDEBUG
CXX_FLAGS := $(CXX_OPT_FLAGS) $(CXX_EIGEN_FLAGS)

simple_demo: simple_demo.cpp
	g++ $(CXX_FLAGS) $(EIGEN_INCLUDE) -I../src/ simple_demo.cpp -o simple_demo

all: simple_demo

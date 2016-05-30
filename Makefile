ARCH := $(shell getconf LONG_BIT)

#Change eigen version here if needed
EIGEN_INCLUDE := $(shell pkg-config --cflags eigen3)

#To remove nasty Eigen warnings when compiling with gcc 6
CXX_EIGEN_FLAGS := -Wno-ignored-attributes -Wno-misleading-indentation -Wno-deprecated-declarations
CXX_FLAGS := -fPIC -shared -O3 -DNDEBUG $(CXX_EIGEN_FLAGS)

LIBNAME := kmeansrex

all:
	g++ $(CXX_FLAGS) -o lib$(LIBNAME)$(ARCH).so KMeansRexCore.cpp $(EIGEN_INCLUDE)
clean:
	$(RM) lib$(LIBNAME)$(ARCH).so

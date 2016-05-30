ARCH := $(shell getconf LONG_BIT)

#Change eigen version here if needed
EIGEN_INCLUDE := $(shell pkg-config --cflags eigen3)

#To remove nasty Eigen warnings when compiling with gcc 6
CXX_EIGEN_FLAGS := -Wno-ignored-attributes -Wno-misleading-indentation -Wno-deprecated-declarations
CXX_FLAGS := -fPIC -shared -O3 -DNDEBUG $(CXX_EIGEN_FLAGS)

LIBNAME := kmeansrex

#CXX_SOURCES := main.cpp
#CXX_INCLUDE := -Iinclude0 ... -IincludeN

KMEANSREX_PATH := ..

all:
	g++ $(CXX_SOURCES) -o example $(CXX_INCLUDE) $(EIGEN_INCLUDE) -I$(KMEANSREX_PATH) -L$(KMEANSREX_PATH) -Wl,-rpath=$(KMEANSREX_PATH) -l$(LIBNAME)$(ARCH)
clean:
	rm -f example

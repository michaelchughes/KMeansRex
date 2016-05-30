all:
	g++ --shared -o libkmeansrex.so KMeansRexCore.cpp -I/usr/include/eigen3 -O3 -DNDEBUG -Wno-ignored-attributes -Wno-misleading-indentation -Wno-deprecated-declarations -fPIC
clean:
	rm -f libkmeansrex.so

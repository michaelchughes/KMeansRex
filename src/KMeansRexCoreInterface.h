#pragma once
#ifndef KMEANSREX_INTERFACE_H
#define KMEANSREX_INTERFACE_H

extern "C" {
	void RunKMeans(double *X_IN,  int N,  int D, int K, int Niter, \
	           int seed, char* initname, double *Mu_OUT, double *Z_OUT);
	void SampleRowsPlusPlus(double *X_IN,  int N,  int D, int K, int seed, double *Mu_OUT);
}
#endif

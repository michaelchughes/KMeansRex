#pragma once
void RunKMeans(double *X_IN,  int N,  int D, int K, int Niter, \
               int seed, char* initname, double *Mu_OUT, double *Z_OUT);

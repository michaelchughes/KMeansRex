/* KMeansRexCore.cpp
A fast, easy-to-read implementation of the K-Means clustering algorithm.
allowing customized initialization (random samples or plus plus)
and vectorized execution via the Eigen matrix template library.

Intended to be compiled as a shared library which can then be utilized
from high-level interactive environments, such as Matlab or Python.

Contains
--------

Utility Fcns 
* discrete_rand : sampling discrete random variable
* select_without_replacement : sample without replacement

Cluster Location Mu Initialization:
* sampleRowsRandom : sample rows of X at random (w/out replacement)
* sampleRowsPlusPlus : sample rows of X via kmeans++ procedure of Arthur et al.
    see http://en.wikipedia.org/wiki/K-means%2B%2B

K-Means Algorithm (aka Lloyd's Algorithm)
* run_lloyd : executes lloyd for specfied number of iterations

External "C" function interfaces (for calling from Python)
* RunKMeans          : compute cluster centers and assignments via lloyd
* SampleRowsPlusPlus : get just a plusplus initialization

Dependencies:
  mersenneTwister2002.c : random number generator

Author: Mike Hughes (www.michaelchughes.com)
Date:   2 April 2013
*/

#include <iostream>
#include "KMeansRexCoreInterface.h"
#include "KMeansRexCore.h"
#include "mersenneTwister2002.c"

namespace KMeansRex {

    using namespace Eigen;
    using namespace std;

    template<typename T>
    KMeansRex<T>::KMeansRex() {};

    template<typename T>
    KMeansRex<T>::KMeansRex(T *X_IN, int N, int D, int K, int Niter,
                            int seed, char *initname, T *Mu_OUT, T *Z_OUT) {
        set_seed(seed);

        ExtMat X(X_IN, N, D);
        ExtMat Mu(Mu_OUT, K, D);
        ExtMat Z(Z_OUT, N, 1);

        init_Mu(X, Mu, initname);
        run_lloyd(X, Mu, Z, Niter);
    }

    template<typename T>
    void KMeansRex<T>::SampleRowsPlusPlus(T *X_IN, int N, int D,
                                          int K, int seed, T *Mu_OUT) {
        set_seed(seed);

        ExtMat X(X_IN, N, D);
        ExtMat Mu(Mu_OUT, K, D);

        sampleRowsPlusPlus(X, Mu);
    }


    // ====================================================== Utility Functions

    template<typename T>
    void KMeansRex<T>::set_seed(int seed) {
        init_genrand(seed);
    }

    template<typename T>
    int KMeansRex<T>::discrete_rand(Vec &p) {
        double total = p.sum();
        int K = (int) p.size();

        double r = total * genrand_double();
        double cursum = p(0);
        int newk = 0;
        while (r >= cursum && newk < K - 1) {
            newk++;
            cursum += p[newk];
        }
        if (newk < 0 || newk >= K) {
            cerr << "Badness. Chose illegal discrete value." << endl;
            return -1;
        }
        return newk;
    }

    template<typename T>
    void KMeansRex<T>::select_without_replacement(int N, int K, Vec &chosenIDs) {
        Vec p = Vec::Ones(N);
        for (int kk = 0; kk < K; kk++) {
            int choice;
            int doKeep = false;
            while (doKeep == false) {
                doKeep = true;
                choice = discrete_rand(p);

                for (int previd = 0; previd < kk; previd++) {
                    if (chosenIDs[previd] == choice) {
                        doKeep = false;
                        break;
                    }
                }
            }
            chosenIDs[kk] = choice;
        }
    }

    // ======================================================= Init Cluster Locs Mu

    template<typename T>
    void KMeansRex<T>::sampleRowsRandom(ExtMat &X, ExtMat &Mu) {
        int N = X.rows();
        int K = Mu.rows();
        Vec ChosenIDs = Vec::Zero(K);
        select_without_replacement(N, K, ChosenIDs);
        for (int kk = 0; kk < K; kk++) {
            Mu.row(kk) = X.row(ChosenIDs[kk]);
        }
    }

    template<typename T>
    void KMeansRex<T>::sampleRowsPlusPlus(ExtMat &X, ExtMat &Mu) {
        int N = X.rows();
        int K = Mu.rows();
        if (K > N) {
            // User requested more clusters than we have available.
            // So, we'll fill only first N rows of Mu
            // and leave all remaining rows of Mu uninitialized.
            K = N;
        }
        Vec ChosenIDs = Vec::Ones(K);
        int choice = discrete_rand(ChosenIDs);
        Mu.row(0) = X.row(choice);
        ChosenIDs[0] = choice;
        Vec minDist(N);
        Vec curDist(N);
        for (int kk = 1; kk < K; kk++) {
            curDist = (X.rowwise() - Mu.row(kk - 1)).square().rowwise().sum().sqrt();
            if (kk == 1) {
                minDist = curDist;
            } else {
                minDist = curDist.min(minDist);
            }
            choice = discrete_rand(minDist);
            ChosenIDs[kk] = choice;
            Mu.row(kk) = X.row(choice);
        }
    }

    template<typename T>
    void KMeansRex<T>::init_Mu(ExtMat &X, ExtMat &Mu, char *initname) {
        if (string(initname) == "random") {
            sampleRowsRandom(X, Mu);
        } else if (string(initname) == "plusplus") {
            sampleRowsPlusPlus(X, Mu);
        }
    }

    // ======================================================= Update Assignments Z
    template<typename T>
    void KMeansRex<T>::pairwise_distance(ExtMat &X, ExtMat &Mu, Mat &Dist) {
        int N = X.rows();
        int D = X.cols();
        int K = Mu.rows();

        // For small dims D, for loop is noticeably faster than fully vectorized.
        // Odd but true.  So we do fastest thing
        if (D <= 16) {
            for (int kk = 0; kk < K; kk++) {
                Dist.col(kk) = (X.rowwise() - Mu.row(kk)).square().rowwise().sum();
            }
        } else {
            Dist = -2 * (X.matrix() * Mu.transpose().matrix());
            Dist.rowwise() += Mu.square().rowwise().sum().transpose().row(0);
        }
    }

    template<typename T>
    double KMeansRex<T>::assignClosest(ExtMat &X, ExtMat &Mu, ExtMat &Z, Mat &Dist) {
        double totalDist = 0;
        int minRowID;

        pairwise_distance(X, Mu, Dist);

        for (int nn = 0; nn < X.rows(); nn++) {
            totalDist += Dist.row(nn).minCoeff(&minRowID);
            Z(nn, 0) = minRowID;
        }
        return totalDist;
    }

    // ======================================================= Update Locations Mu
    template<typename T>
    void KMeansRex<T>::calc_Mu(ExtMat &X, ExtMat &Mu, ExtMat &Z) {
        //Mu = Mat::Zero(Mu.rows(), Mu.cols());
        Mu.fill(0);
        Vec NperCluster = Vec::Zero(Mu.rows());
        for (int nn = 0; nn < X.rows(); nn++) {
            Mu.row((int) Z(nn, 0)) += X.row(nn);
            NperCluster[(int) Z(nn, 0)] += 1;
        }
        NperCluster += 1e-100; // avoid division-by-zero
        for (int k = 0; k < Mu.rows(); k++) {
            Mu.row(k) /= NperCluster(k);
        }
    }

    // ======================================================= Overall Lloyd Alg.
    template<typename T>
    void KMeansRex<T>::run_lloyd(ExtMat &X, ExtMat &Mu, ExtMat &Z, int Niter) {
        double prevDist, totalDist = 0;
        Mat Dist = Mat::Zero(X.rows(), Mu.rows());

        for (int iter = 0; iter < Niter; iter++) {
            totalDist = assignClosest(X, Mu, Z, Dist);
            calc_Mu(X, Mu, Z);
            if (prevDist == totalDist) {
                break;
            }
            prevDist = totalDist;
        }
    }

    template class KMeansRex<float>;
    template class KMeansRex<double>;
}

// ===========================================================================
// ===========================================================================
// ===========================  EXTERNALLY CALLABLE FUNCTIONS ================
// ===========================================================================
// ===========================================================================

void RunKMeans(double *X_IN,  int N,  int D, int K, int Niter, \
               int seed, char* initname, double *Mu_OUT, double *Z_OUT) {
    KMeansRex::KMeansRex<double>(X_IN, N, D, K, Niter, seed, initname, Mu_OUT, Z_OUT);
}

void SampleRowsPlusPlus(double *X_IN,  int N,  int D, int K, \
                        int seed, double *Mu_OUT) {
    KMeansRex::KMeansRex<double> kmeansrex;
    kmeansrex.SampleRowsPlusPlus(X_IN, N, D, K, seed, Mu_OUT);
}

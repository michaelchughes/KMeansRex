/* KMeansRexCore.h
   Provides header file function declarations for Matlab MEX compilation.
*/

#include "Eigen/Dense"
#include <iostream>


namespace KMeansRex {

    using namespace Eigen;
    using namespace std;

/*
 * DEFINE Custom Type Names to make code more readable
 * ExtMat :  2-dim matrix/array externally defined (e.g. in Matlab or Python)
 */

    template<typename T>
    class KMeansRex {

        typedef Array<T, Dynamic, Dynamic> Mat;
        typedef Array<T, Dynamic, 1> Vec;
        typedef Map<Mat> ExtMat;

    public:
        KMeansRex();

        KMeansRex(T *X_IN,  int N,  int D, int K, int Niter,
                  int seed, char* initname, T *Mu_OUT, T *Z_OUT);

        void SampleRowsPlusPlus(T *X_IN,  int N,  int D,
                                int K, int seed, T *Mu_OUT);

    private:
        // ====================================================== Utility Functions
        void set_seed( int seed );
        int discrete_rand( Vec &p );
        void select_without_replacement( int N, int K, Vec &chosenIDs);

        // ======================================================= Init Cluster Locs Mu
        void sampleRowsRandom( ExtMat &X, ExtMat &Mu );
        void sampleRowsPlusPlus( ExtMat &X, ExtMat &Mu );
        void init_Mu( ExtMat &X, ExtMat &Mu, char* initname );

        // ======================================================= Update Assignments Z
        void pairwise_distance( ExtMat &X, ExtMat &Mu, Mat &Dist );
        double assignClosest( ExtMat &X, ExtMat &Mu, ExtMat &Z, Mat &Dist);

        // ======================================================= Update Locations Mu
        void calc_Mu( ExtMat &X, ExtMat &Mu, ExtMat &Z);

        // ======================================================= Overall Lloyd Alg.
        void run_lloyd( ExtMat &X, ExtMat &Mu, ExtMat &Z, int Niter );
    };

}


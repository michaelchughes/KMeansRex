#include <iostream>
#include "Eigen/Dense"
#include "KMeansRexCore.cpp"

using namespace Eigen;
using namespace std;

IOFormat CleanFmt(3, 0, " ", "\n", "[", "]");

int main(int, char*[]) {
    int K = 3;
    int n_examples_per_cluster = 25;
    int n_features = 2;
    int n_iters = 100;
    int seed = 42;
    int n_examples_ttl = K * n_examples_per_cluster;

    // Create random arrays of data from 3 clusters
    // Each example observation is a vector of size 2
    // Cluster 1 is centered at (-5, -5) with uniform random noise
    // Cluster 2 is centered at (0, 0) with uniform random noise
    // Cluster 3 is centered at (+5, +5) with uniform random noise
    Eigen::ArrayXXd data_1 = -5 + Eigen::ArrayXXd::Random(
        n_examples_per_cluster, n_features);
    Eigen::ArrayXXd data_2 = 0 + Eigen::ArrayXXd::Random(
        n_examples_per_cluster, n_features);
    Eigen::ArrayXXd data_3 = 5 + Eigen::ArrayXXd::Random(
        n_examples_per_cluster, n_features);

    // Stack all data together into larger 2D array
    Eigen::ArrayXXd data_all(n_examples_ttl, n_features);
    data_all << data_1, data_2, data_3;

    // Display the data
    cout << "data_all\n";
    cout << data_all.format(CleanFmt) << "\n";

    Eigen::ArrayXXd mu = Eigen::ArrayXXd::Zero(K, n_features);
    Eigen::ArrayXd z = Eigen::ArrayXd::Zero(n_examples_ttl);

    cout << "running kmeans ... \n";
    RunKMeans(
        data_all.data(),
        n_examples_ttl, n_features, K,
        n_iters, seed, "plusplus",
        mu.data(),
        z.data());
    cout << "done.\n";

    cout << "estimated clusters:\n";
    cout << mu.format(CleanFmt) << "\n";

}
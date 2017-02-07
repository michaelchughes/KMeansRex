import numpy as np
import os
import sys
sys.path.append(os.path.abspath("../../python/KMeansRex"))
import KMeansRex

def make_data(n_examples_per_cluster=20, seed=0):
    prng = np.random.RandomState(int(seed))
    x1 = -5 + 0.1 * prng.randn(n_examples_per_cluster, 2)
    x2 =  0 + 0.1 * prng.randn(n_examples_per_cluster, 2)
    x3 = +5 + 0.1 * prng.randn(n_examples_per_cluster, 2)
    return np.vstack([x1, x2, x3])

if __name__ == '__main__':
    x_ND = make_data()

    seed = 42
    m_KD = KMeansRex.SampleRowsPlusPlus(x_ND, 3, seed=seed)
    print 'INIT mu (KxD):'
    print m_KD

    m_KD, z_N = KMeansRex.RunKMeans(
        x_ND, 3, Niter=25, initname='plusplus', seed=seed)
    print
    print "FINAL mu (KxD):"
    print m_KD

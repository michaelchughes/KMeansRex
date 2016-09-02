''' KMeansRex.py

Provides python interface to the KMeansRex C++ library
  
Provides two key functions:
* RunKMeans : runs kmeans on provided data matrix X

* SampleRowsPlusPlus : runs plusplus sampling on the rows of data matrix X

Notes
-------
Eigen by default expects the matrices to be fortran formated (column-major ordering).
In contrast, Numpy defaults to C-format (row-major ordering)
All functions here take care of this under the hood (so end-user doesn't need to worry about alignment)
This explains the mysterious line: X=np.asarray(X, order='F')
However, we do *return* values that are F-ordered by default. 
This should be fine with most code, but just in case wanted to issue a warning.
'''
import os
import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes
import platform
import warnings

curdir = os.path.split(os.path.abspath( __file__))[0]
curdirList = curdir.split(os.path.sep)
reporootdir = os.path.sep.join(curdirList[:-2])

if platform.architecture()[0] == '32bit':
    lib = ctypes.cdll.LoadLibrary(
        os.path.join(reporootdir,'lib/libkmeansrex32.so'))
else:
    lib = ctypes.cdll.LoadLibrary(
        os.path.join(reporootdir,'lib/libkmeansrex64.so'))

lib.SampleRowsPlusPlus.restype = None
lib.SampleRowsPlusPlus.argtypes = \
    [ndpointer(ctypes.c_double),
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ndpointer(ctypes.c_double)]

lib.RunKMeans.restype = None
lib.RunKMeans.argtypes = [
    ndpointer(ctypes.c_double),
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_char_p,
    ndpointer(ctypes.c_double),
    ndpointer(ctypes.c_double)]

def SampleRowsPlusPlus( X, K, seed=42 ):
    X = np.asarray(X, order='F')
    N,D = X.shape
    Kfeasible = np.minimum(K, N)
    if Kfeasible < K:
        showKTooLargeWarning(K, Kfeasible)
    K = Kfeasible
    Mu = np.zeros((K,D), order='F')
    lib.SampleRowsPlusPlus(X, N, D, K, seed, Mu)
    return Mu

def RunKMeans( X, K, Niter=100, initname='plusplus', seed=42 ):
    X = np.asarray(X, order='F')
    N,D = X.shape
    Kfeasible = np.minimum(K, N)
    if Kfeasible < K:
        showKTooLargeWarning(K, Kfeasible)
    K = Kfeasible
    Mu = np.zeros((K,D), order='F')
    Z  = np.zeros((N,1), order='F')
    lib.RunKMeans(X, N, D, K, Niter, seed, initname, Mu, Z)
    return Mu, Z

def showKTooLargeWarning(K, Kfeasible):
    warnings.warn(
        "Provided num of clusters K=%d exceeds total possible." % (K) + \
        " Using K=%d instead." % (Kfeasible),
        RuntimeWarning)


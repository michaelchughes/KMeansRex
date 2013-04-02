''' KMeansRex.py
  Provides python interface to the KMeansRex C++ library
  
  Provides two key functions:
    RunKMeans : runs kmeans from scratch on provided data matrix X (each row is an iid observation)
    SampleRowsPlusPlus : runs plusplus sampling on the rows of provided data matrix X

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

curdir = os.path.split( __file__ )[0]
parentdir = os.path.split( curdir)[0]

lib = ctypes.cdll.LoadLibrary( os.path.join(parentdir,'libkmeansrex.so') )
lib.SampleRowsPlusPlus.restype = None
lib.SampleRowsPlusPlus.argtypes = \
               [ndpointer(ctypes.c_double),
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ndpointer(ctypes.c_double)]

lib.RunKMeans.restype = None
lib.RunKMeans.argtypes = [ndpointer(ctypes.c_double),
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_char_p,
                ndpointer(ctypes.c_double),
                ndpointer(ctypes.c_double)]

def SampleRowsPlusPlus( X, K, seed=42 ):
  X = np.asarray( X, order='F')
  N,D = X.shape

  Mu = np.zeros( (K,D), order='F' )
  lib.SampleRowsPlusPlus( X, N, D, K, seed, Mu)
  return Mu

def RunKMeans( X, K, Niter=100, initname='plusplus', seed=42 ):
  X = np.asarray( X, order='F')
  N,D = X.shape

  Mu = np.zeros( (K,D), order='F'  )
  Z  = np.zeros( (N,1), order='F' )
  lib.RunKMeans( X, N, D, K, Niter, seed, initname, Mu, Z)
  return Mu, Z

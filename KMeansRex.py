''' KMeansRex.py
  Provides python interface to the KMeansRex C++ library
  
  Key function is RunKMeans

  The function "MyWrapper" provides a more "Pythonic" interface.  It takes only the input "A", and creates "B", fills it, and returns it.

  COMPILE (ANY TERMINAL)
  -------
  g++ -O3 -DNDEBUG --shared -o libkmeansrex.so KMeansRexCore.cpp -I/home/mhughes/code/eigen/
  
'''

import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes

lib = ctypes.cdll.LoadLibrary('./libkmeansrex.so')
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

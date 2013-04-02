'''
  Modules shows how to call a C++ function that does matrix/array math.
  Using numpy arrays as both input and output.

  The core function of the C++ call is to fill a designated output array "B"
    so that its entries are twice the corresponding entry in A
  Essentially, we're using C++ to perform:
    >> B = 2*A

  The function "MyFunc" here executes the raw C++ command, assuming the output array "B" has already be allocated.

  The function "MyWrapper" provides a more "Pythonic" interface.  It takes only the input "A", and creates "B", fills it, and returns it.

  COMPILE (ANY TERMINAL)
  -------
  gcc --shared -o cfun.so cfun.c

  RUN (ANY PYTHON PROMPT)
  -------
  >>import MyFunc
  >>A = np.ones( (2,2) )
  >>B = np.zeros( A.shape )
  >>MyFunc.MyFunc( A, B)
  >>print B
  [[ 2.  2.]
   [ 2.  2.]]
  >>B = MyFunc.MyWrapper( A )
  >>print B
  [[ 2.  2.]
   [ 2.  2.]]
'''

import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes

lib = ctypes.cdll.LoadLibrary('./cfun.so')
fun = lib.cfun
fun.restype = None
fun.argtypes = [ndpointer(ctypes.c_double),
                ctypes.c_size_t,
                ndpointer(ctypes.c_double)]

def MyFunc( A, B):
  assert A.size == B.size
  fun( A, A.size, B)

def MyWrapper( A ):
  B = np.empty( A.shape )
  fun( A, A.size, B)
  return B

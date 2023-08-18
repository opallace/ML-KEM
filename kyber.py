import numpy as np
from numpy.polynomial.polynomial import Polynomial

import random

def add_poly(a, b, q):
  # adds two polynomials modulo q
  result = [0] * max(len(a), len(b))
  for i in range(max(len(a), len(b))):
    if i < len(a):
      result[i] += a[i]
    if i < len(b):
      result[i] += b[i]
    result[i] %= q
  return result


def inv_poly(a, q):
  return list(map(lambda x: -x % q, a))


def sub_poly(a, b, q):
  return add_poly(a, inv_poly(b, q), q)


def mul_poly_simple(a, b, f, q):
  tmp = [0] * (len(a) * 2 - 1) # the product of two degree n polynomial cannot exceed 2n
  
  # schoolbook multiplication
  for i in range(len(a)):
    # perform a_i * b
    for j in range(len(b)):
      tmp[i + j] += a[i] * b[j]
  
  # take polynomial modulo f
  # since Kyber's poly modulus is always x^n + 1,
  # we can efficiently compute the remainder
  degree_f = len(f) - 1
  for i in range(degree_f, len(tmp)):
    tmp[i - degree_f] -= tmp[i]
    tmp[i] = 0

  # take coefficients modulo q
  tmp = list(map(lambda x: x % q, tmp))
  return tmp[:degree_f]

def add_vec(v0, v1, q):
  assert(len(v0) == len(v1)) # sizes need to be the same

  result = []

  for i in range(len(v0)):
    result.append(add_poly(v0[i], v1[i], q))
  
  return result


def mul_vec_simple(v0, v1, f, q):
  assert(len(v0) == len(v1)) # sizes need to be the same

  degree_f = len(f) - 1
  result = [0 for i in range(degree_f - 1)]

  # textbook vector inner product
  for i in range(len(v0)):
    result = add_poly(result, mul_poly_simple(v0[i], v1[i], f, q), q)
  
  return result


def mul_mat_vec_simple(m, a, f, q):
  result = []
  
  # textbook matrix-vector multiplication
  for i in range(len(m)):
    result.append(mul_vec_simple(m[i], a, f, q))
  
  return result


def transpose(m):
  result = [[None for i in range(len(m))] for j in range(len(m[0]))]

  for i in range(len(m)):
    for j in range(len(m[0])):
      result[j][i] = m[i][j]
  
  return result

def encrypt(A, t, m_b, f, q, r, e_1, e_2):
  half_q = int(q / 2 + 0.5)
  m = list(map(lambda x: x * half_q, m_b))

  u = add_vec(mul_mat_vec_simple(transpose(A), r, f, q), e_1, q)
  v = add_poly(add_poly(mul_vec_simple(t, r, f, q), e_2, q), m, q)
  
  return u,v


def decrypt(s, u, v, f, q):
  m_n = sub_poly(v, mul_vec_simple(s, u, f, q), q)

  half_q = int(q / 2 + 0.5)

  def round(val, center, bound):
    dist_center = np.abs(center - val)
    dist_bound = min(val, bound - val)
    return center if dist_center < dist_bound else 0

  m_n = list(map(lambda x: round(x, half_q, q), m_n))
  m_b = list(map(lambda x: x // half_q, m_n))
  
  return m_b

q = 17 # plain modulus
f = [1, 0, 0, 0, 1] # poly modulus, x**4 + 1

s = [[0, 1, -1, -1], [0, -1, 0, -1]] # secret key, [-x**3-x**2+x, -x**3-x]
A = [[[11, 16, 16, 6], [3, 6, 4, 9]], [[1, 10, 3, 5], [15, 9, 1, 6]]] # public key
e = [[0, 0, 1, 0], [0, -1, 1, 0]] # noise
m_b = [1, 1, 0, 1] # message in binary

t = add_vec(mul_mat_vec_simple(A, s, f, q), e, q)

r = [[0, 0, 1, -1], [-1, 0, 1, 1]] # blinding vector for encrypt
e_1 = [[0, 1, 1, 0], [0, 0, 1, 0]] # noise vector for encrypt
e_2 = [0, 0, -1, -1] # noise poly for encrypt

u, v = encrypt(A, t, m_b, f, q, r, e_1, e_2)
# m_b2 = decrypt(s, u, v, f, q)

print(u)
print(v)


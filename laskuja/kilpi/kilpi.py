"""
Copyright (c) 2012 Pauli Henrikki Rikula 

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
"""

#ensimmäinen versio. toimii, jos sitäkään.

import math

h = 7.0
l = 70.0

r = ((0.5 * l)**2 + h**2 ) / (2*h)
print('r',r)

alpha = math.asin(0.5 * l / r)

alpha_degrees = math.degrees(alpha)

print('alpha',alpha, 'alpha_degrees', alpha_degrees)

s_l = alpha * r

print('s_l', s_l)

s_n = 8.

alpha_n_s = 2 * math.pi / s_n

alpha_n_s_degrees = math.degrees(alpha_n_s)

print('s_n', s_n)
print('alpha_n_s', alpha_n_s, 'alpha_n_s_degrees', alpha_n_s_degrees)

#mittapisteiden lkm m_n

m_n = 13

print('m_n', m_n)

alpha_m = {}

for i in range(m_n):
    alpha_m[i] = alpha - alpha/(2*m_n) * 2 * i

print('alpha_m', alpha_m)


l_m = {}


for i in range(m_n):
    l_m[i] = 2*r*math.sin(alpha_m[i])

print('l_m', l_m)

h_m = {}

for i in range(m_n):
    h_m[i] = (2*r - (
        (2*r)**2 -
        4 * (0.5 * l_m[i])**2
        )**0.5 )/2.

print('h_m', h_m)


#tarkistus ->
r_m = {}

for i in range(m_n):
    r_m[i] = ((0.5 * l_m[i])**2 + h_m[i]**2 ) / (2*h_m[i])

print('r_m', r_m)

#<- tarkistus

k_n = 8


print('k_n', k_n)

alpha_k = math.pi / k_n

alpha_k_degrees = math.degrees(alpha_k)

print('alpha_k',alpha_k, 'alpha_k_degrees', alpha_k_degrees)


s_u_m = {}

for i in range(m_n):
    s_u_m[i] = (0.5 * l_m[i] ) * 2 * math.sin(math.pi / k_n)


print('s_u_m', s_u_m)


s_s_m = {}

for i in range(m_n):
    s_s_m[i] = (0.5 * l_m[i] ) * math.cos(math.pi / k_n)


print('s_s_m', s_s_m)


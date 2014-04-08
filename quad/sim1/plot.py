import pylab as pl
import numpy as np
import os
import struct

vec_size = 3 * 8
vec4_size = 4 * 8

def read(f, N, c):

	v = np.zeros((N,c))
	
	for ti in range(N):
		v[ti] = struct.unpack('d'*c, f.read(8*c))

	return v

def plots(x,Y,xl,yl,L = None,S = None):
	
	if L is None:
		L = ['']
	if S is None:
		S = ['b']
	
	fig = pl.figure()
	ax = fig.add_subplot(111)
	
	leg = []
	
	for y,s,l in zip(Y,S,L):
		ax.plot(x,y,s)
		leg += [l]
	
	
	ax.set_xlabel(xl)
	ax.set_ylabel(yl)

	ax.legend(leg)

def plotv(x,Y,xl,yl,L = None,S = None):
	
	if L is None:
		L = ['']
		S = ['-']
	
	fig = pl.figure()
	ax = fig.add_subplot(111)

	leg = []
	
	clr = ['b','g','r','c']

	for y,s,l in zip(Y,S,L):
		for i in range(np.size(y,1)):
			ax.plot(x,y[:,i], clr[i] + s)
		leg += ['x'+l,'y'+l,'z'+l]
	
	
	ax.set_xlabel(xl)
	ax.set_ylabel(yl)

	ax.legend(leg)

def plotvn(x,Y,xl,L = None,S = None):
	
	if L is None:
		L = ['']
		S = ['-']
	
	fig = pl.figure()
	ax = fig.add_subplot(111)

	leg = []
	
	clr = ['b','g','r','c']
	
	for y,s,l in zip(Y,S,L):
		y = y / np.max(y)

		for i in range(np.size(y,1)):
			ax.plot(x,y[:,i], clr[i] + s)
		leg += ['x'+l,'y'+l,'z'+l]
	
	
	ax.set_xlabel(xl)

	ax.legend(leg)

def size(f):
	old_file_position = f.tell()
	f.seek(0, os.SEEK_END)
	size = f.tell()
	f.seek(old_file_position, os.SEEK_SET)
	return size

with open("pos.txt","rb") as f:
	
	types = 9*3 + 2*1

	N = size(f)/(8 * types)
	
	print N
	
	e1		= read(f ,N, 3)
	e2		= read(f ,N, 3)
	e3		= read(f ,N, 3)

	x		= read(f ,N, 3)
	x_ref		= read(f ,N, 3)
	x_ref_d		= read(f ,N, 3)
	x_ref_dd	= read(f ,N, 3)
	a		= read(f ,N, 3)
	i		= read(f ,N, 3)

	e1_mag_d	= read(f, N, 1)
	e1_mag_dd	= read(f, N, 1)


with open("att.txt","rb") as f:
	
	N = size(f)/(vec_size * 7)

	print N
	
	att_e3		= read(f ,N, 3)
	q		= read(f ,N, 3)
	o		= read(f, N, 3)
	q_ref		= read(f ,N, 3)
	q_ref_d		= read(f ,N, 3)
	q_ref_dd	= read(f ,N, 3)
	tau_RB		= read(f ,N, 3)

with open("plant.txt","rb") as f:
	
	types = 4 + 4 + 3 + 3 + 1 + 1
	
	N = size(f)/(types * 8)
	
	print N
	
	gamma1		= read(f, N, 4)
	gamma1_act	= read(f, N, 4)
	
	pl_tau_RB	= read(f, N, 3)
	pl_f_RB		= read(f, N, 3)
	
	gamma0		= read(f, N, 1)
	gamma0_act	= read(f, N, 1)
	
with open("brain.txt","rb") as f:
	types = 1*1
	N = size(f)/(types * 8)
	print N

	thrust		= read(f, N, 1)


t = np.arange(N) * 0.01

plotv(t,[e1],'t','e1')
plotv(t,[e2],'t','e2')
plotv(t,[e3],'t','e3')

plotv(t,[o],'t','o')

plots(t,[thrust],'t','thrust')

#plotvn(t,[e1,e3,a,i],'t',['e1','e3','a','i'],['-','--',':','-.'])


"""

#plots(t,[e1_mag_d],'t','e1_mag_d')
#plots(t,[e1_mag_dd],'t','e1_mag_dd')
plotv(t,[x,x_ref],'t','x',['','_ref'],['-','--'])
plotv(t,[x_ref_d],'t','x_ref_d')
plotv(t,[x_ref_dd],'t','x_ref_dd')
#plotv(t,[f_R],'t','f_R')

#plotv(t,[e3],'t','e3')
plotv(t,[q,q_ref],	't','q',	['','_ref'],['-','--'])
plotv(t,[o,q_ref_d],	't','q_ref_d',	['','_ref'],['-','--'])
plotv(t,[q_ref_dd],'t','q_ref_dd')
#plotv(t,[tau_RB],'t','tau_RB')
"""

#plotv(t,[gamma1,gamma1_act],'t','gamma',	['','_act'],['-','--'])

#plotv(t,[pl_tau_RB],'t','plant tau_RB')
#plotv(t,[pl_f_RB],'t','plant f_RB')



pl.show()




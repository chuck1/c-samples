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

def size(f):
	old_file_position = f.tell()
	f.seek(0, os.SEEK_END)
	size = f.tell()
	f.seek(old_file_position, os.SEEK_SET)
	return size

with open("pos.txt","rb") as f:
	
	N = size(f)/(vec_size * 6 + 8 * 2)
	
	print N
	
	e1 = np.zeros((N,3))
	x = np.zeros((N,3))
	x_ref = np.zeros((N,3))
	x_ref_d = np.zeros((N,3))
	x_ref_dd = np.zeros((N,3))
	f_R  = np.zeros((N,3))

	e1_mag_d = np.zeros(N)
	e1_mag_dd = np.zeros(N)

	for ti in range(N):
		e1[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		x[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		x_ref[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		x_ref_d[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		x_ref_dd[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		f_R[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		e1_mag_d[ti] = struct.unpack('d', f.read(8))[0]
	for ti in range(N):
		e1_mag_dd[ti] = struct.unpack('d', f.read(8))[0]


with open("att.txt","rb") as f:
	
	N = size(f)/(vec_size * 7)

	print N
	
	e3 = np.zeros((N,3))
	q = np.zeros((N,3))
	o = np.zeros((N,3))
	q_ref = np.zeros((N,3))
	q_ref_d = np.zeros((N,3))
	q_ref_dd = np.zeros((N,3))
	tau_RB  = np.zeros((N,3))
	
	
	for ti in range(N):
		e3[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		q[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		o[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		q_ref[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		q_ref_d[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		q_ref_dd[ti] = struct.unpack('ddd', f.read(vec_size))
	for ti in range(N):
		tau_RB[ti] = struct.unpack('ddd', f.read(vec_size))


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
	


t = np.arange(N) * 0.01


plotv(t,[e1],'t','e1', S=['o'])
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

plotv(t,[gamma1,gamma1_act],'t','gamma',	['','_act'],['-','--'])

plotv(t,[pl_tau_RB],'t','plant tau_RB')
plotv(t,[pl_f_RB],'t','plant f_RB')

pl.show()




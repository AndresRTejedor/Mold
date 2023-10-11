import numpy as np


f = open('all','r')
T=0.617
w=[]
ks=[]
b=0
for line in f:
	kT=line.split()[0]; a=0; eps=float(kT)*T; ks.append(eps); Nw=[]
	log = open('{}kT/log.lammps'.format(kT),'r')
	for i in log:
		try:
			ja=i.split()[0]
		except:
			a=0
		else:
			if ja=='Step':
				a=1
			elif ja=='Loop':
				a=0;
				if b: 
					w.append(np.mean(Nw)); b=0
		if a==1 and ja!='Step':
			t=float(ja)
			if t>9000:
				c=abs(float(i.split()[13]))
				N=abs(float(i.split()[15]))
				Nw.append(c*N/eps); b=1
f=open('fill.txt','w')
for i in range(len(w)):
	f.write('{0} {1}\n'.format(ks[i],w[i]))
f.close()
	

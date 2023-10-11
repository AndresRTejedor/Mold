import numpy as np
import re
import sys



i=0; Nw=[]; Nw1=[]; Eps=[];
match_number = re.compile('-?\ *[0-9]+\.?[0-9]*(?:[Ee]\ *-?\ *[0-9]+)?')
for line in open('Run.sh','r'):
    i+=1
    if i==1:
        SOL = re.findall(match_number,line);   T=int(SOL[0])
    elif i==2:
        SOL = re.findall(match_number,line);   Nwmax=int(SOL[0])
    elif i==3:
        SOL = re.findall(match_number,line);   rw=float(SOL[0])*3.405e-10
    elif i==4:
        SOL = re.findall(match_number,line);   steps=int(int(SOL[0])/1000+1)
        break

i=0
for line in open('./listaALL','r'):
    i+=1
    nw = np.loadtxt('{0}nkT/log.lammps'.format(line.split()[0]),skiprows=96,max_rows=int(steps-10),dtype=float)
    wells0=nw[:,7]; time0=nw[:,0]; Epav=np.mean(nw[50:200,3]); Epstd=np.std(nw[50:200,3])
    if nw[-1,3]-Epav<-1000: ###FILTER OF CRYSTALIZED DEPTHS
        print('WARNING: DEPTH {0} HAS CRYSTALIZED!!'.format(line.split()[0]))
        wells0=wells0[nw[:,3]>Epav-2*Epstd]; time0=time0[nw[:,3]>Epav-2*Epstd]
    
    wellsav=np.mean(wells0); wellsstd=np.std(wells0);  N=len(wells0)
    if abs((np.mean(wells0[-200:]))*4184/8.314/T/float(line.split()[0]))<Nwmax*0.8:
        print(float(line.split()[0]))
        index=(wells0>np.mean(wells0[-200:])-2*np.std(wells0[-200:]))
    else:
        index=(wells0>wellsav-5*wellsstd) * (wells0<wellsav+5*wellsstd)
    
    time1=time0[index]; wells1=wells0[index]
    wellsstd1=np.std(wells1); wellsav1=np.mean(wells1)
    index1=(wells1>wellsav1-20*wellsstd1)* (wells1<wellsav1+20*wellsstd1)
    time2=time1[index1]; wells2=wells1[index1]
    data0=np.zeros((len(time0),2)); data0[:,0]=time0*3; data0[:,1]=wells0*4184/T/8.314; np.savetxt('./FILL/OUTPUT0_{0}'.format(line.split()[0]),data0)
    data1=np.zeros((len(time1),2)); data1[:,0]=time1*3; data1[:,1]=wells1*4184/T/8.314; np.savetxt('./FILL/OUTPUT1_{0}'.format(line.split()[0]),data1)
    data2=np.zeros((len(time2),2)); data2[:,0]=time2*3; data2[:,1]=wells2*4184/T/8.314; np.savetxt('./FILL/OUTPUT2_{0}'.format(line.split()[0]),data2)
    eps = float(line.split()[0])*8.314*T/4184
    nm=np.mean(nw[50:,7]); nm1=np.mean(wells2)
    Nw.append(-nm/eps); Nw1.append(-nm1/eps)
    Eps.append(eps)
Eps1=Eps
Eps, Nw = zip(*sorted(zip(Eps, Nw)))
Eps1, Nw1 = zip(*sorted(zip(Eps1, Nw1)))
f = open('fill.txt','w+')
for i in range(len(Eps)):
    f.write(str(Eps[i]*(4184/8.314/T)) + ' ' + str(Nw[i]) + ' ' + str(Nw1[i]) + '\n')
f.close()
em = np.max(Eps)
print('DG* gives without filter ' + str((Nwmax*em-np.trapz(Nw,Eps))*4184/8.314/T) + ' kT')
print('DG is without filter ' + str((Nwmax*em-np.trapz(Nw,Eps))*4184/8.314/T-np.log(8*np.pi**2/(2.6e28*4/3*np.pi*rw**3))) + ' kT')
print('DG* gives with filter ' + str((Nwmax*em-np.trapz(Nw1,Eps))*4184/8.314/T) + ' kT')
print('DG is with filter ' + str((Nwmax*em-np.trapz(Nw1,Eps))*4184/8.314/T-np.log(8*np.pi**2/(2.6e28*4/3*np.pi*rw**3))) + ' kT')

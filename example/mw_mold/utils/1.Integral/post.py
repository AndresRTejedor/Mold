import numpy as np
import re

a=np.loadtxt('fill.txt')
i=0; Nw=[]; Eps=[];
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
em = np.max(a[:,0])
print('DG* gives ' + str((Nwmax*em)-np.trapz(a[:,1],a[:,0])) + ' kT')
print('DG is ' + str((Nwmax*em)-np.trapz(a[:,1],a[:,0])-np.log(8*np.pi**2/(3.386e28*4/3*np.pi*rw**3))) + ' kT')

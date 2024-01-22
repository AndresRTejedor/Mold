import numpy as np
import re
import sys



i=0; Time=[]; tim=[]
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
    elif i==5:
        SOL = re.findall(match_number,line);   nkT=SOL[0]
        break
i=0
for line in open('./list','r'):
    i+=1
    try:
        nw = np.loadtxt('{0}kT_{1}seed/log.lammps'.format(nkT,line.split()[0]),skiprows=94,max_rows=int(steps),dtype=float)
    except:
        print('Seed {0} has not run all steps...'.format(line.split()[0]))
        nw = np.loadtxt('{0}kT_{1}seed/log.lammps'.format(nkT,line.split()[0]),skiprows=99,dtype=float)
    t=nw[:,0]
    Ep=nw[:,3]
    A=Ep[-1]-np.min(Ep[:50])
    if (A)<-1000:
        auxt=t[Ep>(np.min(Ep[:100])-400)]
        print(np.min(Ep[:100]))
        Time.append(auxt[-1]*3)
    else:
        print('Seed {0} has not crystalized apparently'.format(line.split()[0]))
        tim.append(t[-1]*3)
print('\nThe times for all succesful seeds are:')
print(np.sort(Time))
print(Time)
print('Current time:')
print(tim)
print('\nAverage crystalization time is ' + str(np.mean(Time)) + ' fs')

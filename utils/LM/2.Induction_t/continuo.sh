T='240'
Nw='256'
rw='0.25'
kT='8'
while read s lista_cont; do
        echo ${kT}kT_${s}seed
        cd ${kT}kT_${s}seed
        cp  /home/r797/r797996/6_Lattice_Mold/set_up/Simulations_Ic/in_pocillos_continue.minimal .
        sed -i "5s/240.0/${T}/" in_pocillos_continue.minimal
        sed -i "6s/8/${kT}/" in_pocillos_continue.minimal
        sed -i "8s/0.25/${rw}/" in_pocillos_continue.minimal
        sbatch LAMMPS_continue.job
        cd ../
done <lista_cont

T='235'
Nw='239'
rw='0.33'
steps=20000000
kT='8'
while read s lista; do
        echo kT${kT}_seed${s}
        mkdir ${kT}kT_${s}seed
        cd ${kT}kT_${s}seed
        cp  /home/r797/r797996/6_Lattice_Mold/set_up/Simulations_Ic/in_* .
        cp  /home/r797/r797996/6_Lattice_Mold/set_up/Simulations_Ic/LAMMPS* .
        cp  /home/r797/r797996/6_Lattice_Mold/set_up/Simulations_Ic/mW.sw .
        cp  /home/r797/r797996/6_Lattice_Mold/set_up/Simulations_Ic/Data/data${Nw}.xyz .
        sed -i "5s/240.0/${T}/" in_pocillos.minimal
        sed -i "5s/240.0/${T}/" in_pocillos_continue.minimal
        sed -i "6s/8/${kT}/" in_pocillos.minimal
        sed -i "6s/8/${kT}/" in_pocillos_continue.minimal
        sed -i "8s/0.25/${rw}/" in_pocillos.minimal
        sed -i "8s/0.25/${rw}/" in_pocillos_continue.minimal
        sed -i "12s/data.xyz/data${Nw}.xyz/" in_pocillos.minimal
        sed -i "37s/91792/$(( $RANDOM*3+50000 ))/" in_pocillos.minimal
        sed -i "43s/1000000/$steps/" in_pocillos.minimal
        sed -i "3s/Ic/${Nw}Ic${s}seed/" LAMMPS.job
        sed -i "3s/Icont/${Nw}Icont${s}seed/" LAMMPS_continue.job
        sed -i "26s/#cp /cp /" LAMMPS.job
        sed -i "27s/#sbatch /sbatch /" LAMMPS.job
        sbatch LAMMPS.job
        cd ../
done <lista

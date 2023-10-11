T='0.617'
P='-0.02'
rw='0.30'
steps=250000
kT='8'
dump=5000
path='../../'
while read s list; do
        echo kT${kT}_seed${s}
        mkdir ${kT}kT_${s}seed
        cd ${kT}kT_${s}seed
        cp  ${path}/lj_moldint.in .
        cp  ${path}/LAMMPS.job .
        sed -i "9s/400000/$steps/"    lj_moldint.in
        sed -i "15s/0.33/${rw}/"      lj_moldint.in
        sed -i "17s/8.0/${kT}/"      lj_moldint.in
        sed -i "19s/0.617/${T}/"       lj_moldint.in
        sed -i "20s/-0.02/${P}/"       lj_moldint.in
        sed -i "25s/5000/${dump}/"  lj_moldint.in
        sed -i "18s/12345/$(( $RANDOM*3+50000 ))/" lj_moldint.in
        sbatch LAMMPS.job
        cd ../
done <list

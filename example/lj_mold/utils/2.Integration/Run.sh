T='0.617'
P='-0.02'
rw='0.34'
steps=400000
dump=50000
path='../../'
while read kT list; do
        echo ${kT}kT
        mkdir ${kT}kT
        cd ${kT}kT
        cp  ${path}lj_moldint.in .
        cp  ${path}50.sub .
        sed -i "9s/50000/$steps/"    lj_moldint.in
        sed -i "15s/0.2/${rw}/"      lj_moldint.in
        sed -i "17s/1.0/${kT}/"      lj_moldint.in
        sed -i "18s/12345/$(( $RANDOM*3+50000 ))/" lj_moldint.in
        sed -i "19s/1.0/${T}/"       lj_moldint.in
        sed -i "20s/0.0/${P}/"       lj_moldint.in
        sed -i "25s/5000/${dump}/"  lj_moldint.in
        sbatch LAMMPS.job
        cd ../
done <list

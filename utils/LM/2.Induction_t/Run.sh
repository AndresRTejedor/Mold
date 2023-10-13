T='220'
Nw='39'
rw='0.31'
steps=8000000
dump='200000'
kT='8'
path='../../'
while read s lista; do
        echo ${kT}kT_seed${s}
        mkdir ${kT}kT_${s}seed
        cd ${kT}kT_${s}seed
        cp  ${path}mw_lattmold.in .
        cp  ${path}mW.sw .
        cp  ${path}${Nw}mold.lmp .
        sed -i "7s/220.0/${T}/"      mw_lattmold.in
        sed -i "8s/8/${kT}/"         mw_lattmold.in
        sed -i "10s/0.25/${rw}/"     mw_lattmold.in
        sed -i "11s/12345/$(( $RANDOM*3+50000 ))/" mw_lattmold.in
        sed -i "15s/50000/$dump/"    mw_lattmold.in
        sed -i "16s/100000/$steps/"  mw_lattmold.in
        sbatch LAMMPS.job
        cd ../
done <lista

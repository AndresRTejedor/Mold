kT='8'
while read s lista; do
        echo kT${kT}_seed${s}
        mkdir ${kT}kT_${s}seed/order
        cd ${kT}kT_${s}seed
        cp  result.xyz order
        cd ../
done <lista

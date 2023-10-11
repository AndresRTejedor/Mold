kT='8'
while read s lista; do
        echo kT${kT}_seed${s}
	cd ${kT}kT_${s}seed
        rm water*
        rm restart.1*
        rm restart.2*
        rm restart.3*
        rm restart.4*
        rm restart.5*
        rm restart.6*
        rm restart.7*
        rm restart.8*
        rm restart.9*
	cd ../
done <lista

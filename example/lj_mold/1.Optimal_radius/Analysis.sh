kT='8'
while read s lista; do
        echo kT${kT}_seed${s}
        cd ${kT}kT_${s}seed/order
        cp  ~/rds/rds-t2-cs092/andres/5_Mold_integration/set_up/Simulations/Radius_sweep/Results/order_parameter/a.out .
        cp  ~/rds/rds-t2-cs092/andres/5_Mold_integration/set_up/Simulations/Radius_sweep/Results/order_parameter/prepara-todas-WITH-BOX.sh .
        cp  ~/rds/rds-t2-cs092/andres/5_Mold_integration/set_up/Simulations/Radius_sweep/Results/order_parameter/* .
        sh prepara-todas-WITH-BOX.sh > kk2supreme
	echo run_app
	./a.out > kkpi
	sh dobi.sh
	cd ../../
done <lista
while read s lista; do
        cd ${kT}kT_${s}seed/order
	sh dobi.sh
	cp nbig.dat ../../results/nbig_${s}seed.dat 
	cd ../../
done <lista

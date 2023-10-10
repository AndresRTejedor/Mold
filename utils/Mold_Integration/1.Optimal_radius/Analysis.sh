kT='8'
path='../../'
while read s lista; do
        echo kT${kT}_seed${s}
        cd ${kT}kT_${s}seed/
        cp  ${path}a.out .
        cp  ${path}prepare-all-WITH-BOX.sh .
        cp  ${path}* .
        sh prepare-all-WITH-BOX.sh > kk2supreme
	echo run_app
	./a.out > kkpi
	sh dobi.sh
	cd ../
done <lista
while read s lista; do
        cd ${kT}kT_${s}seed/
	sh dobi.sh
	cp nbig.dat nbig_${s}seed.dat 
	cd ../
done <lista

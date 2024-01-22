kT='8'
path='../../'
while read s list; do
        echo kT${kT}_seed${s}
        cd ${kT}kT_${s}seed/
        cp  ${path}a.out .
        cp  ${path}*.sh .
        cp  ${path}*inc .
        cp  ${path}*inp .
        sh prepare-all-WITH-BOX.sh > kk2supreme
	./a.out > kkpi
	cd ../
done <list
while read s list; do
        cd ${kT}kT_${s}seed/
	sh dobi.sh
	cp nbig.dat nbig_${s}seed.data
	rm *dat
	rm fort*
	rm *xyz
	cd ../
done <list

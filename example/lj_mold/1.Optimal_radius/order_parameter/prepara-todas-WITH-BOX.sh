
rm todaslasconf.dat caja klkl kkk
touch todaslasconf.dat caja klkl kkk

nmol=1960
num=1969 

split -$num result.xyz

ls -1 x?? > lista 

cat lista | while read nome
do
echo $nome

tail -$nmol $nome  > $nome.conf
sort -n $nome.conf > ll
mv ll $nome.conf
awk '{print "  ",$3,"       ",$4,"       ",$5}' $nome.conf > $nome.xyz


head -6 $nome |awk '{print $2-$1}' |tail -1  > $nome.boxa
head -7 $nome |awk '{print $2-$1}' |tail -1  > $nome.boxb
head -8 $nome |awk '{print $2-$1}' |tail -1  > $nome.boxc
head -4 $nome |awk '{print $1}'  |tail -1 > $nome.part
head -2 $nome |awk '{print $1}'  |tail -1 > $nome.step 


rm  caja
touch  caja

cat $nome.boxa |while read a
do
echo $a

#awk -v BB=$a '{print $1,$2,$3,$1/BB}' $nome.xyz

awk -v BB=$a  '{print BB,"0  ","0  "}' $nome.part   >> caja

done

cat $nome.boxb |while read b
do
echo $b

#awk -v BB=$a '{print $1,$2,$3,$1/BB}' $nome.xyz

awk -v BB=$b  '{print "0  ", BB, "0  "}' $nome.part   >> caja

done
cat $nome.boxc |while read c
do
echo $c

#awk -v BB=$a '{print $1,$2,$3,$1/BB}' $nome.xyz

awk -v BB=$c  '{print "0  ", "0  ", BB}' $nome.part  >> caja

done


echo "otra    " > k1
paste k1 $nome.step > kkk
echo "0 0 0 0"  >> kkk


cat kkk $nome.xyz caja >> todaslasconf.dat 

done
rm x??.* x??


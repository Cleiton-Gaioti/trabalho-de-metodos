#!/bin/sh
inst="1 2 3"
seed="0 398647 839458745"

for i in $inst; 
do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i25.sol'"${s}"        60      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i100.sol'"${s}"        60      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i250.sol'"${s}"        60      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i500.sol'"${s}"        60      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i750.sol'"${s}"        60      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i1000.sol'"${s}"        120      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i13206.sol'"${s}"        1800      saida.txt
    done
done

do
	for s in $seed; 
	do
        #EXEC    SEM        INST                            TEMPO   SAIDA
        ./main   "${s}"     'arquivos/i13206p8.sol'"${s}"      1800      saida.txt
    done
done
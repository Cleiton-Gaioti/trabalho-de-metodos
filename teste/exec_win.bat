SET inst=1 2 3
SET seed=0 398647 839458745

REM[            EXEC    SEM     INST    TEMPO   SAIDA]

REM[i25]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i25.sol%%I	    60		saida.txt

REM[i100]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i100.sol%%I	60		saida.txt

REM[i250]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i250.sol%%I	60		saida.txt

REM[i500]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i500.sol%%I	60		saida.txt

REM[i750]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i750.sol%%I	60		saida.txt

REM[i1000]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i1000.sol%%I	120		saida.txt

REM[i13206]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i13206.sol%%I  1800	saida.txt

REM[i13206p8]
FOR %%I IN (%inst%) DO	FOR %%S IN (%seed%) DO		main.exe		%%S 	arquivos/i13206p8.sol%%I 1800	saida.txt
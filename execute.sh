#!/bin/bash

#Compilando o arquivo
make
#Criando a pasta de resultados

cabecalho="Resultados TSP"
echo $cabecalho > results.txt
#Executando as instancias
PATH_INSTANCES="src/instances<=535"
for file in "$PATH_INSTANCES"/*; 
do

  echo "----> [.] $(basename $file)"
  result=$(./tsp $file)
  str="$(basename $file) $result"
  echo $str >> results.txt
  feasible=$(echo "$str" | awk '{print $NF}')
  if [ "$feasible" == "1" ];then
    echo "----> feasible"
  else
    echo "----> infeasible"
  fi

  echo "----> [v] $(basename $file)"
done


#ifndef STRUCTS_H
#define STRUCTS_H
#include <list>
#include <vector>


// Tipo de uma determina solução, possui a sequencia e o valor objetivo, que é a soma de todos os custos
typedef struct {
  std::list<int> sequence;

	double valueObj;

}Solution;



typedef struct {

	int nodeChoosen; // Valor de K que será inserido
  std::list<int>::iterator posInsertNode; // Aresta que será removida {i, j} para ser inserido o valor de k
  double insertionCost; // o custo delta de inserção de k na aresta {i, j};
  std::list<int>::iterator posNodeCL; // Posição do iterator daquele nó no cl para remoção
  int edgeRemoved;
}InsertionInfo;

#endif

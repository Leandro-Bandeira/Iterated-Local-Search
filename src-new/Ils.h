#ifndef ILS_H
#define ILS_H

#include <vector>
#include <list>

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






class Ils {
	

	public:
		Ils(int maxIter, int maxIterILS, double** costs, int n);
    
    double algorithm();
		void greedyConstruction();
		void randomizedSolution(Solution* s, std::vector<bool>& nodesInserted);
    void calculateCostInsertion(Solution*s, std::vector<InsertionInfo>& allCosts, std::list<int>& cl);
    void addNodeSolution(int edge);
	
	private:
	double** m_costs;
    int m_n;
    int m_maxIter;
    int m_maxIterILS;
	  Solution m_bestSolution;

};


#endif

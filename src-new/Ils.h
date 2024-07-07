#ifndef ILS_H
#define ILS_H

#include <vector>
#include <list>
#include "Structs.h"

class Ils{
	
	public:
		Ils(int maxIter, int maxIterILS, double** costs, int n);
    double algorithm();
		void greedyConstruction();
		void randomizedSolution(Solution* s, std::vector<bool>& nodesInserted);
    void calculateCostInsertion(Solution*s, std::vector<InsertionInfo>& allCosts, std::list<int>& cl);
    void addNodeSolution(int edge);
    void debug();
    Solution perturbation(Solution s);
    bool overLap(std::list<int>::iterator pos_i, const int tam_i, std::list<int>::iterator pos_j, const int tam_j);
	private:
	double** m_costs;
    int m_n;
    int m_maxIter;
    int m_maxIterILS;
	  Solution m_bestSolution;
    Solution m_bestAllSolution;
    Solution m_best;
};


#endif

#include "Ils.h"
#include <cstdlib>
#include <random>
#include<bits/stdc++.h>
#include <algorithm>
#include <functional>
#include <iostream>

Ils::Ils(int maxIter, int maxIterILS, double** costs, int n){
  m_costs = costs;
  m_maxIter = maxIter;
  m_maxIterILS = maxIterILS;
  m_n = n;
};


double Ils::algorithm(){
  double obj = 0;
  greedyConstruction();
  return obj;
}


/* Método responsável pela construção gulosa */
void Ils::greedyConstruction() {
  
  std::srand(std::time(0));

  std::vector < bool > nodesInserted(m_n, false);
  Solution initSolution;
  initSolution.valueObj = 0;
  randomizedSolution(&initSolution, nodesInserted);
  

	std::list<int> cl; // Cl são os vertices que precisam ser inseridos
  cl.clear();

  const int nodesInsertedSize = nodesInserted.size();
  for(int i = 1; i < nodesInsertedSize; i++){
    if(nodesInserted[i] != true){
      cl.push_back(i);
    }
  }
    
  while(!cl.empty()){
    std::vector < InsertionInfo > allCosts;
    calculateCostInsertion(&initSolution, allCosts, cl);
    std::sort(allCosts.begin(), allCosts.end(), [](const InsertionInfo& first,const InsertionInfo& second)
    {
      return first.insertionCost < second.insertionCost;
    });
    double alpha = (double) std::rand() / RAND_MAX;
    int indexChoosen = std::rand() %  ((int) std::ceil(alpha *allCosts.size()));
    /* 
    std::cout << "solucao antes da insercao: ";
    for(auto k: initSolution.sequence){
      std::cout << k << " ";
    }
    std::cout << "\n";
    std::cout << "Nó escolhido para inserção: " << allCosts[indexChoosen].nodeChoosen << "\n";
    std::cout << "Primeiro vértice de inserção: " << allCosts[indexChoosen].edgeRemoved << "\n";
    */
    initSolution.sequence.insert(allCosts[indexChoosen].posInsertNode, allCosts[indexChoosen].nodeChoosen);
    
    /*std::cout << "Solucao apos a insercao: ";
   for(auto k : initSolution.sequence){
      std::cout << k << " ";
    }
    std::cout << "\n";
    
    std::cout << "cl antes erase: ";
    for(auto k : cl){
      std::cout << k << " ";
    }
    std::cout << "\n";
    */
    cl.erase(allCosts[indexChoosen].posNodeCL);
    //std::cout << "cl pos erase: ";
    //for(auto k : cl){
    //  std::cout << k << " ";
    //}
    //std::cout << "\n";

    //getchar();
  }

  std::cout << "solucao inicial: ";
  for(auto k: initSolution.sequence){
    std::cout << k << " ";
  }
    std::cout << "\n";
  auto it = initSolution.sequence.begin();
  for(int  a= 0; a < initSolution.sequence.size() - 1; a++){
    int i = *it;
    it = std::next(it, 1);
    int j = *it;
    initSolution.valueObj += m_costs[i][j];
  }
    std::cout << "FO: " << initSolution.valueObj << "\n";
}

/* Calcula os custos de inserção para o algoritmo de inserção mais barata */ 
void Ils::calculateCostInsertion(Solution*s, std::vector<InsertionInfo>& allCosts, std::list<int>& cl){
  
  allCosts.clear(); 
  allCosts.resize((s->sequence.size() - 1) * cl.size());

  int l  = 0;
  const int sequenceSize = s->sequence.size() - 1;
  
  auto it = s->sequence.begin();
  
  for(int a = 0; a < sequenceSize; a++){
    int i = *it;
    it = std::next(it, 1);
    int j = *it;

    for(std::list<int>::iterator k = cl.begin(); k != cl.end(); ++k){
      allCosts[l].insertionCost = m_costs[i][*k] + m_costs[j][*k] - m_costs[i][j];
      allCosts[l].nodeChoosen = *k;
      allCosts[l].posInsertNode = it;
      allCosts[l].edgeRemoved = a;
      allCosts[l].posNodeCL = k;
      l++;
    }
  }
}


/* Função responsável por colocar 3 vértices aleatorios na solução inicial */
void Ils::randomizedSolution(Solution *s, std::vector<bool>& nodesInserted) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector < int > nodes;
  for(int i = 1; i < m_n; i++){
    nodes.push_back(i);
  }

  std::shuffle(nodes.begin(), nodes.end(), gen);
  int pos = 0;
  while(pos < 3){
    auto it = std::next(s->sequence.begin(), pos);
    nodesInserted[nodes[pos]] = true;
    s->sequence.insert(it, nodes[pos]);
    pos++;
  }
  s->sequence.push_front(0);
  s->sequence.push_back(0);
}




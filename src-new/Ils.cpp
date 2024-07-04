#include "Ils.h"
#include "LocalSearch.h"
#include <cstdlib>
#include <random>
#include<bits/stdc++.h>
#include <algorithm>
#include <iostream>

Ils::Ils(int maxIter, int maxIterILS, double** costs, int n){
  m_costs = costs;
  m_maxIter = maxIter;
  m_maxIterILS = maxIterILS;
  m_n = n;
};


void Ils::debug(){
  double FO = 0;
  auto it = m_bestAllSolution.sequence.begin();
  std::vector<int> occurrNodes(m_bestAllSolution.sequence.size()-1,0);
  for(int a = 0; a < m_bestAllSolution.sequence.size() - 1 ; a++){
    occurrNodes[a]++;
    int i = *it;
    it = std::next(it, 1);
    int j = *it;
    FO += m_costs[i][j];
  }
  if(FO == m_bestAllSolution.valueObj){
    std::cout << "FO são iguais = " << FO << "\n";
  }else{
    std::cout << "As FO não batem\n" << "FO REAL: " << FO << "\n" << "FO MODELO: " << m_bestAllSolution.valueObj << "\n";
  }

  if(m_bestAllSolution.sequence.back() == 0){
    occurrNodes[0]++;
  }
  bool occurNodesOnce = true;
  for(int i = 0; i < occurrNodes.size(); i++){
    if(i != 0){
      if(occurrNodes[i] > 1 or occurrNodes[i] == 0){
        std::cout << i << "\n";
        occurNodesOnce = false;
        break;
      }
    }
  }
  if(!occurNodesOnce){
    std::cout << "algum vértice além do zero repetiu mais de uma vez ou tem vértice faltando\n";
  }
  else if(occurrNodes[0] == 2){
    std::cout << "Todos os vértices apareceram uma vez e o zero duas vezes, inicio e fim\n";
  }


}
double Ils::algorithm(){
  double bestAllSolution = 999999999;
  for(int i = 0; i < m_maxIterILS; i++){
  
    greedyConstruction();
  
    LocalSearch localSearch(m_costs, &m_bestSolution);
    localSearch.algorithm();
    
    if(m_bestSolution.valueObj < bestAllSolution){
      bestAllSolution = m_bestSolution.valueObj;
      m_bestAllSolution = m_bestSolution;
    }
    m_bestSolution.sequence.clear();
    m_bestSolution.valueObj = 0;
  }
  
  std::cout << "best FO: " << bestAllSolution << "\n";
  debug(); 
  return bestAllSolution;
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
    initSolution.sequence.insert(allCosts[indexChoosen].posInsertNode, allCosts[indexChoosen].nodeChoosen);
    
    initSolution.valueObj += allCosts[indexChoosen].insertionCost;
    cl.erase(allCosts[indexChoosen].posNodeCL);


  }
  m_bestSolution = initSolution;
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
    
    if(pos == 0){
      s->valueObj += m_costs[0][nodes[pos]];
    }else{
      s->valueObj += m_costs[nodes[pos - 1]][nodes[pos]];
    }
    pos++;
  }
  s->sequence.push_front(0);
  s->sequence.push_back(0);
  s->valueObj += m_costs[nodes[pos-1]][0];


}






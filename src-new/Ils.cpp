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
  //double bestAllSolution = 999999999;
  //for(int i = 0; i < m_maxIterILS; i++){
  
    greedyConstruction();
  
  //  LocalSearch localSearch(m_costs, &m_bestSolution);
   // localSearch.algorithm();
    
    //if(m_bestSolution.valueObj < bestAllSolution){
     // bestAllSolution = m_bestSolution.valueObj;
      //m_bestAllSolution = m_bestSolution;
    //}
    //m_bestSolution.sequence.clear();
    //m_bestSolution.valueObj = 0;
  //}
  
  //std::cout << "best FO: " << bestAllSolution << "\n";
  //debug(); 
  perturbation();
  //return bestAllSolution;
  return 1;
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


/* Função responsável por verificar se dois segmentos são sobrepostos */ 
bool Ils::overLap(std::list<int>::iterator pos_i, const int tam_i, std::list<int>::iterator pos_j, const int tam_j){



}

/* Função responsável por realizar o movimento de double bridge
  * como perturbação
  * O movimento de double bridge precisa de dois segmentos não sobrepostos
  * de forma aleatória de tamanho 2 até v/10 */
void Ils::perturbation(){
  std::random_device rd;
  std::mt19937 gen(rd());
  /* 1 2 3 4 5 6 7 8 9 10 11 12 13 1 */
  const int min_size = 2;
  const int max_size = std::ceil((double)m_bestSolution.sequence.size() / 10);
  const int size = m_bestSolution.sequence.size();

  std::uniform_int_distribution<> distTam(min_size, max_size);
  int pos_i = 0;
  int pos_j = 0;
  int tam_i = 0;
  int tam_j = 0;
  std::cout << size << "\n";
  while(1){
    tam_i = distTam(gen);
    std::uniform_int_distribution<> distPos_i(1, size - tam_i -1);
    pos_i = distPos_i(gen);
    tam_j = distTam(gen);
    std::uniform_int_distribution<>distPos_j(1, size - tam_j - 1);
    pos_j = distPos_j(gen);
    
    /* Para verificar se dois segmentos são sobrepostos, basta pegar
      * o indice do menor, somar seu indice com o seu tamanho - 1 
      * e verificar se ele chega até a posição do maior
      * somamos com tamanho - 1, pq no tamanho ele já está sendo contado */ 
    if(pos_i == pos_j){
      continue;
    }
    int pos_f = 0;
    if(pos_i < pos_j){
      pos_f = pos_i + tam_i - 1; 
      if(pos_f >= pos_j and pos_f <= pos_j + tam_j - 1){
        continue;
      }else{
        break;
      }
    }else{
      pos_f = pos_j + tam_j - 1;
      if(pos_f >= pos_i and pos_f <= pos_i + tam_i - 1){
        continue;
      }
      else{
        break;
      }
    }
  }
  for(auto k: m_bestSolution.sequence)
    std::cout << k << " ";
  std::cout << "\n";
  std::cout << "pos_i: " << pos_i << "\n";
  std::cout << tam_i << "\n";
  std::cout << "elemento_i: " << *(std::next(init, pos_i)) << "\n"; 
  std::cout << "pos_j: " << pos_j << "\n";
  std::cout << tam_j << "\n";
  
  std::cout << "elemento_j: " << *(std::next(init, pos_j)) << "\n"; 

   auto init = m_bestSolution.sequence.begin();
  /* para trocar os elementos vamos utilizar a função swap range
    * para isso, os segmentos precisam ter mesmo tamanho, então
    * vamos pegar o menor tamanho e adicionar -1 até ter o tamanho do maior */
  
  /*auto it_f = init;
  int count_insert = 0;
  int new_pos_j = pos_j;
  int new_pos_i =  pos_i;
  if(tam_i < tam_j){
    it_f = std::next(init, tam_i + pos_i);
    
    std::cout << "it_f: " << *it_f << "\n";
    count_insert = tam_j - tam_i;
    int f = 0;
    while(f < count_insert){
      m_bestSolution.sequence.insert(it_f, -1);
      f++;
    }
    if(pos_i < pos_j){
      new_pos_j = pos_j + count_insert;
    }
    
    
  }else{
    it_f = std::next(init, tam_j + pos_j);
    
    std::cout << "it_f: " << *it_f << "\n";
    count_insert = tam_i - tam_j;
    int f = 0;
    while(f < count_insert){
      m_bestSolution.sequence.insert(it_f, -1);
      f++;
    }
    if(pos_j < pos_i){
      new_pos_i = pos_i + count_insert;
    }
  }
  
 
   
  getchar();

  /* Para usar a função swap ranges, os dois primeiros iterator tem que vir antes do terceiro 
  if(pos_i < pos_j){
    auto init_i = std::next(init, pos_i);
    auto end_i = std::next(init_i, tam_i);
    auto init_j = std::next(init, pos_j);
    std::swap_ranges(init_i, end_i, init_j);
  }else{
    auto init_j = std::next(init, pos_j);
    auto end_j = std::next(init_j, tam_j);
    auto init_i = std::next(init, pos_i);
    std::swap_ranges(init_j, end_j, init_i);
  }
 for(auto k: m_bestSolution.sequence)
    std::cout << k << " ";

  while(count_insert > 0){
    m_bestSolution.sequence.erase(it_f);
    count_insert--;
  }
   
  */
 for(auto k: m_bestSolution.sequence)
    std::cout << k << " ";

  /* Vamos agora trocar as posições dos segmentos */
  if(pos_i < pos_j){
    std::vector < int> values_j;
    auto end_j = std::next(init, tam_j);
    for(auto j = std::next(init, pos_j); j != end_j; ++j){
      std::cout <<  *j << " ";
    }
    getchar();
  }
  getchar();
}





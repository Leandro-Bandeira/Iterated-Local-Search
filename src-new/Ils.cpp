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


Solution Ils::algorithm(){
  
  m_bestAllSolution.valueObj = 999999999;
  m_best.valueObj = 999999999;
  
  for(int i = 0; i < m_maxIter; i++){
  
    greedyConstruction(); /* atua sobre o m_bestSolution */
  
    int iterIls = 0;
    while(iterIls <= m_maxIterILS){

      LocalSearch localSearch(m_costs, &m_bestSolution);
      localSearch.algorithm();
      if(m_bestSolution.valueObj < m_best.valueObj){
        m_best  = m_bestSolution;
        iterIls = 0;
      }
      m_bestSolution = perturbation(m_best);
      iterIls++;

    }

    if(m_best.valueObj < m_bestAllSolution.valueObj){
      m_bestAllSolution = m_best;
    }
  }
  
  
  
  return m_bestAllSolution;
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



/* Função responsável por realizar o movimento de double bridge
  * como perturbação
  * O movimento de double bridge precisa de dois segmentos não sobrepostos
  * de forma aleatória de tamanho 2 até v/10 */
Solution Ils::perturbation(Solution s){
  std::random_device rd;
  std::mt19937 gen(rd());
  const int min_size = 2;
  const int max_size = std::ceil((double)s.sequence.size() / 10);
  const int size = s.sequence.size();

  std::uniform_int_distribution<> distTam(min_size, max_size);
  int pos_i = 0;
  int pos_j = 0;
  int tam_i = 0;
  int tam_j = 0;
  
  
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
      
      if(pos_j <= pos_f and pos_j >= pos_i){
        continue;
      }else{
        break;
      }
    }else{
      pos_f = pos_j + tam_j - 1;
      if(pos_i <= pos_f and pos_i >= pos_j){
        continue;
      }
      else{
        break;
      }
    }
  }
 

   auto init = s.sequence.begin();
  
  
  auto it_i = std::next(init, pos_i);
  int v_i = *(it_i);
  int v_i_next = *(std::next(it_i, tam_i));
  int v_i_last = *(std::next(it_i, tam_i - 1));
  auto it_previ = std::prev(it_i, 1);
  int v_i_prev = *(it_previ);
  auto it_j = std::next(init, pos_j);
  int v_j = *(it_j);
  auto it_prevj = std::prev(it_j, 1);
  int v_j_prev = (*it_prevj);
  int v_j_next = *(std::next(it_j, tam_j));
  int v_j_last = *(std::next(it_j, tam_j - 1));
  
  /* Para realizar a troca, vamos identificar
   * o iterator do elementot anterior, dos dois conjuntos,
   * remover os elementos e inserir nas posições invertidas */
  /* pos_i  == 1 e tam = 2, pos_j = 6 e tam =3
   * 1 (2 3) 4 5 6 (7 8 9) 1 */
  /* 1 4 5 6 1 
  /* 1 7 8 9 4 5 6 2 3 1 
   *  c[i-1][j] + c[v_j_last][v_i_next] + c[j - 1][i] + c[v_j_next][v_i_last]
   * - c[i-1][i] + c[v_i_last][v_i_next] + c[j-1][j] + c[v_j_last][v_j_next]
   * caso pos_j == 1 e tam = 2, pos_i = 6 e tam = 3
   * 1 7 8 9 4 5 6 2 3 1, resulta na mesma coisa 
   * caso forem adjacentes
   * 1 2 3 4 5 (6 7) (8 9) 1, pos_j =  7 tam = 2 e pos_i = 5 , tam 2 
   * 1 2 3 4 5 (8 9 6 7) 1
   *
   * c[i - 1][j] + c[v_i_last][v_j_next] + [v_j_last][i] - c[i-1][i] - v[v_j_last][v_j_next] -  c[v_i_last][v_j]
   * caso que fosse ao contrario, i = 7 e j = 5
   * 1 2 3 4 5 (6 7) (8 9) 1
   * 1 2 3 4 5 (8 9 6 7) 1 
   
   * c[j-1][i] + c[v_j_last][v_i_next] + c[v_i_last][v_j] - c[j - 1][j] - c[v_i_last][v_i_next] - c[v_j_last][v_i]*/ 
  
  std::vector < int > elements_i;
  for(int k = 0; k < tam_i; k++){
   elements_i.push_back(*it_i);
   it_i =  s.sequence.erase(it_i);
  }
  
  std::vector <int> elements_j;
  for(int k = 0; k < tam_j; k++){
    elements_j.push_back(*it_j);
    it_j = s.sequence.erase(it_j);
  }

  //std::cout << "apos deletar:\n";
  //for(auto k : m_bestSolution.sequence)
   // std::cout << k << " ";
  //std::cout << "\n";
  
  
  double delta = 0;
  if(pos_i + tam_i == pos_j or pos_j + tam_j == pos_i){ /* Indica adjacencia */  
    
    /* Se a posição de ir vier antes do j, os elementos de j serão colocados primeiros */ 
    if(pos_i < pos_j){
      for(int k = 0; k < tam_i; k++){
        elements_j.push_back(elements_i[k]);
      }
      for(int k = 0; k < elements_j.size(); k++){
        it_previ = s.sequence.insert(std::next(it_previ, 1), elements_j[k]);
      }
      delta = m_costs[v_i_prev][v_j] + m_costs[v_i_last][v_j_next] + m_costs[v_j_last][v_i]
        - m_costs[v_i_prev][v_i] - m_costs[v_j_last][v_j_next] - m_costs[v_i_last][v_j];
    }
    else{

      for(int k = 0; k < tam_j; k++){
        elements_i.push_back(elements_j[k]);
      }
      for(int k = 0; k < elements_i.size(); k++){
        it_prevj = s.sequence.insert(std::next(it_prevj, 1), elements_i[k]);
      }

      delta = m_costs[v_j_prev][v_i] + m_costs[v_j_last][v_i_next] + m_costs[v_i_last][v_j]
        - m_costs[v_j_prev][v_j] - m_costs[v_i_last][v_i_next] - m_costs[v_j_last][v_i];
    }
    
  }
  else{
    for(int  k = 0; k < tam_j; k++){
      it_previ = s.sequence.insert(std::next(it_previ, 1), elements_j[k]);
    }
    for(int k = 0; k < tam_i; k++){
      it_prevj = s.sequence.insert(std::next(it_prevj, 1), elements_i[k]);
    }

    delta = m_costs[v_i_prev][v_j] + m_costs[v_j_last][v_i_next] + m_costs[v_j_prev][v_i] + m_costs[v_j_next][v_i_last]
      -m_costs[v_i_prev][v_i] - m_costs[v_i_last][v_i_next] - m_costs[v_j_prev][v_j] - m_costs[v_j_last][v_j_next];
  }

  s.valueObj += delta;
  
  return s;
}





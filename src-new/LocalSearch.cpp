#include "LocalSearch.h"
#include <iterator>
#include <iostream>


LocalSearch::LocalSearch(double** costs, Solution* s){
  m_costs =costs;
  m_solution = s;
}


void LocalSearch::algorithm(){
  std::vector<int> NL = {1};
  bool improved = false;
  
  
  while(!NL.empty()){
    int n = std::rand() % NL.size();

    switch (NL[n]){
      case 1:
        improved = bestImprovementSwap();
        break;
    }

    if(improved){
      NL = {1};
    }
    else{
      NL.erase(NL.begin() + n);
    }
  }
  
  std::cout << "solution antes do swap: ";
   for(auto k : m_solution->sequence)
    std::cout << k << " ";
  std::cout << "\n";

 // bestImprovementSwap();

  std::cout << "solution  pos swap: ";
   for(auto k : m_solution->sequence)
    std::cout << k << " ";
  std::cout << "\n";
 
  std::cout << "FO: " << m_solution->valueObj << "\n";
  double FO = 0;
  auto it = m_solution->sequence.begin();
  for(int a = 0; a < m_solution->sequence.size() - 1 ; a++){
    int i = *it;
    it = std::next(it, 1);
    int j = *it;
    std::cout  << i << "---" << j << ": " << m_costs[i][j] <<  "\n";
    FO += m_costs[i][j];
  }
  std::cout << "FO real: " << FO << "\n";
}

/* O algoritmo inicia o swap a partir do primeiro vértice
* e termina quando chegar na origem */
bool LocalSearch::bestImprovementSwap(){
  double bestDelta = 0;

  std::list<int>::iterator best_i = m_solution->sequence.begin();
  std::list<int>::iterator best_j = m_solution->sequence.begin();
  
  for(auto i = std::next(m_solution->sequence.begin(),1); *i != 0; ++i){
    int v_i = *i;
    int v_i_prev = (*std::prev(i, 1));
    int v_i_next = (*std::next(i, 1));
    
    for(auto j = std::next(i, 1); *j != 0; ++j){
      int v_j = *j;
      int v_j_prev = (*std::prev(j, 1));
      int v_j_next = (*std::next(j, 1));
      
      double delta = 0;
      if(v_j != v_i_next){
        delta = +m_costs[v_j][v_i_prev] + m_costs[v_j][v_i_next] + m_costs[v_i][v_j_prev]
        +m_costs[v_i][v_j_next]  -m_costs[v_i][v_i_prev] - m_costs[v_i][v_i_next] - m_costs[v_j][v_j_prev]
        -m_costs[v_j][v_j_next];
      }else{
        delta = +m_costs[v_j][v_i_prev] + m_costs[v_i][v_j_next] - m_costs[v_i_prev][v_i] + m_costs[v_j][v_j_next];
      }

      if(delta < bestDelta){
        best_i = i;
        best_j = j;
 
        std::cout << *best_i << "\n";
        std::cout << *best_j << "\n";
        std::cout << delta << "\n";
        bestDelta = delta;

        //getchar();
      }
  
    }
  }
  if(bestDelta < 0){
    std::cout << "bestDelta: " << bestDelta << "\n";
    std::iter_swap(best_i, best_j);
    m_solution->valueObj += bestDelta;
    return true;
  }
  return false;
}

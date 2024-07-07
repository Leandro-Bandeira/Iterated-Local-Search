#include "LocalSearch.h"
#include <algorithm>
#include <iterator>
#include <iostream>


LocalSearch::LocalSearch(double** costs, Solution* s){
  m_costs =costs;
  m_solution = s;
}


void LocalSearch::algorithm(){
  std::vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;
  while(!NL.empty()){
    int n = std::rand() % NL.size();

    switch (NL[n]){
      case 1:
       improved = bestImprovementSwap();
        break;
      case 2:
        improved = bestImprovement2Opt();
        break;
      case 3:
        improved = bestImprovementOrOpt();
        break;
      case 4:
        improved = bestImprovementOrOpt2();
        break;
      case 5:
        improved = bestImprovementOrOpt3();
        break;
    }

    if(improved){
      NL = {1, 2, 3, 4, 5};
    }
    else{
      NL.erase(NL.begin() + n);
    }
  }
  
  
  

/*
  std::cout << "solution  pos moviments: ";
   for(auto k : m_solution->sequence)
    std::cout << k << " ";
  std::cout << "\n";
 
  std::cout << "FO calculado: " << m_solution->valueObj << "\n";
  double FO = 0;
  auto it = m_solution->sequence.begin();
  for(int a = 0; a < m_solution->sequence.size() - 1 ; a++){
    int i = *it;
    it = std::next(it, 1);
    int j = *it;
    //std::cout  << i << "---" << j << ": " << m_costs[i][j] <<  "\n";
    FO += m_costs[i][j];
  }
  
  std::cout << "FO real: " << FO << "\n";
  */
}

/* O algoritmo abaixo realiza o movimento swap
 * Para exemplificar, suponha que temos a rota
 * 1 2 3 4 5 6 1 e estamos aplicando o swap entre i  = 1 e j = 4
 * logo teremos a nova rota dada por
 *  1 5 3 4 2 6 1
 *  Os calculos para da função objetivo estão descritos abaixo
 *  Para o calculo do custo no swap, se eles não forem adjacentes
 *  temos o seguinte (novo - antigo);
 *  c[v_j][v_i_prev] + c[v_j][v_i_next] + c[v_i][v_j_prev]+c[v_i][v_j_next] - c[v_i][v_i_prev]
 *  - c[v_i][v_i_next] - c[v_j][v_j_prev] - c[v_j][v_j_next]
 *  se eles forem adjacentes teremos apenas:
 *  delta = +m_costs[v_j][v_i_prev] + m_costs[v_i][v_j_next] - m_costs[v_i_prev][v_i] + m_costs[v_j][v_j_next]*/
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
        bestDelta = delta;
      }
  
    }
  }
  if(bestDelta < 0){
    std::iter_swap(best_i, best_j);

    m_solution->valueObj += bestDelta;
    return true;
  }
  return false;
}

/* O movimento 2-Opt, escolhe 2 vértices 
  * não adjacentes e reinsere eles na rota com a rota entre eles invertida
  * Por exemplo: Para i = 2 e j = 5
  * temos a situação:
  * 0 1 2 3 4 5 6 7 8 9 0
  * 0 1 2 5 4 3 6 7 8 9 0 
  * Para o cálculo do custo
  * Para as posições 0 a i-1 e j+i até n, o custo se mantém o mesmo, sendo assim o delta =
  * Novo - antigo = (C[i][j] + C[i+1][j+1]) - (C[i][i+1] + C[j][j+1])
  * Perceba que o que está entre, mesmo estando invertido, mantém o custo
  * Tanto swap e 2-opt j tem que ser consecutivo a i */ 
bool LocalSearch::bestImprovement2Opt(){
  
  
  double bestDelta = 0;
  std::list<int>::iterator init = m_solution->sequence.begin();
  std::list<int>::iterator end = m_solution->sequence.end();
  std::list<int>::iterator best_i =  init;
  std::list<int>::iterator best_j = init;

    for(auto i = std::next(init, 1); i!= std::prev(end, 3); ++i){
    int v_i = *i;
    int v_i_next = *(std::next(i, 1));
    for(auto j = std::next(i, 2); j != std::prev(end, 1); ++j){
      int v_j = *j;
      int v_j_next = *(std::next(j, 1));
      double delta = m_costs[v_i][v_j] + m_costs[v_i_next][v_j_next] - m_costs[v_i][v_i_next] - m_costs[v_j][v_j_next];
      if(delta < bestDelta){
        best_i = i;
        best_j = j;
        bestDelta = delta;
      }
    }
  }
  
  if(bestDelta < 0){
    std::reverse(std::next(best_i, 1), std::next(best_j,1 ));
    m_solution->valueObj += bestDelta;
    return true;
  }

  return false;

}

/* A função reinsertion, Or-opt2,Or-opt3
  * São similares.
  * No reinsertion devemos pegar um elemento e mudar sua posição
  * dessa forma:
  * 1 2 3 4 5 6 1
  * Se temos i  = 1, podemos colocá-lo em qualquer posição 
  * Então para j  = 5, temos:
  * 1 2 3 4 5 6 1 
  * 1 3 4 5 6 2 1 
  * Então a ideia é, coloca na posição e depois exclui a posição anterior 
  * O calculo do custo para o reinsertion é dado pela seguinte maneira: (novo - antigo)
  * para i < j 
  * c[i-1][i+1] +  c[j][i] + c[j + 1][i] - c[i-1][i] - c[i][i+1] - c[j][j+1]
  * se i > j, temos o seguinte exemplo:
  * 1 2 3 4 5 6 1// i = 5 e j = 1 
  * 1 6 2 3 4 5 1 
  * c[i-1][j] + c[j][i] + c[j-1][j+1] - c[i-1][i] - c[j][j+1] - c[j][j-1]
  * caso eles forem adjacente 
  * ou seja v_i_next = v_j
  * 1 2 3 4 5 6 1 // i = 2 e j = 3
  * 1 3 2 4 5 6 1
  * c[i-1][j] + c[i][j+1] - c[i-1][i] - c[j][j+1]
  * se eles forem adjacentes e i > j 
  * 1 2 3 4 5 6 1 // i = 5 e j = 4 
  * 1 2 3 4 6 5 1
  * c[i][j-1] + c[j][i+1] - c[j][j-1] - c[i][i+1]*/ 
bool LocalSearch::bestImprovementOrOpt(){
  double bestDelta = 0;
  
   std::list<int>::iterator init = m_solution->sequence.begin();
  std::list<int>::iterator best_i = init;
  std::list<int>::iterator best_j = init;
  int best_k = 0;
  int best_l = 0;
  std::list<int>::iterator end = m_solution->sequence.end();
  int  k =1;
  for(auto i = std::next(init, 1); *i != 0; ++i){
    int v_i = *i;
    int v_i_prev = *(std::prev(i, 1));
    int v_i_next = *(std::next(i, 1));
    int l = 1;
    for(auto j = std::next(init, 1); *j != 0; ++j){
      if (*i == *j){ 
        l++;
        continue;
      }
      int v_j = *j;
      int v_j_next = *(std::next(j, 1));

      double delta = 0;
      if(k < l){
        if(v_i_next  == v_j){ // caso em que são adjacentes
          delta = m_costs[v_i_prev][v_j] + m_costs[v_i][v_j_next] - m_costs[v_i_prev][v_i] - m_costs[v_j][v_j_next];
        }
        else{
          delta = m_costs[v_i_prev][v_i_next] + m_costs[v_j][v_i] + m_costs[v_j_next][v_i] - m_costs[v_i_prev][v_i]
          -m_costs[v_i][v_i_next] - m_costs[v_j][v_j_next];
        }
      }else{
        int v_j_prev = *(std::prev(j, 1));
        if(v_j_next == v_i){
          delta = m_costs[v_i][v_j_prev] + m_costs[v_j][v_i_next] - m_costs[v_j][v_j_prev] - m_costs[v_i][v_i_next];
        }
        else{
          delta = m_costs[v_i_prev][v_j] + m_costs[v_j][v_i] + m_costs[v_j_prev][v_j_next] - m_costs[v_i_prev][v_i]
          -m_costs[v_j][v_j_next] - m_costs[v_j][v_j_prev];
        }
      }
      if(delta < bestDelta){
        best_i = i;
        best_j = j;
        best_k  = k;
        best_l = l;
        bestDelta = delta;

      }
      
      l++;
    }
    k++;
  }

  if(bestDelta < 0){
    int value = *best_i;

    if(best_k < best_l){
      m_solution->sequence.erase(best_i);
      m_solution->sequence.insert(std::next(best_j, 1), value);
    }else{
      if(best_l + 1 == best_k){
        m_solution->sequence.insert(best_j, value);

        m_solution->sequence.erase(best_i);
      }else{
        m_solution->sequence.insert(std::next(best_j, 1), value);
        m_solution->sequence.erase(best_i);
      }
    }
    m_solution->valueObj += bestDelta;
    return true;
  }

  return false;
}

/* A função abaixo 
 * é similar ao movimento de reinsertion, no entanto, no lugar de 
 * pegar um único vértice, vamos pegar os dois consecutivos
 * Exemplo para i  < j e  i = 1 e j = 5 
 * 1 2 3 4 5 6 1
 * 1 4 5 6 2 3 1
 * O calculo do custo será dado por:
 * c[i-1][i + 2] + c[j][i] + c[i+1][j+1] - c[i-1][i] - c[i+1][i+2] - c[j][j+1]
 * caso que i < j e são adjancetes
 * i = 3 e j = 5
 * 1 2 3 (4 5) 6 1
 * 1 2 3 6 (4 5) 1
 * c[i-1][i+2] + c[j][i] + c[i+1][j+1] - c[i-1][i] - c[i+1][i+2] - c[j][j+1]
 * Caso o i > j, ex i = 4 e j = 1 
 * 1 2 3 4 5 6 1 
 * 1 5 6 2 3 4 1
 * c[j-1][i] + c[i+1][j] + c[i-1][i+2] - c[j-1][j] - c[i][i-1] - c[i+1][i+2]
 * exemplo i = 3 e j = 1 
 * 1 2 3 4 5 6 1
 * 1 4 5 2 3 6 1
 * caso eles sejam adjances e i > j, ou seja,  i = 3 e j = 2
 * 1 2 3 (4 5) 6 1
 * 1 2 4 5 3 6 1
 * c[j-1][i] + c[i+1][j] + c[i-1][i+2] - c[j-1][j] -c[i-1][i] - c[i +1][i+2]*/
bool LocalSearch::bestImprovementOrOpt2(){
  
  std::list<int>::iterator init = m_solution->sequence.begin();
  std::list<int>::iterator best_i = init;
  std::list<int>::iterator best_j = init;
  int best_k = 0;
  int best_l = 0;
  std::list<int>::iterator end = m_solution->sequence.end();
  int  k =1;
  double bestDelta = 0;

   /* O valor de i so pode ir até o antepenultimo valor da sequencia*/
  for(auto i = std::next(init,1); i != std::prev(end, 2); ++i){
    int v_i = *i;
    int v_i_prev = *(std::prev(i, 1));
    int v_i_next = *(std::next(i, 1));
    int v_i_nnext = *(std::next(i, 2));
    int l = 1;
    for(auto j = std::next(init, 1); *j != 0; ++j){
      int v_j = *j;
      int v_j_prev = *(std::prev(j, 1));
      int v_j_next = *(std::next(j, 1));

      /* if para evitar sobreposição */ 
      if(v_i == v_j or v_i_next == v_j){
        l++;
        continue;
      }
      double delta = 0;
      if(k<l){
        delta = m_costs[v_i_prev][v_i_nnext] + m_costs[v_j][v_i] + m_costs[v_i_next][v_j_next]
          -m_costs[v_i_prev][v_i] - m_costs[v_i_next][v_i_nnext] - m_costs[v_j][v_j_next];
      }
      else{
        delta = m_costs[v_j_prev][v_i] + m_costs[v_i_next][v_j] + m_costs[v_i_prev][v_i_nnext]
          - m_costs[v_j_prev][v_j] - m_costs[v_i][v_i_prev] - m_costs[v_i_next][v_i_nnext];
      }
      if(delta < bestDelta){
        best_i = i;
        best_j = j;
        best_k = k;
        best_l = l;
        bestDelta = delta;
      }
      l++;
    }
    k++;
  }

  if(bestDelta < 0){
    std::list<int>::iterator v_i_next = (std::next(best_i, 1));
    int value1 = *best_i;
    int value2 = *(std::next(best_i, 1));
    int value = value1;
    
    if(best_k < best_l){
      m_solution->sequence.erase(best_i);
      m_solution->sequence.erase(v_i_next);
      m_solution->sequence.insert(std::next(best_j, 1), value1);
      m_solution->sequence.insert(std::next(best_j, 2), value2);

    }else{
      m_solution->sequence.insert(best_j, value1);
      m_solution->sequence.insert(best_j, value2);
      m_solution->sequence.erase(best_i);
      m_solution->sequence.erase(v_i_next);
    }
    m_solution->valueObj += bestDelta;
    return true;
  }

  return false;
}

/* O best improvement Or-opt3 segue a mesma lógica dos anteriores
  * Por exemplo, para i < j, temos i = 1 e j = 4 caso em que são adjacentes
* 1 (2 3 4) 5 6 1
* 1 5 (2 3 4) 6 1 
* Para o calculo do custo, vamos ter o seguinte:
*
* c[i-1][i+3] + c[i+2][j+1] + c[j][i] - c[i-1][i] - c[i+2][i+3] - c[j][j+1]
* caso em que não são adjacentes
* 1 (2 3 4) 5 6 7 8 9 1, para i = 1 e j =6
* 1 5 6 7 (2 3 4) 8 9 1
* cai na mesma formula do adjacente
* e caso j = 9?
* 1 5 6 7 8 9 (2 3 4) 1, então para i < j, todos os 3 casos possuem o mesmo custo
* agora vamos analisar para o caso i > j
* 1 2 3 4 5 6 (7 8 9) 1, i = 6 e j = 1, vamos ter o seguinte caso
* 1 (7 8 9) 2 3 4 5 6 1
* então o custo será dado por:
* c[j-1][i] + c[i+2][j] + c[i-1][i+3] - c[j][j-1] - c[i-1][i] - c[i+2][i+3]*/ 
bool LocalSearch::bestImprovementOrOpt3(){
  std::list<int>::iterator init = m_solution->sequence.begin();
  std::list<int>::iterator best_i = init;
  std::list<int>::iterator best_j = init;
  int best_k = 0;
  int best_l = 0;
  std::list<int>::iterator end = m_solution->sequence.end();
  int  k =1;
  double bestDelta = 0;
  
   for(auto i = std::next(init,1); i != std::prev(end, 3); ++i){
    int v_i = *i;
    int v_i_prev = *(std::prev(i, 1));
    int v_i_next = *(std::next(i, 1));
    int v_i_nnext = *(std::next(i, 2));
    int v_i_nnnext = *(std::next(i, 3));
    
    int l = 1;
    for(auto j = std::next(init, 1); *j != 0; ++j){
      int v_j = *j;
      int v_j_prev = *(std::prev(j, 1));
      int v_j_next = *(std::next(j, 1));
      
      double delta = 0;
      /* feito para evitar a sobreposição */
      if(v_i == v_j or v_i_next == v_j or v_i_nnext == v_j){
        l++;
        continue;
      }
      if (k < l){
        delta = m_costs[v_i_prev][v_i_nnnext] + m_costs[v_i_nnext][v_j_next] + m_costs[v_j][v_i]
           -m_costs[v_i_prev][v_i] - m_costs[v_i_nnext][v_i_nnnext] - m_costs[v_j][v_j_next];
      }
      else{
        delta = m_costs[v_j_prev][v_i] + m_costs[v_i_nnext][v_j] + m_costs[v_i_prev][v_i_nnnext]
          - m_costs[v_j][v_j_prev] - m_costs[v_i_prev][v_i] - m_costs[v_i_nnext][v_i_nnnext];
      }
      if(delta < bestDelta){
        best_k = k;
        best_l = l;
        best_i = i;
        best_j = j;
        bestDelta = delta;
      }
      l++;
    }
    k++;
  }
  
  if(bestDelta < 0){

    std::list<int>::iterator v_i_next = (std::next(best_i, 1));
    std::list<int>::iterator v_i_nnext = (std::next(best_i, 2));
    int value1 = *best_i;
    int value2 = *(std::next(best_i, 1));
    int value3 = *(std::next(best_i, 2));
    
    int value = value1;
    int count = 3;
    
    while(count > 0){
      best_i = m_solution->sequence.erase(best_i);
      count--;
    }

    if(best_k < best_l){
      count = 1;
      while(count <= 3){
        if(count == 1){
          value = value1;
        }
        else if(count == 2){
          value = value2;
        }else{
          value = value3;
        }
        best_j = m_solution->sequence.insert(std::next(best_j, 1), value);
        count++;
      }
    }
    else{
     
      count = 1;
      while(count <= 3){
        if(count == 1){
          value = value1;
        }else if(count == 2){
          value = value2;
        }else if(count == 3){
          value = value3;
        }
        m_solution->sequence.insert(best_j, value);
        count++;
      }

    }
    m_solution->valueObj += bestDelta;
    return true;
 
  }
  return false;
}

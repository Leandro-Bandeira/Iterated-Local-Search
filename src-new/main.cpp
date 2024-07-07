#include <iostream>
#include <vector>
#include "Ils.h"
#include <ios>
#include <limits>
#include "Data.h"
#include <chrono>


void debug(Solution* s, double** m_costs){
  double FO = 0;
  auto it = s->sequence.begin();
  std::vector<int> occurrNodes(s->sequence.size()-1,0);
  for(int a = 0; a < s->sequence.size() - 1 ; a++){
    occurrNodes[a]++;
    int i = *it;
    it = std::next(it, 1);
    int j = *it;
    FO += m_costs[i][j];
  }
  if(FO == s->valueObj){
    std::cout << "FO são iguais = " << FO << "\n";
  }else{
    std::cout << "As FO não batem\n" << "FO REAL: " << FO << "\n" << "FO MODELO: " << s->valueObj << "\n";
  }

  if(s->sequence.back() == 0){
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
    std::cout << "Todos os vértices apareceram uma vez e o zero duas vezes no inicio e fim\n";
  }


}



int main(int argc, char** argv) {

  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();
	
  double** costs = data.getMatrixCost();

  int maxIter = 50;
  int maxIterIls = n;
  if (n >= 150){
    maxIterIls = std::ceil((double)n/2);
  }
  
  Solution bestSolution;
  bestSolution.valueObj = 9999999;
  auto start = std::chrono::high_resolution_clock::now();
  for(int i =0 ; i < 10; i++){
    Ils ils(maxIter, maxIterIls, costs, n);
    Solution currentSolution = ils.algorithm();
    if(currentSolution.valueObj < bestSolution.valueObj){
      bestSolution = currentSolution;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "O tempo de execução foi de " << (double)duration.count() / 10 << " segundos." << std::endl;
  std::cout << "FO: " << bestSolution.valueObj << "\n";
  debug(&bestSolution, costs);
	
	

}

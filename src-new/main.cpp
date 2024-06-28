#include <iostream>
#include "Leitor.h"
#include <vector>
#include "Ils.h"
#include <ios>
#include <limits>
#include "Data.h"







int main(int argc, char** argv) {

  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();
	
  double** costs = data.getMatrixCost();

	Ils ils(50, 50, costs, n); 

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      std::cout << costs[i][j] << " ";
    }
    std::cout << "\n";
  }
  ils.algorithm();
	

}

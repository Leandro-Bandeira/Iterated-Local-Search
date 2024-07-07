#include <iostream>
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

  int maxIter = 50;
  int maxIterIls = n;
  if (n >= 150){
    maxIterIls = std::ceil((double)n/2);
  }
	Ils ils(maxIter, maxIterIls, costs, n); 
  ils.algorithm();
	

}

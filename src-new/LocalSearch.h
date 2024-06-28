 #ifndef LOCALSEARCH_H
 #define LOCALSEARCH_H

 #include "Structs.h"
class LocalSearch{
  public:
    LocalSearch(double** costs, Solution* s);
    bool bestImprovementSwap();
    double calculateSwapCosts(int i, int j);
    void algorithm();
  private:
    double** m_costs;
    Solution* m_solution;
 };




 #endif

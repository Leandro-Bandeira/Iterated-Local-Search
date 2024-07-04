 #ifndef LOCALSEARCH_H
 #define LOCALSEARCH_H

 #include "Structs.h"
class LocalSearch{
  public:
    LocalSearch(double** costs, Solution* s);
    bool bestImprovementSwap();
    bool bestImprovement2Opt();
    bool bestImprovementOrOpt();
    bool bestImprovementOrOpt2();
    bool bestImprovementOrOpt3(); 
    void algorithm();
  private:
    double** m_costs;
    Solution* m_solution;
 };




 #endif

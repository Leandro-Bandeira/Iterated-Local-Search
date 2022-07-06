#include <iostream>
#include <vector>



using namespace std;

int main()
{
   std::vector < int > sequencia = {1, 8, 9, 5, 7, 2, 10,  1};

   int posicaoInicial = 2;
   int posicaoFinal =  4;


   while(posicaoInicial <= posicaoFinal) {

      swap(sequencia[posicaoInicial], sequencia[posicaoFinal]);
      posicaoInicial++;
      posicaoFinal--;
    
   }



   for(int i = 0; i < sequencia.size(); i++) {
      std::cout << sequencia[i] << " ";
   }
}
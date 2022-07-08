#include <iostream>
#include <vector>

using namespace std;

int main()
{
   std::vector<int> sequencia = {1, 8, 9, 5, 7, 2, 10, 1};

   int quantiaVertice = 3;

   int best_i = 1;
   int best_j = 5;

   std::vector<int> valores;
   int valor = sequencia[best_j];

   int contador = quantiaVertice;
   while (contador--)
   {
      valores.push_back(sequencia[best_i]);
      sequencia.erase(sequencia.begin() + best_i);
   }

   contador = quantiaVertice;

   for (int i = 0; i < sequencia.size(); i++)
   {

      if (sequencia[i] == valor)
      {

         int k = i;
         int j = 0;
         while (contador--)
         {
            k++;
            sequencia.insert(sequencia.begin() + k, valores[j]);
            j++;
         }
      }
   }

   for (int i = 0; i < sequencia.size(); i++)
   {
      std::cout << sequencia[i] << " ";
   }
   std::cout << std::endl;
}
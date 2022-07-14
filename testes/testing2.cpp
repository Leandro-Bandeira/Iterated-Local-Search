#include <iostream>
#include <vector>

using namespace std;








int main() {


    std::vector < int > valores;

    for(int i = 0 ; i < 10; i++) {
        valores.push_back(i);
    }

    //0 1 2
    valores.insert(valores.begin() + 3, 5);

    for(int i = 0; i < valores.size(); i++) {

        std::cout << valores[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
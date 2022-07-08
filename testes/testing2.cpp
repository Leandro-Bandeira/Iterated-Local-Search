#include <iostream>
#include <vector>

using namespace std;








int main() {


    std::vector < int > valores;

    for(int i = 0 ; i < 3; i++) {
        valores.push_back(i);
    }

    valores.insert(valores.begin() + 2, 2);

    for(int i = 0; i < valores.size(); i++) {

        std::cout << valores[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
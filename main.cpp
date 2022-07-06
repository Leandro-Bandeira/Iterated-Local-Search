#include <iostream>
#include "Leitor.h"
#include <vector>
#include "Ils.h"







int main() {

	
	int dimensao; // Tamanho da matriz
	
	int **custos; // Matriz de custos

	Leitor leitura; // Objeto da classe leitura

	// Dados do grafo, de custos e o seu tamanho
	custos = leitura.retornaMatriz();

	dimensao = leitura.retornaDimensao(); // Dimensão do grafo

	Ils ils;  // Objeto do tipo ILS
	Solucao s; // Solucao do sistema

	ils.construcao(&s,dimensao, custos);

	ils.buscaLocal(&s, custos);
	for(int i = 0; i < s.sequencia.size(); i++) {
			std::cout << s.sequencia[i] << " ";
	}
	
	std::cout << std::endl;
	for(int i = 0; i < s.sequencia.size() - 1; i++) {
		
		s.valorObj += custos[s.sequencia[i] - 1][s.sequencia[i + 1] - 1];
	}

	std::cout << "valorobj: " << s.valorObj << std::endl;

}

#include "Leitor.h"
#include <iostream>
#include <string>


Leitor::Leitor() {
	
	std::string linha;
	std::string valor;

	std::getline(std::cin, linha);
	
	// Algoritmo para pegar o tamanho da dimensao
	for(int i = 0; i < linha.size(); i++) {
		if(linha[i] == ' ') {
			for(int j = i; j < linha.size(); j++) {
				valor.push_back(linha[j]);
			}
			break;
		}
	}
	
	// Transforma a dimensao em inteiro
	dimensao = stoi(valor);
	
	// Cria a matriz
	matriz = new int *[dimensao];

	for(int i = 0; i < dimensao; i++) {

		matriz[i] = new int[dimensao];
	}
	
	// Armazena todos os dados de custos
	for(int i = 0; i< dimensao; i++) {

		for(int j = 0; j < dimensao; j++) {
			std::cin >> matriz[i][j];
		}
	}

}



int **Leitor::retornaMatriz() {
	
	return matriz;
}

int Leitor::retornaDimensao() {
	
	return dimensao;
}

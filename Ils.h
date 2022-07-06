#ifndef ILS_H
#define ILS_H
#include <vector>


// Tipo de uma determina solução, possui a sequencia e o valor objetivo, que é a soma de todos os custos
typedef struct {
	std::vector < int > sequencia = {1 , 1};

	double valorObj;

}Solucao;


typedef struct {

	int verticeInserido; // Valor de K que será inserido
	int arestaRemovida; // Aresta que será removida {i, j} para ser inserido o valor de k
	double custo; // o custo delta de inserção de k na aresta {i, j};

}InsertionInfo;






class Ils {
	

	public:
		Ils();

		void construcao(Solucao *, int, int **custos); // Metodo responsável por construir a primeira solução

		void escolher3NosAleatorios(Solucao *, int); // Escolhe inicialmente tres valores aleatorios e diferentes

		std::vector < InsertionInfo> calcularCustoInsercao(Solucao *, std::vector <int> &, int **custo); // Calcula o custo de inserção de um vértice entre dois vértices

		void ordenaEmOrdemCrescente(std::vector < InsertionInfo > &); // Ordena os custos em ordem crescente

		void inserirNaSolucao(Solucao *s, InsertionInfo*); // Insere na solução

		static bool comparaCustos( const InsertionInfo&, const InsertionInfo&); // Função responspavel por comparar os custos

		bool bestImprovementSwap(Solucao *s, int **custos); // Tenta realizar a melhora por swap, trocar dois vertices em uma sequencia
		double calculateSwapCost(Solucao *s, int, int, int **custos); // Recebe os vertices e a matriz de custos

		bool bestImprovement2Opt(Solucao *s, int **custos); // Tenta melhoramento pela tecnica 2opt, troca dos vertices não adjacentes e inverte o caminho
		double calculate2OptCost(Solucao *s, int, int, int**custos); // Calcula o custo de 2opt

		void buscaLocal(Solucao *s, int **custos);

	private:
		
		Solucao solucao;

};


#endif

#ifndef LEITOR_H
#define LEITOR_H










class Leitor {
	
	public:
	
		Leitor();

		int **retornaMatriz(); // Retorna a matriz de custos
		int retornaDimensao(); // Retorna o tamanho da dimensao

	private:
		int **matriz; // Matriz de custos
		int dimensao; // tamanho da dimensao
};




#endif

#ifndef HEAD_HPP_INCLUDED
#define HEAD_HPP_INCLUDED

#define MAXIMO_PONTOS 13206
#define MAXIMO_POSICOES 8
#define MAXIMO_CONFLITOS 50

int numeroDePontos;
int numeroDePosicoesCandidatas;
int posicoesCandidatas[MAXIMO_PONTOS*MAXIMO_POSICOES][MAXIMO_CONFLITOS];
int quantidadeDeConflitos[MAXIMO_PONTOS*MAXIMO_POSICOES];
int pontosLivres[MAXIMO_PONTOS];

typedef struct solucao {
    int posicaoDosPontos[MAXIMO_PONTOS];
    int funcao_objetivo;
}Solucao;

void lerArquivo(std::string arq);

int gerarNumero(int lim_inf, int lim_sup);

void heuConAle(Solucao &s);

void calcularFO(Solucao &s);

void clonarSolucao(Solucao &s1, Solucao &s2);

void testarDados(Solucao &s, std::string arq);

void lerSolucaoDeArquivo(Solucao &s, std::string path);

void testarF0(Solucao &s);

void testarHeuristica(Solucao &s);

#endif
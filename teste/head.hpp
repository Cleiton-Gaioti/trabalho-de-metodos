#ifndef HEAD_HPP_INCLUDED
#define HEAD_HPP_INCLUDED

#define MAXIMO_PONTOS 13206
#define MAXIMO_POSICOES 8
#define MAXIMO_CONFLITOS 50
#define TAMANHO_MAXIMO 13206*8

typedef struct campo {
    int id;
    int quantidadeDeConflitos;
    int conflitos[MAXIMO_CONFLITOS];
} Campo;

int numeroDePontos;
int numeroDePosicoesCandidatas;
Campo posicoesCandidatas[TAMANHO_MAXIMO];
int posicoesConflitantes[MAXIMO_PONTOS];
double vetorDePesos[TAMANHO_MAXIMO];

typedef struct solucao {
    int posicaoDosPontos[MAXIMO_PONTOS];
    int funcao_objetivo;
    int numeroDeConflitos;
}Solucao;

void vns(const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total);

void gerarVizinha(Solucao &s, int qtd);

void heuBLPM(Solucao &s);

void lerArquivo(std::string arq);

void heuConGul(Solucao &s);

void calcularFO(Solucao &s);

void calcularConflitos(Solucao &s);

void clonarSolucao(Solucao &s1, Solucao &s2);

void testarDados(Solucao &s, std::string arq);

void lerSolucaoDeArquivo(Solucao &s, std::string path);

void testarF0(Solucao &s);

void testarHeuristicaConstrutivaGulosa(Solucao &s);

void solucaoInicialGulosa(Solucao &s);

/*
int gerarNumero(int lim_inf, int lim_sup);

void heuConAle(Solucao &s);

void testarHeuristicaConstrutivaAleatoria(Solucao &s);

void solucaoInicialAleatoria(Solucao &s);
*/

#endif
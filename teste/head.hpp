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
int tempo_limite;

typedef struct solucao {
    int posicaoDosPontos[MAXIMO_PONTOS];
    int funcao_objetivo;
    int numeroDeConflitos;
}Solucao;

void vns(Solucao &s, double &tempo_melhor, double &tempo_total);

int converter(int num);

void gerarVizinha(Solucao &s, int qtd);

void heuBLPM1(Solucao &s, clock_t hi);

void heuBLPM2(Solucao &s, clock_t hi);

void heuBLPM3(Solucao &s, clock_t hi);

void lerArquivo(std::string arq);

void heuConGul(Solucao &s);

void calcularFO(Solucao &s);

void calcularFO2(Solucao &s);

void calcularConflitos(Solucao &s);

void clonarSolucao(Solucao &s1, Solucao &s2);

void testarDados(Solucao &s, std::string arq);

void testarDadosComTempo(Solucao &s, std::string arq, double tempo_total, double tempo_melhor);

void lerSolucaoDeArquivo(Solucao &s, std::string path);

void testarF0(Solucao &s);

void testarHeuristicaConstrutivaGulosa(Solucao &s);

void testarVNS(Solucao &s);

void testarHeuBLPMs(Solucao &s, int funcao);

void solucaoInicialGulosa(Solucao &s);

int gerarNumero(int lim_inf, int lim_sup);

/*
void heuConAle(Solucao &s);

void testarHeuristicaConstrutivaAleatoria(Solucao &s);

void solucaoInicialAleatoria(Solucao &s);
*/

#endif
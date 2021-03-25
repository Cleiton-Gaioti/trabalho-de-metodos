#include <iostream>
#include <string>
#include <memory.h>
#include <time.h>
#include "head.hpp"

using namespace std;

#define MAX(A,B) ((A > B) ? A : B)

void lerArquivo(string arq){
    FILE* f = fopen(arq.c_str(), "r");

    fscanf(f, "%d %d", &numeroDePontos, &numeroDePosicoesCandidatas);
    memset(&posicoesCandidatas, -1, sizeof(posicoesCandidatas));

    for(int i = 0; i < (numeroDePontos*numeroDePosicoesCandidatas); i++){
        fscanf(f, "%d", &quantidadeDeConflitos[i]);
        for(int j = 0; j < quantidadeDeConflitos[i]; j++) {
            fscanf(f, "%d", &posicoesCandidatas[i][j]);
        }
    }

    fclose(f);
}

/*bool sobrepoe(int linha, int num) {
    for (int i = 0; i < numeroDePosicoesCandidatas; i++) {
        if(num == posicoesCandidatas[linha][i])
            return false;
    }
    return true;
}*/

int gerarNumero(int lim_inf, int lim_sup){
    return(lim_inf + rand() % (lim_sup-lim_inf+1));
}

void heuConAle(Solucao &s) {
    for(int j = 0; j < numeroDePontos; j++) {
        s.posicaoDosPontos[j] = gerarNumero(j*numeroDePosicoesCandidatas + 1, (j+1)*numeroDePosicoesCandidatas);
    }
}

void calcularFO(Solucao &s){

    memset(&pontosLivres, -1, sizeof(pontosLivres));

    for (int i = 0; i < numeroDePontos; i++) {
        for (int j = i + 1; j < numeroDePontos; j++) {
            for (int k = 0; k < quantidadeDeConflitos[s.posicaoDosPontos[i]-1]; k++){
                if(posicoesCandidatas[s.posicaoDosPontos[i] - 1][k] == s.posicaoDosPontos[j]) {
                    pontosLivres[i] = pontosLivres[j] = 0;
                    break;
                }
            }
        }
    }

    int contador = 0;
    for(int i = 0; i < numeroDePontos; i++) {
        if (pontosLivres[i] == -1) {
            contador++;
        }
    }
    s.funcao_objetivo = contador;
}

/*void calcularFO(Solucao &s){
    int contador = 0;
    for (int i = 0; i < numeroDePontos; i++) {
        for (int j = i + 1; j < numeroDePontos; j++) {
            for (int k = i + 1; k < quantidadeDeConflitos[s.posicaoDosPontos[i]-1]; k++){
                if(posicoesCandidatas[s.posicaoDosPontos[i] - 1][k] == s.posicaoDosPontos[j]) {
                    contador++;
                    break;
                }
            }
        }
    }
    contador *= 2;
    s.funcao_objetivo = numeroDePontos - contador;
}*/

void clonarSolucao(Solucao &s1, Solucao &s2){
    s2.funcao_objetivo = s2.funcao_objetivo;
    for (int i = 0; i < numeroDePontos; i++) {
        s2.posicaoDosPontos[i] = s1.posicaoDosPontos[i];
    } 
}

void testarDados(Solucao &s, string arq)
{
    FILE *f;
    if(arq.empty())
        f = stdout;
    else
        f = fopen(arq.c_str(), "w");
    fprintf(f,"Numero de pontos %d\n", numeroDePontos);
    fprintf(f, "Numero de posicoes candidatas %d\n", numeroDePosicoesCandidatas);
    fprintf(f, "Numero de conflitos %d\n", numeroDePontos - s.funcao_objetivo);
    fprintf(f,"Valor da FO %d\n", s.funcao_objetivo);
    //for(int i = 0; i < numeroDePontos; i++)
    //  fprintf(f, "%d\n", s.posicaoDosPontos[i]);
    if(arq != "")
        fclose(f);
}

void lerSolucaoDeArquivo(Solucao &s, string path){
    FILE* f = fopen(path.c_str(), "r");

    numeroDePontos = fscanf(f, "%d");
    numeroDePosicoesCandidatas = fscanf(f, "%d");
    s.funcao_objetivo = fscanf(f, "%d");

    for (int i = 0; i < numeroDePontos; i++) {
        s.posicaoDosPontos[i] = fscanf(f, "%d");
    }

    fclose(f);
}

void testarF0(Solucao &s){
    clock_t h;
    double tempo;
    heuConAle(s);
    
    h = clock();
    for (int i = 0; i < 1000; i++) {
        calcularFO(s);
    }
    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << tempo << "s" << endl;
}

void testarHeuristica(Solucao &s) {
    clock_t h;
    double tempo;
    
    h = clock();
    for (int i = 0; i < 1000; i++) {
        heuConAle(s);
    }
    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << tempo << "s" << endl;
}

int main(){
    //srand(time(NULL));
    Solucao sol;
    lerArquivo("i13206p8.txt");
    //testarHeuristica(sol);
    heuConAle(sol);
    calcularFO(sol);
    cout << sol.funcao_objetivo << endl;
    //testarDados(sol, "");
    //testarF0(sol);

    return 0;
}
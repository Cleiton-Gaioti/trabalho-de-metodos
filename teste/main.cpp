#include <iostream>
#include <string>
#include <memory.h>
#include <time.h>
#include "head.hpp"

using namespace std;

void lerArquivo(string arq){
    FILE* f = fopen(arq.c_str(), "r");

    fscanf(f, "%d %d", &numeroDePontos, &numeroDePosicoesCandidatas);
    memset(&posicoesCandidatas->id, -1, sizeof(posicoesCandidatas));

    for(int i = 0; i < (numeroDePontos*numeroDePosicoesCandidatas); i++){
        posicoesCandidatas[i].id = i+1;
        fscanf(f, "%d", &posicoesCandidatas[i].quantidadeDeConflitos);
        vetorDePesos[i] = (double)posicoesCandidatas[i].quantidadeDeConflitos/numeroDePontos;
        //cout << vetorDePesos[i] << " ";
        for(int j = 0; j < posicoesCandidatas[i].quantidadeDeConflitos; j++) {
            fscanf(f, "%d", &posicoesCandidatas[i].conflitos[j]);
        }
    }

    fclose(f);
}

int gerarNumero(int lim_inf, int lim_sup){
    return(lim_inf + rand() % (lim_sup-lim_inf+1));
}

void heuConAle(Solucao &s) {
    for(int j = 0; j < numeroDePontos; j++) {
        s.posicaoDosPontos[j] = gerarNumero(j*numeroDePosicoesCandidatas + 1, (j+1)*numeroDePosicoesCandidatas);
    }
}

void heuConGul(Solucao &s) {

    for (int i = 0; i < numeroDePontos; i++) {
        double menorPeso = __DBL_MAX__;
        int indMenorPeso = 0;
        for(int j = i * numeroDePosicoesCandidatas; j < (i + 1) * numeroDePosicoesCandidatas; j++) {
            if(vetorDePesos[j] < menorPeso) {
                menorPeso = vetorDePesos[j];
                indMenorPeso = j + 1;
            }
        }
        s.posicaoDosPontos[i] = indMenorPeso;
        //cout << s.posicaoDosPontos[i] << " ";
    }
    
}

void calcularFO(Solucao &s){
    
    memset(&posicoesConflitantes, -1, sizeof(posicoesConflitantes));

    for(int i = 0; i < numeroDePontos; i++) {
        for(int j = i + 1; j < numeroDePontos; j++) {
                for(int k = 0; k < posicoesCandidatas[s.posicaoDosPontos[i] - 1].quantidadeDeConflitos; k++) {
                    if(s.posicaoDosPontos[j] == posicoesCandidatas[s.posicaoDosPontos[i] - 1].conflitos[k]) {
                        posicoesConflitantes[i] = posicoesConflitantes[j] = 0;
                        s.numeroDeConflitos++;
                        break;
                    }
                }
        }
    }

    int contador = 0;
    for(int i = 0; i < numeroDePontos; i++) {
        if (posicoesConflitantes[i] == -1) {
            contador++;
        }
    }
    s.funcao_objetivo = contador;
}

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
    fprintf(f, "Numero de pontos %d\n", numeroDePontos);
    fprintf(f, "Numero de posicoes candidatas %d\n", numeroDePosicoesCandidatas);
    fprintf(f, "Numero de conflitos %d\n", s.numeroDeConflitos);
    fprintf(f,"Valor da FO %d\n", s.funcao_objetivo);
    for(int i = 0; i < numeroDePontos; i++)
        fprintf(f, "%d\n", s.posicaoDosPontos[i]);
    if(arq != "")
        fclose(f);
}

void lerSolucaoDeArquivo(Solucao &s, string path){
    FILE* f = fopen(path.c_str(), "r");

    fscanf(f, "%d", &numeroDePontos);
    cout << numeroDePontos << endl;
    fscanf(f, "%d", &numeroDePosicoesCandidatas);
    cout << numeroDePosicoesCandidatas << endl;
    fscanf(f, "%d", &s.funcao_objetivo);
    cout << s.funcao_objetivo << endl;

    for (int i = 0; i <= numeroDePontos; i++) {
        fscanf(f, "%d", &s.posicaoDosPontos[i]);
        cout << s.posicaoDosPontos[i] << endl;
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

void testarHeuristicaConstrutivaAleatoria(Solucao &s) {
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

void testarHeuristicaConstrutivaGulosa(Solucao &s) {
    clock_t h;
    double tempo;
    
    h = clock();
    for (int i = 0; i < 1000; i++) {
        heuConGul(s);
    }
    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << tempo << "s" << endl;
}

void solucaoInicialAleatoria(Solucao &s){
    clock_t h;
    double tempo;
    h = clock();
    
    heuConAle(s);
    calcularFO(s);

    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    printf("Tempo de execução: %.5lfs\n", tempo);
    cout << "Valor da FO: " << s.funcao_objetivo << endl;
}

void solucaoInicialGulosa(Solucao &s) {
    clock_t h;
    double tempo;
    h = clock();

    heuConGul(s);
    calcularFO(s);

    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    printf("Tempo de execução: %.5lfs\n", tempo);
    cout << "Valor da FO: " << s.funcao_objetivo << endl;
}


int main(){

    Solucao sol1, sol2;
    lerArquivo("i13206p8.txt");
    //lerSolucaoDeArquivo(sol1, "arq.txt");

    int opcao;

    do {
        cout << "1 - SOLUÇÃO INICIAL ALEATÓRIA" << endl;
        cout << "2 - SOLUÇÃO INICIAL GULOSA" << endl;
        cout << "3 - TESTAR HEURÍSTICA CONSTRUTIVA ALEATÓRIA" << endl;
        cout << "4 - TESTAR HEURÍSTICA CONSTRUTIVA GULOSA" << endl;
        cout << "5 - TESTAR FUNÇÃO OBJETIVO ALEATÓRIA" << endl;
        cout << "6 - TESTAR FUNÇÃO OBJETIVO GULOSA" << endl;
        cout << "0 - SAIR" << endl;

        cin >> opcao;

        switch (opcao) {
        case 1:
            solucaoInicialAleatoria(sol1);
            testarDados(sol1, "solHeuConAle.txt");
            break;
        case 2:
            solucaoInicialGulosa(sol2);
            testarDados(sol2, "solHeuConGul.txt");
            break;
        case 3:
            testarHeuristicaConstrutivaAleatoria(sol1);
            break;
        case 4:
            testarHeuristicaConstrutivaGulosa(sol2);
            break;
        case 5:
            calcularFO(sol1);
            break;
        case 6:
            calcularFO(sol2);
            break;
        case 0:
            break;
        default:
            cout << "OPÇÃO INVÁLIDA!" << endl;
        }

    }  while(opcao != 0);

    return 0;
}
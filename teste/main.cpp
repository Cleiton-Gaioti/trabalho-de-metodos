#include <iostream>
#include <string>
#include <memory.h>
#include <time.h>
#include "head.hpp"

using namespace std;

/*
 *  Lê os dados de entrada e armazena no vetor de Posições Candidatas,
 *  cada posição do vetor é um struct Campo, que armazena o id da posição,
 *  a quantidade de conflitos dela e os pontos conflitantes. Essa função 
 *  também inicializa e preeenche o vetor de pesos das posições, o peso é 
 *  dado pelo (nº de conflitos)/(nº de pontos).
 */

void lerArquivo(string arq) {
    FILE* f = fopen(arq.c_str(), "r");

    fscanf(f, "%d %d", &numeroDePontos, &numeroDePosicoesCandidatas);
    memset(&posicoesCandidatas->id, -1, sizeof(posicoesCandidatas));

    for(int i = 0; i < (numeroDePontos*numeroDePosicoesCandidatas); i++) {
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

/*
 *  Atribui a cada ponto a sua posição com o menor peso.
 */

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

/*
 *  Percorre cada ponto do vetor de posições e verifica se os outros pontos da solução estão
 *  presentes na sua lista de conflitos, se houver conflitos, marcam os pontos no vetor  de 
 *  posições conflitantes, no fim da função contabiliza o nº de pontos livres verificando se 
 *  o valor original da posição do vetor foi alterado.
 */

void calcularFO(Solucao &s){
    
    memset(&posicoesConflitantes, -1, sizeof(posicoesConflitantes));

    for(int i = 0; i < numeroDePontos; i++) {
        for(int j = i + 1; j < numeroDePontos; j++) {
                for(int k = 0; k < posicoesCandidatas[s.posicaoDosPontos[i] - 1].quantidadeDeConflitos; k++) {
                    if(s.posicaoDosPontos[j] == posicoesCandidatas[s.posicaoDosPontos[i] - 1].conflitos[k]) {
                        posicoesConflitantes[i] = posicoesConflitantes[j] = 0;
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

void calcularConflitos(Solucao &s) {
    int conflitos = 0;

    for (int i = 0; i < numeroDePontos; i++) {
        for(int j = i + 1; j < numeroDePontos; j++) {
            for(int k = 0; k < posicoesCandidatas[s.posicaoDosPontos[i] - 1].quantidadeDeConflitos; k++) {
                if(s.posicaoDosPontos[j] == posicoesCandidatas[s.posicaoDosPontos[i] - 1].conflitos[k]) {
                    conflitos++;
                }
            }
        }
    }
    s.numeroDeConflitos = conflitos;
}

void clonarSolucao(Solucao &s1, Solucao &s2) {
    memcpy(&s2, &s1, sizeof(s1));
    testarDados(s2, "");
}

void testarDados(Solucao &s, string arq) {
    FILE *f;
    if(arq.empty()) 
        f = stdout;
    else 
        f = fopen(arq.c_str(), "w");
    fprintf(f, "Numero de pontos %d\n", numeroDePontos);
    fprintf(f, "Numero de posicoes candidatas %d\n", numeroDePosicoesCandidatas);
    fprintf(f, "Numero de conflitos %d\n", s.numeroDeConflitos);
    fprintf(f, "Valor da FO %d\n", s.funcao_objetivo);

    if(f != stdout){
        fprintf(f, "Posição de cada ponto\n");
        for(int i = 0; i < numeroDePontos; i++)
            fprintf(f, "%d\n", s.posicaoDosPontos[i]);
    } else
        cout << "Solução completa gerada no arquivo solHeuConGul.txt\n" << endl;

    if(arq != "")
        fclose(f);
}

void lerSolucaoDeArquivo(Solucao &s, string path) {
    try {
        FILE* f = fopen(path.c_str(), "r");

        fscanf(f, "%d", &numeroDePontos);
        fscanf(f, "%d", &numeroDePosicoesCandidatas);
        fscanf(f, "%d", &s.funcao_objetivo);

        for (int i = 0; i < numeroDePontos; i++) {
            fscanf(f, "%d", &s.posicaoDosPontos[i]);
            s.posicaoDosPontos[i] += numeroDePosicoesCandidatas*i;
        }

        cout << "Solução copiada\n" << endl;
        fclose(f);
    } catch (exception e){
        cout << "Não foi possível abrir o arquivo!\n" << endl;
    }
}

/*
 *  Roda o método de calcular a FO 1000 vezes e captura o tempo de execução.
 */

void testarF0(Solucao &s){
    clock_t h;
    double tempo;

    heuConGul(s);    

    h = clock();
    for (int i = 0; i < 1000; i++) {
        calcularFO(s);
    }
    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    cout << "Tempo de execução: " << tempo << "s" << endl;
}

/*
 *  Roda o método da heurística 1000 vezes e captura o tempo de execução.
 */

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

/*
 *  Captura o tempo de execução dos métodos da heurística e de calcular a FO 1 vez
 */

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


/*
 *  Criar uma função a parte para calcular o número de conflitos
 *  Ajeitar a leitura da solucao
 *  Ajeitar a função de testar dados
 */

int main(){

    Solucao sol;
    lerArquivo("i25.txt");
    //lerSolucaoDeArquivo(sol, "arq.txt");
    //testarDados(sol, "solHeuConGul");

    int opcao;
    string arq;

    do {
        cout << endl;
        cout << "1 - SOLUÇÃO INICIAL" << endl;
        cout << "2 - TESTAR HEURÍSTICA" << endl;
        cout << "3 - TESTAR FUNÇÃO OBJETIVO" << endl;
        cout << "4 - IMPRIMIR SOLUÇÃO" << endl;
        cout << "0 - SAIR" << endl;

        cout << "\nEscolha uma opção: ";
        cin >> opcao;
        cout << endl;

        switch (opcao) {
        case 1:
            solucaoInicialGulosa(sol);
            calcularConflitos(sol);
            testarDados(sol, "solHeuConGul.txt");
            break;
        case 2:
            testarHeuristicaConstrutivaGulosa(sol);
            break;
        case 3:
            testarF0(sol);
            break;
        case 4:
            testarDados(sol, "");
            break;
        case 0:
            break;
        default:
            cout << "OPÇÃO INVÁLIDA!" << endl;
        }

    }  while(opcao != 0);

    return 0;
}

/*
int gerarNumero(int lim_inf, int lim_sup){
    return(lim_inf + rand() % (lim_sup-lim_inf+1));
}

void heuConAle(Solucao &s) {
    for(int j = 0; j < numeroDePontos; j++) {
        s.posicaoDosPontos[j] = gerarNumero(j*numeroDePosicoesCandidatas + 1, (j+1)*numeroDePosicoesCandidatas);
    }
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
*/
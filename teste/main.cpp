#include <iostream>
#include <string>
#include <memory.h>
#include <time.h>
#include "head.hpp"

#define DBG

using namespace std;

/*
 *  Criar uma função a parte para calcular o número de conflitos
 *  Ajeitar a leitura da solucao
 *  Ajeitar a função de testar dados
 */

int main() {

    Solucao sol;
    clock_t h;
    double tempo;
    srand(time(NULL));
    const double tempo_limite = 5;
    double tempo_melhor, tempo_total;
    lerArquivo("arquivos/i1000.txt");

    int opcao;

    do {
        cout << endl;
        cout << "1 - SOLUÇÃO INICIAL" << endl;
        cout << "2 - TESTAR HEURÍSTICA" << endl;
        cout << "3 - TESTAR FUNÇÃO OBJETIVO" << endl;
        cout << "4 - IMPRIMIR SOLUÇÃO" << endl;
        cout << "5 - MELHORAR SOLUÇÃO" << endl;
        cout << "6 - VNS" << endl;
        cout << "0 - SAIR" << endl;

        cout << "\nEscolha uma opção: ";
        cin >> opcao;
        cout << endl;

        switch (opcao) {
        case 1:
            solucaoInicialGulosa(sol);
            testarDados(sol, "arquivos/solHeuConGul.txt");
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
        case 5:
            h = clock();
            heuBLPM2(sol);
            h = clock() - h;
            printf("Tempo execução BLPM2: %.5f\n", (double) h/CLOCKS_PER_SEC);
            testarDados(sol, "");
            break;
        case 6:
            h = clock();
            vns(tempo_limite, sol, tempo_melhor, tempo_total);
            h = clock() - h;
            printf("Tempo execução VNS: %.5f\n", (double) h/CLOCKS_PER_SEC);
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

void vns(const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total) {
    int v;
    clock_t hi,hf;
    Solucao s_vizinha;
    cout << "EXECUTANDO O VNS...\n" << endl;
    hi = clock();
    heuConGul(s);
    calcularFO2(s);
    hf = clock();
    tempo_melhor = ((double)(hf - hi))/CLOCKS_PER_SEC;

    #ifdef DBG
        printf("FO: %d\tTempo: %.2f\n", s.funcao_objetivo, tempo_melhor);
    #endif

    tempo_total = tempo_melhor;

    while (tempo_total < tempo_max) {
        v = 1;
        while (v <= 3) {
            memcpy(&s_vizinha, &s, sizeof(s));
            if(v == 1) {
                gerarVizinha(s_vizinha, 1);
            } else if(v == 2) {
                gerarVizinha(s_vizinha, 2);
            } else if(v == 3) {
                gerarVizinha(s_vizinha, 3);
            } else if(v == 4) {
                gerarVizinha(s_vizinha, 4);
            } else
                gerarVizinha(s_vizinha, 5);

            //heuBLPM2(s_vizinha);
            (rand()%2) == 0 ? heuBLPM1(s_vizinha) : heuBLPM2(s_vizinha);

            if(s_vizinha.funcao_objetivo > s.funcao_objetivo) {
                memcpy(&s, &s_vizinha, sizeof(s_vizinha));
                hf = clock();
                tempo_melhor = ((double)(hf - hi))/CLOCKS_PER_SEC;

                #ifdef DBG
                    printf("FO: %d\tTempo: %.2f\n", s.funcao_objetivo, tempo_melhor);
                #endif
                
                v = 1;
            } else
                v++;
        }
        hf = clock();
        tempo_total = ((double)(hf - hi))/CLOCKS_PER_SEC;
    }
}

int converter(int num) {
    return (num % numeroDePosicoesCandidatas == 0) ? numeroDePosicoesCandidatas : num % numeroDePosicoesCandidatas;
}

void gerarVizinha(Solucao &s, int qtd) {
    int ponto1, ponto2, ponto3, ponto4, posicao, aux;

    if(qtd == 1) {
        ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            posicao = gerarNumero(ponto1 * numeroDePosicoesCandidatas, (ponto1 + 1) * numeroDePosicoesCandidatas);
        } while(posicao == s.posicaoDosPontos[ponto1]);
        
        s.posicaoDosPontos[ponto1] = posicao;

    } else if(qtd == 2) {
            ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            ponto2 = gerarNumero(0, numeroDePontos - 1);
        } while(converter(s.posicaoDosPontos[ponto1]) == converter(s.posicaoDosPontos[ponto2]));
        
        aux = converter(s.posicaoDosPontos[ponto1]);
        s.posicaoDosPontos[ponto1] = ponto1 * numeroDePosicoesCandidatas + (s.posicaoDosPontos[ponto2]);
        s.posicaoDosPontos[ponto2] = ponto2 * numeroDePosicoesCandidatas + aux;

    } else if(qtd == 3) {

        // Posições aleatórias para 2 pontos

        ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            ponto2 = gerarNumero(0, numeroDePontos - 1);
        } while(converter(s.posicaoDosPontos[ponto1]) == converter(s.posicaoDosPontos[ponto2]));

        int p1 = s.posicaoDosPontos[ponto1];
        int p2 = s.posicaoDosPontos[ponto2];

        do {
            s.posicaoDosPontos[ponto1] = gerarNumero(ponto1 * numeroDePosicoesCandidatas, (ponto1+1)*numeroDePosicoesCandidatas);
            s.posicaoDosPontos[ponto2] = gerarNumero(ponto2 * numeroDePosicoesCandidatas, (ponto2+1)*numeroDePosicoesCandidatas);
        } while(s.posicaoDosPontos[ponto1] == p1 && s.posicaoDosPontos[ponto2] == p2);

    } else if(qtd == 4){
        //melhor posicao de 1 ponto

        int ponto = gerarNumero(0, numeroDePontos - 1);
        int melhorPosicao, melhorPeso = INT16_MAX;

        for (int i = 0; i < numeroDePosicoesCandidatas; i++) {
            if(vetorDePesos[ponto * numeroDePosicoesCandidatas + i] < melhorPeso) 
                melhorPeso = vetorDePesos[ponto * numeroDePosicoesCandidatas + i];
                melhorPosicao = ponto * numeroDePosicoesCandidatas + i;
        }
        
        s.posicaoDosPontos[ponto] = melhorPosicao;
    } else {
        //pior posição de 1 ponto

        int ponto = gerarNumero(0, numeroDePontos - 1);
        int piorPosicao, piorPeso = 0;

        for (int i = 0; i < numeroDePosicoesCandidatas; i++) {
            if(vetorDePesos[ponto * numeroDePosicoesCandidatas + i] > piorPeso) { 
                piorPeso = vetorDePesos[ponto * numeroDePosicoesCandidatas + i];
                piorPosicao = ponto * numeroDePosicoesCandidatas + i;
            }    
        }
        
        s.posicaoDosPontos[ponto] = piorPosicao;
    }
    calcularFO2(s);
}

void heuBLPM1(Solucao &s) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;

    int indice[numeroDePontos];

    for (int i = 0; i < numeroDePontos; i++) {
        indice[i] = i;
    }

    INICIO: ;
    foOriginal = s.funcao_objetivo;
 
    for (int k = 0; k < numeroDePontos; k++) {  
        int i = k + rand()%(numeroDePontos - k);

        posicaoOriginal = s.posicaoDosPontos[i];
        for (int j = 1; j <= numeroDePosicoesCandidatas; j++) {
            novaPosicao = i*numeroDePosicoesCandidatas + j;

            if(novaPosicao != posicaoOriginal) {
                s.posicaoDosPontos[i] = novaPosicao;
                calcularFO2(s);
                
                if (s.funcao_objetivo > melhorFO) {
                    melhorFO = s.funcao_objetivo;
                    goto INICIO;
                } else {
                    s.posicaoDosPontos[i] = posicaoOriginal;
                    s.funcao_objetivo = foOriginal;
                }
            }
        }
        
        aux = indice[k];
        indice[k] = indice[i];
        indice[i] = aux;

    }
    calcularFO2(s);
    calcularConflitos(s);
}

void heuBLPM2(Solucao &s) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;

    INICIO: ;
    foOriginal = s.funcao_objetivo;
 
    for (int k = 0; k < (int)(numeroDePontos * 0.2); k++) {  
        int i = rand()%numeroDePontos;
        posicaoOriginal = s.posicaoDosPontos[i];
        for (int j = 1; j <= numeroDePosicoesCandidatas; j++) {
            novaPosicao = i*numeroDePosicoesCandidatas + j;

            if(novaPosicao != posicaoOriginal) {
                s.posicaoDosPontos[i] = novaPosicao;
                calcularFO2(s);
                
                if (s.funcao_objetivo > melhorFO) {
                    melhorFO = s.funcao_objetivo;
                    goto INICIO;
                } else {
                    s.posicaoDosPontos[i] = posicaoOriginal;
                    s.funcao_objetivo = foOriginal;
                }
            }
        }
    }
    calcularFO2(s);
    calcularConflitos(s);
}

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

void calcularFO2(Solucao &s){
    
    memset(&posicoesConflitantes, -1, sizeof(posicoesConflitantes));

    for(int i = 0; i < numeroDePontos; i++) {
        if (posicoesConflitantes[i] != 0) {
            for(int j = i + 1; j < numeroDePontos; j++) {
                    for(int k = 0; k < posicoesCandidatas[s.posicaoDosPontos[i] - 1].quantidadeDeConflitos; k++) {
                        if(s.posicaoDosPontos[j] == posicoesCandidatas[s.posicaoDosPontos[i] - 1].conflitos[k]) {
                            posicoesConflitantes[i] = posicoesConflitantes[j] = 0;
                            break;
                        }
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
            //fprintf(f, "%d\n", s.posicaoDosPontos[i]);
            fprintf(f, "%d\n", (s.posicaoDosPontos[i] % numeroDePosicoesCandidatas) == 0 ? numeroDePosicoesCandidatas : (s.posicaoDosPontos[i] % numeroDePosicoesCandidatas));
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
    calcularFO2(s);

    h = clock() - h;

    tempo = (double) h/CLOCKS_PER_SEC;
    printf("Tempo de execução: %.5lfs\n", tempo);
    cout << "Valor da FO: " << s.funcao_objetivo << endl;
    calcularConflitos(s);

}

int gerarNumero(int lim_inf, int lim_sup){
    return(lim_inf + rand() % (lim_sup - lim_inf + 1));
}

/*
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

void heuBLPM(Solucao &s) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;

    INICIO: ;
    foOriginal = s.funcao_objetivo;

    int indice[numeroDePontos];

    for (int i = 0; i < numeroDePontos; i++)
    {
        indice[i] = i;
    }

    for (int i = 0; i < numeroDePontos; i++) {  
        posicaoOriginal = s.posicaoDosPontos[i];
        for (int j = 1; j <= numeroDePosicoesCandidatas; j++) {
            novaPosicao = i*numeroDePosicoesCandidatas + j;
            novaPosicao = gerarNumero(i*numeroDePosicoesCandidatas, (i+1)*numeroDePosicoesCandidatas);

            if(novaPosicao != posicaoOriginal) {
                s.posicaoDosPontos[i] = novaPosicao;
                calcularFO(s);
                
                if (s.funcao_objetivo > melhorFO) {
                    melhorFO = s.funcao_objetivo;
                    goto INICIO;
                } else {
                    s.posicaoDosPontos[i] = posicaoOriginal;
                    s.funcao_objetivo = foOriginal;
                }
            }
        }
        
        aux = indice[k];
        indice[k] = indice[i];
        indice[i] = aux;

    }
    calcularFO(s);
    calcularConflitos(s);
}
*/
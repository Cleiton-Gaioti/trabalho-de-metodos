#include <iostream>
#include <string>
#include <memory.h>
#include <time.h>
#include "head.hpp"

//#define DBG

using namespace std;

int main(int argc, char *argv[]) {

    Solucao sol;
    double tempo_melhor, tempo_total; 
    int seeds[] = {0, 398647, 839458745};
    int tempo[] = {60, 60, 60, 60, 60, 120, 1800, 1800};
    string instancia[] = {"i25", "i100", "i250", "i500", "i750","i1000", "i13206", "i13206p8"};

    FILE* f = fopen("saida.txt", "w");
    
    for(int i = 0; i < 8; i++) {
        tempo_limite = tempo[i];
        lerArquivo("arquivos/" + instancia[i] + ".txt");

        for(int j = 0; j < 3; j++) {
            srand(seeds[j]);
            vns(sol, tempo_melhor, tempo_total);
            testarDados(sol, "solucoes/" + instancia[i] + ".sol" + to_string(j+1));
            fprintf(f, "%s\t%d\t%d\t\t%d\t\t%.5lf\t\t%.5lf\n", instancia[i].c_str(), j+1, seeds[j], sol.funcao_objetivo, tempo_melhor, tempo_total);
        }
        fprintf(f, "##################################################################n");
    }
    fclose(f);

    return 0;
}

void vns(Solucao &s, double &tempo_melhor, double &tempo_total) {
    int v;
    clock_t hi, hf;
    Solucao s_vizinha;
    
    /* Calcula a solução inicial e captura o tempo dela. */

    hi = clock();
    heuConGul(s);
    calcularFO(s);
    hf = clock();
    tempo_melhor = ((double)(hf - hi))/CLOCKS_PER_SEC;

    #ifdef DBG
        printf("FO: %d\tTempo: %.2f\n", s.funcao_objetivo, tempo_melhor);
    #endif

    tempo_total = tempo_melhor;

    /* Executa o código enquanto não ultrapassar o tempo máximo. */

    while (tempo_total < tempo_limite) {
        v = 1;
        while (v <= 5) {
            memcpy(&s_vizinha, &s, sizeof(s));

            /* Cada mudança no valor de v altera a forma de gerar vizinhos. */

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

            int opcao = rand() % 3;

            /* Escolhe uma busca local aleatóriamente. */

            if(opcao == 0)
                heuBLPM1(s_vizinha, hi); 
            else if(opcao == 1)
                heuBLPM2(s_vizinha, hi);
            else
                heuBLPM3(s_vizinha, hi);

            /* 
             * Se a solução gerada for melhor que a anterior, o valor da FO é atualizado
             * e a geração de vizinhos se mantém a mesma. Caso contrário, a geração de 
             * vizinhos é alterada.
             */

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

/* Converte o ID para uma posição de um ponto */

int converter(int num) {
    return (num % numeroDePosicoesCandidatas == 0) ? numeroDePosicoesCandidatas : num % numeroDePosicoesCandidatas;
}

void gerarVizinha(Solucao &s, int qtd) {
    int ponto1, ponto2, posicao, aux;

    /* Escolhe um ponto aleatório e altera a sua posição */

    if(qtd == 1) {
        ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            posicao = gerarNumero(ponto1 * numeroDePosicoesCandidatas + 1, (ponto1 + 1) * numeroDePosicoesCandidatas);
        } while(posicao == s.posicaoDosPontos[ponto1]);
        
        s.posicaoDosPontos[ponto1] = posicao;

    /* Escolhe dois pontos aleatoriamente, ambos com posições diferentes e troca suas posições */

    } else if(qtd == 2) {
            ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            ponto2 = gerarNumero(0, numeroDePontos - 1);
        } while(converter(s.posicaoDosPontos[ponto1]) == converter(s.posicaoDosPontos[ponto2]));
        
        aux = converter(s.posicaoDosPontos[ponto1]);
        s.posicaoDosPontos[ponto1] = ponto1 * numeroDePosicoesCandidatas + converter(s.posicaoDosPontos[ponto2]);
        s.posicaoDosPontos[ponto2] = ponto2 * numeroDePosicoesCandidatas + aux;

    } else if(qtd == 3) {

        /* Gera posições aleatórias para 2 pontos aleatórios */

        ponto1 = gerarNumero(0, numeroDePontos - 1);

        do {
            ponto2 = gerarNumero(0, numeroDePontos - 1);
        } while(converter(s.posicaoDosPontos[ponto1]) == converter(s.posicaoDosPontos[ponto2]));

        int p1 = s.posicaoDosPontos[ponto1];
        int p2 = s.posicaoDosPontos[ponto2];

        do {
            s.posicaoDosPontos[ponto1] = gerarNumero(ponto1 * numeroDePosicoesCandidatas + 1, (ponto1+1)*numeroDePosicoesCandidatas);
            s.posicaoDosPontos[ponto2] = gerarNumero(ponto2 * numeroDePosicoesCandidatas + 1, (ponto2+1)*numeroDePosicoesCandidatas);
        } while(s.posicaoDosPontos[ponto1] == p1 && s.posicaoDosPontos[ponto2] == p2);

    } else if(qtd == 4){
        
        /* Escolhe a melhor posicao de 1 ponto aleatório  */

        int ponto = gerarNumero(0, numeroDePontos - 1);
        int melhorPosicao, melhorPeso = INT16_MAX;

        for (int i = 0; i < numeroDePosicoesCandidatas; i++) {
            if(vetorDePesos[ponto * numeroDePosicoesCandidatas + i] < melhorPeso) 
                melhorPeso = vetorDePesos[ponto * numeroDePosicoesCandidatas + i];
                melhorPosicao = ponto * numeroDePosicoesCandidatas + i;
        }
        
        s.posicaoDosPontos[ponto] = melhorPosicao;
    } else {
        /* Escolhe a pior posição de 1 ponto aleatório */

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
    calcularFO(s);
}

void heuBLPM1(Solucao &s, clock_t hi) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;
    clock_t hf;

    int indice[numeroDePontos];

    for (int i = 0; i < numeroDePontos; i++) {
        indice[i] = i;
    }

    INICIO: ;

    foOriginal = s.funcao_objetivo;
    
    for (int k = 0; k < numeroDePontos; k++) { 
         
        hf = clock();

        if((double)(hf - hi)/CLOCKS_PER_SEC < tempo_limite) {

            /* Escolhe um ponto aleatório */

            int i = k + rand()%(numeroDePontos - k);

            posicaoOriginal = s.posicaoDosPontos[i];

            for (int j = 1; j <= numeroDePosicoesCandidatas; j++) {

                hf = clock();

                if((double)(hf - hi)/CLOCKS_PER_SEC < tempo_limite) {

                    /* 
                     * Para cada posição do ponto, difente da atual, é calculada a FO.
                     * Se a solução gerada for melhor que a anterior, o valor da FO é 
                     * atualizado e a execução volta ao INICIO.
                     */

                    novaPosicao = i*numeroDePosicoesCandidatas + j;

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
            }
            
            aux = indice[k];
            indice[k] = indice[i];
            indice[i] = aux;

        }
    }

    calcularFO(s);
    calcularConflitos(s);
}

void heuBLPM2(Solucao &s, clock_t hi) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;
    clock_t hf;

    INICIO: ;
    
    foOriginal = s.funcao_objetivo;
    
    /* 
     * Bem parecida com a anterior, com o diferencial de que essa BL escolhe aleatóriamente
     * somente 20% da quantidade total de pontos. Esse método é mais rápido que o anterior,
     * porém, o aumento do valo da FO é menor.
     */

    for (int k = 0; k < (int)(numeroDePontos * 0.2); k++) {

        hf = clock();

        if((double)(hf - hi)/CLOCKS_PER_SEC < tempo_limite) {

            int i = rand()%numeroDePontos;
            posicaoOriginal = s.posicaoDosPontos[i];

            for (int j = 1; j <= numeroDePosicoesCandidatas; j++) {
                
                hf = clock();

                if((double)(hf - hi)/CLOCKS_PER_SEC < tempo_limite) {

                    novaPosicao = i*numeroDePosicoesCandidatas + j;

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
            }
        }
    }
    calcularFO(s);
    calcularConflitos(s);
}

void heuBLPM3(Solucao &s, clock_t hi) {
    int aux, foOriginal, novaPosicao, posicaoOriginal, melhorFO = s.funcao_objetivo;
    clock_t hf;

    INICIO: ;

    foOriginal = s.funcao_objetivo;
    
    /* 
     * Escolhe aleatóriamente 30% do total de pontos e escolhe uma posição aleatória 
     * para esse ponto. Se melhorar, atualiza o valor da FO e volta ao INICIO. Caso 
     * contrário, escolhe outro ponto e outra posição.
     */

    for (int k = 0; k < (int)(numeroDePontos * 0.3); k++) {  

        hf = clock();

        if((double)(hf - hi)/CLOCKS_PER_SEC < tempo_limite) {
            int i = rand()%numeroDePontos;
            posicaoOriginal = s.posicaoDosPontos[i];
            
            do {
                novaPosicao = i * numeroDePosicoesCandidatas + gerarNumero(1, numeroDePosicoesCandidatas);
            } while(novaPosicao == posicaoOriginal);

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
    }
    calcularFO(s);
    calcularConflitos(s);
}

/* Gera números aleatórios entre um intervalo recebido por parâmetro. */

int gerarNumero(int lim_inf, int lim_sup){
    return(lim_inf + rand() % (lim_sup - lim_inf + 1));
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

    fprintf(f, "Posição de cada ponto\n");
    for(int i = 0; i < numeroDePontos; i++)
        //fprintf(f, "%d\n", s.posicaoDosPontos[i]);
        fprintf(f, "%d\n", (s.posicaoDosPontos[i] % numeroDePosicoesCandidatas) == 0 ? numeroDePosicoesCandidatas : (s.posicaoDosPontos[i] % numeroDePosicoesCandidatas));

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

void testarHeuBLPMs(Solucao &s, int funcao) {

    solucaoInicialGulosa(s);
    calcularConflitos(s);
    int melhorFO = s.funcao_objetivo;

    for(int i = 0; i < 100000; i++) {
        if(funcao == 1)
            heuBLPM1(s, clock());
        else if(funcao == 2)
            heuBLPM2(s, clock());
        else
            heuBLPM3(s, clock());

        if (s.funcao_objetivo > melhorFO) {
            melhorFO = s.funcao_objetivo;
        }
    }

    cout << "Melhor FO: " << melhorFO << endl;
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
    calcularConflitos(s);

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
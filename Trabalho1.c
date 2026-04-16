#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Localização dos elementos na matriz
typedef struct {
    double valor;
    int linha;
    int coluna;
} Elemento;

// Struct com informação da matriz
typedef struct {
    int total_linhas;
    int total_colunas;
    int num_elementos;
    int capacidade;
    Elemento *elementos;
} Matriz;

// Inicializa e aloca a memoria da matriz esparsa
Matriz criar_matriz(int linhas, int colunas, int capacidade_inicial) {
    Matriz m;
    m.total_linhas = linhas;
    m.total_colunas = colunas;
    m.num_elementos = 0;
    m.capacidade = capacidade_inicial;
    
    // Aloca espaço na memoria para armazenar os Elementos iniciais
    m.elementos = (Elemento *)malloc(capacidade_inicial * sizeof(Elemento));
    
    // Retorna o id da matriz criada
    return m;
}

// Retorna o valor de uma posicao (linha, coluna) ou 0.0 caso não exista
double obter_valor(Matriz *m, int linha, int coluna) {
    for (int i = 0; i < m->num_elementos; i++) {
        if (m->elementos[i].linha == linha && m->elementos[i].coluna == coluna) {
            return m->elementos[i].valor;
        }
    }
    return 0.0;
}

// Insere ou atualiza um valor na matriz esparsa
void inserir_valor(Matriz *m, int linha, int coluna, double valor) {
    
    // Validação dos limites da matriz
    if (linha < 0 || linha >= m->total_linhas || coluna < 0 || coluna >= m->total_colunas) {
        printf("Erro: Posicao (%d, %d) invalida.\n", linha, coluna);
        return;
    }

    int eh_zero = (fabs(valor) < 1e-6); // Tratamento para ponto flutuante

    // Busca o elemento para ver se ja existe
    for (int i = 0; i < m->num_elementos; i++) {
        if (m->elementos[i].linha == linha && m->elementos[i].coluna == coluna) {
            if (eh_zero) {
                // Se o novo valor for (proximo de) 0, removemos o elemento
                for (int j = i; j < m->num_elementos - 1; j++) {
                    m->elementos[j] = m->elementos[j + 1];
                }
                m->num_elementos--;
            } else {
                // Apenas atualiza
                m->elementos[i].valor = valor; 
            }
            return;
        }
    }

    // Se o elemento não existe e o valor é 0, não fazemos nada
    if (eh_zero) return;

    // Se array estiver cheio, dobra a capacidade com realloc
    if (m->num_elementos >= m->capacidade) {
        m->capacidade = (m->capacidade == 0) ? 4 : m->capacidade * 2;
        m->elementos = (Elemento *)realloc(m->elementos, m->capacidade * sizeof(Elemento));
    }

    // Insere o novo elemento no final
    m->elementos[m->num_elementos].linha = linha;
    m->elementos[m->num_elementos].coluna = coluna;
    m->elementos[m->num_elementos].valor = valor;
    m->num_elementos++;
}

// Visualiza a matriz de forma formatada (em grade)
void visualizar_matriz(Matriz *m) {
    printf("Matriz %dx%d (Elementos não nulos: %d):\n", m->total_linhas, m->total_colunas, m->num_elementos);
    for (int i = 0; i < m->total_linhas; i++) {
        for (int j = 0; j < m->total_colunas; j++) {
            double v = obter_valor(m, i, j);
            // Evitar impressao de "-0.00" 
            if (fabs(v) < 1e-6) v = 0.0;
            printf("%8.2f ", v);
        }
        printf("\n");
    }
}

// Cria uma copia exata de uma matriz esparsa
Matriz copiar_matriz(Matriz *origem) {
    Matriz m = criar_matriz(origem->total_linhas, origem->total_colunas, origem->capacidade);
    m.num_elementos = origem->num_elementos;
    for (int i = 0; i < origem->num_elementos; i++) {
        m.elementos[i] = origem->elementos[i];
    }
    return m;
}

// Troca duas linhas inteiras de lugar dentro da matriz esparsa
void trocar_linhas(Matriz *m, int r1, int r2) {
    if (r1 == r2) return;
    for (int i = 0; i < m->num_elementos; i++) {
        if (m->elementos[i].linha == r1) {
            m->elementos[i].linha = r2;
        } else if (m->elementos[i].linha == r2) {
            m->elementos[i].linha = r1;
        }
    }
}

// Aplica a Eliminação de Gauss para escalonar a matriz (Forma Escalonada / Triangular Superior)
void escalonar_gauss(Matriz *m) {
    int r = 0; // linha atual do pivô
    
    for (int c = 0; c < m->total_colunas && r < m->total_linhas; c++) {
        // Busca o primeiro pivô não nulo nesta coluna (a partir de r) sem pivoteamento parcial
        int pivo_linha = r;
        if (fabs(obter_valor(m, r, c)) < 1e-6) {
            for (int k = r + 1; k < m->total_linhas; k++) {
                if (fabs(obter_valor(m, k, c)) > 1e-6) {
                    pivo_linha = k;
                    break;
                }
            }
        }

        if (fabs(obter_valor(m, pivo_linha, c)) < 1e-6) {
            // Toda a coluna é zero abaixo de r, vai para a próxima
            continue;
        }

        // Troca apenas se o pivô atual for zero e achamos um não nulo abaixo
        trocar_linhas(m, r, pivo_linha);

        // Zera as posicoes correspondentes nas linhas ABAIXO do pivô (Escalonamento Simples)
        double val_pivo = obter_valor(m, r, c);
        for (int i = r + 1; i < m->total_linhas; i++) {
            double fator = obter_valor(m, i, c) / val_pivo;
            
            if (fabs(fator) > 1e-6) {
                for (int j = c; j < m->total_colunas; j++) {
                    double v_r = obter_valor(m, r, j);
                    double v_i = obter_valor(m, i, j);
                    double novo_valor = v_i - fator * v_r;
                    inserir_valor(m, i, j, novo_valor);
                }
            }
        }
        r++; // Avança para a proxima linha para buscar um novo pivô
    }
}

// Aplica a Eliminação de Gauss-Jordan para escalonar a matriz (RREF - Totalmente Reduzida)
void escalonar_gauss_jordan(Matriz *m) {
    int r = 0; // linha atual do pivô
    
    for (int c = 0; c < m->total_colunas && r < m->total_linhas; c++) {
        // Encontra o pivô nesta coluna (k >= r)
        int pivo_linha = r;
        double max_val = 0.0;
        
        for (int k = r; k < m->total_linhas; k++) {
            double val = fabs(obter_valor(m, k, c));
            if (val > max_val) {
                max_val = val;
                pivo_linha = k;
            }
        }

        if (max_val < 1e-6) continue;

        // Troca linha r com a linha do pivô
        trocar_linhas(m, r, pivo_linha);

        // Divide a linha pivô pelo seu próprio valor para que o pivô vire 1
        double val_pivo = obter_valor(m, r, c);
        for (int j = c; j < m->total_colunas; j++) {
            double v = obter_valor(m, r, j);
            if (fabs(v) > 1e-6) inserir_valor(m, r, j, v / val_pivo);
        }

        // Zera as posicoes correspondentes nas demais linhas (Escalonamento Reduzido)
        for (int i = 0; i < m->total_linhas; i++) {
            if (i == r) continue;
            
            double fator = obter_valor(m, i, c);
            if (fabs(fator) > 1e-6) {
                for (int j = c; j < m->total_colunas; j++) {
                    double v_r = obter_valor(m, r, j);
                    double v_i = obter_valor(m, i, j);
                    double novo_valor = v_i - fator * v_r;
                    inserir_valor(m, i, j, novo_valor);
                }
            }
        }
        r++; 
    }
}

int main () {
    int tamx, tamy;
    int menu;
    int id_matriz;
    
    // Armazenar ate 100 matrizes para permitir os acessos por "numero da matriz"
    Matriz matrizes[100];
    int contagem_matrizes = 0;

    while(1) {
        printf("\n=== Menu ===\n");
        printf(" 1. Inicializar nova Matriz\n");
        printf(" 2. Visualizar Matriz\n");
        printf(" 3. Escalonar Matriz (Gauss - Simples)\n");
        printf(" 4. Escalonar Matriz (Gauss-Jordan - Reduzida)\n");
        printf(" 5. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &menu) != 1) break;

        switch (menu) {
            case 1:
                if (contagem_matrizes >= 100) {
                    printf("Limite de matrizes atingido.\n");
                    break;
                }
                
                printf("Digite o tamanho da matriz (linhas colunas): ");
                scanf("%d %d", &tamx, &tamy);
                
                Matriz mz1 = criar_matriz(tamx, tamy, 4); // Capacidade inicial dinamica pequena
                
                printf("Digite os valores da matriz no formato (linha coluna valor).\n");
                printf("Digite -1 na linha para terminar.\n");
                
                while (1) {
                    int linha, coluna;
                    double valor;
                    
                    printf("> ");
                    scanf("%d", &linha);
                    if (linha == -1) break; // Termina a entrada de dados
                    
                    scanf("%d %lf", &coluna, &valor);
                    inserir_valor(&mz1, linha, coluna, valor);
                }
                
                matrizes[contagem_matrizes] = mz1;
                printf("Matriz armazenada com o ID %d.\n", contagem_matrizes);
                contagem_matrizes++;
                break;

            case 2:
                if (contagem_matrizes == 0) {
                    printf("Nenhuma matriz cadastrada ainda.\n");
                    break;
                }
                printf("Digite o ID da matriz a ser visualizada (0 a %d): ", contagem_matrizes - 1);
                scanf("%d", &id_matriz);
                
                if (id_matriz >= 0 && id_matriz < contagem_matrizes) {
                    visualizar_matriz(&matrizes[id_matriz]);
                } else {
                    printf("ID de matriz invalido.\n");
                }
                break;

            case 3:
            case 4:
                if (contagem_matrizes == 0) {
                    printf("Nenhuma matriz cadastrada ainda.\n");
                    break;
                }
                if (contagem_matrizes >= 100) {
                    printf("Limite de matrizes atingido. Nao eh possivel criar uma nova matriz escalonada.\n");
                    break;
                }
                
                printf("Digite o ID da matriz a ser escalonada (0 a %d): ", contagem_matrizes - 1);
                scanf("%d", &id_matriz);
                
                if (id_matriz >= 0 && id_matriz < contagem_matrizes) {
                    // Cria uma copia para preservar a matriz original intacta
                    matrizes[contagem_matrizes] = copiar_matriz(&matrizes[id_matriz]);
                    
                    if (menu == 3) {
                        escalonar_gauss(&matrizes[contagem_matrizes]);
                        printf("Matriz escalonada (Gauss - Simples) com sucesso.\n");
                    } else {
                        escalonar_gauss_jordan(&matrizes[contagem_matrizes]);
                        printf("Matriz escalonada (Gauss-Jordan - Reduzida) com sucesso.\n");
                    }
                    
                    printf("Nova matriz gerada e armazenada com o ID: %d\n", contagem_matrizes);
                    contagem_matrizes++; // Aumenta o contador para a nova matriz
                } else {
                    printf("ID de matriz invalido.\n");
                }
                break;

            case 5:
                // Libera a memoria antes de sair
                for(int i = 0; i < contagem_matrizes; i++){
                    free(matrizes[i].elementos);
                }
                printf("Encerrando programa...\n");
                return 0;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }
    
    return 0;
}
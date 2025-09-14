#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Definição da struct para representar um item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;
} Item;

// Definição da struct para os nós da lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// Variáveis globais para contadores
long long int comparacoes = 0;
clock_t inicio_tempo, fim_tempo;
double tempo_execucao;

//--- Funções de Utilitário ---//

// Limpa o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Inicia a contagem de comparações
void resetContador() {
    comparacoes = 0;
}

// Inicia a medição de tempo
void iniciarTempo() {
    inicio_tempo = clock();
}

// Para a medição de tempo e exibe o resultado
void pararTempo() {
    fim_tempo = clock();
    tempo_execucao = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
}

//--- NÍVEL 1: SISTEMA BÁSICO (VETOR) ---//

// Insere um item em um vetor
void inserirItemVetor(Item inventario[], int* quantidade) {
    if (*quantidade >= 10) {
        printf("\nInventario cheio! Nao e possivel adicionar mais itens.\n");
        return;
    }
    printf("\n--- CADASTRAR NOVO ITEM ---\n");
    limparBuffer();
    printf("Nome: ");
    fgets(inventario[*quantidade].nome, sizeof(inventario[*quantidade].nome), stdin);
    inventario[*quantidade].nome[strcspn(inventario[*quantidade].nome, "\n")] = 0;
    printf("Tipo: ");
    fgets(inventario[*quantidade].tipo, sizeof(inventario[*quantidade].tipo), stdin);
    inventario[*quantidade].tipo[strcspn(inventario[*quantidade].tipo, "\n")] = 0;
    printf("Quantidade: ");
    scanf("%d", &inventario[*quantidade].quantidade);
    *quantidade += 1;
    printf("\nItem adicionado com sucesso!\n");
}

// Remove um item de um vetor pelo nome
void removerItemVetor(Item inventario[], int* quantidade, char nome[]) {
    int i, j;
    bool encontrado = false;
    for (i = 0; i < *quantidade; i++) {
        if (strcasecmp(inventario[i].nome, nome) == 0) {
            encontrado = true;
            for (j = i; j < *quantidade - 1; j++) {
                inventario[j] = inventario[j+1];
            }
            *quantidade -= 1;
            break;
        }
    }
    if (encontrado) {
        printf("\nItem '%s' removido com sucesso.\n", nome);
    } else {
        printf("\nItem '%s' nao encontrado.\n", nome);
    }
}

// Lista todos os itens de um vetor
void listarItensVetor(Item inventario[], int quantidade) {
    if (quantidade == 0) {
        printf("\nO inventario esta vazio.\n");
        return;
    }
    printf("\n--- ITENS NO INVENTARIO (%d de 10) ---\n", quantidade);
    for (int i = 0; i < quantidade; i++) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }
    printf("---------------------------------------\n");
}

// Busca sequencial em um vetor
void buscarSequencialVetor(Item inventario[], int quantidade, char nome[]) {
    resetContador();
    int i;
    for (i = 0; i < quantidade; i++) {
        comparacoes++;
        if (strcasecmp(inventario[i].nome, nome) == 0) {
            printf("\nItem '%s' encontrado apos %lld comparacoes.\n", nome, comparacoes);
            printf("Dados: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
            return;
        }
    }
    printf("\nItem '%s' nao encontrado apos %lld comparacoes.\n", nome, comparacoes);
}

//--- NÍVEL 2: COMPARAÇÃO DE ESTRUTURAS ---//

// Insere um item em uma lista encadeada
void inserirItemLista(No** inicio, Item novoItem) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }
    novoNo->dados = novoItem;
    novoNo->proximo = *inicio;
    *inicio = novoNo;
    printf("\nItem '%s' adicionado a lista com sucesso!\n", novoItem.nome);
}

// Remove um item de uma lista encadeada
void removerItemLista(No** inicio, char nome[]) {
    if (*inicio == NULL) {
        printf("\nLista vazia! Nao ha itens para remover.\n");
        return;
    }
    No* temp = *inicio;
    No* anterior = NULL;
    while (temp != NULL && strcasecmp(temp->dados.nome, nome) != 0) {
        anterior = temp;
        temp = temp->proximo;
    }
    if (temp == NULL) {
        printf("\nItem '%s' nao encontrado na lista.\n", nome);
        return;
    }
    if (anterior == NULL) {
        *inicio = temp->proximo;
    } else {
        anterior->proximo = temp->proximo;
    }
    free(temp);
    printf("\nItem '%s' removido da lista com sucesso.\n", nome);
}

// Lista todos os itens de uma lista encadeada
void listarItensLista(No* inicio) {
    if (inicio == NULL) {
        printf("\nLista de itens esta vazia.\n");
        return;
    }
    printf("\n--- ITENS NA LISTA ENCADAEADA ---\n");
    No* temp = inicio;
    while (temp != NULL) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               temp->dados.nome, temp->dados.tipo, temp->dados.quantidade);
        temp = temp->proximo;
    }
    printf("-------------------------------------\n");
}

// Busca sequencial em uma lista encadeada
void buscarSequencialLista(No* inicio, char nome[]) {
    resetContador();
    No* temp = inicio;
    while (temp != NULL) {
        comparacoes++;
        if (strcasecmp(temp->dados.nome, nome) == 0) {
            printf("\nItem '%s' encontrado apos %lld comparacoes.\n", nome, comparacoes);
            printf("Dados: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   temp->dados.nome, temp->dados.tipo, temp->dados.quantidade);
            return;
        }
        temp = temp->proximo;
    }
    printf("\nItem '%s' nao encontrado apos %lld comparacoes.\n", nome, comparacoes);
}

// Ordena um vetor pelo nome usando Bubble Sort
void ordenarVetor(Item inventario[], int quantidade) {
    resetContador();
    iniciarTempo();
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            comparacoes++;
            if (strcasecmp(inventario[j].nome, inventario[j+1].nome) > 0) {
                Item temp = inventario[j];
                inventario[j] = inventario[j+1];
                inventario[j+1] = temp;
            }
        }
    }
    pararTempo();
    printf("\nVetor ordenado com sucesso em %.6f segundos com %lld comparacoes.\n", tempo_execucao, comparacoes);
}

// Busca binária em um vetor ordenado
void buscarBinariaVetor(Item inventario[], int quantidade, char nome[]) {
    resetContador();
    int baixo = 0;
    int alto = quantidade - 1;
    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        comparacoes++;
        int resultado = strcasecmp(inventario[meio].nome, nome);
        if (resultado == 0) {
            printf("\nItem '%s' encontrado apos %lld comparacoes (busca binaria).\n", nome, comparacoes);
            printf("Dados: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   inventario[meio].nome, inventario[meio].tipo, inventario[meio].quantidade);
            return;
        }
        if (resultado < 0) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }
    printf("\nItem '%s' nao encontrado apos %lld comparacoes (busca binaria).\n", nome, comparacoes);
}


//--- NÍVEL 3: SISTEMA AVANÇADO ---//

// Bubble Sort por nome
void bubbleSortNome(Item componentes[], int n) {
    resetContador();
    iniciarTempo();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcasecmp(componentes[j].nome, componentes[j+1].nome) > 0) {
                Item temp = componentes[j];
                componentes[j] = componentes[j+1];
                componentes[j+1] = temp;
            }
        }
    }
    pararTempo();
}

// Insertion Sort por tipo
void insertionSortTipo(Item componentes[], int n) {
    resetContador();
    iniciarTempo();
    for (int i = 1; i < n; i++) {
        Item chave = componentes[i];
        int j = i - 1;
        while (j >= 0 && strcasecmp(componentes[j].tipo, chave.tipo) > 0) {
            comparacoes++;
            componentes[j+1] = componentes[j];
            j = j - 1;
        }
        componentes[j+1] = chave;
    }
    pararTempo();
}

// Selection Sort por prioridade
void selectionSortPrioridade(Item componentes[], int n) {
    resetContador();
    iniciarTempo();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (componentes[j].prioridade > componentes[min_idx].prioridade) {
                min_idx = j;
            }
        }
        Item temp = componentes[min_idx];
        componentes[min_idx] = componentes[i];
        componentes[i] = temp;
    }
    pararTempo();
}

// Busca binária por nome para o Nível Mestre
int buscaBinariaPorNome(Item componentes[], int n, char nome[]) {
    resetContador();
    int baixo = 0;
    int alto = n - 1;
    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        comparacoes++;
        int resultado = strcasecmp(componentes[meio].nome, nome);
        if (resultado == 0) {
            return meio;
        }
        if (resultado < 0) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }
    return -1;
}

// Exibe a tabela de componentes
void exibirComponentes(Item componentes[], int n) {
    if (n == 0) {
        printf("\nNenhum componente registrado.\n");
        return;
    }
    printf("\n--- COMPONENTES DA TORRE ---\n");
    for (int i = 0; i < n; i++) {
        printf("Nome: %-20s | Tipo: %-15s | Prioridade: %d\n",
               componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("---------------------------\n");
}


//--- Função principal para o menu dos níveis ---//
int main() {
    int opcaoNivel;

    // Definição das estruturas para os níveis
    Item inventarioVetor[10];
    int quantidadeVetor = 0;
    No* inventarioLista = NULL;
    
    // Nível 3: Sistema Avancado
    Item componentes[20];
    int numComponentes = 0;

    printf("=== SISTEMA DE INVENTARIO ===\n");
    printf("1. Modo Basico (Vetor)\n");
    printf("2. Modo Comparativo (Vetor vs Lista)\n");
    printf("3. Modo Avancado (Ordenacao)\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcaoNivel);

    switch (opcaoNivel) {
        case 1: { // Modo Básico (Vetor)
            int opcao;
            do {
                printf("\n--- MODO BASICO (VETOR) ---\n");
                printf("1. Adicionar item\n");
                printf("2. Remover item\n");
                printf("3. Listar itens\n");
                printf("4. Buscar item\n");
                printf("0. Voltar ao menu principal\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1:
                        inserirItemVetor(inventarioVetor, &quantidadeVetor);
                        break;
                    case 2:
                        {
                            char nome[30];
                            limparBuffer();
                            printf("Nome do item a remover: ");
                            fgets(nome, sizeof(nome), stdin);
                            nome[strcspn(nome, "\n")] = 0;
                            removerItemVetor(inventarioVetor, &quantidadeVetor, nome);
                        }
                        break;
                    case 3:
                        listarItensVetor(inventarioVetor, quantidadeVetor);
                        break;
                    case 4:
                        {
                            char nome[30];
                            limparBuffer();
                            printf("Nome do item a buscar: ");
                            fgets(nome, sizeof(nome), stdin);
                            nome[strcspn(nome, "\n")] = 0;
                            buscarSequencialVetor(inventarioVetor, quantidadeVetor, nome);
                        }
                        break;
                }
            } while (opcao != 0);
            break;
        }
        case 2: { // Modo Comparativo (Vetor vs Lista)
            int opcao;
            do {
                printf("\n--- MODO COMPARATIVO ---\n");
                printf("1. Gerenciar Vetor\n");
                printf("2. Gerenciar Lista Encadeada\n");
                printf("0. Voltar ao menu principal\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);

                if (opcao == 1) { // Gerenciar Vetor
                    int opVetor;
                    do {
                        printf("\n--- VETOR ---\n");
                        printf("1. Adicionar item\n");
                        printf("2. Remover item\n");
                        printf("3. Listar itens\n");
                        printf("4. Buscar sequencial\n");
                        printf("5. Ordenar por nome\n");
                        printf("6. Buscar binaria por nome\n");
                        printf("0. Voltar\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &opVetor);
                        switch (opVetor) {
                            case 1:
                                inserirItemVetor(inventarioVetor, &quantidadeVetor);
                                break;
                            case 2: {
                                char nome[30];
                                limparBuffer();
                                printf("Nome do item a remover: ");
                                fgets(nome, sizeof(nome), stdin);
                                nome[strcspn(nome, "\n")] = 0;
                                removerItemVetor(inventarioVetor, &quantidadeVetor, nome);
                                break;
                            }
                            case 3:
                                listarItensVetor(inventarioVetor, quantidadeVetor);
                                break;
                            case 4: {
                                char nome[30];
                                limparBuffer();
                                printf("Nome do item a buscar: ");
                                fgets(nome, sizeof(nome), stdin);
                                nome[strcspn(nome, "\n")] = 0;
                                buscarSequencialVetor(inventarioVetor, quantidadeVetor, nome);
                                break;
                            }
                            case 5:
                                ordenarVetor(inventarioVetor, quantidadeVetor);
                                break;
                            case 6: {
                                char nome[30];
                                limparBuffer();
                                printf("Nome do item a buscar: ");
                                fgets(nome, sizeof(nome), stdin);
                                nome[strcspn(nome, "\n")] = 0;
                                buscarBinariaVetor(inventarioVetor, quantidadeVetor, nome);
                                break;
                            }
                        }
                    } while (opVetor != 0);
                } else if (opcao == 2) { // Gerenciar Lista Encadeada
                    int opLista;
                    do {
                        printf("\n--- LISTA ENCADAEADA ---\n");
                        printf("1. Adicionar item\n");
                        printf("2. Remover item\n");
                        printf("3. Listar itens\n");
                        printf("4. Buscar sequencial\n");
                        printf("0. Voltar\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &opLista);
                        switch (opLista) {
                            case 1: {
                                Item novoItem;
                                limparBuffer();
                                printf("Nome: ");
                                fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
                                novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;
                                printf("Tipo: ");
                                fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
                                novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;
                                printf("Quantidade: ");
                                scanf("%d", &novoItem.quantidade);
                                inserirItemLista(&inventarioLista, novoItem);
                                break;
                            }
                            case 2: {
                                char nome[30];
                                limparBuffer();
                                printf("Nome do item a remover: ");
                                fgets(nome, sizeof(nome), stdin);
                                nome[strcspn(nome, "\n")] = 0;
                                removerItemLista(&inventarioLista, nome);
                                break;
                            }
                            case 3:
                                listarItensLista(inventarioLista);
                                break;
                            case 4: {
                                char nome[30];
                                limparBuffer();
                                printf("Nome do item a buscar: ");
                                fgets(nome, sizeof(nome), stdin);
                                nome[strcspn(nome, "\n")] = 0;
                                buscarSequencialLista(inventarioLista, nome);
                                break;
                            }
                        }
                    } while (opLista != 0);
                }
            } while (opcao != 0);
            break;
        }
        case 3: { // Modo Avançado (Ordenação)
            int opcao;
            do {
                printf("\n--- MODO AVANCADO (ORDENACAO) ---\n");
                printf("Numero de componentes: %d\n", numComponentes);
                printf("1. Adicionar componente\n");
                printf("2. Listar componentes\n");
                printf("3. Ordenar por Nome (Bubble Sort)\n");
                printf("4. Ordenar por Tipo (Insertion Sort)\n");
                printf("5. Ordenar por Prioridade (Selection Sort)\n");
                printf("6. Buscar Binaria por Nome\n");
                printf("0. Sair\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1: {
                        if (numComponentes >= 20) {
                            printf("\nMaximo de 20 componentes atingido.\n");
                            break;
                        }
                        limparBuffer();
                        printf("Nome: ");
                        fgets(componentes[numComponentes].nome, sizeof(componentes[numComponentes].nome), stdin);
                        componentes[numComponentes].nome[strcspn(componentes[numComponentes].nome, "\n")] = 0;
                        printf("Tipo: ");
                        fgets(componentes[numComponentes].tipo, sizeof(componentes[numComponentes].tipo), stdin);
                        componentes[numComponentes].tipo[strcspn(componentes[numComponentes].tipo, "\n")] = 0;
                        printf("Prioridade (1-10): ");
                        scanf("%d", &componentes[numComponentes].prioridade);
                        numComponentes++;
                        break;
                    }
                    case 2:
                        exibirComponentes(componentes, numComponentes);
                        break;
                    case 3:
                        bubbleSortNome(componentes, numComponentes);
                        printf("Ordenacao por nome concluida em %.6f segundos com %lld comparacoes.\n", tempo_execucao, comparacoes);
                        exibirComponentes(componentes, numComponentes);
                        break;
                    case 4:
                        insertionSortTipo(componentes, numComponentes);
                        printf("Ordenacao por tipo concluida em %.6f segundos com %lld comparacoes.\n", tempo_execucao, comparacoes);
                        exibirComponentes(componentes, numComponentes);
                        break;
                    case 5:
                        selectionSortPrioridade(componentes, numComponentes);
                        printf("Ordenacao por prioridade concluida em %.6f segundos com %lld comparacoes.\n", tempo_execucao, comparacoes);
                        exibirComponentes(componentes, numComponentes);
                        break;
                    case 6: {
                        char nome[30];
                        limparBuffer();
                        printf("Nome do componente a buscar: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = 0;
                        int indice = buscaBinariaPorNome(componentes, numComponentes, nome);
                        if (indice != -1) {
                             printf("Componente '%s' encontrado apos %lld comparacoes.\n", nome, comparacoes);
                        } else {
                            printf("Componente '%s' nao encontrado apos %lld comparacoes.\n", nome, comparacoes);
                        }
                        break;
                    }
                }
            } while (opcao != 0);
            break;
        }
    }

    // Libera a memória da lista encadeada, se houver
    No* temp;
    while(inventarioLista != NULL) {
        temp = inventarioLista;
        inventarioLista = inventarioLista->proximo;
        free(temp);
    }
    
    printf("\nSistema finalizado. Ate mais!\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Voo{
    char cia[20];
    int numero;
    char horarioPrevisto[6];
    char destino[100];
    char portao[5];
    char status[20];
    struct Voo* prox;

} Voo;

Voo* lista = NULL;

void inserirVoo() {
    Voo* novo = (Voo*)malloc(sizeof(Voo));
    if (!novo) {
        printf("Não foi possível inserir o voo.\n");
        return;
    }

    while (getchar() != '\n' && getchar() != EOF);  // Limpa qualquer sujeira anterior e ajuda na quebra de linha


    printf("Cia: ");
    fgets(novo->cia, sizeof(novo->cia), stdin);
    novo->cia[strcspn(novo->cia, "\n")] = 0;

    printf("Número do Voo: ");
    scanf("%d", &novo->numero);
    while (getchar() != '\n');

    printf("Horário Previsto (HH:MM): ");
    scanf("%s", novo->horarioPrevisto);
    while (getchar() != '\n');

    printf("Destino: ");
    fgets(novo->destino, sizeof(novo->destino), stdin);
    novo->destino[strcspn(novo->destino, "\n")] = 0;

    printf("Portão: ");
    fgets(novo->portao, sizeof(novo->portao), stdin);
    novo->portao[strcspn(novo->portao, "\n")] = 0;

    strcpy(novo->status, "Previsto");

    novo->prox = NULL;

    // Converter horário do novo voo para minutos
    int horaNovo, minutoNovo;
    sscanf(novo->horarioPrevisto, "%d:%d", &horaNovo, &minutoNovo);
    int minutosNovo = horaNovo * 60 + minutoNovo;

if (lista == NULL) {
    // Inserir no início
    novo->prox = lista;
    lista = novo;
} else {
    int horaPrimeiro, minutoPrimeiro;
    sscanf(lista->horarioPrevisto, "%d:%d", &horaPrimeiro, &minutoPrimeiro);
    int minutosPrimeiro = horaPrimeiro * 60 + minutoPrimeiro;

    if (minutosPrimeiro > minutosNovo) {
        // Inserir no início
        novo->prox = lista;
        lista = novo;
    } else {
        // Inserir no meio ou final
        Voo* atual = lista;
        while (atual->prox != NULL) {
            int horaAtual, minutoAtual;
            sscanf(atual->prox->horarioPrevisto, "%d:%d", &horaAtual, &minutoAtual);
            int minutosAtual = horaAtual * 60 + minutoAtual;

            if (minutosAtual > minutosNovo) {
                break;
            }

            atual = atual->prox;
        }

        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

    printf("Voo inserido com sucesso!\n");
}


//Função exibir os Vôo
void mostrarVoos(){
    if(lista == NULL){
        printf("Nenhum voo cadastrado.\n");
        return;
    }
    printf("\nPAINEL DE VOOS\n");
    printf("Cia       | Vôo   | Prev  | Destino/Escalas         | Portão | Status\n");
    printf("--------------------------------------------------------------------------\n");

    Voo* atual = lista;
    while(atual != NULL){
        printf("%-10s| %-6d| %-6s| %-23s| %-7s| %-15s\n", 
               atual->cia, atual->numero, atual->horarioPrevisto, atual->destino, atual->portao, atual->status);
        atual = atual->prox;
    }
    printf("\n");
}

// Função para encontrar voo pelo número
Voo* buscarVoo(int numero) {
    Voo* temp = lista;
    while (temp != NULL) {
        if (temp->numero == numero)
            return temp;
        temp = temp->prox;
    }
    return NULL;
}

// Função para excluir voo pelo número
void excluirVoo() {
    int numero;
    printf("Número do Vôo para excluir: ");
    scanf("%d", &numero);

    Voo* atual = lista;
    Voo* anterior = NULL;

    while (atual != NULL && atual->numero != numero) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        // Excluir o primeiro da lista
        lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    printf("Voo excluído com sucesso.\n");
}

// Função para cancelar voo (alterar status)
void cancelarVoo() {
    int numero;
    printf("Número do Vôo para cancelar: ");
    scanf("%d", &numero);

    Voo* voo = buscarVoo(numero);
    if (voo == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    strcpy(voo->status, "Cancelado");
    printf("Voo cancelado com sucesso.\n");
}

// Função para atualizar voo (status: Cancelado ou Atrasado)
void atualizarVoo() {
    int numero;
    char novoStatus[20];
    printf("Número do Vôo para atualizar: ");
    scanf("%d", &numero);

    Voo* voo = buscarVoo(numero);
    if (voo == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    int opcao;
    do {
        printf("\nQual campo você deseja atualizar?\n");
        printf("1 - Cia\n");
        printf("2 - Horário\n");
        printf("3 - Destino\n");
        printf("4 - Portão\n");
        printf("5 - Status\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');  // Limpa o buffer

        switch (opcao) {
            case 1:
                printf("Nova Cia: ");
                fgets(voo->cia, sizeof(voo->cia), stdin);
                voo->cia[strcspn(voo->cia, "\n")] = 0;
                printf("Cia atualizada com sucesso.\n");
                break;

            case 2:
                printf("Novo horário (HH:MM): ");
                scanf("%s", voo->horarioPrevisto);
                while (getchar() != '\n');

                // Remover o voo da lista (sem dar free)
                if (lista == voo) {
                    lista = voo->prox;
                } else {
                    Voo* atual = lista;
                    while (atual->prox != NULL && atual->prox != voo) {
                        atual = atual->prox;
                    }
                    if (atual->prox == voo) {
                        atual->prox = voo->prox;
                    }
                }

                // Reinserir o voo na posição correta (igual inserirVoo)
                voo->prox = NULL;

                // Converter horário do voo para minutos
                int horaNovo, minutoNovo;
                sscanf(voo->horarioPrevisto, "%d:%d", &horaNovo, &minutoNovo);
                int minutosNovo = horaNovo * 60 + minutoNovo;

                if (lista == NULL) {
                    // Inserir no início
                    voo->prox = lista;
                    lista = voo;
                } else {
                    int horaPrimeiro, minutoPrimeiro;
                    sscanf(lista->horarioPrevisto, "%d:%d", &horaPrimeiro, &minutoPrimeiro);
                    int minutosPrimeiro = horaPrimeiro * 60 + minutoPrimeiro;

                    if (minutosPrimeiro > minutosNovo) {
                        // Inserir no início
                        voo->prox = lista;
                        lista = voo;
                    } else {
                        Voo* atual = lista;
                        while (atual->prox != NULL) {
                            int horaAtual, minutoAtual;
                            sscanf(atual->prox->horarioPrevisto, "%d:%d", &horaAtual, &minutoAtual);
                            int minutosAtual = horaAtual * 60 + minutoAtual;

                            if (minutosAtual > minutosNovo) {
                                break;
                            }

                            atual = atual->prox;
                        }

                        voo->prox = atual->prox;
                        atual->prox = voo;
                    }
                }
            printf("Horário atualizado com sucesso.\n");
            break;

            case 3:
                printf("Novo destino: ");
                fgets(voo->destino, sizeof(voo->destino), stdin);
                voo->destino[strcspn(voo->destino, "\n")] = 0;
                printf("Destino atualizado com sucesso.\n");
                break;

            case 4:
                printf("Novo portão: ");
                fgets(voo->portao, sizeof(voo->portao), stdin);
                voo->portao[strcspn(voo->portao, "\n")] = 0;
                printf("Portão atualizado com sucesso.\n");
                break;

            case 5:
                printf("Novo status: ");
                fgets(voo->status, sizeof(voo->status), stdin);
                voo->status[strcspn(voo->status, "\n")] = 0;
                printf("Status atualizado com sucesso.\n");
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);
}
// Função para embarcar voo (muda status para "Embarq Imediato")
void embarcarVoo() {
    int numero;
    printf("Número do Vôo para embarcar: ");
    scanf("%d", &numero);

    Voo* voo = buscarVoo(numero);
    if (voo == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    strcpy(voo->status, "Embarq Imediato");
    printf("Voo atualizado para Embarq Imediato.\n");
}

// Função do menu principal
void menu() {
    int opcao;

    do {
        mostrarVoos();
        printf("Opções:\n");
        printf("1 - Inserir Vôo\n");
        printf("2 - Excluir Vôo\n");
        printf("3 - Cancelar Vôo\n");
        printf("4 - Atualizar Vôo\n");
        printf("5 - Embarcar Vôo\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                inserirVoo();
                break;
            case 2:
                excluirVoo();
                break;
            case 3:
                cancelarVoo();
                break;
            case 4:
                atualizarVoo();
                break;
            case 5:
                embarcarVoo();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        printf("\n");
    } while(opcao != 0);
}

int main() {
    menu();
    return 0;
}

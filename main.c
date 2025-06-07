#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void inserirVoo() {
    Voo* novo = (Voo*)malloc(sizeof(Voo));
    if (!novo) {
        printf("Não foi possível inserir o voo.\n");
        return;
    }

    while (getchar() != '\n' && getchar() != EOF);  // Limpa qualquer sujeira anterior e ajuda na quebra de linha

    int valido;
    do {
            printf("\n--- Cias Aéreas disponíveis ---\n");
            printf("GOL\n");
            printf("LATAM\n");
            printf("AZUL\n");
            printf("AVIANCA\n");
            printf("COPA AIRLINES\n");
            printf("AIR FRANCE\n");
            printf("TAP PORTUGAL\n");
            printf("-------------------------------\n");

        printf("Cia: ");
        fgets(novo->cia, sizeof(novo->cia), stdin);
        novo->cia[strcspn(novo->cia, "\n")] = 0;

        valido = 1;
        if(strlen(novo->cia)==0){
            valido = 0;
            printf("Erro: Cia não pode ser vazia. Digite novamente.\n");
        }else{
            for(int i=0; novo->cia[i] !='\0'; i++){
                if(!isalpha(novo->cia[i]) && novo->cia[i] != ' '){
                    valido=0;
                    printf("Erro: Cia só pode conter letras e espaços. Digite novamente.\n");
                    break;
                }
            }
        }
        if(valido){
             // Converter para CAIXA ALTA:
            for (int i = 0; novo->cia[i] != '\0'; i++) {
                novo->cia[i] = toupper(novo->cia[i]);
            }
        }
       
    }while (!valido);
    
    //Número do Voo: 
    do {
        printf("Número do Voo: ");
        if (scanf("%d", &novo->numero) != 1 || novo->numero < 0) {
            printf("Erro: Número inválido. Insira um número positivo.\n");
            while (getchar() != '\n');
        } else if (buscarVoo(novo->numero)) {
            printf("Erro: Já existe um voo com esse número. Insira outro número.\n");
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            break;
        }
    } while (1);

    //Horário Previsto (HH:MM):
    int hora,minuto;
    char temp[10];
    do{
        printf("Horário Previsto (HH:MM): ");
        fgets(temp, sizeof(temp), stdin);

    if (sscanf(temp, "%2d:%2d", &hora, &minuto) != 2 ||
        hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        printf("Erro: Formato inválido. Use HH:MM (ex: 08:30).\n");
    } else {
        sprintf(novo->horarioPrevisto, "%02d:%02d", hora, minuto);
        break;
    }
    }while(1);

    do {
        printf("Destino: ");
        fgets(novo->destino, sizeof(novo->destino), stdin);
        novo->destino[strcspn(novo->destino, "\n")] = 0;

        valido = 1;
        if (strlen(novo->destino) == 0) {
            valido = 0;
            printf("Erro: Destino não pode ser vazio. Digite novamente.\n");
        } else {
            for (int i = 0; novo->destino[i] != '\0'; i++) {
                if (!isalpha(novo->destino[i]) && novo->destino[i] != ' ') {
                    valido = 0;
                    printf("Erro: Destino só pode conter letras e espaços. Digite novamente.\n");
                    break;
                }
            }
        }

        if (valido) {
            // Converter para CAIXA ALTA:
            for (int i = 0; novo->destino[i] != '\0'; i++) {
                novo->destino[i] = toupper(novo->destino[i]);
            }
        }

    } while (!valido);

    printf("Portão: ");
    fgets(novo->portao, sizeof(novo->portao), stdin);
    novo->portao[strcspn(novo->portao, "\n")] = 0;
    // Converter para CAIXA ALTA:
    for (int i = 0; novo->portao[i] != '\0'; i++) {
        novo->portao[i] = toupper(novo->portao[i]);
    }

    strcpy(novo->status, "PREVISTO");

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
        printf("%-10s| %-6d| %-6s| %-24s| %-7s| %-15s\n", 
               atual->cia, atual->numero, atual->horarioPrevisto, atual->destino, atual->portao, atual->status);
        atual = atual->prox;
    }
    printf("\n");
}



// Função para excluir voo pelo número
void excluirVoo() {
    int numero;
    printf("Número do Vôo para excluir: ");
    scanf("%d", &numero);
    while (getchar() != '\n');  // Limpa

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
    // Limpa o buffer
    while (getchar() != '\n');

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
    // Limpa o buffer
    while (getchar() != '\n');

    Voo* voo = buscarVoo(numero);
    if (voo == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    int opcao;
    int hora, minuto;
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
            {
                int valido;
                do {
                    printf("Nova Cia: ");
                    fgets(voo->cia, sizeof(voo->cia), stdin);
                    voo->cia[strcspn(voo->cia, "\n")] = 0;

                    valido = 1;
                    if (strlen(voo->cia) == 0) {
                        valido = 0;
                        printf("Erro: Cia não pode ser vazia. Digite novamente.\n");
                    } else {
                        for (int i = 0; voo->cia[i] != '\0'; i++) {
                            if (!isalpha(voo->cia[i]) && voo->cia[i] != ' ') {
                                valido = 0;
                                printf("Erro: Cia só pode conter letras e espaços. Digite novamente.\n");
                                break;
                            }
                        }
                    }

                    if (valido) {
                        for (int i = 0; voo->cia[i] != '\0'; i++) {
                            voo->cia[i] = toupper(voo->cia[i]);
                        }
                        printf("Cia atualizada com sucesso.\n");
                    }

                } while (!valido);
            }


            case 2:
            
            do{
                printf("Novo horário (HH:MM): ");
                    if (scanf("%2d:%2d", &hora, &minuto) != 2 ||
                        hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
                        printf("Erro: Formato inválido. Use HH:MM (ex: 08:30).\n");
                        while (getchar() != '\n');
                    } else {
                        sprintf(voo->horarioPrevisto, "%02d:%02d", hora, minuto);
                        while (getchar() != '\n');
                        break;
                    }
            }while(1);

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
            {
                int valido;
                    do {
                        printf("Novo destino: ");
                        fgets(voo->destino, sizeof(voo->destino), stdin);
                        voo->destino[strcspn(voo->destino, "\n")] = 0;

                        valido = 1;
                        if (strlen(voo->destino) == 0) {
                            valido = 0;
                            printf("Erro: Destino não pode ser vazio. Digite novamente.\n");
                        } else {
                            for (int i = 0; voo->destino[i] != '\0'; i++) {
                                if (!isalpha(voo->destino[i]) && voo->destino[i] != ' ') {
                                    valido = 0;
                                    printf("Erro: Destino só pode conter letras e espaços. Digite novamente.\n");
                                    break;
                                }
                            }
                        }

                        if (valido) {
                            for (int i = 0; voo->destino[i] != '\0'; i++) {
                                voo->destino[i] = toupper(voo->destino[i]);
                            }
                            printf("Destino atualizado com sucesso.\n");
                        }

                    } while (!valido);
            }
                    break;

            case 4:
                printf("Novo portão: ");
                fgets(voo->portao, sizeof(voo->portao), stdin);
                voo->portao[strcspn(voo->portao, "\n")] = 0;
                for (int i = 0; voo->portao[i] != '\0'; i++) {
                    voo->portao[i] = toupper(voo->portao[i]);
                }
                printf("Portão atualizado com sucesso.\n");
                break;

            case 5:

            do {
                printf("Status disponíveis:\n");
                printf(" - PREVISTO\n");
                printf(" - CANCELADO\n");
                printf(" - EMBARQUE IMEDIATO\n");
                printf(" - EXCLUIDO\n");
                printf(" - ATRASADO\n");
                printf("-----------------------------\n");

                printf("Novo status: ");
                fgets(voo->status, sizeof(voo->status), stdin);
                voo->status[strcspn(voo->status, "\n")] = 0;
                
                // Converter para CAIXA ALTA:
                for (int i = 0; voo->status[i] != '\0'; i++) {
                    voo->status[i] = toupper(voo->status[i]);
                }

                // Verifica se é um status válido
                if (strcmp(voo->status, "PREVISTO") == 0 ||
                    strcmp(voo->status, "CANCELADO") == 0 ||
                    strcmp(voo->status, "EMBARQUE IMEDIATO") == 0 ||
                    strcmp(voo->status, "EXCLUIDO") == 0 ||
                    strcmp(voo->status, "ATRASADO") == 0) 
                {

                printf("Status atualizado com sucesso.\n");
                break;
                } else {
                    printf("Erro: Status inválido. Digite novamente.\n\n");
                } 

            } while (1);
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
    // Limpa o buffer
    while (getchar() != '\n');

    Voo* voo = buscarVoo(numero);
    if (voo == NULL) {
        printf("Voo não encontrado.\n");
        return;
    }

    strcpy(voo->status, "EMBARQ IMEDIATO");
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
        // Limpa o buffer antes de ler a opção do menu
        while (getchar() != '\n' && getchar() != EOF);

        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida. Digite um número de 0 a 5.\n");
            opcao = -1; // Força a repetir o menu
        }

        // Limpa o buffer
        while (getchar() != '\n');

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

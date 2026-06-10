#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define ARQUIVO_DADOS "contas.dat"

typedef struct {
    int numeroConta;
    char nome[MAX_NOME];
    float saldo;
    int ativo;
} Cliente;

void cadastrarCliente(FILE *txt) {
    int posicao;
    Cliente c;

    printf("Digite a posicao onde deseja cadastrar (0, 1, 2...): ");
    scanf("%d", &posicao);

    printf("Numero da Conta: ");
    scanf("%d", &c.numeroConta);
    printf("Nome do Cliente: ");
    scanf(" %[^\n]", c.nome);
    printf("Saldo Inicial: ");
    scanf("%f", &c.saldo);
    c.ativo = 1;

    fseek(txt, posicao * sizeof(Cliente), SEEK_SET);
    fwrite(&c, sizeof(Cliente), 1, txt);
    printf("Cliente cadastrado com sucesso na posicao %d!\n", posicao);
}

void consultarCliente(FILE *txt) {
    int numeroprocurado;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta que busca: ");
    scanf("%d", &numeroprocurado);

    rewind(txt);

    while (fread(&c, sizeof(Cliente), 1, txt) == 1) {
        if (c.ativo == 1 && c.numeroConta == numeroprocurado) {
            printf("\n--- Cliente Encontrado ---\n");
            printf("Conta: %d\nNome: %s\nSaldo: R$ %.2f\n", c.numeroConta, c.nome, c.saldo);
            encontrado = 1;
            break; 
        }
    }

    if (!encontrado) {
        printf("Conta numero %d nao encontrada ou inativa.\n", numeroprocurado);
    }
}

void atualizarSaldo(FILE *txt) {
    int numeroprocurado;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta para atualizar o saldo: ");
    scanf("%d", &numeroprocurado);

    rewind(txt);
    while (fread(&c, sizeof(Cliente), 1, txt) == 1) {
        if (c.ativo == 1 && c.numeroConta == numeroprocurado) {
            printf("Saldo atual: R$ %.2f\nNovo Saldo: ", c.saldo);
            scanf("%f", &c.saldo);

            fseek(txt, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, txt);
            printf("Saldo atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("Conta nao encontrada.\n");
}

void encerrarConta(FILE *txt) {
    int numeroprocurado;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta que deseja encerrar: ");
    scanf("%d", &numeroprocurado);

    rewind(txt);
    while (fread(&c, sizeof(Cliente), 1, txt) == 1) {
        if (c.ativo == 1 && c.numeroConta == numeroprocurado) {
            c.ativo = 0;

            fseek(txt, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, txt);
            printf("Conta encerrada com sucesso (Removida)!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("Conta nao encontrada.\n");
}

void listarClientes(FILE *txt) {
    Cliente c;
    int posicao = 0;

    rewind(txt);

    printf("\n--- LISTA DE CLIENTES ATIVOS ---\n");
    printf("%-10s %-10s %-20s %-10s\n", "Posicao", "Conta", "Nome", "Saldo");
    
    while (fread(&c, sizeof(Cliente), 1, txt) == 1) {
        if (c.ativo == 1) {
            printf("%-10d %-10d %-20s R$ %-10.2f\n", posicao, c.numeroConta, c.nome, c.saldo);
        }
        posicao++;
    }
}

int main() {
    FILE *txt = fopen(ARQUIVO_DADOS, "rb+");
    
    if (txt == NULL) {
        txt = fopen(ARQUIVO_DADOS, "wb+");
        if (txt == NULL) {
            printf("Erro ao criar o arquivo de dados.\n");
            return 1;
        }
    }

    int opcao;
    do {
        printf("\n======= SISTEMA BANCARIO =======");
        printf("\n1. Cadastrar em posicao especifica");
        printf("\n2. Consultar por numero de conta");
        printf("\n3. Atualizar saldo");
        printf("\n4. Encerrar conta (Remover)");
        printf("\n5. Listar todos os clientes");
        printf("\n6. Restaurar leitura do inicio (rewind + listar)");
        printf("\n7. Encerrar");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cadastrarCliente(txt); break;
            case 2: consultarCliente(txt); break;
            case 3: atualizarSaldo(txt); break;
            case 4: encerrarConta(txt); break;
            case 5: listarClientes(txt); break;
            case 6: 
                printf("\nExecutando rewind()...\n");
                listarClientes(txt); 
                break;
            case 7: printf("Encerrando o sistema. Ate logo!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 7);

    fclose(txt);
    return 0;
}
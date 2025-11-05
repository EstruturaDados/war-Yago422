#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_TERRITORIOS 5
#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    char corDono[MAX_NOME];
    int tropas;
} Territorio;

void limparBufferEntrada();
void removerNewline(char *str);

int main() {
    Territorio mapa[NUM_TERRITORIOS];

    printf("=== CADASTRO INICIAL DO MAPA ===\n");
    printf("Por favor, insira os dados para os %d territórios.\n\n", NUM_TERRITORIOS);

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Cadastrando Território %d ---\n", i + 1);

        printf("Digite o nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        removerNewline(mapa[i].nome);

        printf("Digite a cor do exército (ex: Azul): ");
        fgets(mapa[i].corDono, MAX_NOME, stdin);
        removerNewline(mapa[i].corDono);

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();

        printf("\n");
    }

    printf("=========================================================\n");
    printf("           🗺️  MAPA DO MUNDO CADASTRADO 🗺️\n");
    printf("=========================================================\n");
    printf("| %-20s | %-15s | %s |\n", "Território", "Dono (Cor)", "Tropas");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-20s | %-15s | %-6d |\n",
               mapa[i].nome,
               mapa[i].corDono,
               mapa[i].tropas);
    }
    printf("---------------------------------------------------------\n");

    return EXIT_SUCCESS;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

#define NUM_TERRITORIOS 5
#define MAX_NOME 50
#define LADOS_DADO 6

typedef enum {
    INDEFINIDA,
    VERDE,
    VERMELHO,
    AZUL,
    AMARELO
} Cor;

typedef struct {
    char nome[MAX_NOME];
    Cor corDono;
    int tropas;
} Territorio;

void limparBufferEntrada();
void removerNewline(char *str);
Cor stringParaCor(char *str);
const char* corParaString(Cor cor);
void exibirMapa(const Territorio *mapa);
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *mapa, int idAtacante, int idDefensor);

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = NULL;
    mapa = (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro fatal: Falha ao alocar memória para o mapa!\n");
        return 1;
    }

    printf("=== CADASTRO INICIAL DO MAPA (NÍVEL AVENTUREIRO) ===\n");
    printf("Cores disponíveis: Verde, Vermelho, Azul, Amarelo\n\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        char bufferCor[MAX_NOME];

        printf("--- Cadastrando Território %d ---\n", i + 1);

        printf("Digite o nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        removerNewline(mapa[i].nome);

        printf("Digite a cor do exército: ");
        fgets(bufferCor, MAX_NOME, stdin);
        mapa[i].corDono = stringParaCor(bufferCor);

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
        printf("\n");
    }

    printf("========================================\n");
    printf("         ⚔️  QUE COMECE A BATALHA! ⚔️\n");
    printf("========================================\n");

    int escolhaMenu = -1;
    do {
        exibirMapa(mapa);

        printf("\n--- MENU DE AÇÕES ---\n");
        printf("1. Atacar\n");
        printf("0. Sair do Jogo\n");
        printf("Sua escolha: ");

        if (scanf("%d", &escolhaMenu) != 1) {
            escolhaMenu = -1;
        }
        limparBufferEntrada();

        switch (escolhaMenu) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 0:
                printf("Obrigado por jogar. Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }

        if (escolhaMenu != 0) {
             printf("\nPressione ENTER para continuar...");
             getchar();
        }

    } while (escolhaMenu != 0);

    printf("Liberando memória...\n");
    free(mapa);
    mapa = NULL;

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

Cor stringParaCor(char *str) {
    removerNewline(str);
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }

    if (strcmp(str, "VERDE") == 0) return VERDE;
    if (strcmp(str, "VERMELHO") == 0) return VERMELHO;
    if (strcmp(str, "AZUL") == 0) return AZUL;
    if (strcmp(str, "AMARELO") == 0) return AMARELO;

    return INDEFINIDA;
}

const char* corParaString(Cor cor) {
    switch (cor) {
        case VERDE:     return "Verde";
        case VERMELHO:  return "Vermelho";
        case AZUL:      return "Azul";
        case AMARELO:   return "Amarelo";
        default:        return "Indefinida";
    }
}

void exibirMapa(const Territorio *mapa) {
    printf("\n=========================================================\n");
    printf("           🗺️  MAPA DO MUNDO (ESTADO ATUAL) 🗺️\n");
    printf("=========================================================\n");
    printf("| ID | %-20s | %-15s | %s |\n", "Território", "Dono (Cor)", "Tropas");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-2d | %-20s | %-15s | %-6d |\n",
               i + 1,
               mapa[i].nome,
               corParaString(mapa[i].corDono),
               mapa[i].tropas);
    }
    printf("---------------------------------------------------------\n");
}

void faseDeAtaque(Territorio *mapa) {
    int idAtacante = -1;
    int idDefensor = -1;
    int idxAtacante = -1;
    int idxDefensor = -1;

    printf("\n--- FASE DE ATAQUE ---\n");

    printf("Digite o ID (1 a %d) do território ATACANTE: ", NUM_TERRITORIOS);
    if (scanf("%d", &idAtacante) != 1) {
        limparBufferEntrada();
        printf("Entrada inválida.\n");
        return;
    }
    limparBufferEntrada();

    idxAtacante = idAtacante - 1;

    if (idxAtacante < 0 || idxAtacante >= NUM_TERRITORIOS) {
        printf("ID de ataque inválido! (Deve ser de 1 a %d)\n", NUM_TERRITORIOS);
        return;
    }
    if (mapa[idxAtacante].tropas <= 1) {
        printf("Ataque falhou: Você precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    printf("Digite o ID (1 a %d) do território DEFENSOR: ", NUM_TERRITORIOS);
    if (scanf("%d", &idDefensor) != 1) {
        limparBufferEntrada();
        printf("Entrada inválida.\n");
        return;
    }
    limparBufferEntrada();

    idxDefensor = idDefensor - 1;

    if (idxDefensor < 0 || idxDefensor >= NUM_TERRITORIOS) {
        printf("ID de defesa inválido! (Deve ser de 1 a %d)\n", NUM_TERRITORIOS);
        return;
    }
    if (idxAtacante == idxDefensor) {
        printf("Ataque inválido: Um território não pode atacar a si mesmo.\n");
        return;
    }

    simularAtaque(mapa, idxAtacante, idxDefensor);
}

void simularAtaque(Territorio *mapa, int idAtacante, int idDefensor) {

    if (mapa[idAtacante].corDono == mapa[idDefensor].corDono) {
        printf("Ataque falhou: Você não pode atacar um território que já é seu!\n");
        return;
    }

    printf("\nIniciando batalha: %s (%s) vs %s (%s)!\n",
           mapa[idAtacante].nome, corParaString(mapa[idAtacante].corDono),
           mapa[idDefensor].nome, corParaString(mapa[idDefensor].corDono));

    int dadoAtaque = (rand() % LADOS_DADO) + 1;
    int dadoDefesa = (rand() % LADOS_DADO) + 1;

    printf("... Dado de Ataque: %d\n", dadoAtaque);
    printf("... Dado de Defesa: %d\n", dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        printf(">> Vitória do Ataque! O defensor (%s) perdeu 1 tropa.\n", mapa[idDefensor].nome);
        mapa[idDefensor].tropas--;

        if (mapa[idDefensor].tropas == 0) {
            printf("\n>> CONQUISTA! O território %s foi dominado pelo exército %s!\n",
                   mapa[idDefensor].nome, corParaString(mapa[idAtacante].corDono));

            mapa[idDefensor].corDono = mapa[idAtacante].corDono;
            mapa[idDefensor].tropas = 1;
            mapa[idAtacante].tropas--;
        }
    } else {
        printf(">> Vitória da Defesa! O atacante (%s) perdeu 1 tropa.\n", mapa[idAtacante].nome);
        mapa[idAtacante].tropas--;
    }
}
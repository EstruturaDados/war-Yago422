#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 5
#define MAX_NOME 50
#define LADOS_DADO 6
#define NUM_MISSOES 2

typedef enum {
    INDEFINIDA,
    VERDE,
    VERMELHO,
    AZUL
} Cor;

const Cor COR_JOGADOR = AZUL;

typedef struct {
    char nome[MAX_NOME];
    Cor corDono;
    int tropas;
} Territorio;

void limparBufferEntrada();
const char* corParaString(Cor cor);

void inicializarTerritorios(Territorio *mapa);
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int idMissao, Cor corJogador);
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

    inicializarTerritorios(mapa);
    int missaoJogador = sortearMissao();
    int escolhaMenu = -1;
    int jogoAtivo = 1;

    printf("========================================\n");
    printf("        🧠 BEM-VINDO AO NÍVEL MESTRE 🧠\n");
    printf("========================================\n");
    printf("Seu exército é o %s.\n", corParaString(COR_JOGADOR));

    do {
        exibirMapa(mapa);
        exibirMissao(missaoJogador);

        printf("\n--- MENU DE AÇÕES ---\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missão\n");
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
            case 2:
                if (verificarVitoria(mapa, missaoJogador, COR_JOGADOR)) {
                    printf("\n========================================\n");
                    printf("  PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO!\n");
                    printf("              🎉 VITÓRIA! 🎉\n");
                    printf("========================================\n");
                    jogoAtivo = 0;
                } else {
                    printf("\n----------------------------------------\n");
                    printf("  Você ainda não cumpriu sua missão.\n");
                    printf("----------------------------------------\n");
                }
                break;
            case 0:
                printf("Obrigado por jogar. Saindo...\n");
                jogoAtivo = 0;
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }

        if (jogoAtivo) {
             printf("\nPressione ENTER para continuar...");
             getchar();
        }

    } while (jogoAtivo);

    printf("Liberando memória...\n");
    free(mapa);
    mapa = NULL;

    return EXIT_SUCCESS;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

const char* corParaString(Cor cor) {
    switch (cor) {
        case VERDE:     return "Verde";
        case VERMELHO:  return "Vermelho";
        case AZUL:      return "Azul";
        default:        return "Indefinida";
    }
}

void inicializarTerritorios(Territorio *mapa) {
    strcpy(mapa[0].nome, "Brasil");
    mapa[0].corDono = VERDE;
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    mapa[1].corDono = VERDE;
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Alemanha");
    mapa[2].corDono = AZUL;
    mapa[2].tropas = 8;

    strcpy(mapa[3].nome, "Franca");
    mapa[3].corDono = AZUL;
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Japao");
    mapa[4].corDono = VERMELHO;
    mapa[4].tropas = 6;
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

void exibirMissao(int idMissao) {
    printf("\n--- SUA MISSÃO SECRETA ---\n");
    switch (idMissao) {
        case 0:
            printf("OBJETIVO: Destruir completamente o exército VERDE.\n");
            break;
        case 1:
            printf("OBJETIVO: Conquistar 3 territórios no total.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
            break;
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio *mapa, int idMissao, Cor corJogador) {
    int territoriosJogador = 0;
    int territoriosVerdes = 0;

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (mapa[i].corDono == corJogador) {
            territoriosJogador++;
        }
        if (mapa[i].corDono == VERDE) {
            territoriosVerdes++;
        }
    }

    switch (idMissao) {
        case 0:
            return (territoriosVerdes == 0);
        case 1:
            return (territoriosJogador >= 3);
        default:
            return 0;
    }
}

void faseDeAtaque(Territorio *mapa) {
    int idAtacante = -1;
    int idDefensor = -1;
    int idxAtacante = -1;
    int idxDefensor = -1;

    printf("\n--- FASE DE ATAQUE ---\n");

    printf("Digite o ID (1 a %d) do seu território ATACANTE: ", NUM_TERRITORIOS);
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
    if (mapa[idxAtacante].corDono != COR_JOGADOR) {
         printf("Ataque falhou: Você só pode atacar com seu exército (%s)!\n", corParaString(COR_JOGADOR));
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
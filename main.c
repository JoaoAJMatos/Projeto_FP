#include <stdio.h>
#include <assert.h>

#include "sistema/fs.h"
#include "sistema/stdout.h"

#define NUMERO_MAXIMO_DE_ESTUDANTES 5000
#define NUMERO_MAXIMO_DE_ATIVIDAES  200
#define NUMERO_MAXIMO_DE_INSCRICOES 10000


/* ================== ESTRUTURAS ================== */

typedef struct {
   int   identificador;
   char* nome;
   char* escola;
   int   nif;
   char* email;
   int   telefone;
} t_participante;


typedef struct {
    int identificador;
    char* designacao;
    char* data;
    char* hora;
    char* local;
    char* tipo;
    char* associacao_estudantes;
    float valor;
} t_atividade;


typedef struct {
    int identificador;
    int id_participante;
    int id_atividade;
    float valor_pago;
    char* data;
    char* hora;
} t_inscricao;



/* ================== PROTÓTIPOS ================== */

/// Funções básicas de Input ///
int   ler_numero_inteiro(int, int, const char*);
float ler_numero_real(float, float, const char*);
char* ler_string(const char*, int);

/// Funções de gestão de participantes ///
t_participante* participante_criar(int, char*, char*, int, char*, int);
void participante_apagar(t_participante*);
void participante_editar(t_participante*);
__inline__ void participante_mostrar(t_participante*);

/// Funções de gestão de atividades ///
t_atividade* atividade_criar(int, char*, char*, char*, char*, char*, char*, float);
void atividade_apagar(t_atividade*);
void atividade_editar(t_atividade*);
__inline__ void atividade_mostrar(t_atividade*);

/// Funções de gestão de inscrições ///
t_inscricao* inscricao_criar(int, int, int, float, char*, char*);
void inscricao_apagar(t_inscricao*);
void inscricao_editar(t_inscricao*);
__inline__ void inscricao_mostrar(t_inscricao*);



typedef enum {
    GESTAO_PARTICIPANTES = 1,
    GESTAO_ATIVIDADES,
    GESTAO_INSCRICOES,
    GESTAO_PAGAMENTOS,
    SAIR,
} t_opcao_menu_principal;


/* ================== FUNÇÕES ================== */

int main() {
    t_opcao_menu_principal opcao;

    // Definir opcoes do menu principal
    char* opcoes_menu[] = {
            "1. Gestão de Participantes",
            "2. Gestão de Atividades",
            "3. Gestão de Inscrições",
            "4. Estatísticas",
            "5. Sair"
    };

    // Criar um menu com as seguintes opções
    t_menu* menu = criar_menu("PROJETO FP",        // Titulo do menu
                              "Menu Principal",    // Subtitulo do menu
                              opcoes_menu, 5,      // Opcoes do menu e numero de opcoes
                              "Insira a opção",    // Mensagem de pedido de input
                              "Opção inválida",    // Mensagem de erro
                              40, 20, 1,           // Dimensoes do menu e flag de autoajuste
                              COR_TEXTO_BRANCO,    // Cor do texto
                              COR_FUNDO_AZUL,      // Cor de fundo
                              1);                  // Flag de pintura do fundo da consola com a cor de fundo

    // Desenhar o menu criado
    desenhar_menu(menu);

    // Ler a opcao escolhida
    opcao = ler_opcao_menu(menu);

    switch (opcao) {
        case GESTAO_PARTICIPANTES: {
            limpar_ecra();
            printf("Ola\n");
        }
        case GESTAO_ATIVIDADES:
            break;
        case GESTAO_INSCRICOES:
            break;
        case GESTAO_PAGAMENTOS:
            break;
        case SAIR:
            break;
    }   // Sem caso default para que o compilador avise sobre casos não tratados. (Ver notas de desenvolvimento no ficheiro README.md)

    return OK;
}

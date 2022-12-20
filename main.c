/**
 * @file main.c
 * @author Joao Matos (github.com/JoaoAJMatos) & Rúben Lisboa (github.com/Lisboa14)
 * @version 1.0
 * @date 2022-12-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// TODO: Acabar os formulários
// TODO: Fazer testes do processo de salvar/carregar a aplicação, incluindo as funções do fs que são usadas

#include <stdio.h>

#include "include/stdout.h"
#include "include/app.h"

#define COR_TEXTO_PROGRAMA COR_TEXTO_BRANCO
#define COR_FUNDO_PROGRAMA COR_FUNDO_AZUL



/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

void inicializar_vetores(t_participante**, t_atividade**, t_inscricao**);
char confirmar_saida(char*, char*, char*);
void menu_participantes(t_participante**, t_atividade**, t_inscricao**);

/* ========================================================== */

int carregar_dados(char* caminho, t_estado_programa*);

/* ========================================================== */

void inserir_participante(t_estado_programa*);
void inserir_atividade(t_estado_programa*);
void inserir_inscricao(t_estado_programa*);

/* ========================================================== */


/// POSSIVEIS OPCOES DO MENU PRINCIPAL ///
typedef enum {
    GESTAO_PARTICIPANTES = 1,
    GESTAO_ATIVIDADES,
    GESTAO_INSCRICOES,
    GESTAO_PAGAMENTOS,
    SAIR,
} t_opcao_menu_principal;




/* ========================================================== */
/* =                         MAIN                           = */
/* ========================================================== */

int main() {
    /// DECLARAÇÃO DE VARIÁVEIS  ///
    char caminho_save[1024];            // Caminho para o ficheiro onde o estado do programa será guardado

    t_opcao_menu_principal opcao;       // Opção escolhida no menu principal
    int numero_de_participantes = 0;    // Número de participantes registados
    int numero_de_atividades = 0;       // Número de atividades registadas
    int numero_de_inscricoes = 0;       // Número de inscrições registadas
    char confirmacao_saida;             // Confirmar a saída do programa

    t_menu* menu_principal;
    t_menu* menu_participantes;

    t_participante* participantes[NUMERO_MAXIMO_DE_PARTICIPANTES];    // Vetor de participantes
    t_atividade* atividades[NUMERO_MAXIMO_DE_ATIVIDAES];              // Vetor de atividades
    t_inscricao* inscricoes[NUMERO_MAXIMO_DE_INSCRICOES];             // Vetor de inscrições


    /// MENUS ///
    // Definir opções para os menus
    char* opcoes_menu[] = {"1. Gestão de Participantes","2. Gestão de Atividades","3. Gestão de Inscrições","4. Estatísticas","5. Sair"};
    char* opcoes_menu_participantes[] = {"1. Criar Participante","2. Editar Participante","3. Eliminar Participante","4. Mostrar Participantes","5. Voltar"};

    // Criar menus
    menu_principal = criar_menu("PROJETO FP", "Menu Principal", opcoes_menu, 5, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
    menu_participantes = criar_menu("PROJETO FP", "Gestão de Participantes", opcoes_menu_participantes, 5, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);


    /// FORMULÁRIOS ///
    t_formulario_input* formulario_participante;
    t_formulario_input* formulario_atividade;
    char* campos_formulario_participante[] = {"Identificador", "Nome","Escola","NIF","Email","Telefone"};
    char* campos_formulario_atividade[]    = {"Identificador", "Designação","Data","Hora","Local","Tipo", "Associacao de Estudantes", "Valor"};

    // Criar formulários
    t_participante participante;
    t_atividade atividade;
    formulario_participante = criar_formulario_input("Gestão de Participantes", "Insira os dados do novo participante", campos_formulario_participante, 6, &participante, PARTICIPANTE);
    formulario_atividade    = criar_formulario_input("Gestão de Atividades", "Insira os dados da nova atividade", campos_formulario_atividade, 8, &participante, ATIVIDADE);

    /// ESTADO DO PROGRAMA  ///
    // Criação do estado do programa
    //
    // Este estado é partilhado entre funções, de modo que possam aceder e consultar o estado atual
    // do programa.
    t_estado_programa* estado_programa = criar_estado_programa(participantes, atividades, inscricoes, &numero_de_participantes, &numero_de_atividades, &numero_de_inscricoes, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

    pintar_consola(COR_FUNDO_PROGRAMA);

    desenhar_formulario_input(formulario_atividade);
    ler_formulario_input(formulario_atividade, estado_programa);
    limpar_ecra();
    mostrar_atividade(&atividade);
    fflush(stdin);
    getchar();

    /// CARREGAR O ESTADO DO PROGRAMA DO DISCO  ///
    if (carregar_dados(caminho_save, estado_programa) < 0) {
        printf("Erro ao carregar os dados do disco.\n");
        return ERRO;
    }

    // Criar um participante
    participantes[0] = criar_participante(123, "Joao", "Escola Secundaria de Algueirao", 123456789, "mail", 912345678);
    numero_de_participantes++;
    mostrar_estado_programa(estado_programa);
    fflush(stdin);
    getchar();


    /// LOOP PRINCIPAL DO PROGRAMA  ///
    do {
        // Desenhar o menu principal
        desenhar_menu(menu_principal);

        // Ler a opcao escolhida
        opcao = ler_opcao_menu(menu_principal);

        switch (opcao) {
            case GESTAO_PARTICIPANTES: {
                desenhar_menu(menu_participantes);
                opcao = ler_opcao_menu(menu_participantes);
                break;
            }
            case GESTAO_ATIVIDADES:

                break;
            case GESTAO_INSCRICOES:
                limpar_ecra();
                alerta("Gestão de Inscrições", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case GESTAO_PAGAMENTOS:
                limpar_ecra();
                alerta("Gestão de Pagamentos", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case SAIR:
                confirmacao_saida = confirmar_saida("Tem a certeza que pretende sair?", "", "S/N");

        }   // Sem caso default para que o compilador avise sobre casos não tratados. (Ver notas de desenvolvimento no ficheiro README.md)
    } while (confirmacao_saida != 'S');

    if (guardar_estado_programa("save.dat", estado_programa) < 0) {
        printf("Erro ao guardar o estado do programa.\n");
        return ERRO;
    }

    libertar_estado_programa(estado_programa);
    limpar_ecra();
    return OK;
}


/* ========================================================== */
/* =                        FUNÇÕES                         = */
/* ========================================================== */


void inicializar_vetores(t_participante** participantes, t_atividade** atividades, t_inscricao** inscricoes) {
    // Inicializar os arrays de participantes, atividades e inscrições
    for (int i = 0; i < NUMERO_MAXIMO_DE_PARTICIPANTES; i++) participantes[i] = NULL;
    for (int i = 0; i < NUMERO_MAXIMO_DE_ATIVIDAES; i++) atividades[i] = NULL;
    for (int i = 0; i < NUMERO_MAXIMO_DE_INSCRICOES; i++) inscricoes[i] = NULL;
}


char confirmar_saida(char* mensagem, char* subtitulo, char* dica) {
    char confirmacao = 0;
    do {
        limpar_ecra();
        prompt(mensagem, subtitulo, dica, &confirmacao, CHAR, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        confirmacao = toupper(confirmacao);
    } while (confirmacao != 'S' && confirmacao != 'N');

    return confirmacao;
}


int carregar_dados(char* caminho_save, t_estado_programa* estado_programa) {
    char caminho[1024];
    caminho_save = obter_caminho_save();

    if (ficheiro_existe(caminho_save)) {                           // Verificar se o ficheiro de save existe
        estado_programa = carregar_estado_programa(caminho_save);  // Se existir, carregar o estado do programa
        return OK;
    }

    // Se for a primeira vez que o utilizador inicia o programa, o programa deve perguntar onde é que o utilizador
    // pretende guardar os dados do programa.
    prompt("Primeira entrada no programa detetada", "Introduza o caminho relativo/absoluto para o ficheiro de save", "Caminho", caminho, STRING, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

    if (guardar_caminho_save(caminho) < 0) return ERRO;
    inicializar_vetores(estado_programa->participantes, estado_programa->atividades, estado_programa->inscricoes);

    return OK;
}
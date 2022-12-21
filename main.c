/**
 * @file main.c
 * @author Joao Matos (github.com/JoaoAJMatos) & Rúben Lisboa (github.com/Lisboa14)
 * @version 1.0
 * @date 2022-12-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */



// TODO: Fazer testes do processo de salvar/carregar a aplicação, incluindo as funções do fs que são usadas

#include <stdio.h>

#include "include/stdout.h"
#include "include/app.h"

#define COR_TEXTO_PROGRAMA COR_TEXTO_BRANCO
#define COR_FUNDO_PROGRAMA COR_FUNDO_AZUL_CLARO


/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

void inicializar_vetores(t_participante**, t_atividade**, t_inscricao**);
char confirmar_saida(char*, char*, char*);
void menu_participantes(t_estado_programa*);
void menu_atividades(t_estado_programa*);
void menu_inscricoes(t_estado_programa*);
void menu_estatistica(t_estado_programa*);
void menu_consulta(t_estado_programa*);

/* ========================================================== */

int carregar_dados(char* caminho, t_estado_programa*);

/* ========================================================== */

int  inserir_participante(t_estado_programa*);
void editar_participante(t_estado_programa*);
void eliminar_participante(t_estado_programa*);

int  inserir_atividade(t_estado_programa*);
void editar_atividade(t_estado_programa*);
void eliminar_atividade(t_estado_programa*);

void inserir_inscricao(t_estado_programa*);
void editar_inscricao(t_estado_programa*);
void eliminar_inscricao(t_estado_programa*);

/* ========================================================== */

void listar_participantes(t_estado_programa*);
void listar_atividades(t_estado_programa*);
void listar_inscricoes(t_estado_programa*);

/* ========================================================== */

int inicializar_stdout();


/// POSSIVEIS OPCOES DO MENU PRINCIPAL ///
typedef enum {
    GESTAO_PARTICIPANTES = 1,
    GESTAO_ATIVIDADES,
    GESTAO_INSCRICOES,
    ESTATISTICAS,
    CONSULTAS,
    SAIR,
} t_opcao_menu_principal;

typedef enum {
    INSERIR_PARTICIPANTE = 1,
    EDITAR_PARTICIPANTE,
    REMOVER_PARTICIPANTE,
    LISTAR_PARTICIPANTES,
    VOLTAR_MENU_PARTICIPANTES,
} t_opcao_menu_participantes;

typedef enum {
    INSERIR_ATIVIDADE = 1,
    EDITAR_ATIVIDADE,
    REMOVER_ATIVIDADE,
    LISTAR_ATIVIDADES,
    VOLTAR_MENU_ATIVIDADES,
} t_opcao_menu_atividades;

typedef enum {
    INSERIR_INSCRICAO = 1,
    EDITAR_INSCRICAO,
    REMOVER_INSCRICAO,
    LISTAR_INSCRICOES,
    VOLTAR_MENU_INSCRICOES,
} t_opcao_menu_inscricoes;

typedef enum {
    NUMERO_ATIVIDADES_POR_AE = 1,
    PERCENTAGEM_INSCRICOES_POR_ESCOLA,
    INSCRICOES_ENTRE_DATAS
} t_opcao_menu_estatisticas;


/* ========================================================== */
/* =                         MAIN                           = */
/* ========================================================== */

int main() {

    printf("Ola");
    //print("Ola");
    fflush(stdin);
    getchar();

    /// DECLARAÇÃO DE VARIÁVEIS  ///
    char caminho_save[1024];            // Caminho para o ficheiro onde o estado do programa será guardado

    t_opcao_menu_principal opcao;       // Opção escolhida no menu principal
    int numero_de_participantes = 0;    // Número de participantes registados
    int numero_de_atividades = 0;       // Número de atividades registadas
    int numero_de_inscricoes = 0;       // Número de inscrições registadas
    char confirmacao_saida;             // Confirmar a saída do programa

    t_menu* menu_principal;

    t_participante* participantes[NUMERO_MAXIMO_DE_PARTICIPANTES];    // Vetor de participantes
    t_atividade* atividades[NUMERO_MAXIMO_DE_ATIVIDAES];              // Vetor de atividades
    t_inscricao* inscricoes[NUMERO_MAXIMO_DE_INSCRICOES];             // Vetor de inscrições

    /// MENU PRINCIPAL ///
    // Definir opções para os menus
    char* opcoes_menu[] = {"1. Gestão de Participantes","2. Gestão de Atividades","3. Gestão de Inscrições","4. Estatísticas", "5. Consultas", "6. Sair"};
    menu_principal = criar_menu("PROJETO FP", "Menu Principal", opcoes_menu, 6, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);

    /// ESTADO DO PROGRAMA  ///
    // Criação do estado do programa
    //
    // Este estado é partilhado entre funções, de modo que possam aceder e consultar o estado atual
    // do programa.
    t_estado_programa* estado_programa = criar_estado_programa(participantes, atividades, inscricoes, &numero_de_participantes, &numero_de_atividades, &numero_de_inscricoes, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

    inicializar_stdout();
    limpar_ecra();

    /// CARREGAR O ESTADO DO PROGRAMA DO DISCO ///
    if (carregar_dados(caminho_save, estado_programa) < 0) {
        printf("Erro ao carregar os dados do disco.\n");
        return ERRO;
    }

    /// LOOP PRINCIPAL DO PROGRAMA  ///
    do {
        // Desenhar o menu principal
        desenhar_menu(menu_principal);

        // Ler a opcao escolhida
        opcao = ler_opcao_menu(menu_principal);

        switch (opcao) {
            case GESTAO_PARTICIPANTES: {
                menu_participantes(estado_programa);
                break;
            }
            case GESTAO_ATIVIDADES:
                menu_atividades(estado_programa);
                break;
            case GESTAO_INSCRICOES:
                limpar_ecra();
                alerta("Gestão de Inscrições", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case ESTATISTICAS:
                limpar_ecra();
                alerta("Gestão de Pagamentos", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case CONSULTAS:
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

/**
 * @brief Inicializa os vetores de participantes, atividades e inscrições
 *
 * @param participantes
 * @param atividades
 * @param inscricoes
 */
void inicializar_vetores(t_participante** participantes, t_atividade** atividades, t_inscricao** inscricoes) {
    for (int i = 0; i < NUMERO_MAXIMO_DE_PARTICIPANTES; i++) participantes[i] = NULL;
    for (int i = 0; i < NUMERO_MAXIMO_DE_ATIVIDAES; i++) atividades[i] = NULL;
    for (int i = 0; i < NUMERO_MAXIMO_DE_INSCRICOES; i++) inscricoes[i] = NULL;
}


/**
 * @brief Confirma a saida do programa
 *
 * Espera até o utilizador confirmar a saída do programa
 *
 * @param mensagem
 * @param subtitulo
 * @param dica
 * @return
 */
char confirmar_saida(char* mensagem, char* subtitulo, char* dica) {
    char confirmacao = 0;
    do {
        limpar_ecra();
        prompt(mensagem, subtitulo, dica, &confirmacao, T_CHAR, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        confirmacao = toupper(confirmacao);
    } while (confirmacao != 'S' && confirmacao != 'N');

    return confirmacao;
}


/**
 * @brief Carrega os dados do programa guardados no disco
 *
 * Esta função é chamada no início da execução do programa para carregar os dados guardados no disco.
 * Numa primeira instância, a função procura por um ficheiro denominado "proj.cfg" guardado numa localização
 * pré-definida (descrita no ficheiro app.h). Nesse ficheiro, encontra-se guardado o caminho para o ficheiro
 * onde os dados do programa estão guardados.
 *
 * Se o ficheiro (proj.cfg) for encontrado, a função recolhe o caminho guardado no ficheiro e tenta recolher os dados
 * do programa guardados no ficheiro de save (save.dat).
 *
 * Caso o ficheiro "proj.cfg" não exista na localização pré-definida, o utilizador é convidado a inserir o
 * caminho onde pretente que os dados sejam guardados. Após isso, os vetores de dados são inicializados a zero.
 *
 *
 * @param caminho_save
 * @param estado_programa
 * @return
 */
int carregar_dados(char* caminho_save, t_estado_programa* estado_programa) {
    char caminho[1024];
    caminho_save = obter_caminho_save();
    limpar_ecra();

    if (ficheiro_existe(caminho_save)) {                           // Verificar se o ficheiro de save existe
        estado_programa = carregar_estado_programa(caminho_save);  // Se existir, carregar o estado do programa
        return OK;
    }

    // Se for a primeira vez que o utilizador inicia o programa, o programa deve perguntar onde é que o utilizador
    // pretende guardar os dados do programa.
    prompt("Primeira entrada no programa detetada", "Introduza o caminho relativo/absoluto para o ficheiro de save",
           "Caminho", caminho, T_STRING, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

    if (guardar_caminho_save(caminho) < 0) return ERRO;
    inicializar_vetores(estado_programa->participantes, estado_programa->atividades, estado_programa->inscricoes);

    return OK;
}




/**
 * @brief Esta função insere um novo participante no vetor de participantes.
 *
 * A função cria um formulário e mostra-o ao utilizador.
 * Os dados são inseridos pelo utilizador e validados automáticamente, mostrando mensagens de erro
 * nos campos em que os dados inseridos não são válidos.
 * Após a inserção dos dados, o novo participante é inserido no vetor de participantes, referenciado no estado do programa.
 *
 * A função retorna 0 se a inserção for bem sucedida, ou -1 se ocorrer algum erro.
 *
 * @param estado_programa
 */
int inserir_participante(t_estado_programa* estado_programa) {
    t_formulario_input* formulario_participante;
    char* campos_formulario_participante[] = {"Identificador", "Nome","Escola","NIF","Email","Telefone"};
    t_participante* participante;

    if (*estado_programa->numero_participantes_inseridos == NUMERO_MAXIMO_DE_PARTICIPANTES) {
        alerta("Erro", "Não é possível adicionar mais participantes", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return ERRO;
    }

    formulario_participante = criar_formulario_input("Gestão de Participantes", "Insira os dados do novo participante", campos_formulario_participante, 6, &participante, PARTICIPANTE);
    desenhar_formulario_input(formulario_participante);
    ler_formulario_input(formulario_participante, estado_programa);

    estado_programa->participantes[*estado_programa->numero_participantes_inseridos] = participante;
    (*estado_programa->numero_participantes_inseridos)++;

    return OK;
}


/**
 * @brief Menu participantes
 *
 * Esta função orquestra a chamada de funções para o menu de participantes.
 *
 * @param estado_programa
 */
void menu_participantes(t_estado_programa* estado_programa) {
    t_menu* menu_participantes;
    char* opcoes_menu_participantes[] = {"1. Criar Participante","2. Editar Participante","3. Eliminar Participante","4. Mostrar Participantes","5. Voltar"};
    menu_participantes = criar_menu("PROJETO FP", "Gestão de Participantes", opcoes_menu_participantes, 5, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
    t_opcao_menu_participantes opcao;

    do {
        desenhar_menu(menu_participantes);
        opcao = (t_opcao_menu_participantes) ler_opcao_menu(menu_participantes);
        switch (opcao) {
            case INSERIR_PARTICIPANTE:
                inserir_participante(estado_programa);
                break;
            case EDITAR_PARTICIPANTE:
                //editar_participante(estado_programa);
                break;
            case REMOVER_PARTICIPANTE:
                //eliminar_participante(estado_programa);
                break;
            case LISTAR_PARTICIPANTES:
                //listar_participantes(estado_programa);
                break;
            case VOLTAR_MENU_PARTICIPANTES:
                break;
        }
    } while (opcao != VOLTAR_MENU_PARTICIPANTES);
}


int inserir_atividade(t_estado_programa* estado_programa) {
    t_formulario_input* formulario_atividade;
    char* campos_formulario_atividade[] = {"Identificador", "Designação","Data","Hora","Local","Tipo", "Associacao de Estudantes", "Valor"};
    t_atividade* atividade;

    if (*estado_programa->numero_atividadades_inseridas == NUMERO_MAXIMO_DE_ATIVIDAES) {
        alerta("Erro", "Não é possível adicionar mais atividades", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return ERRO;
    }

    formulario_atividade = criar_formulario_input("Gestão de Atividades", "Insira os dados da nova atividade", campos_formulario_atividade, 8, &atividade, ATIVIDADE);
    desenhar_formulario_input(formulario_atividade);
    ler_formulario_input(formulario_atividade, estado_programa);

    estado_programa->atividades[*estado_programa->numero_atividadades_inseridas] = atividade;
    (*estado_programa->numero_atividadades_inseridas)++;

    return OK;
}

void menu_atividades(t_estado_programa* estado_programa) {
    t_menu* menu_atividades;
    char* opcoes_menu_atividades[] = {"1. Criar Atividade","2. Editar Atividade","3. Eliminar Atividade","4. Mostrar Atividades","5. Voltar"};
    menu_atividades = criar_menu("PROJETO FP", "Gestão de Atividades", opcoes_menu_atividades, 5, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
    t_opcao_menu_atividades opcao;

    do {
        desenhar_menu(menu_atividades);
        opcao = (t_opcao_menu_atividades) ler_opcao_menu(menu_atividades);
        switch (opcao) {
            case INSERIR_ATIVIDADE:
                inserir_atividade(estado_programa);
                break;
            case EDITAR_ATIVIDADE:
                //editar_atividade(estado_programa);
                break;
            case REMOVER_ATIVIDADE:
                //eliminar_atividade(estado_programa);
                break;
            case LISTAR_ATIVIDADES:
                //listar_atividades(estado_programa);
                break;
            case VOLTAR_MENU_ATIVIDADES:
                break;
        }
    } while (opcao != VOLTAR_MENU_ATIVIDADES);
}


/* =============================================================== */

/**
 * Verifica se a consola tem as dimensões mínimas necessárias para o bom funcionamento do programa
 * e altera o modo da consola para UTF-16 caso esteja a ser executado no Windows. 
 * 
 * @return int
 */
int inicializar_stdout() {
    t_tamanho_consola* tamanho_consola = obter_tamanho_consola();

#if defined(_WIN32)
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif

    if (tamanho_consola->largura < LARGURA_MINIMA_RECOMENDADA || tamanho_consola->altura < ALTURA_MINIMA_RECOMENDADA) {
        printf("Aconselha-se um terminal com pelo menos %dx%d caracteres.\n", LARGURA_MINIMA_RECOMENDADA, ALTURA_MINIMA_RECOMENDADA);
        getchar();
    }

    free(tamanho_consola);
    return OK;
}
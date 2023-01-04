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

#include "include/cli.h"
#include "include/app.h"

#define COR_TEXTO_PROGRAMA COR_TEXTO_VERDE
#define COR_FUNDO_PROGRAMA COR_FUNDO_PRETO

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

void inserir_dados_teste(t_estado_programa*);   // Insere alguns dados automáticamente para testes
void salvar_programa(t_estado_programa*);

/* ========================================================== */



/// POSSIVEIS OPCOES DO MENU PRINCIPAL ///
typedef enum {
    GESTAO_PARTICIPANTES = 1,
    GESTAO_ATIVIDADES,
    GESTAO_INSCRICOES,
    ESTATISTICAS,
    CONSULTAS,
    SALVAR,
    SAIR
} t_opcao_menu_principal;

typedef enum {
    INSERIR_PARTICIPANTE = 1,
    EDITAR_PARTICIPANTE,
    LISTAR_PARTICIPANTES,
    VOLTAR_MENU_PARTICIPANTES,
} t_opcao_menu_participantes;

typedef enum {
    INSERIR_ATIVIDADE = 1,
    EDITAR_ATIVIDADE,
    LISTAR_ATIVIDADES,
    VOLTAR_MENU_ATIVIDADES,
} t_opcao_menu_atividades;

typedef enum {
    INSERIR_INSCRICAO = 1,
    EDITAR_INSCRICAO,
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
    char* opcoes_menu[] = {"1. Gestão de Participantes","2. Gestão de Atividades","3. Gestão de Inscrições","4. Estatísticas", "5. Consultas", "6. Salvar", "7. Sair"};
    menu_principal = criar_menu("PROJETO FP", "Menu Principal", opcoes_menu, 7, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);

    /// ESTADO DO PROGRAMA  ///
    // Criação do estado do programa
    //
    // Este estado é partilhado entre funções, de modo que possam aceder e consultar o estado atual
    // do programa.
    t_estado_programa* estado_programa = criar_estado_programa(participantes, atividades, inscricoes, &numero_de_participantes, &numero_de_atividades, &numero_de_inscricoes, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

#if DEBUG
    // Insere alguns dados para testes
    //inserir_dados_teste(estado_programa);
#else
    /// CARREGAR O ESTADO DO PROGRAMA DO DISCO ///
    if (carregar_dados(caminho_save, estado_programa) < 0) {
        printf("Erro ao carregar os dados do disco.\n");
        return ERRO;
    }
#endif

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
                menu_inscricoes(estado_programa);
                break;
            case ESTATISTICAS:
                limpar_ecra();
                alerta("Gestão de Pagamentos", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case CONSULTAS:
                limpar_ecra();
                alerta("Gestão de Pagamentos", "Ainda não implementado", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
                break;
            case SALVAR:
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

/**
 * @brief Inicializa os vetores de participantes, atividades e inscrições
 *
 * @param participantes
 * @param atividades
 * @param inscricoes
 */
void inicializar_vetores(t_participante** participantes, t_atividade** atividades, t_inscricao** inscricoes) {
    for (int i = 0; i < NUMERO_MAXIMO_DE_PARTICIPANTES; i++) participantes[i] = NULL;
    for (int i = 0; i < NUMERO_MAXIMO_DE_ATIVIDAES; i++) atividades[i]  = NULL;
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
        prompt(mensagem, subtitulo, dica, &confirmacao, CHAR, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
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

    if (ficheiro_existe(caminho_save)) {                           // Verificar se o ficheiro de save existe
        estado_programa = carregar_estado_programa(caminho_save);  // Se existir, carregar o estado do programa
        return OK;
    }

    // Se for a primeira vez que o utilizador inicia o programa, o programa deve perguntar onde é que o utilizador
    // pretende guardar os dados do programa.
    prompt("Primeira entrada no programa detetada", "Introduza o caminho relativo/absoluto para o ficheiro de save",
           "Caminho", caminho, STRING, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

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

void editar_participante(t_estado_programa* estado_programa) {
    t_formulario_input* formulario_participante;
    t_participante* participante;
    char* campos_formulario_participante[] = {"Identificador", "Nome", "Escola", "NIF", "Email", "Telefone"};
    int id_participante, indice_participante;

    if (*estado_programa->numero_participantes_inseridos == 0) {
        alerta("Erro ao tentar editar participante", "Não existem participantes registados", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return;
    }

    // Perguntar qual o ID do participante que pretende alterar
    prompt("Editar Participante", "Insira o ID do participante que pretende alterar", "ID", &id_participante, INT, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);

    // Verificar se o ID inserido existe
    indice_participante = procurar_participante_por_id(estado_programa->participantes, *estado_programa->numero_participantes_inseridos, id_participante);
    if (indice_participante == -1) {
        alerta("Erro", "Não existe nenhum participante com o ID inserido", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return;
    }

    participante = estado_programa->participantes[indice_participante];
    formulario_participante = criar_formulario_input("Gestão de Participantes", "Insira os dados do novo participante", campos_formulario_participante, 6, &participante, PARTICIPANTE);
    desenhar_formulario_input(formulario_participante);
    ler_formulario_input(formulario_participante, estado_programa);
    estado_programa->participantes[indice_participante] = participante;
}

void listar_participantes(t_estado_programa* estado_programa) {
    int i;
    t_participante* participante;
    limpar_ecra();

    if (*estado_programa->numero_participantes_inseridos == 0) {
        alerta("Erro ao tentar listar participantes", "Não existem participantes registados", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return;
    }

    /*for (i = 0; i < *estado_programa->numero_participantes_inseridos; i++) {
        participante = estado_programa->participantes[i];
        printf("======================================\n");
        printf("Participante %d:\n\n", i + 1);
        mostrar_participante(participante);
        printf("======================================\n");
    }*/

    mostrar_estado_programa(estado_programa);
    fflush(stdin);
    getchar();
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
    char* opcoes_menu_participantes[] = {"1. Criar Participante","2. Editar Participante","3. Mostrar Participantes","4. Voltar"};
    menu_participantes = criar_menu("PROJETO FP", "Gestão de Participantes", opcoes_menu_participantes, 4, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
    t_opcao_menu_participantes opcao;

    do {
        desenhar_menu(menu_participantes);
        opcao = (t_opcao_menu_participantes) ler_opcao_menu(menu_participantes);
        switch (opcao) {
            case INSERIR_PARTICIPANTE:
                inserir_participante(estado_programa);
                mostrar_estado_programa(estado_programa);
                fflush(stdin);
                getchar();
                break;
            case EDITAR_PARTICIPANTE:
                editar_participante(estado_programa);
                break;
            case LISTAR_PARTICIPANTES:
                listar_participantes(estado_programa);
                break;
            case VOLTAR_MENU_PARTICIPANTES:
                break;
        }
    } while (opcao != VOLTAR_MENU_PARTICIPANTES);
}


int inserir_atividade(t_estado_programa* estado_programa) {
    t_formulario_input* formulario_atividade;
    char* campos_formulario_atividade[]    = {"Identificador", "Designação","Data","Hora","Local","Tipo", "Associacao de Estudantes", "Valor"};
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
    char* opcoes_menu_atividades[] = {"1. Criar Atividade","2. Editar Atividade","3. Mostrar Atividades","4. Voltar"};
    menu_atividades = criar_menu("PROJETO FP", "Gestão de Atividades", opcoes_menu_atividades, 4, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
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
            case LISTAR_ATIVIDADES:
                //listar_atividades(estado_programa);
                break;
            case VOLTAR_MENU_ATIVIDADES:
                break;
        }
    } while (opcao != VOLTAR_MENU_ATIVIDADES);
}

void inserir_inscricao(t_estado_programa* estado_programa) {
    t_formulario_input* formulario_inscricao;
    char* campos_formulario_inscricao[]    = {"Identificador", "ID Participante", "ID Atividade"};
    t_inscricao* inscricao;

    if (*estado_programa->numero_de_inscricoes == NUMERO_MAXIMO_DE_INSCRICOES) {
        alerta("Erro", "Não é possível adicionar mais inscrições", COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA);
        return;
    }

    formulario_inscricao = criar_formulario_input("Gestão de Inscricoes", "Insira os dados da nova inscricao", campos_formulario_inscricao, 3, &inscricao, INSCRICAO);
    desenhar_formulario_input(formulario_inscricao);
    ler_formulario_input(formulario_inscricao, estado_programa);

    estado_programa->inscricoes[*estado_programa->numero_de_inscricoes] = inscricao;
    (*estado_programa->numero_de_inscricoes)++;
}

void menu_inscricoes(t_estado_programa* estado_programa) {
    t_menu* menu_inscricoes;
    char* opcoes_menu_inscricoes[] = {"1. Criar Inscricao","2. Editar Inscricao","3. Mostrar Inscricoes","4. Voltar"};
    menu_inscricoes = criar_menu("PROJETO FP", "Gestão de Inscricoes", opcoes_menu_inscricoes, 4, "Insira a opção", "Opção inválida", 40, 20, 1, COR_TEXTO_PROGRAMA, COR_FUNDO_PROGRAMA, 1);
    t_opcao_menu_inscricoes opcao;

    do {
        desenhar_menu(menu_inscricoes);
        opcao = (t_opcao_menu_inscricoes) ler_opcao_menu(menu_inscricoes);
        switch (opcao) {
            case INSERIR_INSCRICAO:
                inserir_inscricao(estado_programa);
                break;
            case EDITAR_INSCRICAO:
                //editar_inscricao(estado_programa);
                break;
            case LISTAR_INSCRICOES:
                //listar_inscricoes(estado_programa);
                break;
            case VOLTAR_MENU_INSCRICOES:
                break;
        }
    } while (opcao != VOLTAR_MENU_INSCRICOES);
}

/* ========================================================== */

void inserir_dados_teste(t_estado_programa* estado_programa) {
    t_participante *participante1 = criar_participante(1, "Joao", "ESTG", 123, "mail@mail.com", 123456789);
    t_atividade *atividade1 = criar_atividade(2, "Descida do rio", "30/04/2023", "10:40", "Tejo", "Lazer", "AE-ESTG", 15);
    estado_programa->participantes[0] = participante1;
    estado_programa->atividades[0] = atividade1;
    t_inscricao *inscricao1 = criar_inscricao(1, 1, 2, estado_programa->atividades);
    estado_programa->inscricoes[0] = inscricao1;
    (*estado_programa->numero_participantes_inseridos)++;
    (*estado_programa->numero_atividadades_inseridas)++;
    (*estado_programa->numero_de_inscricoes)++;
}
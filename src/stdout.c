//
// Created by João Matos on 14/12/2022.
//

#include "../include/stdout.h"

#include <ctype.h>


/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

static void escrever_caixa_do_menu(t_menu* menu);                  // Escreve a caixa que envolve o menu
static void escrever_titulo_do_menu(t_menu* menu);                 // Escreve o título do menu
static void escrever_opcoes_do_menu(t_menu* menu);                 // Escreve as opções do menu
static void escrever_caixa_de_input(t_menu* menu);                 // Escreve a caixa que envolve o input da opcao do menu
static int  obter_tamanho_do_maior_item_do_menu(t_menu* menu);     // Obtém o tamanho do maior item do menu
static void limpar_caixa_de_input(t_menu* menu);                   // Limpa a caixa que envolve o input da opcao do menu
static void escrever_mensagem_de_erro(t_menu* menu);               // Escreve a mensagem de erro de input no menu
static void escrever_conteudo_do_prompt(int, int, char*, char*, char*);    // Escreve o conteúdo do prompt
static void ler_input_do_prompt(void*, t_tipo_primitivo);          // Lê o input do prompt e guarda-a na variável passada por referência
static void esperar_input_do_utilizador(char*, int, int);          // Espera que o utilizador pressione uma tecla para continuar

static void escrever_linha(int, int, int, char);                   // Escreve uma linha na consola

static t_participante * ler_participante();                     // Lê um participante do formulário de input
static t_atividade*  ler_atividade();                           // Lê uma atividade do formulário de input
static t_inscricao*  ler_inscricao(t_estado_programa*);         // Lê uma inscrição do formulário de input

static void ler_identificador_participante_inscricao(int*, int, t_estado_programa*);        // Lê o identificador de um participante do formulário de input
static void ler_identificador_atividade_inscricao(int*, int, t_estado_programa*);           // Lê o nome de um participante do formulário de input

static void ler_identificador(int*, int);                          // Lê um identificador do formulário de input
static void ler_string(char*, int, int);                           // Lê um nome do formulário de input
static void ler_escola(char*, int);                                // Lê uma escola do formulário de input
static void ler_nif(int*, int);                                    // Lê um nif do formulário de input
static void ler_email(char*, int);                                 // Lê um email do formulário de input
static void ler_telefone(int*, int);                               // Lê um telefone do formulário de input
static void ler_data(char*, int);                                  // Lê uma data do formulário de input
static void ler_hora(char*, char*, int);                           // Lê uma hora do formulário de input
static void ler_tipo_de_atividade(char*, int);                     // Lê um tipo de atividade do formulário de input
static void ler_associacao_estudantes(char*, int);                 // Lê uma associação de estudantes do formulário de input
static void ler_float(float*, float, float, int);                  // Lê um float do formulário de input
static void ler_int(int*, int, int, int);                          // Lê um int do formulário de input

static int  validar_hora_minuto(int, int, char*);                  // Valida a hora ou minuto do formulário de input
static int  hora_valida(char*, char*, char*);                      // Verifica se uma hora é válida
static int  data_valida(char*, char*);                             // Verifica se uma data é válida
static int  validar_dia_mes(int, int, char*);                      // Valida uma data
static int  validar_ano(int, int, char*);                          // Valida uma data
static int  nif_valido(int);                                       // Verifica se o nif é válido
static int  email_valido(char*);                                   // Verifica se o email é válido

static int  contem_opcao(char*, char**, int);                      // Verifica se uma string está dentro das opções válidas



/* ========================================================== */
/* =                       FUNÇÕES                          = */
/* ========================================================== */


/**
 * Verifica se a consola tem as dimensões mínimas necessárias para o bom funcionamento do programa.
 * @return int
 */
__attribute__ ((constructor)) int inicializar_stdout() {
    t_tamanho_consola* tamanho_consola = obter_tamanho_consola();

    if (tamanho_consola->largura < LARGURA_MINIMA_RECOMENDADA || tamanho_consola->altura < ALTURA_MINIMA_RECOMENDADA) {
        printf("Aconselha-se um terminal com pelo menos %dx%d caracteres.\n", LARGURA_MINIMA_RECOMENDADA, ALTURA_MINIMA_RECOMENDADA);
        getchar();
    }

    free(tamanho_consola);
    return OK;
}

/**
 * @brief Devolve o tamanho da consola
 * @return t_tamanho_consola*
 */
t_tamanho_consola* obter_tamanho_consola() {
    t_tamanho_consola* tamanho_consola = (t_tamanho_consola*) malloc(sizeof(t_tamanho_consola));
#if defined(_WIN32) || defined(_WIN64)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int colunas, linhas;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    colunas = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    linhas = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    tamanho_consola->largura = colunas;
    tamanho_consola->altura = linhas;
#else
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    tamanho_consola->largura = w.ws_col;
    tamanho_consola->altura = w.ws_row;
#endif

    return tamanho_consola;
}


/**
 * @brief Imprime a string passada como argumento no centro da consola
 * @param texto
 * @param tamanho_consola
 */
void imprimir_centrado(char* texto, t_tamanho_consola* tamanho_consola) {
    int tamanho_texto = strlen(texto);
    int posicao = (tamanho_consola->largura - tamanho_texto) / 2;
    for (int i = 0; i < posicao; i++) {
        printf(" ");
    }
    printf("%s", texto);
}


/**
 * @brief Coloca o cursor numa determinada posição na consola
 */
void gotoxy(int x, int y) {
#if defined(_WIN32) || defined(_WIN64)
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y, x);
#endif
}



/* ========================================================== */
/* =                         MENU                           = */
/* ========================================================== */


/**
 * @brief Cria um novo menu
 * @param titulo
 * @param subtitulo
 * @param opcoes
 * @param num_opcoes
 * @param mensagem
 * @param mensagem_erro
 * @param largura
 * @param altura
 * @param auto_ajustar
 * @param cor_letras
 * @param cor_fundo
 * @param sombra
 * @return t_menu*
 */
t_menu* criar_menu(char* titulo, char* subtitulo, char** opcoes, int num_opcoes, char* mensagem, char* mensagem_erro, \
                   int largura, int altura, int auto_ajustar, char* cor_letras, char* cor_fundo, int pintar_consola)
{
    t_tamanho_consola *tamanho_consola = obter_tamanho_consola();
    t_menu* menu = (t_menu*) malloc(sizeof(t_menu));
    menu->titulo = titulo;
    menu->subtitulo = subtitulo;
    menu->opcoes = opcoes;
    menu->numero_opcoes = num_opcoes;
    menu->mensagem = mensagem;
    menu->mensagem_erro = mensagem_erro;

    if (obter_tamanho_do_maior_item_do_menu(menu) > largura && auto_ajustar) {
        largura = obter_tamanho_do_maior_item_do_menu(menu) + 7;
    }

    menu->largura = largura;
    menu->altura = altura;

    menu->posicao_x = (tamanho_consola->largura - largura) / 2;
    menu->posicao_y = (tamanho_consola->altura - menu->altura) / 2;
    menu->auto_ajustar = auto_ajustar;
    menu->cor_letras = cor_letras;
    menu->cor_fundo = cor_fundo;
    menu->pintar_consola = pintar_consola;

    return menu;
}


/**
 * @brief Imprime o menu na consola
 * @param menu
 */
void desenhar_menu(t_menu* menu) {
    setlocale(LC_CTYPE, "");        // Para imprimir caracteres especiais

    limpar_ecra();
    definir_cor_texto(menu->cor_letras);
    definir_cor_fundo(menu->cor_fundo);

    if (menu->pintar_consola) {
        pintar_consola(menu->cor_fundo);
    }

    escrever_caixa_do_menu(menu);   // Escreve a caixa que envolve o menu
    escrever_titulo_do_menu(menu);  // Escreve o título do menu
    escrever_opcoes_do_menu(menu);  // Escreve as opções do menu
    escrever_caixa_de_input(menu);  // Escreve a caixa de input
}


/**
 * @brief Desenha uma caixa na posição e com o tamanho especificado
 * @param posicao_x
 * @param posicao_y
 * @param largura
 * @param altura
 */
void desenhar_caixa(int posicao_x, int posicao_y, int largura, int altura)
{
    gotoxy(posicao_x, posicao_y);
    printf(CANTO_SUPERIOR_ESQUERDO);
    for (int i = 0; i < largura - 2; i++) {
        printf(LINHA_HORIZONTAL);
    }
    printf(CANTO_SUPERIOR_DIREITO);

    for (int i = 0; i < altura - 2; i++) {
        gotoxy(posicao_x, posicao_y + i + 1);
        printf(LINHA_VERTICAL);
        gotoxy(posicao_x + largura - 1, posicao_y + i + 1);
        printf(LINHA_VERTICAL);
    }

    gotoxy(posicao_x, posicao_y + altura - 1);
    printf(CANTO_INFERIOR_ESQUERDO);
    for (int i = 0; i < largura - 2; i++) {
        printf(LINHA_HORIZONTAL);
    }
    printf(CANTO_INFERIOR_DIREITO);
}



int ler_opcao_menu(t_menu* menu) {
    int opcao = 0;

    do {
        fflush(stdin);
        scanf("%d", &opcao);
        if (opcao < 1 || opcao > menu->numero_opcoes) {
            escrever_mensagem_de_erro(menu);
        }
    } while (opcao < 1 || opcao > menu->numero_opcoes);

    return opcao;
}


void pintar_consola(char* cor) {
    // Obter tamanho da consola
    t_tamanho_consola *tamanho_consola = obter_tamanho_consola();

    // Pintar a consola
    for (int i = 0; i < tamanho_consola->altura; i++) {
        for (int j = 0; j < tamanho_consola->largura; j++) {
            gotoxy(j, i);
            printf(" ");
        }
    }
}



/* ========================================================== */
/* =                     FORMULÁRIOS                        = */
/* ========================================================== */


t_formulario_input* criar_formulario_input(char* titulo, char* subtitulo, char** rotulos, int numero_campos, \
                                           void* estrutura_outpur, t_tipo_estrutura tipo_estrutura)
{
    t_formulario_input* formulario = (t_formulario_input*) malloc(sizeof(t_formulario_input));
    formulario->titulo = titulo;
    formulario->subtitulo = subtitulo;
    formulario->rotulos = rotulos;
    formulario->numero_campos = numero_campos;
    formulario->estrutura_output = estrutura_outpur;
    formulario->tipo_estrutura_output = tipo_estrutura;

    return formulario;
}


void desenhar_formulario_input(t_formulario_input* formulario) {
    limpar_ecra();
    t_tamanho_consola *tamanho_consola = obter_tamanho_consola();

    // Escrever caixa do formulário
    desenhar_caixa(2, 1, tamanho_consola->largura - 2, tamanho_consola->altura);

    // Escrever título e subtitulo do formulário
    gotoxy((tamanho_consola->largura - strlen(formulario->titulo)) / 2, 2);
    printf("%s", formulario->titulo);
    gotoxy((tamanho_consola->largura - strlen(formulario->subtitulo)) / 2, 4);
    printf("%s", formulario->subtitulo);

    // Escrever campos do formulário
    int posicao_y = 6;
    for (int i = 0; i < formulario->numero_campos; i++) {
        desenhar_caixa(4, posicao_y, tamanho_consola->largura - 6, 3);
        gotoxy(6, posicao_y);
        printf("%s", formulario->rotulos[i]);

        if (formulario->numero_campos > 7)
            posicao_y += 3;
        else
            posicao_y += 4;
    }
}

t_participante* ler_participante() {
    char nome[50], email[50], escola[8];
    int nif, telefone, identificador;
    t_participante* participante;

    ler_identificador(&identificador, 7);
    ler_string(nome, 50, 11);
    ler_escola(escola, 15);
    ler_nif(&nif, 19);
    ler_email(email, 23);
    ler_telefone(&telefone, 27);

    participante = criar_participante(identificador, nome, escola, nif, email, telefone);
    return participante;
}

t_atividade* ler_atividade() {
    char designacao[50], data[11], hora[6], local[50], tipo[50], associacao_estudantes[20];
    int identificador;
    float valor;

    t_atividade *atividade;

    ler_identificador(&identificador, 7);
    ler_string(designacao, 50, 10);
    ler_data(data, 13);
    ler_hora(hora, data, 16);
    ler_string(local, 50, 19);
    ler_tipo_de_atividade(tipo, 22);
    ler_associacao_estudantes(associacao_estudantes, 25);
    ler_float(&valor, 0, 999, 28);

    atividade = criar_atividade(identificador, designacao, data, hora, local, tipo, associacao_estudantes, valor);
    return atividade;
}

t_inscricao* ler_inscricao(t_estado_programa* estado_programa) {
    int identificador, identificador_participante, identificador_atividade;
    t_inscricao* inscricao;

    ler_identificador(&identificador, 7);
    ler_identificador_participante_inscricao(&identificador_participante, 11, estado_programa);
    ler_identificador_atividade_inscricao(&identificador_atividade, 15, estado_programa);

    inscricao = criar_inscricao(identificador, identificador_participante, identificador_atividade, estado_programa->atividades);

    return inscricao;
}

int ler_formulario_input(t_formulario_input* formulario, t_estado_programa* estado_programa) {
    void* estrutura_output = formulario->estrutura_output;

    switch (formulario->tipo_estrutura_output) {
        case PARTICIPANTE:
            estrutura_output = ler_participante();
            break;
        case ATIVIDADE:
            estrutura_output = ler_atividade();
            break;
        case INSCRICAO:
            estrutura_output = ler_inscricao(estado_programa);
            break;
    }   // Sem caso default para que o compilador nos avise de casos não tratados

    return 1;
}



/* ========================================================== */
/* =                    AÇÕES RÁPIDAS                       = */
/* ========================================================== */

void prompt(char* mensagem, char* subtitulo, char* dica, void* variavel_output, t_tipo_primitivo tipo_variavel_output, char* cor_texto, char* cor_fundo)
{
    int comprimento_do_maior_elemento = 0;
    int tamanho_mensagem = strlen(mensagem);
    int tamanho_subtitulo = strlen(subtitulo);
    comprimento_do_maior_elemento = tamanho_mensagem > tamanho_subtitulo ? tamanho_mensagem : tamanho_subtitulo;

    t_tamanho_consola *tamanho_consola = obter_tamanho_consola();
    int posicao_x = (tamanho_consola->largura - comprimento_do_maior_elemento) / 2;
    int posicao_y = (tamanho_consola->altura - 6) / 2;

    definir_cor_texto(cor_texto);
    definir_cor_fundo(cor_fundo);

    // Escrever caixa do formulário
    desenhar_caixa(posicao_x, posicao_y, comprimento_do_maior_elemento + 4, 8);

    // Escrever o conteudo da caixa do prompt
    escrever_conteudo_do_prompt(posicao_x, posicao_y, mensagem, subtitulo, dica);

    // Ler input do utilizador
    gotoxy(posicao_x + strlen(dica) + 5, posicao_y + 5);
    ler_input_do_prompt(variavel_output, tipo_variavel_output);

    limpar_formatacao();
}


void alerta(char* titulo, char* alerta, char* cor_texto, char* cor_fundo) {
    int comprimento_do_maior_elemento = 0;
    int tamanho_titulo = strlen(titulo);
    int tamanho_alerta = strlen(alerta);
    comprimento_do_maior_elemento = tamanho_titulo > tamanho_alerta ? tamanho_titulo : tamanho_alerta;

    t_tamanho_consola *tamanho_consola = obter_tamanho_consola();
    int posicao_x = (tamanho_consola->largura - comprimento_do_maior_elemento) / 2;
    int posicao_y = (tamanho_consola->altura - 6) / 2;

    definir_cor_texto(cor_texto);
    definir_cor_fundo(cor_fundo);

    // Escrever caixa do formulário
    desenhar_caixa(posicao_x, posicao_y, comprimento_do_maior_elemento + 4, 8);

    // Escrever o conteudo da caixa do prompt
    gotoxy(posicao_x + 2, posicao_y + 1);
    printf("%s", titulo);

    esperar_input_do_utilizador(alerta, posicao_x + 2, posicao_y + 6);

    limpar_formatacao();
}




/* ========================================================== */
/* =                      UTILITÁRIOS                       = */
/* ========================================================== */

char* string_maiusculas(char* string) {
    int i;
    for (i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
    return string;
}

/**
 * @brief Escreve a caixa que envolve o menu
 * @param menu
 */
static void escrever_caixa_do_menu(t_menu* menu) {
    int largura = menu->largura;
    int altura = menu->altura;
    int posicao_x = menu->posicao_x;
    int posicao_y = menu->posicao_y;

    desenhar_caixa(posicao_x, posicao_y, largura, altura);
}


/**
 * @brief Escreve o título do menu
 * @param menu
 */
static void escrever_titulo_do_menu(t_menu* menu) {
    desenhar_caixa(menu->posicao_x, menu->posicao_y - 2, menu->largura, 3);

    gotoxy(menu->posicao_x + (menu->largura - strlen(menu->titulo)) / 2, menu->posicao_y - 3);
    printf("%s", menu->titulo);

    gotoxy(menu->posicao_x + (menu->largura - strlen(menu->subtitulo)) / 2, menu->posicao_y - 1);
    printf("%s", menu->subtitulo);

    gotoxy(menu->posicao_x, menu->posicao_y);
    printf(ENTRONCAMENTO_ESQUERDA);
    gotoxy(menu->posicao_x + menu->largura - 1, menu->posicao_y);
    printf(ENTRONCAMENTO_DIREITA);
}


/**
 * @brief Escreve as opções do menu
 * @param menu
 */
static void escrever_opcoes_do_menu(t_menu* menu) {
    int posicao_x_opcoes = menu->posicao_x + (menu->largura - 2) / 2;
    int posicao_y_opcoes = menu->posicao_y + (menu->altura - 2) / 4;
    for (int i = 0; i < menu->numero_opcoes; i++) {
        gotoxy(posicao_x_opcoes - strlen(menu->opcoes[i]) / 2, posicao_y_opcoes + i * 2);
        printf("%s", menu->opcoes[i]);
    }
}

/**
 * @brief Escreve a caixa onde o utilizador vai escrever a sua opção
 * @param menu
 */
static void escrever_caixa_de_input(t_menu* menu) {
    gotoxy(menu->posicao_x, menu->posicao_y + menu->altura);

    desenhar_caixa(menu->posicao_x, menu->posicao_y + menu->altura, menu->largura, 3);

    gotoxy(menu->posicao_x + 2, menu->posicao_y + menu->altura + 1);
    printf("%s", menu->mensagem);

    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 4, menu->posicao_y + menu->altura + 1);
    printf("%s", LINHA_VERTICAL);
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 4, menu->posicao_y + menu->altura + 2);
    printf("%s", ENTRONCAMENTO_BAIXO);
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 4, menu->posicao_y + menu->altura);
    printf("%s", ENTRONCAMENTO_CIMA);

    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 5, menu->posicao_y + menu->altura + 1);
}


/**
 * @brief Devolve o tamanho do maior item do menu
 * @param menu
 * @return int
 */
static int obter_tamanho_do_maior_item_do_menu(t_menu* menu) {
    int tamanho_maior_item = 0;
    for (int i = 0; i < menu->numero_opcoes; i++) {
        if (strlen(menu->opcoes[i]) > tamanho_maior_item) {
            tamanho_maior_item = strlen(menu->opcoes[i]);
        }
    }
    return tamanho_maior_item;
}

static void limpar_caixa_de_input(t_menu* menu) {
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 5, menu->posicao_y + menu->altura + 1);
    for (int i = 0; i < menu->largura - strlen(menu->mensagem) - 6; i++) {
        printf(" ");
    }
}

static void escrever_mensagem_de_erro(t_menu* menu) {
    limpar_caixa_de_input(menu);

    // Posicionar o cursor depois da linha separadora da caixa de input e escrever a mensagem
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 6, menu->posicao_y + menu->altura + 1);
    printf("%s", menu->mensagem_erro);

    // Esperar que o utilizador carrergue numa tecla
    fflush(stdin);
    getchar();

    // Limpar a mensagem de erro e posicionar o cursor para a nova input
    limpar_caixa_de_input(menu);
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 6, menu->posicao_y + menu->altura + 1);
}


static void escrever_conteudo_do_prompt(int posicao_x, int posicao_y, char* mensagem, char* subtitulo, char* dica) {
    // Escrever mensagem no topo da caixa
    gotoxy(posicao_x + 2, posicao_y + 1);
    printf("%s", mensagem);

    // Escrever subtitulo
    gotoxy(posicao_x + 2, posicao_y + 3);
    printf("%s", subtitulo);

    // Escrever dica no centro da caixa
    gotoxy(posicao_x + 2, posicao_y + 5);
    printf("(%s):", dica);
}



static void ler_input_do_prompt(void* variavel_output, t_tipo_primitivo tipo_variavel_output) {
    switch (tipo_variavel_output) {
        case INT:
            scanf("%d", (int*) variavel_output);
            break;
        case FLOAT:
            scanf("%f", (float*) variavel_output);
            break;
        case CHAR:
            scanf(" %c", (char*) variavel_output);
            break;
        case STRING:
            scanf("%s", (char*) variavel_output);
            break;
    } // Sem caso default para que o compilador nos avise de casos não tratados
}


static void esperar_input_do_utilizador(char* mensagem, int x, int y) {
    gotoxy(x, y);
    printf("%s", mensagem);

    fflush(stdin);
    getchar();
}

static void escrever_linha(int posicao_x, int posicao_y, int tamanho, char caracter) {
    gotoxy(posicao_x, posicao_y);
    for (int i = 0; i < tamanho; i++) {
        printf("%c", caracter);
    }
}

void remover_espacos_a_mais(char* string) {
    int i = 0;
    int j = 0;
    while (string[i] != '\0') {
        if (string[i] != ' ' || (string[i] == ' ' && string[i + 1] != ' ')) {
            string[j] = string[i];
            j++;
        }
        i++;
    }
    string[j] = '\0';
}

/* ========================================================== */
/* =                      UTILITÁRIOS 2                     = */
/* ========================================================== */

static void ler_identificador(int* identificador, int pos_y) {
    do {
        gotoxy(6, pos_y);
        scanf("%d", identificador);
        if (*identificador < 0) {
            gotoxy(6, pos_y);
            printf("O identificador tem de ser positivo!");
            fflush(stdin);
            getchar();
            escrever_linha(6, pos_y, 50, ' ');
        }
    } while (*identificador < 0);
}

static void ler_string(char* nome, int tamanho, int pos_y) {
    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", nome);
        if (strlen(nome) > tamanho) {
            gotoxy(6, pos_y);
            printf("O campo deve ter menos de %d caracteres!", tamanho);
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (strlen(nome) > tamanho);
}

static void ler_escola(char* escola, int pos_y) {
    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", escola);
        if (strcmp(escola, "ESTG") != 0 && strcmp(escola, "ESECS") != 0 && strcmp(escola, "ESSLEI") != 0 && strcmp(escola, "ESAD") != 0 && strcmp(escola, "ESTM") != 0) {
            gotoxy(6, pos_y);
            printf("Insira uma escola válida! (ESTG, ESECS, ESSLEI, ESAD ou ESTM)");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 62, ' ');
        }
    } while (strcmp(escola, "ESTG") != 0 && strcmp(escola, "ESECS") != 0 && strcmp(escola, "ESSLEI") != 0 && strcmp(escola, "ESAD") != 0 && strcmp(escola, "ESTM") != 0);
}

static void ler_nif(int* nif, int pos_y) {
    do {
        gotoxy(6, pos_y);
        scanf("%d", nif);
        if (!(nif_valido(*nif))) {
            gotoxy(6, pos_y);
            printf("O NIF inserido não é válido!");
            fflush(stdin);
            getchar();
            escrever_linha(6, pos_y, 50, ' ');
        }
    } while (!(nif_valido(*nif)));
}

static void ler_email(char* email, int pos_y) {
    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", email);
        if (!(email_valido(email))) {
            gotoxy(6, pos_y);
            printf("O email inserido não é um email válido!");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (!(email_valido(email)));
}

static void ler_telefone(int* telefone, int pos_y) {
    char telefone_str[10];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", telefone_str);
        if (strlen(telefone_str) != 9) {
            gotoxy(6, pos_y);
            printf("O número de telemóvel tem de ter 9 dígitos!");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (strlen(telefone_str) != 9);

    *telefone = atoi(telefone_str);
}


static void ler_data(char* data, int pos_y) {
    char mensagem[100];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", data);
        if (!(data_valida(data, mensagem))) {
            gotoxy(6, pos_y);
            printf("%s", mensagem);
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (!(data_valida(data, mensagem)));
}


static void ler_hora(char* hora, char* data_inserida, int pos_y) {
    char mensagem[100];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", hora);
        if (!(hora_valida(hora, data_inserida, mensagem))) {
            gotoxy(6, pos_y);
            printf("%s", mensagem);
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (!(hora_valida(hora, data_inserida, mensagem)));
}

static void ler_tipo_de_atividade(char* tipo_de_atividade, int pos_y) {
    char* opcoes_validas[] = {"ACADEMICA", "LAZER", "CULTURA", "FORMACAO", "DESPORTO", "OUTRA"};
    char temp[50];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", tipo_de_atividade);
        remover_espacos_a_mais(tipo_de_atividade);
        strcpy(temp, tipo_de_atividade);
        strcpy(tipo_de_atividade, string_maiusculas(temp));
        if (tipo_de_atividade == NULL || !(contem_opcao(tipo_de_atividade, opcoes_validas, 6))) {
            gotoxy(6, pos_y);
            printf("Insira uma opção válida! (Academica, Lazer, Cultura, Formacao, Desporto ou Outra)");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 75, ' ');
        }
    } while (tipo_de_atividade == NULL || !(contem_opcao(tipo_de_atividade, opcoes_validas, 6)));
}

static void ler_associacao_estudantes(char* associacao_estudantes, int pos_y) {
    char* opcoes_validas[] = {"AE-ESTG", "AE-ESECS", "AE-ESSLEI", "AE-ESAD", "AE-ESTM"};
    char temp[10];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", associacao_estudantes);
        remover_espacos_a_mais(associacao_estudantes);
        strcpy(temp, associacao_estudantes);
        strcpy(associacao_estudantes, string_maiusculas(temp));
        if (associacao_estudantes == NULL || !(contem_opcao(associacao_estudantes, opcoes_validas, 5))) {
            gotoxy(6, pos_y);
            printf("Insira uma opção válida! (AE-ESTG, AE-ESECS, AE-ESSLEI, AE-ESAD ou AE-ESTM)");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 75, ' ');
        }
    } while (associacao_estudantes == NULL || !(contem_opcao(associacao_estudantes, opcoes_validas, 5)));
}

static void ler_float(float* valor, float minimo, float maximo, int pos_y) {
    char valor_str[10];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", valor_str);
        *valor = atof(valor_str);
        if (*valor < minimo || *valor > maximo) {
            gotoxy(6, pos_y);
            printf("Insira um valor entre %.2f e %.2f!", minimo, maximo);
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (*valor < minimo || *valor > maximo);
}

static void ler_int(int* valor, int minimo, int maximo, int pos_y) {
    char valor_str[10];

    do {
        fflush(stdin);
        gotoxy(6, pos_y);
        scanf("%[^\n]", valor_str);
        *valor = atoi(valor_str);
        if (*valor < minimo || *valor > maximo) {
            gotoxy(6, pos_y);
            printf("Insira um valor entre %d e %d!", minimo, maximo);
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (*valor < minimo || *valor > maximo);
}

static void ler_identificador_participante_inscricao(int* id, int pos_y, t_estado_programa* estado_programa) {
    int identificador_participante;
    int posicao_participante;

    do {
        ler_int(&identificador_participante, 1, 999999, pos_y);
        posicao_participante = procurar_participante_por_id(estado_programa->participantes, *estado_programa->numero_participantes_inseridos, identificador_participante);
        if (posicao_participante == -1) {
            gotoxy(6, pos_y);
            printf("Não existe nenhum participante com esse identificador!");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (posicao_participante == -1);

    id = &identificador_participante;
}

static void ler_identificador_atividade_inscricao(int* id, int pos_y, t_estado_programa* estado_programa) {
    int identificador_atividade;
    int posicao_atividade;

    do {
        ler_int(&identificador_atividade, 1, 999999, pos_y);
        posicao_atividade = procurar_atividade_por_id(estado_programa->atividades, *estado_programa->numero_atividadades_inseridas, identificador_atividade);
        if (posicao_atividade == -1) {
            gotoxy(6, pos_y);
            printf("Não existe nenhuma atividade com esse identificador!");
            fflush(stdin);
            getchar();
            gotoxy(6, pos_y);
            escrever_linha(6, pos_y, 60, ' ');
        }
    } while (posicao_atividade == -1);

    id= &identificador_atividade;
}

/// VALIDAÇÔES ESPECIAIS  ///

static int contem_opcao(char* opcao, char* opcoes[], int tamanho) {
    int i;

    for (i = 0; i < tamanho; i++) {
        if (strcmp(opcao, opcoes[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Esta função valida o nif introduzido pelo utilizador ao verificar o check digit da sequência de 9 dígitos
 *
 * Segundo a documentação, o check digit é calculado da seguinte forma:
 * 1. Multiplicar o 8º digito por 2, o 7º por 3, o 6º por 4, o 5º por 5, o 4º por 6, o 3º por 7, o 2º por 8 e o 1º por 9.
 * 2. Somar os resultados obtidos.
 * 3. Calcular o resto da divisão do número por 11.
 * 4. Se o resto for 0 ou 1 o check digit será 0.
 * 5. Se for outro qualquer algarismo X, o check digit será 11 - X.
 *
 * @param nif
 * @return
 */
static int nif_valido(int nif) {
    int check_digit = nif % 10;
    int soma = 0;
    int multiplicador = 2;
    int resto;

    nif /= 10;

    for (int i = 0; i < 8; i++) {
        soma += (nif % 10) * multiplicador;
        nif /= 10;
        multiplicador++;
    }

    resto = soma % 11;

    if (resto == 0 || resto == 1) {
        return check_digit == 0;
    }

    return check_digit == 11 - resto;
}

/**
 * @brief Esta função valida o email introduzido pelo utilizador
 *
 * Os emails não podem conter caracteres especiais, exceto o ponto (.) e o hífen (-).
 * O email tem que ter um @ e um ponto (.) após o @.
 * O email não pode começar nem terminar com um ponto (.) ou um hífen (-).
 * O email não pode ter mais de um ponto (.) após o @.
 *
 * @param email
 * @return
 */
static int email_valido(char* email) {
    int arroba = 0, ponto = 0, hifen = 0, tamanho = strlen(email);

    if (email[0] == '.' || email[0] == '-') {
        return 0;
    }

    if (email[tamanho - 1] == '.' || email[tamanho - 1] == '-') {
        return 0;
    }

    for (int i = 0; i < tamanho; i++) {
        if (email[i] == '@') {
            arroba++;
        } else if (email[i] == '.') {
            ponto++;
        } else if (email[i] == '-') {
            hifen++;
        } else if (!isalnum(email[i])) {
            return 0;
        }
    }

    return arroba == 1 && ponto == 1 && hifen == 0;
}

static int validar_dia_mes(int dia, int mes, char* mensagem) {
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dia < 1 || dia > 31) {
        strcpy(mensagem, "O dia tem de estar entre 1 e 31!");
        return 0;
    }

    if (dia > dias_mes[mes - 1]) {
        strcpy(mensagem, "O dia inserido não é válido para o mês inserido!");
        return 0;
    }

    if (mes < 1 || mes > 12) {
        strcpy(mensagem, "O mês tem de estar entre 1 e 12!");
        return 0;
    }

    return 1;
}

static int validar_ano(int ano, int ano_atual, char* mensagem) {
    if (ano < ano_atual) {
        strcpy(mensagem, "Impossivel inserir uma data anterior ao ano atual!");
        return 0;
    }

    return 1;
}

static int data_valida(char* data, char* mensagem_erro) {
    int dia = atoi(data), mes = atoi(data + 3), ano = atoi(data + 6);
    int dia_atual = obter_dia_atual(), mes_atual = obter_mes_atual(), ano_atual = obter_ano_atual();

    if (ano == ano_atual) {
        if (mes < mes_atual) {
            strcpy(mensagem_erro, "Não pode inserir uma data anterior à data atual!");
            return 0;
        }

        if (mes == mes_atual) {
            if (dia < dia_atual) {
                strcpy(mensagem_erro, "Não pode inserir uma data anterior à data atual!");
                return 0;
            }
        }
    }

    if (!validar_dia_mes(dia, mes, mensagem_erro)) return 0;
    if (!validar_ano(ano, ano_atual, mensagem_erro)) return 0;

    return 1;
}

static int validar_hora_minuto(int hora, int minuto, char* mensagem) {
    if (hora < 0 || hora > 23) {
        strcpy(mensagem, "A hora tem de estar entre 0 e 23!");
        return 0;
    }

    if (minuto < 0 || minuto > 59) {
        strcpy(mensagem, "O minuto tem de estar entre 0 e 59!");
        return 0;
    }

    return 1;
}

static int hora_valida(char* hora, char* data_inserida, char* mensagem_erro) {
    int hora_inserida = atoi(hora), minuto_inserido = atoi(hora + 3);
    int hora_atual = obter_hora_atual(), minuto_atual = obter_minuto_atual();
    int dia_inserido = atoi(data_inserida), mes_inserido = atoi(data_inserida + 3), ano_inserido = atoi(data_inserida + 6);
    int dia_atual = obter_dia_atual(), mes_atual = obter_mes_atual(), ano_atual = obter_ano_atual();

    if (ano_inserido == ano_atual && mes_inserido == mes_atual && dia_inserido == dia_atual) {
        if (hora_inserida < hora_atual) {
            strcpy(mensagem_erro, "Não pode inserir uma hora anterior à hora atual!");
            return 0;
        }

        if (hora_inserida == hora_atual) {
            if (minuto_inserido < minuto_atual) {
                strcpy(mensagem_erro, "Não pode inserir uma hora anterior à hora atual!");
                return 0;
            }
        }
    }

    if (!validar_hora_minuto(hora_inserida, minuto_inserido, mensagem_erro)) return 0;

    return 1;
}
//
// Created by João Matos on 14/12/2022.
//

#include "stdout.h"

#include <ctype.h>


/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

void escrever_caixa_do_menu(t_menu* menu);                  // Escreve a caixa que envolve o menu
void escrever_titulo_do_menu(t_menu* menu);                 // Escreve o título do menu
void escrever_opcoes_do_menu(t_menu* menu);                 // Escreve as opções do menu
void escrever_caixa_de_input(t_menu* menu);                 // Escreve a caixa que envolve o input da opcao do menu
int  obter_tamanho_do_maior_item_do_menu(t_menu* menu);     // Obtém o tamanho do maior item do menu
void limpar_caixa_de_input(t_menu* menu);                   // Limpa a caixa que envolve o input da opcao do menu
void escrever_mensagem_de_erro(t_menu* menu);               // Escreve a mensagem de erro de input no menu






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
        printf("Aconselha-se um terminal com pelo menos 80x24 caracteres.\n");
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


// TODO: Corrigir a funcao de input
// TODO: Corrgir mensagem de erro
// TODO: Adicionar funcao para rescrever por completo a caixa de input com a mensagem de erro
// TODO: Adicionar suporte para cores
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
/* =                      UTILITÁRIOS                       = */
/* ========================================================== */

/**
 * @brief Escreve a caixa que envolve o menu
 * @param menu
 */
void escrever_caixa_do_menu(t_menu* menu) {
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
void escrever_titulo_do_menu(t_menu* menu) {
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
void escrever_opcoes_do_menu(t_menu* menu) {
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
void escrever_caixa_de_input(t_menu* menu) {
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
int obter_tamanho_do_maior_item_do_menu(t_menu* menu) {
    int tamanho_maior_item = 0;
    for (int i = 0; i < menu->numero_opcoes; i++) {
        if (strlen(menu->opcoes[i]) > tamanho_maior_item) {
            tamanho_maior_item = strlen(menu->opcoes[i]);
        }
    }
    return tamanho_maior_item;
}

void limpar_caixa_de_input(t_menu* menu) {
    gotoxy(menu->posicao_x + strlen(menu->mensagem) + 5, menu->posicao_y + menu->altura + 1);
    for (int i = 0; i < menu->largura - strlen(menu->mensagem) - 6; i++) {
        printf(" ");
    }
}

void escrever_mensagem_de_erro(t_menu* menu) {
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
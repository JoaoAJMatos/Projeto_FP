//
// Created by João Matos on 14/12/2022.
//

#ifndef PROJETO1_STDOUT_H
#define PROJETO1_STDOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "comum.h"
#include "app.h"
#include "formatacao_texto.h"
#include "data_hora.h"

#if defined(_WIN32) || defined(_WIN64)          // Windows
#include <windows.h>                            // Para interagir com a API do Windows
#else                                           // Posix
#include <sys/ioctl.h>                          // Para obter o tamanho da janela
#include <curses.h>
#endif                                          // _WIN32

#define ESCAPE_CODE_LIMPAR_CONSOLA \
                      "\033[H\033[J"            // ANSI escape code para limpar a consola para evitar system calls

#define limpar_ecra() \
    printf(ESCAPE_CODE_LIMPAR_CONSOLA)          // Definir uma macro para limpar o ecrã

#define limpar_formatacao() \
    printf(TEXTO_RESET)                         // Definir uma macro para limpar a formatação

#define definir_cor_texto(cor) \
    printf("%s", cor)                           // Definir uma macro para definir a cor do texto

#define definir_cor_fundo(cor) \
    definir_cor_texto(cor)

#define ALTURA_MINIMA_RECOMENDADA 30
#define LARGURA_MINIMA_RECOMENDADA 90


#define CANTO_SUPERIOR_ESQUERDO "╔"
#define CANTO_SUPERIOR_DIREITO "╗"
#define CANTO_INFERIOR_ESQUERDO "╚"
#define CANTO_INFERIOR_DIREITO "╝"
#define LINHA_HORIZONTAL "═"
#define LINHA_VERTICAL "║"
#define INTERSECCAO "╬"
#define ENTRONCAMENTO_ESQUERDA "╠"
#define ENTRONCAMENTO_DIREITA "╣"
#define ENTRONCAMENTO_CIMA "╦"
#define ENTRONCAMENTO_BAIXO "╩"


/* =========== ESTRUTURAS =========== */

typedef struct {                                // Estrutura para guardar as dimensões da janela
    int largura;                                // Largura da janela
    int altura;                                 // Altura da janela
} t_tamanho_consola;

typedef t_tamanho_consola t_resolucao;


/**
 * Estrutura para guardar as informações de um menu
 * Os menus são utilizados para apresentar opções ao utilizador de forma organizada
 * e com uma interface amigável, com diversas opções de personalização.
 */
typedef struct {               // Estrutura para guardar as informações de um menu
    char*  titulo;             // Título do menu a ser mostrado
    char*  subtitulo;          // Subtítulo do menu a ser mostrado (em baixo do menu)
    char** opcoes;             // Opções do menu
    int    numero_opcoes;      // Número de opções do menu
    char*  mensagem;           // Mensagem a ser mostrada no menu (mensagem de informação)
    char*  mensagem_erro;      // Mensagem de erro a ser mostrada no menu (quando o utilizador introduz uma opção inválida)
    int    largura;            // Largura do menu
    int    altura;             // Altura do menu
    int    posicao_x;          // Posição X do menu
    int    posicao_y;          // Posição Y do menu
    int    auto_ajustar;       // Indica se o menu deve ser auto-ajustado ou não (se as dimensões não forem suficientes, o menu é ajustado automaticamente)
    char*  cor_letras;         // Cor das letras do menu
    char*  cor_fundo;          // Cor do fundo do menu
    int    pintar_consola;     // Indica se a consola deve ser pintada com a mesma cor de fundo do menu
} t_menu;


/**
 * Estrutura para guardar as informações de um formulário de input
 * Este modelo pode ser utilizado para criar formulários de input com um número finito de campos,
 * facilitando a introdução de dados por parte do utilizador.
 *
 * Os formulários de input apresentam o contexto da input (título e subtítulo) e os campos a introduzir.
 * Os campos são apresentados com um rótulo e um espaço para o utilizador introduzir o valor.
 *
 * Os formulários de input podem ser auto-ajustados, fazendo com que este se apresente no meio da consola
 * e com as dimensões necessárias para apresentar todos os campos.
 */
typedef struct {
    char*  titulo;                               // Título do formulário de input
    char*  subtitulo;                            // Subtítulo do formulário de input (pode mudar consoante o campo a inserir)
    char** rotulos;                              // Rótulos dos campos a inserir
    int    numero_campos;                        // Número de campos a inserir
    void*  estrutura_output;                     // Estrutura para guardar os dados inseridos pelo utilizador
    t_tipo_estrutura tipo_estrutura_output;      // Tipo da estrutura para guardar os dados inseridos pelo utilizador
} t_formulario_input;


/**
 * Esta função com o atributo constructor é executada sempre que o programa é iniciado, antes da função main.
 * Verifica o tamanho da consola e ajusta o tamanho do buffer de output para que o programa não tenha problemas.
 *
 * @return int
 */
__attribute__((constructor)) int inicializar_stdout();



/// MENU ///
t_menu* criar_menu(char*, char*, char**, int, char*, char*, int, int, int, char*, char*, int);
void desenhar_menu(t_menu*);                                    // Desenhar um menu
int  ler_opcao_menu(t_menu*);                                   // Ler uma opção do menu



/// FORMULÁRIOS ///
t_formulario_input* criar_formulario_input(char*, char*, char**, int, void*, t_tipo_estrutura);
void desenhar_formulario_input(t_formulario_input*);            // Desenhar um formulário de input
void* ler_formulario_input(t_formulario_input*, t_estado_programa*);                 // Lê os campos do formulário de input e guarda os dados na estrutura


/// AÇÕES RÁPIDAS ///
void prompt(char*, char*, char*, void*, t_tipo_primitivo, char*, char*);   // Mostra um prompt pre definido para o utilizador introduzir dados rapidamente
void alerta(char*, char*, char*, char*);                                   // Mostra um alerta pre definido ao utilizador



/// UTILITÁRIOS ///
t_tamanho_consola* obter_tamanho_consola();                     // Obter o tamanho da consola
void desenhar_caixa(int, int, int, int);                        // Desenhar uma caixa com as dimensões especificadas (posição X, posição Y, largura, altura)
void gotoxy(int, int);                                          // Posicionar o cursor na consola
void imprimir_centrado(char*, t_tamanho_consola*);              // Imprimir um texto centrado na consola
void pintar_consola(char*);                                     // Pintar a consola com uma cor
void remover_espacos_a_mais(char*);                             // Remover os espaços a mais no inicio, fim e meio de uma string
char* string_maiusculas(char*);                                  // Converter uma string para maiúsculas


#endif //PROJETO1_STDOUT_H

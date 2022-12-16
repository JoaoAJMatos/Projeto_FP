//
// Created by João Matos on 13/12/2022.
//

#ifndef PROJETO_FS_H
#define PROJETO_FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../comum.h"                           // Para a definição da constante __inline__

#if defined(_WIN32) || defined(_WIN34)          // Windows
#define SEPARADOR_DIRETORIO "\\"                // Separador de diretório para Windows

#include <direct.h>                             // Para usar o mkdir
#include <io.h>                                 // Para usar a função _access()
#define F_OK 0                                  // Arquivo existe
#define access _access                          // Para verificar se um arquivo existe

#else                                           // Posix
#define SEPARADOR_DIRETORIO "/"                 // Separador de diretório para sistemas POSIX

#include <sys/stat.h>

#include <sys/types.h>
#include <unistd.h>

#endif // defined(_WIN32) || defined(_WIN34)

/// MODOS DE MANIPULAÇÃO DE FICHEIROS ///
#define LEITURA "r"                             // Modo de leitura                   (o ficheiro deve existir)
#define ESCRITA "w"                             // Modo de escrita                   (cria um ficheiro vazio para escrita)
#define APPEND "a"                              // Modo de append                    (adiciona ao fim do ficheiro. cria-o se não existir)
#define LEITURA_BINARIA "rb"                    // Modo de leitura binária           (o ficheiro deve existir)
#define ESCRITA_BINARIA "wb"                    // Modo de escrita binária           (cria um ficheiro vazio para escrita)
#define APPEND_BINARIA "ab"                     // Modo de append binário            (adiciona ao fim do ficheiro. cria-o se não existir)
#define LEITURA_ESCRITA "r+"                    // Modo de leitura e escrita         (o ficheiro deve existir)
#define LEITURA_ESCRITA_BINARIA "rb+"           // Modo de leitura e escrita binária (o ficheiro deve existir)
#define ESCRITA_LEITURA "w+"                    // Modo de escrita e leitura         (cria um ficheiro vazio para leitura e escrita)
#define ESCRITA_LEITURA_BINARIA "wb+"           // Modo de escrita e leitura binária (cria um ficheiro vazio para leitura e escrita)
#define APPEND_LEITURA "a+"                     // Modo de append e leitura          (abre o ficheiro para leitura e append)

#define MAXIMO_NUMERO_ELEMENTOS_CAMINHO 200     // Número máximo de elementos que um caminho pode ter (subdiretórios)
#define TAMANHO_MAXIMO_CAMINHO \
(MAXIMO_NUMERO_ELEMENTOS_CAMINHO * 255)         // Tamanho máximo de um caminho

typedef long long t_tamanho_ficheiro;


typedef enum {                                  // CODIGOS DE ERRO PARA A CRIAÇÃO DE UM CAMINHO
    CAMINHO_OK = 0,                             // O caminho foi criado com sucesso
    LIMITE_ELEMENTOS_CAMINHO,                   // O caminho tem mais elementos do que o permitido
    CAMINHO_NAO_EXISTE,                         // O caminho não existe
} t_erro_fs;


/* =========== ESTRUTURAS =========== */

typedef struct {
    char*  elementos[MAXIMO_NUMERO_ELEMENTOS_CAMINHO]; // Elementos do caminho
    char*  string_caminho;                             // String com o caminho
    char*  caminho_absoluto;                           // Caminho absoluto (caminho desde a raiz do disco)
    int    erro;                                       // Indica se ocorreu um erro ao criar o caminho
} t_caminho;



/* ======= UTILITÁRIOS PARA OS CAMINHOS DO FS ======= */

t_caminho* caminho_criar(const char*, ...);               // Cria um caminho a partir de uma sequencia de elementos
t_caminho* caminho_criar_a_partir_de_string(const char*); // Cria um caminho a partir de uma string
void caminho_destruir(t_caminho*);                        // Liberta a memória alocada para o caminho
__inline__ void caminho_mostrar(t_caminho*);              // Mostra o caminho na stdout
__inline__ void caminho_mostrar_absoluto(t_caminho*);     // Mostra o caminho absoluto na stdout
char* caminho_relativo_para_absoluto(const char*);        // Retorna o caminho absoluto para um determinado caminho
int   caminho_obter_erro(t_caminho*, int);                // Retorna o código de erro do caminho e mostra a mensagem de erro se a flag for 1


/* ======= I/O FICHEIROS ======= */

// Algumas das funções são antecedidas pelo atributo "unused" para evitar avisos de compilação
// quando o código é compilado com o GCC em modo de otimização (-O2 ou superior)

__attribute__((unused)) FILE* abrir_ficheiro_leitura(const char*);            // Abre um ficheiro para leitura
__attribute__((unused)) FILE* abrir_ficheiro_leitura_binaria(const char*);    // Abre um ficheiro para leitura binária
__attribute__((unused)) FILE* abrir_ficheiro_escrita(const char*);            // Abre um ficheiro para escrita
__attribute__((unused)) FILE* abrir_ficheiro_escrita_binaria(const char*);    // Abre um ficheiro para escrita em modo binário
__attribute__((unused)) FILE* abrir_ficheiro_append(const char*);             // Abre um ficheiro para append
__attribute__((unused)) FILE* abrir_ficheiro_append_binario(const char*);     // Abre um ficheiro para append em modo binário
FILE* abrir_ficheiro(const char*, char*);                                     // Abre um ficheiro com o modo especificado
int   fechar_ficheiro(FILE*);                                                 // Fecha um ficheiro



/* ======= UTILITÁRIOS FS ======= */
int   ficheiro_existe(const char*);          // Retorna 1 se o ficheiro existir, 0 caso contrário
int   caminho_existe(const char*);           // Retorna 1 se o caminho existir, 0 caso contrário
int   criar_arvore_diretorios(const char*);  // Cria uma árvore de diretórios
char* diretorio_atual();                     // Retorna o diretório atual
char* extenssao_ficheiro(const char*);       // Retorna a extensão de um ficheiro
int   numero_linhas_ficheiro(FILE*);         // Retorna o número de linhas de um ficheiro
t_tamanho_ficheiro tamanho_ficheiro(const char*);  // Retorna o tamanho de um ficheiro


#endif //PROJETO_FS_H

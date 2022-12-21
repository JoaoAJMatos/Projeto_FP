//
// Created by João Matos on 13/12/2022.
//

#ifndef PROJETO_FS_H
#define PROJETO_FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "comum.h"                           // Para a definição da constante __inline__

#if defined(_WIN32) || defined(_WIN34)          // Windows
#define SEPARADOR_DIRETORIO "\\"                // Separador de diretório para Windows

#include <windows.h>
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


/* ======= I/O FICHEIROS ======= */

// Algumas das funções são antecedidas pelo atributo "unused" para evitar avisos de compilação
// quando o código é compilado com o GCC em modo de otimização (-O2 ou superior)

FILE* abrir_ficheiro(const char*, char*);                                     // Abre um ficheiro com o modo especificado
int   fechar_ficheiro(FILE*);                                                 // Fecha um ficheiro



/* ======= UTILITÁRIOS FS ======= */
int   ficheiro_existe(const char*);          // Retorna 1 se o ficheiro existir, 0 caso contrário
int   caminho_existe(const char*);           // Retorna 1 se o caminho existir, 0 caso contrário
int   criar_arvore_diretorios(const char*);  // Cria uma árvore de diretórios
char* caminho_sem_nome_ficheiro(const char*); // Retorna o caminho sem o nome do ficheiro
char* diretorio_atual();                     // Retorna o diretório atual
char* extenssao_ficheiro(const char*);       // Retorna a extensão de um ficheiro
int   numero_linhas_ficheiro(FILE*);         // Retorna o número de linhas de um ficheiro
t_tamanho_ficheiro tamanho_ficheiro(const char*);  // Retorna o tamanho de um ficheiro
char* caminho_relativo_para_absoluto(const char*); // Retorna o caminho absoluto de um caminho relativo


#endif //PROJETO_FS_H

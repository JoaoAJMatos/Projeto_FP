//
// Created by João Matos on 13/12/2022.
//

#include "../include/fs.h"

/* ========================================================== */
/* =                     I/O FICHEIROS                      = */
/* ========================================================== */

FILE* abrir_ficheiro(const char* caminho, char* modo) {
    FILE* ficheiro = fopen(caminho, modo);
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro %s\n", caminho);
        return NULL;
    }
    return ficheiro;
}

int fechar_ficheiro(FILE* ficheiro) {
    if (fclose(ficheiro) != 0) {
        printf("Erro ao fechar o ficheiro\n");
        return ERRO;
    }
    return OK;
}


/* ========================================================== */
/* =                          FS                            = */
/* ========================================================== */

/**
 * @brief Verifica se um determinado ficheiro existe
 * @param caminho
 * @return int
 */
int ficheiro_existe(const char* caminho) {
    if (access(caminho, F_OK) != -1) {
        return 1;
    }

    return 0;
}

/**
 * @brief Indica se um determinado caminho existe no sistema de ficheiros.
 * @param caminho
 * @return int
 */
int caminho_existe(const char* caminho) {
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
}


/**
 * @brief Cria uma árvore de diretórios a partir de um caminho.
 * @param caminho
 * @return int
 */
int criar_arvore_diretorios(const char* caminho) {
    char *p = caminho_relativo_para_absoluto(caminho);;

    while ((p = strchr(p + 1, '/')) != NULL) {
        *p = '\0';
        if (mkdir(caminho, S_IRWXU) == -1) {
            return ERRO;
        }
        *p = '/';
    }

    return OK;
}


/**
 * @brief Devolve a localização do diretório atual.
 * @return char*
 */
char* diretorio_atual() {
    char* caminho = malloc(TAMANHO_MAXIMO_CAMINHO);
    getcwd(caminho, TAMANHO_MAXIMO_CAMINHO);
    return caminho;
}


/**
 * @brief Devolve a extensão de um ficheiro.
 * @param caminho
 * @return
 */
char* extensao_ficheiro(const char* caminho) {
    char* extensao = strrchr(caminho, '.');
    if (extensao == NULL) {
        return NULL;
    }
    return extensao;
}

/**
 * @brief Devolve o tamanho de um ficheiro em bytes.
 * @param caminho
 * @return
 */
t_tamanho_ficheiro tamanho_ficheiro(const char* caminho) {
    struct stat st;
    stat(caminho, &st);
    return st.st_size;
}

char* caminho_relativo_para_absoluto(const char* caminho) {
    char* caminho_absoluto = malloc(TAMANHO_MAXIMO_CAMINHO);
    realpath(caminho, caminho_absoluto);
    return caminho_absoluto;
}
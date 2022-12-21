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
#ifdef _WIN32
    DWORD ftyp = GetFileAttributesA(caminho);
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
#else
    struct stat buffer;
    return (stat(caminho, &buffer) == 0);
#endif
}


/**
 * @brief Cria uma árvore de diretórios a partir de um caminho.
 * @param caminho
 * @return int
 */
int criar_arvore_diretorios(const char* caminho) {
    char *p = caminho_relativo_para_absoluto(caminho);
    if (caminho_existe(p)) {
        free(p);
        return OK;
    }

    while ((p = strchr(p + 1, '/')) != NULL) {
        *p = '\0';
        #ifdef _WIN32
            if (mkdir(caminho) == ERRO) return ERRO;
        #else
            if (mkdir(caminho, S_IRWXU) == ERRO) return ERRO;
        #endif
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
#ifdef _WIN32
    HANDLE hFile = CreateFile(caminho, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    LARGE_INTEGER size;
    GetFileSizeEx(hFile, &size);
    CloseHandle(hFile);
    return size.QuadPart;
#else
    struct stat st;
    stat(caminho, &st);
    return st.st_size;
#endif
}

char* caminho_relativo_para_absoluto(const char* caminho) {
    
#ifdef _WIN32
    char* caminho_absoluto = malloc(TAMANHO_MAXIMO_CAMINHO);
    _fullpath(caminho_absoluto, caminho, TAMANHO_MAXIMO_CAMINHO);
    return caminho_absoluto;
#else
    char* caminho_absoluto = malloc(TAMANHO_MAXIMO_CAMINHO);
    realpath(caminho, caminho_absoluto);
    return caminho_absoluto;
#endif
}

char* caminho_sem_nome_ficheiro(const char* caminho) {
    char* caminho_sem_nome = malloc(TAMANHO_MAXIMO_CAMINHO);
    strcpy(caminho_sem_nome, caminho);
    char* nome_ficheiro = strrchr(caminho_sem_nome, '/');
    if (nome_ficheiro != NULL) {
        *nome_ficheiro = '\0';
    }
    return caminho_sem_nome;
}
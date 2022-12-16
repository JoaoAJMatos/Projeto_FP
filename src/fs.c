//
// Created by João Matos on 13/12/2022.
//

#include "../include/fs.h"

/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */


void construir_caminho_final(va_list* argumentos, char* caminho_final);
int  numero_argumentos_passados(va_list* argumentos);




/* ========================================================== */
/* =                        CAMINHO                         = */
/* ========================================================== */

/**
 * @brief Cria um novo caminho a partir de uma sequência de strings.
 * @param raiz
 * @param ...
 * @return t_caminho*
 *
 * @warning O último argumento deve ser NULL/0 para indicar o fim da lista, de modo a evitar
 *          erros de segmentação quando tentamos aceder ao próximo argumento.
 *
 * @example caminho_criar("src", "main.c", NULL)
 */
t_caminho* caminho_criar(const char* raiz, ...) {
    va_list argumentos;                                  // Lista de argumentos passados para a função
    t_caminho* caminho;                                  // Ponteiro para o caminho
    char* caminho_final;                                 // Caminho final

    caminho = malloc(sizeof(t_caminho));            // Alocar memória para a estrutura do caminho
    caminho_final = malloc(TAMANHO_MAXIMO_CAMINHO); // Alocar memória para o caminho final
    strcat(caminho_final, raiz);                         // Adicionar a raiz ao caminho final

    va_start(argumentos, raiz);
    if (numero_argumentos_passados(&argumentos) > MAXIMO_NUMERO_ELEMENTOS_CAMINHO) {
        caminho->erro = LIMITE_ELEMENTOS_CAMINHO;
        return caminho;
    }
    construir_caminho_final(&argumentos, caminho_final);
    va_end(argumentos);

    caminho->caminho_absoluto = caminho_relativo_para_absoluto(caminho_final);
    caminho->string_caminho = caminho_final;
    if (!caminho_existe(caminho->caminho_absoluto)) {
        caminho->erro = CAMINHO_NAO_EXISTE;
        return caminho;
    }

    caminho->erro = CAMINHO_OK;
    return caminho;
}


/**
 * @brief Cria um novo caminho a partir de uma string.
 * @param string_caminho
 * @return t_caminho*
 */
t_caminho* criar_caminho_a_partir_de_string(const char* string_caminho) {
    t_caminho* caminho = malloc(sizeof(t_caminho));
    caminho->string_caminho = malloc(TAMANHO_MAXIMO_CAMINHO);
    strcpy(caminho->string_caminho, string_caminho);
    caminho->caminho_absoluto = caminho_relativo_para_absoluto(string_caminho);

    if (!caminho_existe(caminho->caminho_absoluto)) {
        caminho->erro = CAMINHO_NAO_EXISTE;
        return caminho;
    }

    caminho->erro = CAMINHO_OK;
    return caminho;
}


/**
 * @brief Liberta a memória alocada pelo caminho.
 * @param caminho
 */
void caminho_destruir(t_caminho* caminho) {
    free(caminho->string_caminho);
    free(caminho->caminho_absoluto);
    free(caminho);
}


__inline__ void caminho_mostrar(t_caminho* caminho) {
    printf("%s", caminho->string_caminho);
}

__inline__ void caminho_mostrar_absoluto(t_caminho* caminho) {
    printf("%s", caminho->caminho_absoluto);
}


/**
 * @brief Devolve o último erro e mostra a mensagem de erro correspondente se a flag verbose estiver ativa
 * @param caminho
 * @return int
 */
int caminho_obter_erro(t_caminho* caminho, int verbose) {
    if (verbose) {
        switch (caminho->erro) {
            case CAMINHO_OK:
                printf("Caminho OK\n");
                break;
            case CAMINHO_NAO_EXISTE:
                printf("Caminho não existe\n");
                break;
            case LIMITE_ELEMENTOS_CAMINHO:
                printf("Limite de elementos do caminho excedido\n");
                break;
            default:
                printf("Erro desconhecido\n");
                break;
        }
    }
    return caminho->erro;
}


/**
 * @brief Converte um caminho relativo para um caminho absoluto
 * @param caminho_relativo
 * @return char*
 */
char* caminho_relativo_para_absoluto(const char* caminho_relativo) {
    char* caminho_absoluto = malloc(TAMANHO_MAXIMO_CAMINHO);
    realpath(caminho_relativo, caminho_absoluto);
    return caminho_absoluto;
}



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



/* ========================================================== */
/* =                      UTILITÁRIOS                       = */
/* ========================================================== */

/**
 * @breif Produz o caminho final através dos argumentos passados para a construção do caminho.
 * @param argumentos
 * @param caminho_final
 */
void construir_caminho_final(va_list* argumentos, char* caminho_final) {
    const char* elemento_caminho;   // Elemento do caminho

    while ((elemento_caminho = va_arg(*argumentos, const char*)) != NULL) {  // Enquanto houver elementos na lista de argumentos
        strcat(caminho_final, SEPARADOR_DIRETORIO);                          // Adicionar o separador de diretório ao caminho final
        strcat(caminho_final, elemento_caminho);                             // Adicionar o elemento ao caminho final
    }
}

/**
 * @brief Devolve o numero de elementos contidos numa lista de argumentos.
 * @param argumentos
 * @return int
 */
int numero_argumentos_passados(va_list* argumentos) {
    int num_args = 0;
    va_list args_copy;
    va_copy(args_copy, *argumentos);
    while (va_arg(args_copy, const char*) != NULL) {
        num_args++;
    }
    va_end(args_copy);
    return num_args;
}
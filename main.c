/**
 * @file main.c
 * @author Joao Matos (github.com/JoaoAJMatos) & Rúben Lisboa (github.com/Lisboa14)
 * @version 1.0
 * @date 18-12-2022
 *
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A keyword "inline" parecia causar problemas com o GCC na minha
// máquina arm64, então eu apenas redefino a constante aqui para um dos atributos do GCC
#if defined(__aarch64__)
#define inline_ __attribute__((always_inline))
#else
#define inline_ inline
#endif // defined(__aarch64__)

#if defined(_WIN32) || defined(_WIN34)          // Windows
#include <io.h>                                 // Para usar a função _access()
#define F_OK 0                                  // Arquivo existe
#define access _access                          // Para verificar se um arquivo existe
#else
#include <unistd.h>                             // Para usar a função access()
#endif // defined(_WIN32) || defined(_WIN34)

/// MACROS ///
#define ESCAPE_CODE_LIMPAR_CONSOLA "\033[H\033[J"            // ANSI escape code para limpar a consola para evitar system calls
#define limpar_ecra() printf(ESCAPE_CODE_LIMPAR_CONSOLA)     // Definir uma macro para limpar o ecrã
#define TESTE 1                                              // Flag de teste

// Ficheiros //
#define FICHEIRO_SAVE "save.dat"         // Nome do ficheiro de save
#define LEITURA_BINARIA "rb"             // Modo de leitura binária (o ficheiro deve existir)
#define ESCRITA_BINARIA "wb"             // Modo de escrita binária (cria um ficheiro vazio para escrita)

/// CONSTANTES DA APLICAÇÃO ///
#define NUMERO_MAXIMO_DE_PARTICIPANTES 5000
#define NUMERO_MAXIMO_DE_ATIVIDAES  200
#define NUMERO_MAXIMO_DE_INSCRICOES 10000
#define TAMANHO_MAXIMO_NOME 50
#define TAMANHO_MAXIMO_EMAIL TAMANHO_MAXIMO_NOME
#define TAMANHO_MAXIMO_ESCOLA 10
#define TAMANHO_MAXIMO_DESIGNACAO 100
#define TAMANHO_MAXIMO_LOCAL TAMANHO_MAXIMO_NOME
#define TAMANHO_MAXIMO_TIPO_ATIVIDADE 20
#define TAMANHO_MAXIMO_AE 10
#define TAMANHO_DATA 11
#define TAMANHO_HORA 6


/// TIPOS E ESTRUTURAS ///
typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef enum {
    OK = 0,
    ERRO = -1
} codigo_erro_t;

typedef enum {
    MENU_PARTICIPANTES = 1,
    MENU_ATIVIDADES,
    MENU_INSCRICAO,
    ESTATISTICAS,
    SALVAR,
    SAIR
} opcao_menu_principal_t;


/* ========================================================== */
/* =                      ESTRUTURAS                        = */
/* ========================================================== */

typedef struct {
    int  identificador;                 // Identificador único do participante
    char nome[TAMANHO_MAXIMO_NOME];     // Nome do participante
    char escola[TAMANHO_MAXIMO_ESCOLA]; // Escola do participante
    int  nif;                           // NIF do participante
    char email[TAMANHO_MAXIMO_EMAIL];   // Email do participante
    int  telefone;                      // Telefone do participante
} participante_t;

typedef struct {
    int   identificador;                            // Identificador único da atividade
    char  designacao[TAMANHO_MAXIMO_DESIGNACAO];    // Designação da atividade (ex: "Maratona Solidária")
    char  data[TAMANHO_DATA];                       // Data da atividade (ex: "30/04/2023")
    char  hora[TAMANHO_HORA];                       // Hora da atividade (ex: "09:00")
    char  local[TAMANHO_MAXIMO_LOCAL];              // Local da atividade (ex: "Praça do Município")
    char  tipo[TAMANHO_MAXIMO_TIPO_ATIVIDADE];      // Tipo da atividade (ex: "Desporto")
    char  associacao_estudantes[TAMANHO_MAXIMO_AE]; // Associação de estudantes responsável pela atividade (ex: "AE-ESTG")
    float valor;
} atividade_t;

typedef struct {
    int   identificador;        // Identificador único da inscrição
    int   id_participante;      // Identificador do participante
    int   id_atividade;         // Identificador da atividade
    float valor_pago;           // Valor pago pelo participante
    char  data[TAMANHO_DATA];   // Data da inscrição
    char  hora[TAMANHO_HORA];   // Hora da inscrição
} inscricao_t;

/**
 * Este struct representa o estado atual do programa.
 * Nele estão contidas todas as informações necessárias para o funcionamento do programa.
 *
 * As funções que orquestram a chamada de funções do programa devem receber este struct como parâmetro, de modo
 * a que possam compartilhar informação entre si.
 *
 * Por exemplo, a função main() cria uma instância deste struct e passa-a como argumento para a função menu_participantes().
 * Assim evita-se a necessidade de passar ponteiros para cada uma das estruturas de dados definidas na função main.
 * Desta forma, o estado do programa é acessível a todas as funções que o precisam de manipular ou consultar.
 */
typedef struct {
    participante_t** participantes;   // Vetor de participantes
    atividade_t**    atividades;      // Vetor de atividades
    inscricao_t**    inscricoes;      // Vetor de inscrições
    int*           numero_de_participantes;                         // Contadores
    int*           numero_de_atividades;
    int*           numero_de_inscricoes;
    bool_t*        dados_guardados;                                 // Flag para indicar se os dados foram guardados
} estado_programa_t;

/* ========================================================== */



/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

// TODO: Criar implementações para estas funções
participante_t* criar_participante(char*, char*, int, char*, int, estado_programa_t*);
atividade_t* criar_atividade(char*, char*, char*, char*, char*, char*, float, estado_programa_t*);
inscricao_t* criar_inscricao(int, int, estado_programa_t*);
estado_programa_t* criar_estado_programa(participante_t**, atividade_t**, inscricao_t**, int*, int*, int*, bool_t*);

void libertar_participante(participante_t*);
void libertar_atividade(atividade_t*);
void libertar_inscricao(inscricao_t*);
void libertar_estado_programa(estado_programa_t*);

inline_ void mostrar_participante(participante_t*);
inline_ void mostrar_atividade(atividade_t*);
inline_ void mostrar_inscricao(inscricao_t*);
inline_ void mostrar_estado_programa(estado_programa_t*);

inline_ void mostrar_participantes(estado_programa_t*);
inline_ void mostrar_atividades(estado_programa_t*);
inline_ void mostrar_inscricoes(estado_programa_t*);

/* ========================================================== */

void guardar_participantes(estado_programa_t*, FILE*);
void guardar_atividades(estado_programa_t*, FILE*);
void guardar_inscricoes(estado_programa_t*, FILE*);
codigo_erro_t guardar_estado_programa(const char*, estado_programa_t*);

void carregar_participantes(estado_programa_t*, FILE*);
void carregar_atividades(estado_programa_t*, FILE*);
void carregar_inscricoes(estado_programa_t*, FILE*);
estado_programa_t* carregar_estado_programa(const char*);

codigo_erro_t carregar_dados(const char*, estado_programa_t*);
codigo_erro_t guardar_dados(const char*, estado_programa_t*);

/* ========================================================== */

void  ler_string(const char*, char*, int);
int   ler_inteiro_intervalo(const char*, int, int);
float ler_float_intervalo(const char*, float, float);

participante_t* ler_participante(estado_programa_t*);
atividade_t* ler_atividade(estado_programa_t*);
inscricao_t* ler_inscricao(estado_programa_t*);

codigo_erro_t inserir_participante(estado_programa_t*);
codigo_erro_t inserir_atividade(estado_programa_t*);
codigo_erro_t inserir_inscricao(estado_programa_t*);

/* ========================================================== */

void inicializar_vetores(estado_programa_t*);
bool_t ficheiro_existe(const char*);

/* ========================================================== */

void listar_participantes(estado_programa_t*);
void listar_atividades(estado_programa_t*);
void listar_inscricoes(estado_programa_t*);

/* ========================================================== */

int procurar_atividade_por_id(int, estado_programa_t*);
int procurar_participante_por_id(int, estado_programa_t*);
int procurar_inscricao_por_id(int, estado_programa_t*);

/* ========================================================== */

char* obter_data_atual();
char* obter_hora_atual();
char* obter_hora_atual_com_segundos();
int timestamp();

/* ========================================================== */

opcao_menu_principal_t menu_principal();



/* ========================================================== */
/* =                         MAIN                           = */
/* ========================================================== */

int main() {
    // DECLARAÇÃO DE VARIÁVEIS //
    int numero_de_participantes = 0;
    int numero_de_atividades = 0;
    int numero_de_inscricoes = 0;
    char confirmacao_saida;

    participante_t* participantes[NUMERO_MAXIMO_DE_PARTICIPANTES];
    atividade_t*    atividades[NUMERO_MAXIMO_DE_ATIVIDAES];
    inscricao_t*    inscricoes[NUMERO_MAXIMO_DE_INSCRICOES];
    bool_t         dados_guardados = FALSE;

    /// ESTADO DO PROGRAMA  ///
    // Criação do estado do programa
    //
    // Este estado é partilhado entre funções, de modo que possam aceder e consultar o estado atual
    // do programa.
    estado_programa_t* estado_programa = criar_estado_programa(participantes, atividades, inscricoes, &numero_de_participantes, &numero_de_atividades, &numero_de_inscricoes, &dados_guardados);

    if (carregar_dados(FICHEIRO_SAVE, estado_programa) == ERRO) {
        printf("Erro ao carregar dados do ficheiro \"%s\". A aplicação irá continuar sem dados pré-existentes.\n", FICHEIRO_SAVE);
    }

    /// LOOP PRINCIPAL ///
    do {
        limpar_ecra();
    } while(confirmacao_saida != 'S');

    return OK;
}

/* ========================================================== */

/**
 * @brief Verifica se um ficheiro existe.
 * @param caminho
 * @return bool_t
 */
bool_t ficheiro_existe(const char* caminho) {
    bool_t existe = FALSE;

    if (access(caminho, F_OK) != ERRO) existe = TRUE;
    return existe;
}


/// GUARDAR/CARREGAR DADOS ///

/**
 * @brief Guarda os participantes do vetor no ficheiro
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void guardar_participantes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Guardar o número de participantes inseridos
    int indice;
    fwrite(*&estado_programa->numero_de_participantes, sizeof(int), 1, ficheiro);

    // Guardar os participantes
    for (indice = 0; indice < *estado_programa->numero_de_participantes; indice++) {
        fwrite(&estado_programa->participantes[indice], sizeof(participante_t), 1, ficheiro);
    }
}

/**
 * @brief Guarda as atividades do vetor no ficheiro
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void guardar_atividades(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Guardar o número de atividades inseridas
    int indice;
    fwrite(*&estado_programa->numero_de_atividades, sizeof(int), 1, ficheiro);

    // Guardar as atividades
    for (indice = 0; indice < *estado_programa->numero_de_atividades; indice++) {
        fwrite(&estado_programa->atividades[indice], sizeof(atividade_t), 1, ficheiro);
    }
}

/* ========================================================== */

void guardar_inscricoes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Guardar o número de inscrições inseridas
    int indice;
    fwrite(*&estado_programa->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Guardar as inscrições
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        fwrite(&estado_programa->inscricoes[indice], sizeof(inscricao_t), 1, ficheiro);
    }
}

void carregar_participantes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de participantes inseridos
    int indice;
    fread(*&estado_programa->numero_de_participantes, sizeof(int), 1, ficheiro);

    // Carregar os participantes
    for (indice = 0; indice < *estado_programa->numero_de_participantes; indice++) {
        fread(&estado_programa->participantes[indice], sizeof(participante_t), 1, ficheiro);
    }
}

void carregar_atividades(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de atividades inseridas
    int indice;
    fread(*&estado_programa->numero_de_atividades, sizeof(int), 1, ficheiro);

    // Carregar as atividades
    for (indice = 0; indice < *estado_programa->numero_de_atividades; indice++) {
        fread(&estado_programa->atividades[indice], sizeof(atividade_t), 1, ficheiro);
    }
}

void carregar_inscricoes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de inscrições inseridas
    int indice;
    fread(*&estado_programa->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Carregar as inscrições
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        fread(&estado_programa->inscricoes[indice], sizeof(inscricao_t), 1, ficheiro);
    }
}

/* ========================================================== */


/**
 * @brief Guarda o estado do programa num ficheiro
 * @param caminho
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t guardar_estado_programa(const char* caminho, estado_programa_t* estado_programa) {
    codigo_erro_t codigo_erro = OK;
    FILE* ficheiro = fopen(caminho, ESCRITA_BINARIA);
    if (ficheiro == NULL) codigo_erro = ERRO;

    guardar_participantes(estado_programa, ficheiro);
    guardar_atividades(estado_programa, ficheiro);
    guardar_inscricoes(estado_programa, ficheiro);

    fclose(ficheiro);
    return codigo_erro;
}

/**
 * @brief Carrega o estado do programa de um ficheiro
 * @param caminho
 * @return estado_programa_t*
 */
estado_programa_t* carregar_estado_programa(const char* caminho) {
    estado_programa_t* estado_programa = NULL;
    FILE* ficheiro = fopen(caminho, LEITURA_BINARIA);

    if (ficheiro != NULL) {
        estado_programa = (estado_programa_t*) malloc(sizeof(estado_programa_t));
        carregar_participantes(estado_programa, ficheiro);
        carregar_atividades(estado_programa, ficheiro);
        carregar_inscricoes(estado_programa, ficheiro);

        fclose(ficheiro);
    }

    return estado_programa;
}

/**
 * @brief Carrega os dados do programa salvos no ficheiro save e guarda-os na estrutura do estado do programa.
 * @param caminho
 * @param estado_programa
 * @return bool_t
 */
codigo_erro_t carregar_dados(const char* caminho, estado_programa_t* estado_programa) {
    codigo_erro_t resultado = ERRO;

    inicializar_vetores(estado_programa);
    if (ficheiro_existe(caminho)) {
        estado_programa = carregar_estado_programa(caminho);
        if (estado_programa != NULL) resultado = OK;
    }

    return resultado;
}

/// INSERÇÃO DE DADOS ///

participante_t* ler_participante(estado_programa_t* estado_programa) {
    int id = *estado_programa->numero_de_participantes;
    char nome[TAMANHO_MAXIMO_NOME];
}

/**
 * @brief Insere um participante no vetor de participantes
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t inserir_participante(estado_programa_t* estado_programa) {
    codigo_erro_t resultado = ERRO;
    participante_t* participante = ler_participante(estado_programa);

    if (participante != NULL) {
        estado_programa->participantes[*estado_programa->numero_de_participantes] = participante;
        (*estado_programa->numero_de_participantes)++;
        resultado = OK;
    }

    return resultado;
}

/**
 * @brief Insere uma atividade no vetor de atividades
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t inserir_atividade(estado_programa_t* estado_programa) {
    codigo_erro_t resultado = ERRO;
    atividade_t* atividade = ler_atividade(estado_programa);

    if (atividade != NULL) {
        estado_programa->atividades[*estado_programa->numero_de_atividades] = atividade;
        (*estado_programa->numero_de_atividades)++;
        resultado = OK;
    }

    return resultado;
}

/**
 * @brief Insere uma inscrição no vetor de inscrições
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t inserir_inscricao(estado_programa_t* estado_programa) {
    codigo_erro_t resultado = ERRO;
    inscricao_t* inscricao = ler_inscricao(estado_programa);

    if (inscricao != NULL) {
        estado_programa->inscricoes[*estado_programa->numero_de_inscricoes] = inscricao;
        (*estado_programa->numero_de_inscricoes)++;
        resultado = OK;
    }

    return resultado;
}


/* ========================================================== app.c*/

participante_t* criar_participante(char* nome, char* escola, int nif, char* email, int telefone, estado_programa_t* estado_programa) {
    participante_t* participante = (participante_t*) malloc(sizeof(participante_t));
    participante->identificador = *estado_programa->numero_de_participantes;
    strcpy(participante->nome, nome);
    strcpy(participante->escola, escola);
    participante->nif = nif;
    strcpy(participante->email, email);
    participante->telefone = telefone;
    return participante;
}

atividade_t* criar_atividade(char* designacao, char* data, char* hora, char* local, char* tipo, char* associacao_estudantes, float valor, estado_programa_t* estado_programa) {
    atividade_t* atividade = (atividade_t*) malloc(sizeof(atividade_t));
    atividade->identificador = *estado_programa->numero_de_atividades;
    strcpy(atividade->designacao, designacao);
    strcpy(atividade->data, data);
    strcpy(atividade->hora, hora);
    strcpy(atividade->local, local);
    strcpy(atividade->tipo, tipo);
    strcpy(atividade->associacao_estudantes, associacao_estudantes);
    atividade->valor = valor;
    return atividade;
}

inscricao_t* criar_inscricao(int id_participante, int id_atividade, estado_programa_t* estado_programa) {
    int indice_procura;
    int numero_atividades = *estado_programa->numero_de_atividades;

    inscricao_t* inscricao = (inscricao_t*) malloc(sizeof(inscricao_t));
    inscricao->identificador = *estado_programa->numero_de_inscricoes;
    inscricao->id_participante = id_participante;
    inscricao->id_atividade = id_atividade;

    // Procurar o valor associado à atividade com o ID passado
    indice_procura = procurar_atividade_por_id(id_atividade, estado_programa);
    if (indice_procura != ERRO) {
        inscricao->valor_pago = estado_programa->atividades[indice_procura]->valor;
        strcpy(inscricao->data, obter_data_atual());
        strcpy(inscricao->hora, obter_hora_atual_com_segundos());
    }

    return inscricao;
}


estado_programa_t* criar_estado_programa(participante_t** vetor_participantes, atividade_t** vetor_atividades,
                                         inscricao_t** vetor_inscricoes, int* contador_participantes,
                                         int* contador_atividades, int* contador_inscricoes, bool_t* programa_salvo)
{
    estado_programa_t* estado_programa = (estado_programa_t*) malloc(sizeof(estado_programa_t));
    estado_programa->participantes = vetor_participantes;
    estado_programa->atividades = vetor_atividades;
    estado_programa->inscricoes = vetor_inscricoes;
    estado_programa->numero_de_atividades = contador_participantes;
    estado_programa->numero_de_atividades = contador_atividades;
    estado_programa->numero_de_inscricoes = contador_inscricoes;
    estado_programa->dados_guardados = programa_salvo;
    return estado_programa;
}

void libertar_estado_programa(t_estado_programa* estado_programa) {
    int indice;
    for (indice= 0; indice < *estado_programa->numero_participantes_inseridos; indice++) libertar_participante(estado_programa->participantes[indice]);
    for (indice= 0; indice < *estado_programa->numero_atividadades_inseridas; indice++) libertar_atividade(estado_programa->atividades[indice]);
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) libertar_inscricao(estado_programa->inscricoes[indice]);
    free(estado_programa);
}


inline_ void libertar_participante(participante_t* participante) {free(participante);}
inline_ void libertar_atividade(atividade_t* atividade) {free(atividade);}
inline_ void libertar_inscricao(inscricao_t* inscricao) {free(inscricao);}

void mostrar_estado_programa(t_estado_programa* estado_programa) {
    int indice;
    printf("Participantes:\n");
    
    for (indice = 0; indice < *estado_programa->numero_participantes_inseridos; indice++) {
        mostrar_participante(estado_programa->participantes[indice]);
    }
    printf("Atividades:\n");
    for (indice= 0; indice < *estado_programa->numero_atividadades_inseridas; indice++) {
        mostrar_atividade(estado_programa->atividades[indice]);
    }
    printf("Inscrições:\n");
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        mostrar_inscricao(estado_programa->inscricoes[indice]);
    }
}

/**
 * @brief Mostra um participante
 *
 * @param participante
 */
inline_ void mostrar_participante(participante_t* participante) {
    printf("    Identificador: %d\n", participante->identificador);
    printf("    Nome: %s\n", participante->nome);
    printf("    Escola: %s\n", participante->escola);
    printf("    NIF: %d\n", participante->nif);
    printf("    E-mail: %s\n", participante->email);
    printf("    Telefone: %d\n\n", participante->telefone);
}


/**
 * @brief Mostra uma atividade
 * @param atividade
 */
inline_ void mostrar_atividade(atividade_t* atividade) {
    printf("    Identificador: %d\n", atividade->identificador);
    printf("    Designação: %s\n", atividade->designacao);
    printf("    Data: %s\n", atividade->data);
    printf("    Hora: %s\n", atividade->hora);
    printf("    Local: %s\n", atividade->local);
    printf("    Tipo: %s\n", atividade->tipo);
    printf("    Associação de Estudantes: %s\n", atividade->associacao_estudantes);
    printf("    Valor: %.2f\n", atividade->valor);
}

/**
 * @brief Mostra uma inscrição
 * @param inscricao
 */
inline_ void mostrar_inscricao(inscricao_t* inscricao) {
    printf("    Identificador: %d\n", inscricao->identificador);
    printf("    ID do Participante: %d\n", inscricao->id_participante);
    printf("    ID da Atividade: %d\n", inscricao->id_atividade);
    printf("    Valor Pago: %.2f\n", inscricao->valor_pago);
    printf("    Data: %s\n", inscricao->data);
    printf("    Hora: %s\n", inscricao->hora);
}
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
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <locale.h>

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
#define obter_timestamp() time(NULL)                         // Defini uma macro para obter uma timestamp
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
#define TAMANHO_MAXIMO_EMAIL 320         // 64 caracteres para a parte local + 1 para o @ + 255 caracteres para o domínio segundo o RFC 5321
#define TAMANHO_MAXIMO_ESCOLA 10
#define TAMANHO_MAXIMO_DESIGNACAO 100
#define TAMANHO_MAXIMO_LOCAL TAMANHO_MAXIMO_NOME
#define TAMANHO_MAXIMO_TIPO_ATIVIDADE 20
#define TAMANHO_MAXIMO_AE 10
#define TAMANHO_DATA 11
#define TAMANHO_HORA 6
#define TAMANHO_TELEFONE 9

#define DATA_FORMATO "%02d/%02d/%04d"           // (DD/MM/AAAA)
#define HORA_FORMATO "%02d:%02d"                // (HH:MM)
#define HORA_FORMATO_COMPLETO "%02d:%02d:%02d"  // (HH:MM:SS)

#define TESTE 1                         // Flag de teste


/* ========================================================== */
/* =                 UTILITÁRIOS DE OUTPUT                  = */
/* ========================================================== */


typedef enum {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef enum {
    OK = 0,
    ERRO = -1
} codigo_erro_t;

// A enumeração dos tipos primitivos é usada para que algumas funções genéricas no programa possam
// interpretar os parâmetros de entrada corretamente
// (ex: função vetor_contem_elemento(void*, int, void*, tipo_primitivo_t))
// Os templates e o meta-programming do C++ seriam uma alternativa melhor mas como é óbvio, não existem no C
typedef enum {
    INT = 0,
    FLOAT = 1,
    STRING = 2,
    CHAR = 3,
} tipo_primitivo_t;

// Enumerações com as opções de cada menu do programa
typedef enum {                          // Menu principal
    MENU_PARTICIPANTES = 1,
    MENU_ATIVIDADES,
    MENU_INSCRICAO,
    ESTATISTICAS,
    SALVAR,
    SAIR
} opcao_menu_principal_t;

typedef enum {                          // Menu de participantes
    INSERIR_PARTICIPANTE = 1,
    LISTAR_PARTICIPANTES,
    VOLTAR_MENU_PRINCIPAL_PARTICIPANTES
} opcao_menu_participantes_t;

typedef enum {                          // Menu de atividades
    INSERIR_ATIVIDADE = 1,
    LISTAR_ATIVIDADES,
    VOLTAR_MENU_PRINCIPAL_ATIVIDADES
} opcao_menu_atividades_t;

typedef enum {                          // Menu de inscrições
    INSERIR_INSCRICAO = 1,
    LISTAR_INSCRICOES,
    VOLTAR_MENU_PRINCIPAL_INSCRICOES
} opcao_menu_inscricoes_t;

typedef enum {                          // Menu de estatísticas
    NUMERO_DE_ATIVIDADES_POR_ASSOCIACAO = 1,
    PERCENTAGEM_DE_INSCRICOES_POR_ESCOLA,
    VALOR_TOTAL_DAS_INSCRICOES_EM_HORIZONTE_TEMPORAL,
    VOLTAR_MENU_PRINCIPAL_ESTATISTICAS
} opcao_menu_estatisticas_t;


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
    participante_t** participantes;         // Vetor de participantes
    atividade_t**    atividades;            // Vetor de atividades
    inscricao_t**    inscricoes;            // Vetor de inscrições
    int*           numero_de_participantes; // Contadores
    int*           numero_de_atividades;    //
    int*           numero_de_inscricoes;    //
    bool_t*        dados_guardados;         // Flag para indicar se os dados foram guardados
} estado_programa_t;

/* ========================================================== */



/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

participante_t* criar_participante(char*, char*, int, char*, int, estado_programa_t*);
atividade_t* criar_atividade(char*, char*, char*, char*, char*, char*, float, estado_programa_t*);
inscricao_t* criar_inscricao(int, int, estado_programa_t*);
estado_programa_t* criar_estado_programa(participante_t**, atividade_t**, inscricao_t**, int*, int*, int*, bool_t*);

void libertar_participante(participante_t*);
void libertar_atividade(atividade_t*);
void libertar_inscricao(inscricao_t*);
void libertar_estado_programa(estado_programa_t*);

// Usamos a keyword static para evitar o aviso de compilação "'printf' is static but used in inline function"
inline_ static void mostrar_participante(participante_t*);
inline_ static void mostrar_atividade(atividade_t*);
inline_ static void mostrar_inscricao(inscricao_t*);
inline_ static void mostrar_estado_programa(estado_programa_t*);

inline_ static void mostrar_participantes(estado_programa_t*);
inline_ static void mostrar_atividades(estado_programa_t*);
inline_ static void mostrar_inscricoes(estado_programa_t*);

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
int   ler_inteiro_intervalo(const char*, int, int, bool_t);
float ler_float_intervalo(const char*, float, float);
char  ler_char(const char*);

void  ler_escola(const char*, char*);
void  ler_data(const char*, char*);
void  ler_hora(const char*, char*);

void  ler_email(const char*, char*);
int   ler_nif(const char*);
int   ler_telefone(const char*);

participante_t* ler_participante(estado_programa_t*);
atividade_t* ler_atividade(estado_programa_t*) {};
inscricao_t* ler_inscricao(estado_programa_t*) {};

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

bool_t confirmar_saida(estado_programa_t*);
bool_t vetor_contem_elemento(void*, int, void*, tipo_primitivo_t);
bool_t nif_valido(int);

bool_t email_parte_local_valida(char*, int, int);     // Para validar as diversas partes do email segundo as definicoes do RFC 5322 e 5321
bool_t email_parte_dominio_valida(char*, int, int);   //
bool_t email_valido(char*);                           // Para validar o email como um todo

bool_t ano_bissexto(int);
inline_ int dias_mes(int, int);

bool_t data_valida(char*);
bool_t hora_valida(char*);

int posicao_char_na_string(char, char*, int);

/* ========================================================== */

opcao_menu_principal_t menu_principal();

opcao_menu_participantes_t ler_opcao_menu_participantes();
void menu_participantes(estado_programa_t*);

opcao_menu_atividades_t ler_opcao_menu_atividades();
void menu_atividades(estado_programa_t*);

opcao_menu_inscricoes_t ler_opcao_menu_inscricoes();
void menu_inscricoes(estado_programa_t*);

opcao_menu_estatisticas_t ler_opcao_menu_estatisticas();
void menu_estatisticas(estado_programa_t*);

/* ========================================================== */

void string_para_minusculas(char*);
void string_para_maiusculas(char*);
void esperar_tecla(const char*);

/* ========================================================== */

void inserir_dados_teste(estado_programa_t*);



/* ========================================================== */
/* =                         MAIN                           = */
/* ========================================================== */

int main() {
    // DECLARAÇÃO DE VARIÁVEIS //
    int numero_de_participantes = 0;
    int numero_de_atividades = 0;
    int numero_de_inscricoes = 0;

    participante_t* participantes[NUMERO_MAXIMO_DE_PARTICIPANTES];
    atividade_t*    atividades[NUMERO_MAXIMO_DE_ATIVIDAES];
    inscricao_t*    inscricoes[NUMERO_MAXIMO_DE_INSCRICOES];
    bool_t         dados_guardados = TRUE;                 
    bool_t         sair = FALSE;
    opcao_menu_principal_t opcao_menu;

    /// ESTADO DO PROGRAMA  ///
    // Criação do estado do programa
    //
    // Este estado é partilhado entre funções, de modo que possam aceder e consultar o estado atual
    // do programa.
    estado_programa_t* estado_programa = criar_estado_programa(participantes, atividades, inscricoes, &numero_de_participantes, &numero_de_atividades, &numero_de_inscricoes, &dados_guardados);

    setlocale(LC_ALL, "Portuguese"); 

    if (carregar_dados(FICHEIRO_SAVE, estado_programa) == ERRO) {
        printf("Erro ao carregar dados do ficheiro \"%s\". A aplicação irá continuar sem dados pré-existentes.\n", FICHEIRO_SAVE);
    }

    /// LOOP PRINCIPAL ///
    do {
        opcao_menu = menu_principal();

        switch (opcao_menu) {
            case MENU_PARTICIPANTES:
                menu_participantes(estado_programa);
                break;
            case MENU_ATIVIDADES:
                menu_atividades(estado_programa);
                break;
            case MENU_INSCRICAO:
                menu_inscricoes(estado_programa);
                break;
            case ESTATISTICAS:
                menu_estatisticas(estado_programa);
                break;
            case SALVAR:
                if (dados_guardados == FALSE) {
                    if (guardar_dados(FICHEIRO_SAVE, estado_programa) == ERRO) {
                        printf("Erro ao guardar dados no ficheiro \"%s\".\n", FICHEIRO_SAVE);
                    } else {
                        printf("Dados guardados com sucesso no ficheiro \"%s\".\n", FICHEIRO_SAVE);
                        dados_guardados = TRUE;
                    }
                } else {    // Se os dados já tiverem sido guardados
                    printf("Os dados já se encontram guardados no ficheiro \"%s\".\n", FICHEIRO_SAVE);
                }

                esperar_tecla("Pressione qualquer tecla para continuar...");
                break;
            case SAIR:
                sair = confirmar_saida(estado_programa);
        } // Sem caso default para que o compilador nos avise de casos não tratados
    } while(!sair);

    limpar_ecra();
    return OK;
}


/* ========================================================== */
/* =               GUARDAR & CARREGAR DADOS                 = */
/* ========================================================== */

void inicializar_vetores(estado_programa_t* estado_programa) {
    int i;
    for (i = 0; i < NUMERO_MAXIMO_DE_PARTICIPANTES; i++) estado_programa->participantes[i] = NULL;
    for (i = 0; i < NUMERO_MAXIMO_DE_ATIVIDAES; i++) estado_programa->atividades[i] = NULL;
    for (i = 0; i < NUMERO_MAXIMO_DE_INSCRICOES; i++) estado_programa->inscricoes[i] = NULL;
}

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

/**
 * @brief Guarda as inscrições do vetor no ficheiro
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void guardar_inscricoes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Guardar o número de inscrições inseridas
    int indice;
    fwrite(*&estado_programa->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Guardar as inscrições
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        fwrite(&estado_programa->inscricoes[indice], sizeof(inscricao_t), 1, ficheiro);
    }
}

/**
 * @brief Carrega os participantes do ficheiro para o vetor
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void carregar_participantes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de participantes inseridos
    int indice;
    fread(*&estado_programa->numero_de_participantes, sizeof(int), 1, ficheiro);

    // Carregar os participantes
    for (indice = 0; indice < *estado_programa->numero_de_participantes; indice++) {
        fread(&estado_programa->participantes[indice], sizeof(participante_t), 1, ficheiro);
    }
}

/**
 * @brief Carrega as atividades do ficheiro para o vetor
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void carregar_atividades(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de atividades inseridas
    int indice;
    fread(*&estado_programa->numero_de_atividades, sizeof(int), 1, ficheiro);

    // Carregar as atividades
    for (indice = 0; indice < *estado_programa->numero_de_atividades; indice++) {
        fread(&estado_programa->atividades[indice], sizeof(atividade_t), 1, ficheiro);
    }
}

/**
 * @brief Carrega as inscrições do ficheiro para o vetor
 * @param estado_programa
 * @param ficheiro
 * @return void
 */
void carregar_inscricoes(estado_programa_t* estado_programa, FILE* ficheiro) {
    // Carregar o número de inscrições inseridas
    int indice;
    fread(*&estado_programa->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Carregar as inscrições
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        fread(&estado_programa->inscricoes[indice], sizeof(inscricao_t), 1, ficheiro);
    }
}


/// GUARDAR & CARREGAR ESTADO ///


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


/// GUARDAR & CARREGAR DADOS ///

/**
 * @brief Guarda os dados do programa num ficheiro
 * @param caminho
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t guardar_dados(const char* caminho, estado_programa_t* estado_programa) {
    codigo_erro_t codigo_erro = guardar_estado_programa(caminho, estado_programa);
    return codigo_erro;
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


/* ========================================================== */
/* =                 UTILITÁRIOS DE INPUT                   = */
/* ========================================================== */

/**
 * @brief Lê uma string com o tamanho especificado
 * @param string
 * @param tamanho
 * @return void
 */
void ler_string(const char* mensagem, char* string, int tamanho) {
    printf("%s", mensagem);
    fflush(stdin);
    fgets(string, tamanho, stdin);
    string[strcspn(string, "\n")] = '\0'; // Remover o '\n' do final da string
}


/**
 * @brief Lê um inteiro compreendido entre os valores especificados
 * @param mensagem
 * @param minimo
 * @param maximo
 * @param mostrar_mensagem_erro
 * @return int
 */
int ler_inteiro_intervalo(const char* mensagem, int minimo, int maximo, bool_t mostrar_mensagem_erro) {
    int inteiro;

    do {
        printf("%s", mensagem);
        scanf("%d", &inteiro);
        getchar();
        if (inteiro < minimo || inteiro > maximo && mostrar_mensagem_erro)
            printf("Valor inválido. Introduza um valor entre %d e %d.\n", minimo, maximo);

    } while (inteiro < minimo || inteiro > maximo);

    return inteiro;
}

/**
 * @brief Lê um float compreendido entre os valores especificados
 * @param mensagem
 * @param minimo
 * @param maximo
 * @return float
 */
float ler_float_intervalo(const char* mensagem, float minimo, float maximo) {
    float flutuante;

    do {
        printf("%s", mensagem);
        scanf("%f", &flutuante);
        getchar();
        if (flutuante < minimo || flutuante > maximo)
            printf("Valor inválido. Introduza um valor entre %.2f e %.2f.\n", minimo, maximo);

    } while (flutuante < minimo || flutuante > maximo);

    return flutuante;
}

char ler_char(const char* mensagem) {
    char caracter;
    printf("%s", mensagem);
    scanf(" %c", &caracter);
    return caracter;
}

/**
 * @brief Lê a escola de um participante até que a escola inserida esteja dentro das opções possíveis
 * @param mensagem
 * @param output
 * @param tamanho_maximo
 */
void ler_escola(const char* mensagem, char* output) {
    char escola[TAMANHO_MAXIMO_ESCOLA];
    char* escolas_possiveis[5] = {"ESTG", "ESECS", "ESSLEI", "ESAD", "ESTM"};

    do {
        ler_string(mensagem, escola, TAMANHO_MAXIMO_ESCOLA);
        string_para_maiusculas(escola);
        if (!vetor_contem_elemento(escolas_possiveis, 5, escola, STRING))
            printf("Escola inválida. Escolas possíveis: ESTG, ESECS, ESSLEI, ESAD, ESTM.\n");

    } while (!vetor_contem_elemento(escolas_possiveis, 5, escola, STRING));

    strcpy(output, escola);
}

/**
 * @brief Lê um NIF até que o NIF inserido seja válido
 *
 * @warning O NIF deve ser composto por 9 dígitos e deve seguir o algoritmo de validação do NIF
 *
 * @param mensagem
 * @return int
 */
int ler_nif(const char* mensagem) {
    int nif;
    
    do {
        nif = ler_inteiro_intervalo(mensagem, 100000000, 999999999, FALSE);
        if (!nif_valido(nif))
            printf("NIF inválido. Introduza um NIF válido.\n");
    } while (!nif_valido(nif));
    
    return nif;
}

int ler_telefone(const char* mensagem) {
    char telefone[TAMANHO_TELEFONE];

    do {
        ler_string(mensagem, telefone, TAMANHO_TELEFONE);
        if (!strlen(telefone) != TAMANHO_TELEFONE)
            printf("O numero de telemovel deve ter 9 digitos\n");
    } while (!strlen(telefone) != TAMANHO_TELEFONE);

    return atoi(telefone);
}

void ler_email(const char* mensagem, char* output) {
    char email[TAMANHO_MAXIMO_EMAIL];
    
    do {
        ler_string(mensagem, email, TAMANHO_MAXIMO_EMAIL);
        if (!email_valido(email))
            printf("Email inválido. Introduza um email válido.\n");
    } while (!email_valido(email));
    
    strcpy(output, email);
}

/**
 * @brief Lê os dados para a criação de um participante e guarda-os na estrutura
 * @param estado_programa
 * @return participante_t*
 */
participante_t* ler_participante(estado_programa_t* estado_programa) {
    participante_t* participante = NULL;
    char nome[TAMANHO_MAXIMO_NOME];
    char escola[TAMANHO_MAXIMO_ESCOLA];
    char email[TAMANHO_MAXIMO_EMAIL];
    int  nif, telefone, identificador;

    ler_string("Nome do participante: ", nome, TAMANHO_MAXIMO_NOME);
    ler_escola("Escola do participante: ", escola);
    nif = ler_nif("NIF do participante: ");
    ler_email("Email do participante: ", email);
    telefone = ler_telefone("Telefone do participante: ");

    participante = criar_participante(nome, escola, nif, email, telefone, estado_programa);
    return participante;
}

/**
 * @brief Insere um participante no vetor de participantes
 * @param estado_programa
 * @return codigo_erro_t
 */
codigo_erro_t inserir_participante(estado_programa_t* estado_programa) {
    codigo_erro_t resultado = ERRO;
    participante_t* participante;

    limpar_ecra();

    if (*estado_programa->numero_de_participantes == NUMERO_MAXIMO_DE_PARTICIPANTES) {
        printf("Número máximo de participantes atingido.\n");
        esperar_tecla("Pressione qualquer tecla para continuar...");
    }
    else {
        participante = ler_participante(estado_programa);
        if (participante != NULL) {
            estado_programa->participantes[*estado_programa->numero_de_participantes] = participante;
            (*estado_programa->numero_de_participantes)++;
            resultado = OK;
        }
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
    atividade_t* atividade;

    limpar_ecra();

    if (*estado_programa->numero_de_atividades == NUMERO_MAXIMO_DE_ATIVIDAES) {
        printf("Número máximo de atividades atingido.\n");
        esperar_tecla("Pressione qualquer tecla para continuar...");
    }
    else {
        atividade = ler_atividade(estado_programa);
        if (atividade != NULL) {
            estado_programa->atividades[*estado_programa->numero_de_atividades] = atividade;
            (*estado_programa->numero_de_atividades)++;
            resultado = OK;
        }
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
    inscricao_t* inscricao;

    limpar_ecra();

    if (*estado_programa->numero_de_inscricoes == NUMERO_MAXIMO_DE_INSCRICOES) {
        printf("Número máximo de inscrições atingido.\n");
        esperar_tecla("Pressione qualquer tecla para continuar...");
    }
    else {
        inscricao = ler_inscricao(estado_programa);
        if (inscricao != NULL) {
            estado_programa->inscricoes[*estado_programa->numero_de_inscricoes] = inscricao;
            (*estado_programa->numero_de_inscricoes)++;
            resultado = OK;
        }
    }

    return resultado;
}


/* ========================================================== */
/* =                UTILITÁRIOS DOS STRUCTS                 = */
/* ========================================================== */


/**
 * @brief Aloca memória para um participante e inicializa os seus campos
 * @param nome
 * @param escola
 * @param nif
 * @param email
 * @param telefone
 * @param estado_programa
 * @return participante_t*
 */
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

/**
 * @brief Aloca memória para uma atividade e inicializa os seus campos
 * @param nome
 * @param descricao
 * @param data
 * @param hora_inicio
 * @param hora_fim
 * @param estado_programa
 * @return atividade_t*
 */
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

/**
 * @brief Aloca memória para uma inscrição e inicializa os seus campos
 * 
 * Os valor pago é atribuído automaticamente com base no valor da atividade
 * 
 * @param participante
 * @param atividade
 * @param estado_programa
 * @return inscricao_t*
 */
inscricao_t* criar_inscricao(int id_participante, int id_atividade, estado_programa_t* estado_programa) {
    int indice_procura_atividades, indice_procura_participantes;
    int numero_atividades = *estado_programa->numero_de_atividades;
    inscricao_t* inscricao = NULL;

    // Procurar o participante e a atividade com os IDs especificados
    indice_procura_atividades = procurar_atividade_por_id(id_atividade, estado_programa);
    indice_procura_participantes = procurar_participante_por_id(id_participante, estado_programa);

    if (indice_procura_atividades != ERRO && indice_procura_participantes != ERRO) {
        inscricao = (inscricao_t*) malloc(sizeof(inscricao_t));

        // Atribuir os valores aos campos da inscrição
        inscricao->identificador = *estado_programa->numero_de_inscricoes;
        inscricao->id_participante = id_participante;
        inscricao->id_atividade = id_atividade;
        inscricao->valor_pago = estado_programa->atividades[indice_procura_atividades]->valor;

        // A inscrição tem a data e hora da sua criação
        strcpy(inscricao->data, obter_data_atual());
        strcpy(inscricao->hora, obter_hora_atual_com_segundos());
    }

    return inscricao;
}

/**
 * @brief Aloca memória para o estado do programa e inicializa os seus campos
 * @param vetor_participantes
 * @param vetor_atividades
 * @param vetor_inscricoes
 * @param contador_participantes
 * @param contador_atividades
 * @param contador_inscricoes
 * @param programa_salvo
 * @return estado_programa_t*
 */
estado_programa_t* criar_estado_programa(participante_t** vetor_participantes, atividade_t** vetor_atividades,
                                         inscricao_t** vetor_inscricoes, int* contador_participantes,
                                         int* contador_atividades, int* contador_inscricoes, bool_t* programa_salvo)
{
    estado_programa_t* estado_programa = (estado_programa_t*) malloc(sizeof(estado_programa_t));
    estado_programa->participantes = vetor_participantes;
    estado_programa->atividades = vetor_atividades;
    estado_programa->inscricoes = vetor_inscricoes;
    estado_programa->numero_de_participantes = contador_participantes;
    estado_programa->numero_de_atividades = contador_atividades;
    estado_programa->numero_de_inscricoes = contador_inscricoes;
    estado_programa->dados_guardados = programa_salvo;
    return estado_programa;
}


/* ========================================================== */
/* =          UTILITÁRIOS DE DESACOLAÇÃO DE MEMÓRIA         = */
/* ========================================================== */


void libertar_estado_programa(estado_programa_t* estado_programa) {
    int indice;
    for (indice= 0; indice < *estado_programa->numero_de_participantes; indice++) libertar_participante(estado_programa->participantes[indice]);
    for (indice= 0; indice < *estado_programa->numero_de_atividades; indice++) libertar_atividade(estado_programa->atividades[indice]);
    for (indice= 0; indice < *estado_programa->numero_de_inscricoes; indice++) libertar_inscricao(estado_programa->inscricoes[indice]);
    free(estado_programa);
}


inline_ void libertar_participante(participante_t* participante) {free(participante);}
inline_ void libertar_atividade(atividade_t* atividade) {free(atividade);}
inline_ void libertar_inscricao(inscricao_t* inscricao) {free(inscricao);}


/* ========================================================== */
/* =                 UTILITÁRIOS DE OUTPUT                  = */
/* ========================================================== */

inline_ static void mostrar_estado_programa(estado_programa_t* estado_programa) {
    int indice;
    
    printf("Participantes:\n");
    mostrar_participantes(estado_programa);

    printf("Atividades:\n");
    mostrar_atividades(estado_programa);

    printf("Inscrições:\n");
    mostrar_inscricoes(estado_programa);

    printf("Dados guardados: %s\n", *estado_programa->dados_guardados ? "Sim" : "Não");

    printf("Número de participantes: %d (%d restantes)\n", *estado_programa->numero_de_participantes, NUMERO_MAXIMO_DE_PARTICIPANTES - *estado_programa->numero_de_participantes);
    printf("Número de atividades: %d (%d restantes)\n", *estado_programa->numero_de_atividades, NUMERO_MAXIMO_DE_ATIVIDAES - *estado_programa->numero_de_atividades);
    printf("Número de inscrições: %d (%d restantes)\n", *estado_programa->numero_de_inscricoes, NUMERO_MAXIMO_DE_INSCRICOES - *estado_programa->numero_de_inscricoes);
}

inline_ static void mostrar_participante(participante_t* participante) {
    printf("    Identificador: %d\n", participante->identificador);
    printf("    Nome: %s\n", participante->nome);
    printf("    Escola: %s\n", participante->escola);
    printf("    NIF: %d\n", participante->nif);
    printf("    E-mail: %s\n", participante->email);
    printf("    Telefone: %d\n\n", participante->telefone);
}

inline_ static void mostrar_atividade(atividade_t* atividade) {
    printf("    Identificador: %d\n", atividade->identificador);
    printf("    Designação: %s\n", atividade->designacao);
    printf("    Data: %s\n", atividade->data);
    printf("    Hora: %s\n", atividade->hora);
    printf("    Local: %s\n", atividade->local);
    printf("    Tipo: %s\n", atividade->tipo);
    printf("    Associação de Estudantes: %s\n", atividade->associacao_estudantes);
    printf("    Valor: %.2f\n", atividade->valor);
}

inline_ static void mostrar_inscricao(inscricao_t* inscricao) {
    printf("    Identificador: %d\n", inscricao->identificador);
    printf("    ID do Participante: %d\n", inscricao->id_participante);
    printf("    ID da Atividade: %d\n", inscricao->id_atividade);
    printf("    Valor Pago: %.2f\n", inscricao->valor_pago);
    printf("    Data: %s\n", inscricao->data);
    printf("    Hora: %s\n", inscricao->hora);
}

inline_ static void mostrar_participantes(estado_programa_t* estado_programa) {
    int indice, numero_participantes = *estado_programa->numero_de_participantes;

    if (numero_participantes == 0) {
        printf("Não existem participantes registados.\n");
    } else {
        printf("Numero de participantes registados: %d\n", numero_participantes);
        for (indice = 0; indice < numero_participantes; indice++) {
            printf("Participante %d:\n", indice + 1);
            mostrar_participante(estado_programa->participantes[indice]);
        }
    }

    esperar_tecla("Pressione ENTER para continuar...");
}

inline_ static void mostrar_atividades(estado_programa_t* estado_programa) {
    int indice, numero_atividades = *estado_programa->numero_de_atividades;

    if (numero_atividades == 0) {
        printf("Não existem atividades registadas.\n");
    } else {
        printf("Numero de atividades registadas: %d\n", numero_atividades);
        for (indice = 0; indice < numero_atividades; indice++) {
            printf("Atividade %d:\n", indice + 1);
            mostrar_atividade(estado_programa->atividades[indice]);
        }
    }

    esperar_tecla("Pressione ENTER para continuar...");
}

inline_ static void mostrar_inscricoes(estado_programa_t* estado_programa) {
    int indice, numero_inscricoes = *estado_programa->numero_de_inscricoes;

    if (numero_inscricoes == 0) {
        printf("Não existem inscrições registadas.\n");
    } else {
        printf("Numero de inscrições registadas: %d\n", numero_inscricoes);
        for (indice = 0; indice < numero_inscricoes; indice++) {
            printf("Inscrição %d:\n", indice + 1);
            mostrar_inscricao(estado_programa->inscricoes[indice]);
        }
    }

    esperar_tecla("Pressione ENTER para continuar...");
}


/* ========================================================== */
/* =               CONFIRMAÇÕES ESPECÍFICAS                 = */
/* ========================================================== */

/**
 * @brief Função que confirma a saída do programa
 * @param estado_programa
 * @return
 */
bool_t confirmar_saida(estado_programa_t* estado_programa) {
    bool_t confirmacao;
    char mensagem[100];

    // Definir a mensagem consoante o estado do programa
    estado_programa->dados_guardados ? strcpy(mensagem, "Tem a certeza que deseja sair? (s/n): ")
                                     : strcpy(mensagem, "Tem a certeza que deseja sair sem guardar? (s/n): ");

    do {
        confirmacao = ler_char(mensagem);
        confirmacao = tolower(confirmacao);
        if (confirmacao != 's' && confirmacao != 'n')
            printf("Valor inválido. Introduza 's' para sim ou 'n' para não.\n");
    } while (confirmacao != 's' && confirmacao != 'n');

    return confirmacao == 's';
}

/**
 * @brief Função genérica de procura de dados em vetores com tipos primitivos
 * @param vetor
 * @param tamanho
 * @param elemento_procura
 * @param tipo_de_dados_vetor
 * @return bool_t
 */
bool_t vetor_contem_elemento(void* vetor, int tamanho, void* elemento_procura, tipo_primitivo_t tipo_de_dados_vetor) {
    int indice;
    bool_t encontrado = FALSE;
    switch (tipo_de_dados_vetor) {  // Interpreta os parâmetros conforme o tipo de dados do vetor
        case INT:
            for (indice = 0; indice < tamanho; indice++)
                if (*((int*) vetor + indice) == *((int*) elemento_procura)) encontrado = TRUE;
            break;
        case FLOAT:
            for (indice = 0; indice < tamanho; indice++)
                if (*((float*) vetor + indice) == *((float*) elemento_procura)) encontrado = TRUE;
            break;
        case CHAR:
            for (indice = 0; indice < tamanho; indice++)
                if (*((char*) vetor + indice) == *((char*) elemento_procura)) encontrado = TRUE;
            break;
        case STRING:
            for (indice = 0; indice < tamanho; indice++)
                if (strcmp(((char**) vetor)[indice], (char*) elemento_procura) == 0) encontrado = TRUE;
            break;
    } // Sem caso default para que o compilador nos avise de casos não tratados
    return encontrado;
}

/**
 * @brief Verifica se um determinado NIF é válido calculando o dígito de controlo
 *
 * O algoritmo de validação do NIF foi adaptado de:
 * - https://pt.wikipedia.org/wiki/N%C3%BAmero_de_identifica%C3%A7%C3%A3o_fiscal
 *
 * Segundo a documentação, o check digit é calculado da seguinte forma:
 *  1 - Multiplicar o 8º digito por 2, o 7º por 3, o 6º por 4, o 5º por 5, o 4º por 6, o 3º por 7, o 2º por 8 e o 1º por 9.
 *  2 - Somar os resultados obtidos.
 *  3 - Calcular o resto da divisão do número por 11.
 *      3.1 - Se o resto for 0 ou 1 o check digit será 0.
 *      3.1 - Se for outro qualquer algarismo X, o check digit será 11 - X.
 *
 * @param nif
 * @return
 */
bool_t nif_valido(int nif) {
    // Para não ter que converter o NIF numa string podemos usar operações matemáticas para obter os dígitos
    int check_digit = nif % 10;             // O último dígito é o check digit
    int soma = 0;                           // Resultado da soma dos dígitos multiplicados pelos seus respetivos pesos
    int multiplicador = 2;                  // O multiplicador começa em 2 e vai até 9 (1º digito)
    int resto;                              // Resto da divisão da soma por 11
    bool_t valido = FALSE;

    nif /= 10;                              // Eliminar o último dígito do NIF

    for (int i = 0; i < 8; i++) {           // Para cada um dos 8 dígitos restantes do NIF (passo 1 e 2)
        soma += (nif % 10) * multiplicador; // Multiplicar o dígito pelo seu peso e adicionar ao resultado
        nif /= 10;                          // Eliminar o último dígito do NIF
        multiplicador++;                    // Incrementar o multiplicador (peso)
    }

    resto = soma % 11;                      // Calcular o resto da divisão por 11 (passo 3)

    if (resto == 0 || resto == 1) valido = check_digit == 0;  // Se o resto for 0 ou 1 o check digit é 0 (passo 3.1)
    else valido = check_digit == 11 - resto;                  // Se o resto for outro qualquer algarismo X, o check digit é 11 - X (passo 3.2)
    return valido;
}

/**
 * @brief Esta função valida um email de acordo com o RFC 5322
 * 
 * As restrições de validação do email foram adaptadas do FRC 5322, partindo do seguinte artigo:
 * - https://www.mailboxvalidator.com/resources/articles/acceptable-email-address-syntax-rfc/
 *
 * Segundo a IETF, a parte local do email pode conter qualquer sequência de 64 bytes que consista em
 * caracteres alfanuméricos ou qualquer um dos seguintes caracteres: ! # $ % & ' * + - / = ? ^ _ ` { | } ~
 * 
 * NOTA: O "." não pode ser o primeiro ou último caractere da parte local do email. E não deve ser seguido
 *       de outro ".".
 * 
 * A parte do domínio do email não pode exceder os 255 caracteres e deve obedecer às regras de 
 * especificação de nomes de domínio. Consiste numa sequência de DNS labels separadas por pontos, em
 * que cada label não deve exceder os 63 caracteres (RFC 1035). Cada label é uma combinação de caracteres alfanuméricos
 * e hífens.
 * 
 * NOTA: Os TLDs (top level domains) não podem ser apenas números. Os hífens não podem ser o primeiro ou
 *      último caractere de uma label.
 * 
 * A implementacao desta verificação poderia ter sido facilitada através do uso de expressões regulares. Porém,
 * o Windows não tem nenhuma biblioteca nativa para suportar expressões regulares e no projeto não podemos usar
 * bibliotecas externas. Assim, a implementação foi feita através de uma abordagem mais "manual".
 * 
 * NOTA: A funcao nao cobre todos os casos citados nas descricoes dos respetivos RFCs, porem nao deve ser necessaria tanta
 *       verificacao. Ademais, a versao nao suporta caracteres Unicode.
 *
 * @param mail
 * @return bool_t
 */
bool_t email_valido(char* mail) {
    bool_t valido = TRUE;
    int tamanho_email = strlen(mail), posicao_arroba;

    // Se não tiver o caracter @ o email não é válido
    posicao_arroba = posicao_char_na_string('@', mail, tamanho_email);
    if (posicao_arroba == ERRO) valido = FALSE;
    else {
        // Se a parte local do email não for válida o email não é válido
        if (!email_parte_local_valida(mail, tamanho_email, posicao_arroba)) valido = FALSE;
        else {
            // Se a parte do domínio do email não for válida o email não é válido
            if (!email_parte_dominio_valida(mail, tamanho_email, posicao_arroba)) valido = FALSE;
        }
                    
    }

    return valido;
}

/**
 * @brief Valida a parte local do email segundo as regras do RFC 5322 
 * 
 * @param mail 
 * @param tamanho_email 
 * @param posicao_arroba 
 * @return bool_t 
 */
bool_t email_parte_local_valida(char* mail, int tamanho_email, int posicao_arroba) {
    bool_t valido = TRUE;
    int tamanho_parte_local = posicao_arroba;
    char caracteres_validos_parte_local[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!#$%&'*+-/=?^_`{|}~.*";
    char ponto = '.';

    if (tamanho_parte_local < 1 || tamanho_parte_local > 64) valido = FALSE;        // A parte local do email deve ter entre 1 e 64 caracteres
    else {
        if (mail[0] == '.' || mail[tamanho_parte_local - 1] == '.') valido = FALSE; // O '.' não pode ser o primeiro ou último caractere da parte local do email
        else {
            // O '.' não pode ser seguido de outro '.'
            if (vetor_contem_elemento(mail, tamanho_parte_local, &ponto, CHAR)) {
                if (vetor_contem_elemento(mail + 1, tamanho_parte_local - 1, &ponto, CHAR)) valido = FALSE; 
            } else {
                // Verifica se a parte local do email contem apenas caracteres válidos
                if (!vetor_contem_elemento(mail, tamanho_parte_local, caracteres_validos_parte_local, CHAR)) valido = FALSE;
            }
        }
    }

    return valido;
}

/**
 * @brief Valida a parte do domínio do email segundo as regras do RFC 5322
 * 
 * @param email 
 * @param tamanho_email 
 * @param posicao_arroba 
 * @return bool_t 
 */
bool_t email_parte_dominio_valida(char* email, int tamanho_email, int posicao_arroba) {
    bool_t valido = TRUE;
    int tamanho_parte_dominio = tamanho_email - posicao_arroba - 1;
    char caracteres_validos_parte_dominio[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.";
    char ponto = '.';

    if (tamanho_parte_dominio < 1 || tamanho_parte_dominio > 255) valido = FALSE; // A parte do domínio do email deve ter entre 1 e 255 caracteres
    else {
        // Verifica se a parte do domínio do email contem apenas caracteres válidos
        if (!vetor_contem_elemento(email + posicao_arroba + 1, tamanho_parte_dominio, caracteres_validos_parte_dominio, CHAR)) valido = FALSE;
        else {
            // Verifica se a parte do domínio do email tem pelo menos um ponto
            if (!vetor_contem_elemento(email + posicao_arroba + 1, tamanho_parte_dominio, &ponto, CHAR)) valido = FALSE;
            else {
                // Verifica se a parte do domínio do email tem pelo menos um label com pelo menos 2 caracteres
                if (!vetor_contem_elemento(email + posicao_arroba + 2, tamanho_parte_dominio - 1, &ponto, CHAR)) valido = FALSE;
                else {
                    // Verifica se a parte do domínio do email não tem um ponto no início ou no fim
                    if (email[posicao_arroba + 1] == '.' || email[tamanho_email - 1] == '.') valido = FALSE;
                }
            }
        }
    }

    return valido;
}


/**
 * @brief Verifica se um determinado ano é bissexto
 * 
 * @param ano 
 * @return bool_t 
 */
bool_t ano_bissexto(int ano) {
    bool_t bissexto = FALSE;

    if (ano % 4 == 0) {
        if (ano % 100 == 0) {
            if (ano % 400 == 0) bissexto = TRUE;
        } else bissexto = TRUE;
    }

    return bissexto;
}


/**
 * @brief Devolve o numero de dias de um determinado mes de um determinado ano
 * 
 * @param mes 
 * @param ano_bissexto 
 * @return int 
 */
inline_ int dias_mes(int mes, int ano) {
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2 && ano_bissexto(ano)) dias[1] = 29;
    return dias[mes - 1];
}



/**
 * @brief Devolve a posicao da primeira ocurrencia de um caracter numa string (ou -1 se nao existir)
 * 
 * @param caractere 
 * @param string 
 * @param tamanho_string 
 * @return inline_ 
 */
inline_ int posicao_char_na_string(char caractere, char* string, int tamanho_string) {
    int posicao = -1, indice;

    for (indice = 0; indice < tamanho_string; indice++) {
        if (string[indice] == caractere) {
            posicao = indice;
            break;
        }
    }

    return posicao;
}

/* ========================================================== */
/* =                      DATA & HORA                       = */
/* ========================================================== */

/**
 * @brief Função que retorna a data atual no formato definido na constante DATA_FORMATO
 * @return
 */
char* obter_data_atual() {
    time_t tempo = time(NULL);
    struct tm* data = localtime(&tempo);
    char* data_atual = (char*) malloc(sizeof(char) * 11);
    sprintf(data_atual, DATA_FORMATO, data->tm_mday, data->tm_mon + 1, data->tm_year + 1900); // Formato DD/MM/AAAA
    return data_atual;
}

/**
 * @brief Função que retorna a hora atual no formato definido na constante HORA_FORMATO_COMPLETO
 * @return
 */
char* obter_hora_atual_com_segundos() {
    time_t tempo = time(NULL);
    struct tm* data = localtime(&tempo);
    char* hora_atual = (char*) malloc(sizeof(char) * 9);
    sprintf(hora_atual, HORA_FORMATO_COMPLETO, data->tm_hour, data->tm_min, data->tm_sec);
    return hora_atual;
}

/* ========================================================== */

void string_para_minusculas(char* string) {
    int indice;
    for (indice = 0; indice < strlen(string); indice++)
        string[indice] = tolower(string[indice]);
}

void string_para_maiusculas(char* string) {
    int indice;
    for (indice = 0; indice < strlen(string); indice++)
        string[indice] = toupper(string[indice]);
}

void esperar_tecla(const char* mensagem) {
    if (mensagem != NULL) printf("%s", mensagem);
    fflush(stdin);
    getchar();
}



/* ========================================================== */
/* =                UTILITARIOS DE PESQUISA                 = */
/* ========================================================== */

// NOTA:
//
// A utilizacao de early returns iria facilitar a implementacao, porem nao sao permitidos
// de acordo com as recomendações do docente

int procurar_participante_por_id(int id, estado_programa_t* estado_programa) {
    int indice;
    bool_t encontrado = FALSE;      
    
    for (indice = 0; indice < *estado_programa->numero_de_participantes; indice++) {
        if (estado_programa->participantes[indice]->identificador == id) encontrado = TRUE;
    }
    
    return encontrado ? indice : ERRO;
}

int procurar_atividade_por_id(int id, estado_programa_t* estado_programa) {
    int indice;
    bool_t encontrado = FALSE;
    
    for (indice = 0; indice < *estado_programa->numero_de_atividades; indice++) {
        if (estado_programa->atividades[indice]->identificador == id) encontrado = TRUE;
    }

    return encontrado ? indice : ERRO;
}

int procurar_inscricao_por_id(int id, estado_programa_t* estado_programa) {
    int indice;
    bool_t encontrado = FALSE;

    for (indice = 0; indice < *estado_programa->numero_de_inscricoes; indice++) {
        if (estado_programa->inscricoes[indice]->identificador == id) encontrado = TRUE;
    }

    return encontrado ? indice : ERRO;
}



/* ========================================================== */
/* =                         MENUS                          = */
/* ========================================================== */

/**
 * @brief Funcao que le a opcao do menu principal
 * 
 * A funcao main chama esta funcao para ler a opcao do menu principal.
 * 
 * @return opcao_menu_principal_t 
 */
opcao_menu_principal_t menu_principal() {
    int opcao;

    do {
        limpar_ecra();

        printf("Menu Principal\n");
        printf("1. Participantes\n");
        printf("2. Atividades\n");
        printf("3. Inscricoes\n");
        printf("4. Estatisticas\n");
        printf("5. Salvar\n");
        printf("6. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 6) {
            printf("Opcao '%d' invalida. Tente novamente.\n", opcao);
            esperar_tecla(NULL);
        } 
    } while (opcao < 1 || opcao > 6);

    return (opcao_menu_principal_t) opcao;
}

// TODO: Criar uma funcao mais generica para ler opcoes de menus
opcao_menu_participantes_t ler_opcao_menu_participantes() {
    int opcao;

    do {
        limpar_ecra();

        printf("Menu Participantes\n");
        printf("1. Inserir Participante\n");
        printf("2. Listar Participantes\n");
        printf("3. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 3) {
            printf("Opcao '%d' invalida. Tente novamente.\n", opcao);
            esperar_tecla(NULL);
        } 
    } while (opcao < 1 || opcao > 3);

    return (opcao_menu_participantes_t) opcao;
}

opcao_menu_atividades_t ler_opcao_menu_atividades() {
    int opcao;

    do {
        limpar_ecra();

        printf("Menu Atividades\n");
        printf("1. Inserir Atividade\n");
        printf("2. Listar Atividades\n");
        printf("3. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 3) {
            printf("Opcao '%d' invalida. Tente novamente.\n", opcao);
            esperar_tecla(NULL);
        } 
    } while (opcao < 1 || opcao > 3);

    return (opcao_menu_atividades_t) opcao;
}

opcao_menu_inscricoes_t ler_opcao_menu_inscricoes() {
    int opcao;

    do {
        limpar_ecra();

        printf("Menu Inscricoes\n");
        printf("1. Inserir Inscricao\n");
        printf("2. Listar Inscricoes\n");
        printf("3. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 3) {
            printf("Opcao '%d' invalida. Tente novamente.\n", opcao);
            esperar_tecla(NULL);
        } 
    } while (opcao < 1 || opcao > 3);

    return (opcao_menu_inscricoes_t) opcao;
}

opcao_menu_estatisticas_t ler_opcao_menu_estatisticas() {
    int opcao;

    do {
        limpar_ecra();

        printf("Menu Estatisticas\n");
        printf("1. Numero de atividades por associacao\n");
        printf("2. Percentagem de inscricoes por escola\n");
        printf("3. Valor total angariado em horizonte temporal\n");
        printf("4. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 4) {
            printf("Opcao '%d' invalida. Tente novamente.\n", opcao);
            esperar_tecla(NULL);
        } 
    } while (opcao < 1 || opcao > 4);

    return (opcao_menu_estatisticas_t) opcao;
}


/* ========================================================== */


/**
 * @brief Funcao que orquestra a chamada de funcoes para o menu de participantes
 * 
 * @param estado_programa 
 */
void menu_participantes(estado_programa_t* estado_programa) {
    opcao_menu_participantes_t opcao;
    
    do
    {
        opcao = ler_opcao_menu_participantes();
        switch (opcao)
        {
        case INSERIR_PARTICIPANTE:
            inserir_participante(estado_programa);
            break;
        case LISTAR_PARTICIPANTES:
            mostrar_participantes(estado_programa);
            break;
        case VOLTAR_MENU_PRINCIPAL_PARTICIPANTES:
            break;
        }
    } while (opcao != VOLTAR_MENU_PRINCIPAL_PARTICIPANTES);
}

/**
 * @brief Funcao que orquestra a chamada de funcoes para o menu de atividades
 * 
 * @param estado_programa 
 */
void menu_atividades(estado_programa_t* estado_programa) {
    opcao_menu_atividades_t opcao;
    
    do
    {
        opcao = ler_opcao_menu_atividades();
        switch (opcao)
        {
        case INSERIR_ATIVIDADE:
            inserir_atividade(estado_programa);
            break;
        case LISTAR_ATIVIDADES:
            mostrar_atividades(estado_programa);
            break;
        case VOLTAR_MENU_PRINCIPAL_ATIVIDADES:
            break;
        }
    } while (opcao != VOLTAR_MENU_PRINCIPAL_ATIVIDADES);
}

/**
 * @brief Funcao que orquestra a chamada de funcoes para o menu de inscricoes
 * 
 * @param estado_programa 
 */
void menu_inscricoes(estado_programa_t* estado_programa) {
    opcao_menu_inscricoes_t opcao;
    
    do
    {
        opcao = ler_opcao_menu_inscricoes();
        switch (opcao)
        {
        case INSERIR_INSCRICAO:
            inserir_inscricao(estado_programa);
            break;
        case LISTAR_INSCRICOES:
            mostrar_inscricoes(estado_programa);
            break;
        case VOLTAR_MENU_PRINCIPAL_INSCRICOES:
            break;
        }
    } while (opcao != VOLTAR_MENU_PRINCIPAL_INSCRICOES);
}

void menu_estatisticas(estado_programa_t* estado_programa) {
    opcao_menu_estatisticas_t opcao;

    do
    {
        opcao = ler_opcao_menu_estatisticas();
        switch (opcao)
        {
        case NUMERO_DE_ATIVIDADES_POR_ASSOCIACAO:
            break;
        case PERCENTAGEM_DE_INSCRICOES_POR_ESCOLA:
            break;
        case VALOR_TOTAL_DAS_INSCRICOES_EM_HORIZONTE_TEMPORAL:
            break;
        case VOLTAR_MENU_PRINCIPAL_ESTATISTICAS:
            break;
        }
    } while (opcao != VOLTAR_MENU_PRINCIPAL_ESTATISTICAS);
}
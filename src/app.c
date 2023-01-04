//
// Created by João Matos on 17/12/2022.
//

#include <stdarg.h>

#include "../include/app.h"

/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

static void libertar_memoria(void* ponteiro, ...);
static int  tamanho_vetor(void** vetor);            // Retorna o tamanho de um vetor (percorre o vetor até encontrar um elemento nulo)

static void guardar_participantes(t_estado_programa*, FILE*);
static void guardar_atividades(t_estado_programa*, FILE*);
static void guardar_inscricoes(t_estado_programa*, FILE*);

static void carregar_participantes(t_estado_programa*, FILE*);
static void carregar_atividades(t_estado_programa*, FILE*);
static void carregar_inscricoes(t_estado_programa*, FILE*);


/* ========================================================== */
/* =                      ESTRUTURAS                        = */
/* ========================================================== */

/// PARTICIPANTE ///

/**
 * @brief Cria um participante
 *
 * @param identificador
 * @param nome
 * @param escola
 * @param nif
 * @param email
 * @param telefone
 * @return t_participante*
 */
t_participante* criar_participante(int identificador, char* nome, char* escola, int nif, char* email, int telefone) {
    t_participante* participante = (t_participante*) malloc(sizeof(t_participante));
    participante->identificador = identificador;
    participante->nome = nome;
    participante->escola = escola;
    participante->nif = nif;
    participante->email = email;
    participante->telefone = telefone;
    return participante;
}

/**
 * @brief Liberta a memória alocada para um participante
 *
 * @param participante
 */
void libertar_participante(t_participante* participante) {
    free(participante);
}


/**
 * @brief Mostra um participante
 *
 * @param participante
 */
void mostrar_participante(t_participante* participante) {
    printf("    Identificador: %d\n", participante->identificador);
    printf("    Nome: %s\n", participante->nome);
    printf("    Escola: %s\n", participante->escola);
    printf("    NIF: %d\n", participante->nif);
    printf("    E-mail: %s\n", participante->email);
    printf("    Telefone: %d\n\n", participante->telefone);
}

/// ATIVIDADE ///

/**
 * @brief Cria uma atividade
 * @param identificador
 * @param designacao
 * @param data
 * @param hora
 * @param local
 * @param tipo
 * @param associacao_estudantes
 * @param valor
 * @return t_atividade*
 */
t_atividade* criar_atividade(int identificador, char* designacao, char* data, char* hora, char* local, char* tipo, char* associacao_estudantes, float valor) {
    t_atividade* atividade = (t_atividade*) malloc(sizeof(t_atividade));
    atividade->identificador = identificador;
    atividade->designacao = designacao;
    atividade->data = data;
    atividade->hora = hora;
    atividade->local = local;
    atividade->tipo = tipo;
    atividade->associacao_estudantes = associacao_estudantes;
    atividade->valor = valor;
    return atividade;
}

/**
 * @brief Liberta a memória alocada para uma atividade
 * @param atividade
 */
void libertar_atividade(t_atividade* atividade) {
    libertar_memoria(atividade->designacao, atividade->data, atividade->hora, atividade->local, atividade->tipo, atividade->associacao_estudantes, NULL);
}

/**
 * @brief Mostra uma atividade
 * @param atividade
 */
void mostrar_atividade(t_atividade* atividade) {
    printf("    Identificador: %d\n", atividade->identificador);
    printf("    Designação: %s\n", atividade->designacao);
    printf("    Data: %s\n", atividade->data);
    printf("    Hora: %s\n", atividade->hora);
    printf("    Local: %s\n", atividade->local);
    printf("    Tipo: %s\n", atividade->tipo);
    printf("    Associação de Estudantes: %s\n", atividade->associacao_estudantes);
    printf("    Valor: %.2f\n", atividade->valor);
}

/// INSCRIÇÃO ///

/**
 * @brief Cria uma inscrição
 * @param identificador
 * @param participante
 * @param atividade
 * @return t_inscricao*
 * @return t_inscricao*
 */
t_inscricao* criar_inscricao(int identificador, int id_participante, int id_atividade, t_atividade** atividades) {
    int indice_procura;
    int numero_atividades = tamanho_vetor((void**) atividades);

    t_inscricao* inscricao = (t_inscricao*) malloc(sizeof(t_inscricao));
    inscricao->identificador = identificador;
    inscricao->id_participante = id_participante;
    inscricao->id_atividade = id_atividade;

    // Procurar o valor associado à atividade com o ID passado
    indice_procura = procurar_atividade_por_id(atividades, numero_atividades, id_atividade);
    if (indice_procura == -1) {
        return NULL;
    }

    inscricao->valor_pago = atividades[indice_procura]->valor;
    inscricao->data = obter_data_atual();
    inscricao->hora = obter_hora_atual_completa();
    return inscricao;
}

/**
 * @brief Liberta a memória alocada para uma inscrição
 * @param inscricao
 */
void libertar_inscricao(t_inscricao* inscricao) {
    free(inscricao);
}

/**
 * @brief Mostra uma inscrição
 * @param inscricao
 */
void mostrar_inscricao(t_inscricao* inscricao) {
    printf("    Identificador: %d\n", inscricao->identificador);
    printf("    ID do Participante: %d\n", inscricao->id_participante);
    printf("    ID da Atividade: %d\n", inscricao->id_atividade);
    printf("    Valor Pago: %.2f\n", inscricao->valor_pago);
    printf("    Data: %s\n", inscricao->data);
    printf("    Hora: %s\n", inscricao->hora);
}

/* ========================================================== */

/// ESTADO DO PROGRAMA ///

t_estado_programa* criar_estado_programa(t_participante** vetor_participantes, t_atividade** vetor_atividades,
                                         t_inscricao** vetor_inscricoes, int* contador_participantes,
                                         int* contador_atividades, int* contador_inscricoes, const char* cor_texto, const char* cor_fundo)
{
    t_estado_programa* estado_programa = (t_estado_programa*) malloc(sizeof(t_estado_programa));
    estado_programa->participantes = vetor_participantes;
    estado_programa->atividades = vetor_atividades;
    estado_programa->inscricoes = vetor_inscricoes;
    estado_programa->numero_participantes_inseridos = contador_participantes;
    estado_programa->numero_atividadades_inseridas = contador_atividades;
    estado_programa->numero_de_inscricoes = contador_inscricoes;
    estado_programa->ultimo_save = 0;
    strcpy(estado_programa->cor_texto, cor_texto);
    strcpy(estado_programa->cor_fundo, cor_fundo);
    return estado_programa;
}

void libertar_estado_programa(t_estado_programa* estado_programa) {
    for (int i = 0; i < *estado_programa->numero_participantes_inseridos; i++)
        libertar_participante(estado_programa->participantes[i]);

    for (int i = 0; i < *estado_programa->numero_atividadades_inseridas; i++)
        libertar_atividade(estado_programa->atividades[i]);

    for (int i = 0; i < *estado_programa->numero_de_inscricoes; i++)
        libertar_inscricao(estado_programa->inscricoes[i]);

    free(estado_programa->participantes);
    free(estado_programa->atividades);
    free(estado_programa->inscricoes);
    free(estado_programa->numero_participantes_inseridos);
    free(estado_programa->numero_atividadades_inseridas);
    free(estado_programa->numero_de_inscricoes);
    free(estado_programa);
}


void mostrar_estado_programa(t_estado_programa* estado_programa) {
    printf("Participantes:\n");
    for (int i = 0; i < *estado_programa->numero_participantes_inseridos; i++) {
        mostrar_participante(estado_programa->participantes[i]);
    }
    printf("Atividades:\n");
    for (int i = 0; i < *estado_programa->numero_atividadades_inseridas; i++) {
        mostrar_atividade(estado_programa->atividades[i]);
    }
    printf("Inscrições:\n");
    for (int i = 0; i < *estado_programa->numero_de_inscricoes; i++) {
        mostrar_inscricao(estado_programa->inscricoes[i]);
    }

    printf("Número de participantes inseridos: %d\n", *estado_programa->numero_participantes_inseridos);
    printf("Número de atividades inseridas: %d\n", *estado_programa->numero_atividadades_inseridas);
    printf("Número de inscrições: %d\n", *estado_programa->numero_de_inscricoes);
    printf("Último save: %d\n\n", estado_programa->ultimo_save);
}

int guardar_estado_programa(char* nome_ficheiro, t_estado_programa* estado) {
    FILE* ficheiro = abrir_ficheiro(nome_ficheiro, ESCRITA_BINARIA);
    if (ficheiro == NULL) {
        return ERRO;
    }

    guardar_atividades(estado, ficheiro);
    guardar_participantes(estado, ficheiro);
    guardar_inscricoes(estado, ficheiro);

    fwrite(&estado->ultimo_save, sizeof(int), 1, ficheiro); // Guardar o último save
    fwrite(estado->cor_texto, sizeof(char), 1, ficheiro);   // Guardar a cor do texto
    fwrite(estado->cor_fundo, sizeof(char), 1, ficheiro);   // Guardar a cor do fundo

    fechar_ficheiro(ficheiro);
    return OK;
}

t_estado_programa* carregar_estado_programa(char* nome_ficheiro) {
    t_estado_programa *estado_programa = malloc(sizeof(t_estado_programa));
    estado_programa->participantes = malloc(sizeof(t_participante*));
    estado_programa->atividades = malloc(sizeof(t_atividade*));
    estado_programa->inscricoes = malloc(sizeof(t_inscricao*));

    FILE* ficheiro = abrir_ficheiro(nome_ficheiro, LEITURA_BINARIA);
    if (ficheiro == NULL) {
        return NULL;
    }

    carregar_atividades(estado_programa, ficheiro);
    carregar_participantes(estado_programa, ficheiro);
    carregar_inscricoes(estado_programa, ficheiro);
    fread(&estado_programa->ultimo_save, sizeof(int), 1, ficheiro); // Carregar o último save
    fread(estado_programa->cor_texto, sizeof(char), 1, ficheiro);   // Carregar a cor do texto
    fread(estado_programa->cor_fundo, sizeof(char), 1, ficheiro);   // Carregar a cor do fundo

    fechar_ficheiro(ficheiro);
    return estado_programa;
}

/* ========================================================== */

/* ========================================================== */
/* =                UTILITÁRIOS DE PESQUISA                 = */
/* ========================================================== */

int procurar_participante_por_id(t_participante** participantes, int numero_participantes, int id_procurado) {
    for (int i = 0; i < numero_participantes; i++) {
        if (participantes[i]->identificador == id_procurado) {
            return i;
        }
    }
    return -1;
}

int procurar_atividade_por_id(t_atividade** atividades, int numero_atividades, int id_procurado) {
    for (int i = 0; i < numero_atividades; i++) {
        if (atividades[i]->identificador == id_procurado) {
            return i;
        }
    }
    return -1;
}

int procurar_inscricao_por_id(t_inscricao** inscricoes, int numero_inscricoes, int id_procurado) {
    for (int i = 0; i < numero_inscricoes; i++) {
        if (inscricoes[i]->identificador == id_procurado) {
            return i;
        }
    }
    return -1;
}

int procurar_participante_por_nif(t_participante** participantes, int numero_participantes, int nif_procurado) {
    for (int i = 0; i < numero_participantes; i++) {
        if (participantes[i]->nif == nif_procurado) {
            return i;
        }
    }
    return -1;
}

int procurar_participante_por_email(t_participante** participantes, int numero_participantes, char* email_procurado) {
    for (int i = 0; i < numero_participantes; i++) {
        if (strcmp(participantes[i]->email, email_procurado) == 0) {
            return i;
        }
    }
    return -1;
}

int procurar_participante_por_telefone(t_participante** participantes, int numero_participantes, int telefone_procurado) {
    for (int i = 0; i < numero_participantes; i++) {
        if (participantes[i]->telefone == telefone_procurado) {
            return i;
        }
    }
    return -1;
}

/* ========================================================== */

static void libertar_memoria(void* ponteiro, ...) {
    va_list lista;
    va_start(lista, ponteiro);

    free(ponteiro);

    while (ponteiro != NULL) {
        ponteiro = va_arg(lista, void*);
        free(ponteiro);
    }

    va_end(lista);
}

static int tamanho_vetor(void** vetor) {
    int tamanho = 0;
    while (vetor[tamanho] != NULL) {
        tamanho++;
    }
    return tamanho;
}

static void guardar_participantes(t_estado_programa* estado, FILE* ficheiro) {
    // Guardar o número de participantes inseridos
    fwrite(*&estado->numero_participantes_inseridos, sizeof(int), 1, ficheiro);

    // Guardar os participantes
    for (int i = 0; i < *estado->numero_participantes_inseridos; i++) {
        fwrite(estado->participantes[i], sizeof(t_participante), 1, ficheiro);
    }
}

static void guardar_atividades(t_estado_programa* estado, FILE* ficheiro) {
    // Guardar o número de atividades inseridas
    fwrite(*&estado->numero_atividadades_inseridas, sizeof(int), 1, ficheiro);

    // Guardar as atividades
    for (int i = 0; i < *estado->numero_atividadades_inseridas; i++) {
        fwrite(estado->atividades[i], sizeof(t_atividade), 1, ficheiro);
    }
}

static void guardar_inscricoes(t_estado_programa* estado, FILE* ficheiro) {
    // Guardar o número de inscrições
    fwrite(*&estado->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Guardar as inscrições
    for (int i = 0; i < *estado->numero_de_inscricoes; i++) {
        fwrite(estado->inscricoes[i], sizeof(t_inscricao), 1, ficheiro);
    }
}

static void carregar_participantes(t_estado_programa* estado, FILE* ficheiro) {
    // Carregar o número de participantes inseridos
    estado->numero_participantes_inseridos = malloc(sizeof(int));
    fread(estado->numero_participantes_inseridos, sizeof(int), 1, ficheiro);

    // Carregar os participantes
    for (int i = 0; i < *estado->numero_participantes_inseridos; i++) {
        estado->participantes[i] = malloc(sizeof(t_participante));
        fread(estado->participantes[i], sizeof(t_participante), 1, ficheiro);
    }
}

static void carregar_atividades(t_estado_programa* estado, FILE* ficheiro) {
    // Carregar o número de atividades inseridas
    estado->numero_atividadades_inseridas = malloc(sizeof(int));
    fread(estado->numero_atividadades_inseridas, sizeof(int), 1, ficheiro);

    // Carregar as atividades
    for (int i = 0; i < *estado->numero_atividadades_inseridas; i++) {
        estado->atividades[i] = malloc(sizeof(t_atividade));
        fread(estado->atividades[i], sizeof(t_atividade), 1, ficheiro);
    }
}

static void carregar_inscricoes(t_estado_programa* estado, FILE* ficheiro) {
    // Carregar o número de inscrições
    estado->numero_de_inscricoes = malloc(sizeof(int));
    fread(estado->numero_de_inscricoes, sizeof(int), 1, ficheiro);

    // Carregar as inscrições
    for (int i = 0; i < *estado->numero_de_inscricoes; i++) {
        estado->inscricoes[i] = malloc(sizeof(t_inscricao));
        fread(estado->inscricoes[i], sizeof(t_inscricao), 1, ficheiro);
    }
}
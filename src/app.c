//
// Created by João Matos on 17/12/2022.
//

#include <stdarg.h>

#include "../include/app.h"

/* ========================================================== */
/* =                      PROTÓTIPOS                        = */
/* ========================================================== */

static void libertar_memoria(void* ponteiro, ...);
static void concatenar_strings(char* destino, ...);
static char* obter_localizacao_save();                  // Obtem a localização do save do programa a partir do ficheiro de configurações
static int  salvar_localizacao_save(char* localizacao); // Guarda a localização do save do programa no ficheiro de configurações


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
    free(participante->nome);
    free(participante->escola);
    free(participante->email);
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

    t_inscricao* inscricao = (t_inscricao*) malloc(sizeof(t_inscricao));
    inscricao->identificador = identificador;
    inscricao->id_participante = id_participante;
    inscricao->id_atividade = id_atividade;

    // Procurar o valor associado à atividade com o ID passado
    indice_procura = procurar_atividade_por_id(atividades, id_atividade);
    if (indice_procura == -1) {
        return NULL;
    }

    inscricao->valor_pago = atividades[indice_procura]->valor;

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

    int numero_participantes = *estado->numero_participantes_inseridos;
    int numero_atividades = *estado->numero_atividadades_inseridas;
    int numero_inscricoes = *estado->numero_de_inscricoes;


    // Guardar o número de participantes inseridos
    fwrite(&numero_participantes, sizeof(int), 1, ficheiro);

    // Guardar os participantes
    for (int i = 0; i < *estado->numero_participantes_inseridos; i++) {
        fwrite(estado->participantes[i], sizeof(t_participante), 1, ficheiro);
    }

    // Guardar o número de atividades inseridas
    fwrite(&numero_atividades, sizeof(int), 1, ficheiro);

    // Guardar as atividades
    for (int i = 0; i < *estado->numero_atividadades_inseridas; i++) {
        fwrite(estado->atividades[i], sizeof(t_atividade), 1, ficheiro);
    }

    // Guardar o número de inscrições
    fwrite(&numero_inscricoes, sizeof(int), 1, ficheiro);

    // Guardar as inscrições
    for (int i = 0; i < *estado->numero_de_inscricoes; i++) {
        fwrite(estado->inscricoes[i], sizeof(t_inscricao), 1, ficheiro);
    }

    // Guardar o último save
    fwrite(&estado->ultimo_save, sizeof(int), 1, ficheiro);

    // Guardar a cor do texto
    fwrite(estado->cor_texto, sizeof(char), 1, ficheiro);

    // Guardar a cor do fundo
    fwrite(estado->cor_fundo, sizeof(char), 1, ficheiro);

    fechar_ficheiro(ficheiro);
    return OK;
}

t_estado_programa* carregar_estado_programa(char* nome_ficheiro) {
    t_estado_programa *estado_programa = malloc(sizeof(t_estado_programa));
    t_participante **participantes = malloc(sizeof(t_participante*));
    t_atividade **atividades = malloc(sizeof(t_atividade*));
    t_inscricao **inscricoes = malloc(sizeof(t_inscricao*));
    int numero_participantes;
    int numero_atividades;
    int numero_inscricoes;

    FILE* ficheiro = abrir_ficheiro(nome_ficheiro, LEITURA_BINARIA);
    if (ficheiro == NULL) {
        return NULL;
    }

    // Carregar o número de participantes inseridos
    fread(&numero_participantes, sizeof(int), 1, ficheiro);

    // Carregar os participantes
    for (int i = 0; i < numero_participantes; i++) {
        participantes[i] = malloc(sizeof(t_participante));
        fread(participantes[i], sizeof(t_participante), 1, ficheiro);
    }

    // Carregar o número de atividades inseridas
    fread(&numero_atividades, sizeof(int), 1, ficheiro);

    // Carregar as atividades
    for (int i = 0; i < numero_atividades; i++) {
        atividades[i] = malloc(sizeof(t_atividade));
        fread(atividades[i], sizeof(t_atividade), 1, ficheiro);
    }

    // Carregar o número de inscrições
    fread(&numero_inscricoes, sizeof(int), 1, ficheiro);

    // Carregar as inscrições
    for (int i = 0; i < numero_inscricoes; i++) {
        inscricoes[i] = malloc(sizeof(t_inscricao));
        fread(inscricoes[i], sizeof(t_inscricao), 1, ficheiro);
    }

    // Carregar o último save
    fread(&estado_programa->ultimo_save, sizeof(int), 1, ficheiro);

    // Carregar a cor do texto
    fread(estado_programa->cor_texto, sizeof(char), 1, ficheiro);

    // Carregar a cor do fundo
    fread(estado_programa->cor_fundo, sizeof(char), 1, ficheiro);

    estado_programa->participantes = participantes;
    estado_programa->atividades = atividades;
    estado_programa->inscricoes = inscricoes;
    estado_programa->numero_participantes_inseridos = &numero_participantes;
    estado_programa->numero_atividadades_inseridas = &numero_atividades;
    estado_programa->numero_de_inscricoes = &numero_inscricoes;

    fechar_ficheiro(ficheiro);
    return estado_programa;
}

/* ========================================================== */





/* ========================================================== */
/* =                UTILITÁRIOS DE PESQUISA                 = */
/* ========================================================== */

int procurar_participante_por_id(t_participante** participantes, int id_procurado) {
    int i = 0;

    while (participantes[i] != NULL) {
        if (participantes[i]->identificador == id_procurado) {
            return i;
        }
        i++;
    }

    return -1;
}

int procurar_atividade_por_id(t_atividade** atividades, int id_procurado) {
    int i = 0;

    while (atividades[i] != NULL) {
        if (atividades[i]->identificador == id_procurado) {
            return i;
        }
        i++;
    }

    return -1;
}

int procurar_inscricao_por_id(t_inscricao** inscricoes, int id_procurado) {
    int i = 0;

    while (inscricoes[i] != NULL) {
        if (inscricoes[i]->identificador == id_procurado) {
            return i;
        }
        i++;
    }

    return -1;
}

int procurar_participante_por_nif(t_participante** participantes, int nif_procurado) {
    int i = 0;

    while (participantes[i] != NULL) {
        if (participantes[i]->nif == nif_procurado) {
            return i;
        }
        i++;
    }

    return -1;
}

int procurar_participante_por_email(t_participante** participantes, char* email_procurado) {
    int i = 0;

    while (participantes[i] != NULL) {
        if (strcmp(participantes[i]->email, email_procurado) == 0) {
            return i;
        }
        i++;
    }

    return -1;
}

int procurar_participante_por_telefone(t_participante** participantes, int telefone_procurado) {
    int i = 0;

    while (participantes[i] != NULL) {
        if (participantes[i]->telefone == telefone_procurado) {
            return i;
        }
        i++;
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

static void concatenar_strings(char* destino, ...) {
    va_list lista;
    va_start(lista, destino);

    char* string = va_arg(lista, char*);

    while (string != NULL) {
        strcat(destino, string);
        string = va_arg(lista, char*);
    }

    va_end(lista);
}
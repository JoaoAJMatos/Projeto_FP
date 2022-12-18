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
__inline__ static int tamanho_vetor_bencode(char*);     // Devolve o tamanho de um vetor serializado em bencode


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
                                         int* contador_atividades, int* contador_inscricoes, char* cor_texto, char* cor_fundo)
{
    t_estado_programa* estado_programa = (t_estado_programa*) malloc(sizeof(t_estado_programa));
    estado_programa->participantes = vetor_participantes;
    estado_programa->atividades = vetor_atividades;
    estado_programa->inscricoes = vetor_inscricoes;
    estado_programa->numero_participantes_inseridos = contador_participantes;
    estado_programa->numero_atividadades_inseridas = contador_atividades;
    estado_programa->numero_de_inscricoes = contador_inscricoes;
    estado_programa->ultimo_save = 0;
    estado_programa->cor_texto = cor_texto;
    estado_programa->cor_fundo = cor_fundo;
    return estado_programa;
}

void libertar_estado_programa(t_estado_programa* estado_programa) {
    free(estado_programa);
}

char* serializar_estado_programa(t_estado_programa* estado_programa) {
    char* vetor_participantes_serializado = serializar_participantes(estado_programa->participantes, *estado_programa->numero_participantes_inseridos);
    char* vetor_atividades_serializado = serializar_atividades(estado_programa->atividades, *estado_programa->numero_atividadades_inseridas);
    char* vetor_inscricoes_serializado = serializar_inscricoes(estado_programa->inscricoes, *estado_programa->numero_de_inscricoes);
    char* numero_participantes_inseridos_serializado = serializar_int(*estado_programa->numero_participantes_inseridos);
    char* numero_atividades_inseridas_serializado = serializar_int(*estado_programa->numero_atividadades_inseridas);
    char* numero_de_inscricoes_serializado = serializar_int(*estado_programa->numero_de_inscricoes);
    char* ultimo_save_serializado = serializar_int(estado_programa->ultimo_save);

    char* estado_programa_serializado = (char*) malloc(sizeof(char) * (strlen(vetor_participantes_serializado)
                                        + strlen(vetor_atividades_serializado)
                                        + strlen(vetor_inscricoes_serializado)
                                        + strlen(numero_participantes_inseridos_serializado)
                                        + strlen(numero_atividades_inseridas_serializado)
                                        + strlen(numero_de_inscricoes_serializado)
                                        + strlen(ultimo_save_serializado) + 1));

    strcpy(estado_programa_serializado, vetor_participantes_serializado);
    concatenar_strings(estado_programa_serializado, vetor_atividades_serializado,
                       vetor_inscricoes_serializado, numero_participantes_inseridos_serializado, numero_atividades_inseridas_serializado,
                       numero_de_inscricoes_serializado, ultimo_save_serializado, NULL);

    return estado_programa_serializado;
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

// TODO: Fazer a deserialização do estado do programa
t_estado_programa* deserializar_estado_programa(char* estado_programa_serializado) {
    int numero_participantes_inseridos = 0, numero_atividades_inseridas = 0, numero_de_inscricoes = 0, ultimo_save = 0;
    t_participante** participantes;
    t_atividade** atividades;
    t_inscricao** inscricoes;
    t_estado_programa *estado_programa = NULL;

    // Deserializar participantes
    numero_participantes_inseridos = tamanho_vetor_bencode(estado_programa_serializado);
    participantes = (t_participante**) malloc(sizeof(t_participante*) * numero_participantes_inseridos);
    printf("\n\nNúmero de participantes inseridos: %d\n", numero_participantes_inseridos);

    for (int i = 0; i < numero_participantes_inseridos; i++) {
        participantes[i] = deserializar_participante(estado_programa_serializado);
        estado_programa_serializado = estado_programa_serializado + strlen(estado_programa_serializado) + 1;
    }

    for (int i = 0; i < numero_participantes_inseridos; i++) {
        mostrar_participante(participantes[i]);
    }

    return estado_programa;
}

int guardar_estado_programa(t_estado_programa* estado) {
    FILE* ficheiro;
    char caminho_string[1024], caminho_string_final[1024];
    char* caminho_para_ficheiro;
    t_caminho* caminho = NULL;
    char* estado_programa_serializado = serializar_estado_programa(estado);

    caminho_para_ficheiro = obter_localizacao_save();
    if (caminho_para_ficheiro == NULL) {
        do {
            prompt("Nenhum save foi detetado", "Insira o caminho para o ficheiro onde o estado do programa deve ser guardado",
                   "Caminho relativo/absoluto", caminho_string, STRING, estado->cor_texto, estado->cor_fundo);
            caminho = caminho_criar_a_partir_de_string(caminho_string);
        } while (caminho->erro != CAMINHO_OK);
    }

    ficheiro = abrir_ficheiro(caminho_para_ficheiro, ESCRITA_BINARIA);
    if (ficheiro == NULL) {
        return ERRO;
    }

    fwrite(estado_programa_serializado, sizeof(char), strlen(estado_programa_serializado), ficheiro);
    fechar_ficheiro(ficheiro);

    return OK;
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



/* ========================================================== */
/* =                     SERIALIZAÇÃO                       = */
/* ========================================================== */

/// SERIALIZAÇÃO ///

/// Serialização de tipos primitivos ///

char* serializar_string(char* string) {
    int tamanho = strlen(string);
    char* string_serializada = malloc(tamanho + sizeof (int) + 1);

    sprintf(string_serializada, "%d:%s", tamanho, string);

    return string_serializada;
}

char* serializar_int(int numero) {
    char* numero_serializado = malloc(sizeof(char) * 15);

    sprintf(numero_serializado, "i%de", numero);

    return numero_serializado;
}

char* serializar_float(float numero) {
    char* numero_serializado = malloc(sizeof(char) * 15);
    sprintf(numero_serializado, "f%fe", numero);
    return numero_serializado;
}

char* serializar_vetor(char** conteudo_serializado, int tamanho) {
    int i;
    char* vetor_serializado = malloc(sizeof(char) * NUMERO_MAXIMO_DE_INSCRICOES);

    sprintf(vetor_serializado, "l%d&", tamanho);

    for (i = 0; i < tamanho; i++) {
        strcat(vetor_serializado, conteudo_serializado[i]);
        if (i != tamanho - 1) {
            strcat(vetor_serializado, "&");
        }
    }

    strcat(vetor_serializado, "e");

    return vetor_serializado;
}




/// Serialização de estruturas ///

char* serializar_participante(t_participante* participante) {
    char* nome_serializado = serializar_string(participante->nome);
    char* email_serializado = serializar_string(participante->email);
    char* nif_serializado = serializar_int(participante->nif);
    char* telefone_serializado = serializar_int(participante->telefone);
    char* id_serializado = serializar_int(participante->identificador);

    char* participante_serializado = malloc(strlen(nome_serializado) + strlen(email_serializado) + strlen(nif_serializado) +
                                            strlen(telefone_serializado) + strlen(id_serializado) + 1);

    strcpy(participante_serializado, nome_serializado);

    concatenar_strings(participante_serializado, id_serializado, email_serializado, nif_serializado, telefone_serializado, NULL);
    libertar_memoria(nome_serializado, email_serializado, nif_serializado, telefone_serializado, id_serializado, NULL);

    return participante_serializado;
}

char* serializar_atividade(t_atividade* atividade) {
    char* identificador_serializado = serializar_int(atividade->identificador);
    char* designacao_serializada = serializar_string(atividade->designacao);
    char* data_serializado = serializar_string(atividade->data);
    char* hora_serializado = serializar_string(atividade->hora);
    char* local_serializado = serializar_string(atividade->local);
    char* tipo_serializado = serializar_string(atividade->tipo);
    char* associacao_serializado = serializar_string(atividade->associacao_estudantes);
    char* valor_serializado = serializar_float(atividade->valor);

    char* atividade_serializada = malloc(strlen(identificador_serializado) + strlen(designacao_serializada) + strlen(data_serializado) +
                                         strlen(hora_serializado) + strlen(local_serializado) + strlen(tipo_serializado) + strlen(associacao_serializado) +
                                         strlen(valor_serializado) + 1);

    strcpy(atividade_serializada, identificador_serializado);


    concatenar_strings(atividade_serializada, designacao_serializada, data_serializado, hora_serializado, local_serializado,
                       tipo_serializado, associacao_serializado, valor_serializado, NULL);

    libertar_memoria(identificador_serializado, designacao_serializada, data_serializado, hora_serializado,
                       local_serializado, tipo_serializado, associacao_serializado, valor_serializado, NULL);

    return atividade_serializada;
}

char* serializar_inscricao(t_inscricao* inscricao) {
    char* id_serializado = serializar_int(inscricao->identificador);
    char* id_participante_serializado = serializar_int(inscricao->id_participante);
    char* id_atividade_serializado = serializar_int(inscricao->id_atividade);
    char* valor_serializado = serializar_float(inscricao->valor_pago);
    char* data_serializada = serializar_string(inscricao->data);
    char* hora_serializada = serializar_string(inscricao->hora);

    char* inscricao_serializada = malloc(strlen(id_serializado) + strlen(id_participante_serializado) + strlen(id_atividade_serializado) +
                                         strlen(valor_serializado) + strlen(data_serializada) + strlen(hora_serializada) + 1);

    strcpy(inscricao_serializada, id_serializado);
    concatenar_strings(inscricao_serializada, id_participante_serializado, id_atividade_serializado, valor_serializado, data_serializada, hora_serializada, NULL);
    libertar_memoria(id_serializado, id_participante_serializado, id_atividade_serializado, valor_serializado, data_serializada, hora_serializada, NULL);

    return inscricao_serializada;
}


/// Serialização de listas de estruturas ///

char* serializar_participantes(t_participante** participantes, int numero_participantes) {
    int i;
    char** participantes_serializados = malloc(sizeof(char*) * numero_participantes);

    for (i = 0; i < numero_participantes; i++) {
        participantes_serializados[i] = serializar_participante(participantes[i]);
    }

    char* participantes_serializados_string = serializar_vetor(participantes_serializados, numero_participantes);

    for (i = 0; i < numero_participantes; i++) {
        free(participantes_serializados[i]);
    }

    free(participantes_serializados);

    return participantes_serializados_string;
}

char* serializar_atividades(t_atividade** atividades, int numero_atividades) {
    int i;
    char** atividades_serializadas = malloc(sizeof(char*) * numero_atividades);

    for (i = 0; i < numero_atividades; i++) {
        atividades_serializadas[i] = serializar_atividade(atividades[i]);
    }

    char* atividades_serializadas_string = serializar_vetor(atividades_serializadas, numero_atividades);

    for (i = 0; i < numero_atividades; i++) {
        free(atividades_serializadas[i]);
    }

    free(atividades_serializadas);

    return atividades_serializadas_string;
}

char* serializar_inscricoes(t_inscricao** inscricoes, int numero_inscricoes) {
    int i;
    char** inscricoes_serializadas = malloc(sizeof(char*) * numero_inscricoes);

    for (i = 0; i < numero_inscricoes; i++) {
        inscricoes_serializadas[i] = serializar_inscricao(inscricoes[i]);
    }

    char* inscricoes_serializadas_string = serializar_vetor(inscricoes_serializadas, numero_inscricoes);

    for (i = 0; i < numero_inscricoes; i++) {
        free(inscricoes_serializadas[i]);
    }

    free(inscricoes_serializadas);

    return inscricoes_serializadas_string;
}


/* ========================================================== */

/// DESERIALIZAÇÃO ///

/// Deserialização de tipos primitivos ///

int deserializar_int(char* numero_serializado) {
    int numero = 0;
    sscanf(numero_serializado, "i%de", &numero);
    return numero;
}

float deserializar_float(char* numero_serializado) {
    float numero = 0;
    sscanf(numero_serializado, "f%fe", &numero);
    return numero;
}

char* deserializar_string(char* string_serializada) {
    char* string = malloc(sizeof(char) * (atoi(string_serializada) + 1));
    sscanf(string_serializada, "%*d:%s", string);
    return string;
}

char** deserializar_vetor(char* vetor_serializado) {
    int numero_elementos = 0;
    sscanf(vetor_serializado, "l%d&", &numero_elementos);

    char** vetor = malloc(sizeof(char*) * (numero_elementos + 1));
    vetor[numero_elementos] = NULL;

    char* elemento = strtok(vetor_serializado, "&");
    int i = 0;

    while (elemento != NULL) {
        elemento = strtok(NULL, "&");
        if (elemento != NULL) {
            vetor[i] = elemento;
            i++;
        }
    }

    return vetor;
}

/// Deserialização de estruturas ///

t_participante* deserializar_participante(char* participante_serializado) {
    t_participante* participante = malloc(sizeof(t_participante));

    participante->nome = deserializar_string(participante_serializado);
    participante_serializado += strlen(participante->nome) + 1;

    participante->identificador = deserializar_int(participante_serializado);
    participante_serializado += strlen(participante_serializado) + 1;

    participante->email = deserializar_string(participante_serializado);
    participante_serializado += strlen(participante->email) + 1;

    participante->nif = deserializar_int(participante_serializado);
    participante_serializado += strlen(participante_serializado) + 1;

    participante->telefone = deserializar_int(participante_serializado);
    participante_serializado += strlen(participante_serializado) + 1;

    return participante;
}

t_atividade* deserializar_atividade(char* atividade_serializada) {
    t_atividade* atividade = malloc(sizeof(t_atividade));

    atividade->identificador = deserializar_int(atividade_serializada);
    atividade_serializada += strlen(atividade_serializada) + 1;

    atividade->designacao = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->designacao) + 1;

    atividade->data = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->data) + 1;

    atividade->hora = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->hora) + 1;

    atividade->local = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->local) + 1;

    atividade->tipo = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->tipo) + 1;

    atividade->associacao_estudantes = deserializar_string(atividade_serializada);
    atividade_serializada += strlen(atividade->associacao_estudantes) + 1;

    atividade->valor = deserializar_float(atividade_serializada);
    atividade_serializada += strlen(atividade_serializada) + 1;

    return atividade;
}

t_inscricao* deserializar_inscricao(char* inscricao_serializada) {
    t_inscricao* inscricao = malloc(sizeof(t_inscricao));

    inscricao->identificador = deserializar_int(inscricao_serializada);
    inscricao_serializada += strlen(inscricao_serializada) + 1;

    inscricao->id_participante = deserializar_int(inscricao_serializada);
    inscricao_serializada += strlen(inscricao_serializada) + 1;

    inscricao->id_atividade = deserializar_int(inscricao_serializada);
    inscricao_serializada += strlen(inscricao_serializada) + 1;

    inscricao->valor_pago = deserializar_float(inscricao_serializada);
    inscricao_serializada += strlen(inscricao_serializada) + 1;

    inscricao->data = deserializar_string(inscricao_serializada);
    inscricao_serializada += strlen(inscricao->data) + 1;

    inscricao->hora = deserializar_string(inscricao_serializada);
    inscricao_serializada += strlen(inscricao->hora) + 1;

    return inscricao;
}

/// Deserialização de listas ///

t_participante** deserializar_participantes(char* participantes_serializados) {
    char** participantes_serializados_vetor = deserializar_vetor(participantes_serializados);
    t_participante** participantes = malloc(sizeof(t_participante*) * (tamanho_vetor_bencode(participantes_serializados) + 1));
    participantes[tamanho_vetor_bencode(participantes_serializados)] = NULL;

    int i = 0;
    while (participantes_serializados_vetor[i] != NULL) {
        participantes[i] = deserializar_participante(participantes_serializados_vetor[i]);
        i++;
    }

    return participantes;
}

t_atividade** deserializar_atividades(char* atividades_serializadas) {
    char** atividades_serializadas_vetor = deserializar_vetor(atividades_serializadas);
    t_atividade** atividades = malloc(sizeof(t_atividade*) * (tamanho_vetor_bencode(atividades_serializadas) + 1));
    atividades[tamanho_vetor_bencode(atividades_serializadas)] = NULL;

    int i = 0;
    while (atividades_serializadas_vetor[i] != NULL) {
        atividades[i] = deserializar_atividade(atividades_serializadas_vetor[i]);
        i++;
    }

    return atividades;
}

t_inscricao** deserializar_inscricoes(char* inscricoes_serializadas) {
    char** inscricoes_serializadas_vetor = deserializar_vetor(inscricoes_serializadas);
    t_inscricao** inscricoes = malloc(sizeof(t_inscricao*) * (tamanho_vetor_bencode(inscricoes_serializadas) + 1));
    inscricoes[tamanho_vetor_bencode(inscricoes_serializadas)] = NULL;

    int i = 0;
    while (inscricoes_serializadas_vetor[i] != NULL) {
        inscricoes[i] = deserializar_inscricao(inscricoes_serializadas_vetor[i]);
        i++;
    }

    return inscricoes;
}


/* ========================================================== */
/* =                  FUNÇÕES UTILITÁRIAS                   = */
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

static char* obter_localizacao_save() {
    char caminho[1024];

    sprintf(caminho, CAMINHO_CONFIG, nome_utilizador_computador());

    FILE* ficheiro_configuracoes = abrir_ficheiro(caminho, LEITURA);
    if (ficheiro_configuracoes == NULL) {
        return NULL;
    }

    char  localizacao[1024];

    fgets(localizacao, 1024, ficheiro_configuracoes);
    localizacao[strlen(localizacao) - 1] = '\0';

    fechar_ficheiro(ficheiro_configuracoes);
    return strdup(localizacao);
}

static int salvar_localizacao_save(char* localizacao) {
    char caminho[1024];

    sprintf(caminho, CAMINHO_CONFIG, nome_utilizador_computador());

    FILE* ficheiro_configuracoes = abrir_ficheiro(caminho, ESCRITA);
    if (ficheiro_configuracoes == NULL) {
        return 0;
    }

    fprintf(ficheiro_configuracoes, "%s", localizacao);

    fechar_ficheiro(ficheiro_configuracoes);
    return 1;
}

__inline__ static int tamanho_vetor_bencode(char* vetor_serializado) {
    int tamanho = 0;

    // O tamanho da lista é indicado depois do 'l' e antes do ':'
    char* tamanho_serializado = strchr(vetor_serializado, 'l') + 1;
    tamanho_serializado = strtok(tamanho_serializado, ":");
    tamanho = atoi(tamanho_serializado);

    return tamanho;
}
//
// Created by João Matos on 17/12/2022.
//

#ifndef PROJETO1_APP_H
#define PROJETO1_APP_H

#include <stdlib.h>
#include <stdio.h>

#include "fs.h"                  // Para salvar os dados no disco
#include "stdout.h"              // Para interagir com o utilizador
#include "comum.h"               // Para definir os tipos de dados


#if defined(_WIN32) || defined(_WIN64)                          // Windows
#define nome_utilizador_computador() \
        getenv("USERNAME")                                      // Obter o nome do utilizador
#define CAMINHO_CONFIG \
        "C:\\Users\\%s\\AppData\\Local\\Projeto1\\proj.cfg"     // Caminho para o ficheiro de configurações
#else                                                           // Posix
#define nome_utilizador_computador() \
        getenv("USER")                                          // Obter o nome do utilizador
#define CAMINHO_CONFIG \
        "/home/%s/.config/Projeto1/proj.cfg"                    // Caminho para o ficheiro de configurações
#endif

#define NUMERO_MAXIMO_DE_PARTICIPANTES 5000
#define NUMERO_MAXIMO_DE_ATIVIDAES  200
#define NUMERO_MAXIMO_DE_INSCRICOES 10000


/// STRUCTS ///
typedef struct {
    int   identificador;         // Identificador do participante, sequência numérica única
    char* nome;                  // Nome do participante
    char* escola;                // Escola do participante
    int   nif;                   // Número de identificação fiscal do participante
    char* email;                 // E-mail do participante
    int   telefone;              // Telefone do participante
} t_participante;

t_participante* criar_participante(int, char*, char*, int, char*, int);
void libertar_participante(t_participante*);
__inline__ void mostrar_participante(t_participante*);


typedef struct {
    int identificador;           // Identificador do evento, sequência numérica única
    char* designacao;            // Designação do evento
    char* data;                  // Data do evento
    char* hora;                  // Hora do evento
    char* local;                 // Local do evento
    char* tipo;                  // Tipo do evento
    char* associacao_estudantes; // Associação de estudantes responsável pelo evento
    float valor;                 // Preço para a participação no evento
} t_atividade;

t_atividade* criar_atividade(int, char*, char*, char*, char*, char*, char*, float);
void libertar_atividade(t_atividade*);
__inline__ void mostrar_atividade(t_atividade*);


typedef struct {
    int identificador;          // Identificador da inscrição
    int id_participante;        // Identificador do participante que se inscreveu no evento
    int id_atividade;           // Identificador da atividade associada
    float valor_pago;           // Valor pago pelo participante
    char* data;                 // Data da inscrição
    char* hora;                 // Hora da inscrição
} t_inscricao;


t_inscricao* criar_inscricao(int, int, int, t_atividade**);
void libertar_inscricao(t_inscricao*);
__inline__ void mostrar_inscricao(t_inscricao*);


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
    t_participante** participantes;         // Vetor de participantes
    t_atividade** atividades;               // Vetor de atividades
    t_inscricao** inscricoes;               // Vetor de inscrições
    int* numero_participantes_inseridos;    // Número de participantes inseridos
    int* numero_atividadades_inseridas;     // Número de atividades inseridas
    int* numero_de_inscricoes;              // Número de inscrições inseridas
    t_caminho* caminho_ficheiro_save;       // Path para o ficheiro onde são guardadas as informações do programa
    int  ultimo_save;                       // Último save efetuado
    char* cor_texto;                        // Cor do texto
    char* cor_fundo;                        // Cor do fundo
} t_estado_programa;

t_estado_programa* criar_estado_programa(t_participante**, t_atividade**, t_inscricao**, int*, int*, int*, char*, char*);
void  libertar_estado_programa(t_estado_programa*);
int   carregar_estado_programa(t_estado_programa*);
int   guardar_estado_programa(t_estado_programa*);
void  mostrar_estado_programa(t_estado_programa*);


/// PROCURA ///
int procurar_participante_por_id(t_participante**, int);
int procurar_atividade_por_id(t_atividade**, int);
int procurar_inscricao_por_id(t_inscricao**, int);

int procurar_participante_por_nif(t_participante**, int);
int procurar_participante_por_email(t_participante**, char*);
int procurar_participante_por_telefone(t_participante**, int);





/// SERIALIZAÇÃO ///

/**
 * O método de serialização adotado é inspirado no formato Bencode, utilizado pelo protocolo BitTorrent.
 * Este método de serialização foi escolhido por ser simples de implementar (mais fácil do que construir um parser
 * para JSON ou XML) e por não ser afetado pelos problemas inerentes à serialização de dados binários (como Endianness).
 *
 * Tipos de dados compativeis:
 *  - strings de bytes
 *  - Inteiros
 *  - Vetores
 *  - Dicionários
 *  - Floats
 *
 * Os floats não são compatíveis por default no Bencoding, contudo a nossa implementação permite a serialização de floats.
 *
 * Padrão de serialização:
 *
 * INTEIROS: i<numero inteiro>e ou i-<numero inteiro>e
 * STRINGS: <tamanho da string>:<string>
 * VETORES: l<tamanho>&<elemento 1>&<elemento 2>&...&<elemento n>e - onde o "elemento" é um elemento codificado no formato Bencoding
 * DICIONÁRIOS: d<chave 1><valor 1><chave 2><valor 2>...<chave n><valor n>e - onde a "chave" é uma string codificada no formato Bencoding
 *              e o "valor" é um elemento codificado no formato Bencoding
 * FLOATS: f<numero float>e
 *
 * A proibição da utilização de chamdas recursivas a funções dificultou o processo de serialização, porém procurámos
 * implementar a solução da forma mais elegante possível com as restrições que nos foram dadas.
 */

// Serializar

// Tipos base
char* serializar_string(char*);
char* serializar_int(int);
char* serializar_float(float);
char* serializar_vetor(char**, int);
char* serializar_dicionario(char**, char**, int);

// Estruturas
char* serializar_participante(t_participante*);
char* serializar_atividade(t_atividade*);
char* serializar_inscricao(t_inscricao*);

// Vetores de estruturas
char* serializar_participantes(t_participante**, int);
char* serializar_atividades(t_atividade**, int);
char* serializar_inscricoes(t_inscricao**, int);

// Estado do programa
char* serializar_estado_programa(t_estado_programa*);


// Deserializar

// Tipos base
char*  deserializar_string(char*);
int    deserializar_int(char*);
float  deserializar_float(char*);
char** deserializar_vetor(char*);
char** deserializar_dicionario(char*);

// Estruturas
t_participante* deserializar_participante(char*);
t_atividade* deserializar_atividade(char*);
t_inscricao* deserializar_inscricao(char*);

// Vetores de estruturas
t_participante** deserializar_participantes(char*);
t_atividade** deserializar_atividades(char*);
t_inscricao** deserializar_inscricoes(char*);

// Estado do programa
t_estado_programa* deserializar_estado_programa(char*);

#endif //PROJETO1_APP_H

//
// Created by João Matos on 13/12/2022.
//

#ifndef PROJETO1_COMUM_H
#define PROJETO1_COMUM_H

#if defined(__aarch64__)
#define __inline__ __attribute__((always_inline)) // A keyword "inline" parecia causar problemas com o GCC na minha
                                                  // máquina arm64, então eu apenas redefino a constante aqui
#else
#define __inline__ inline
#endif // defined(__aarch64__)

#undef OK

/**
 * Enumeração para para os diferentes estados de uma função.
 */
enum {
    ERRO = -1,
    OK = 0
};

#define DEBUG 1

// ======================================================================================================== //

/**
 * Esta enumeração é usada para representar os tipos de estruturas suportadas pelo programa.
 *
 * Os valores são usados pela função formulário_input() (descrita no ficheiro include/stdout.h)
 * para decidir como deve ser interpretado o void* passado como argumento.
 * O void* passado como argumento deve ser um ponteiro para uma das estruturas suportadas.
 *
 * Os dados inseridos pelo utilizador na rotina do formulário são então inseridos na estrutura.
 */
typedef enum {
    PARTICIPANTE,
    ATIVIDADE,
    INSCRICAO
} t_tipo_estrutura;


/**
 * Esta enumeração é usada por motivos semelhantes à enumeração t_tipo_estrutura.
 *
 * É usada pela função prompt() de modo a decidir como deve ser interpretado o void* passado como argumento.
 */
typedef enum {
    INT,
    FLOAT,
    STRING, // Não é um dado primitivo, mas está aqui na mesma
    CHAR
} t_tipo_primitivo;

// NOTA: estas definições de enumerações poderiam ter sido evitadas se estivessemos a usar C++, recorrendo a templates e
//       ao meta-programming. Mas como usamos C, não há outra maneira de fazer isto que me venha à cabeça.
//       A única forma que me ocorre é mesmo definir os tipos de dados que queremos suportar e depois fazer um switch
//       case para cada um deles. Em que em cada caso, o void* passado como argumento é convertido e tratado consoante
//       o tipo de dados que informamos. Esta solução é muito mais verbosa e menos elegante do que meta-programming e templates,
//       porém temos que nos contentar com o C xD.


#endif //PROJETO1_COMUM_H

//
// Created by João Matos on 13/12/2022.
//

#ifndef PROJETO1_COMUM_H
#define PROJETO1_COMUM_H

#if defined(__aarch64__)
#define __inline__ __attribute__((always_inline)) // A keyword "inline" parecia causar problemas com o GCC em minha
                                                  // máquina arm64, então eu apenas redefino a constante aqui
#else
#define __inline__ inline
#endif // defined(__aarch64__)


#undef OK                                         // Retirar a definição da constante OK, para não entrar em conflito
                                                  // com a constante definida na biblioteca curses.h

/**
 * Enumeração para para os diferentes estados de uma função.
 */
enum {
    ERRO = -1,
    OK = 0
};


/**
 * Esta enumeração é usada para representar os tipos de estruturas suportadas pelo programa.
 *
 * Os valores são usados pela função formulário_input() (descrita no ficheiro sistema/stdout.h)
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


#endif //PROJETO1_COMUM_H

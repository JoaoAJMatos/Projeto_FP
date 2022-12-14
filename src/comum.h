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

#endif //PROJETO1_COMUM_H

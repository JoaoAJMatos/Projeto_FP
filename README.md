# Projeto de Fundamentos de Programação

O presente repositório contém o código fonte do projeto
desenvolvido no âmbito do primeiro semestre do curso __PSI__
para a unidade curricular de __Fundamentos de Programação__, lecionada pelo __professor José Vítor Martins Ramos__.

## Autores

* [João Matos](https://github.com/JoaoAJMatos) (__2220857__)
* [Rúben Lisboa](https://github.com/Lisboa14) (__2220862__)

### Contribuir (instruções para o grupo)

Para contribuir para o projeto, por favor segue as indicações descritas [aqui](CONTRIBUIR.md)

### Notas de desenvolvimento

O código foi desenvolvido com recurso ao IDE __CLion__ da __JetBrains__ e os _build files_ foram gerados com recurso ao __CMake__. 

Todo o código fonte foi escrito em modo de compatibilidade com as implementações da _standard library_ do __C__ para os sistemas operativos __Linux__ e __Windows__, sendo possível compilar o código em qualquer um destes sistemas operativos.

Ademais, o código é compativel com os processadores mais recentes da __Apple__, com redefinições de constantes e atributos do __GCC__ em modo de compatibilidade com a arquitetura __arm64__.

<hr>

Durante o desenvolvimento do projeto, foram levadas a cabo diversas práticas de programação;
práticas que são descritas tanto no relatório de projeto, como na seguinte secção do presente
documento.

> __Nota__: As práticas abaixo descritas não violam as restrições citadas no enunciado do
> projeto prático e não deverão portanto ser consideradas como penalizações. Nada mais são
> do que convenções que foram adotadas durante o desenvolvimento do projeto, pelo grupo que o
> desenvolveu.

- `switch` _statements_ com enumerações (`enum`):

```c
typedef enum {
    OPCAO_1,
    OPCAO_2,
    OPCAO_3,
    OPCAO_4
} t_opcao;

int main(void) {
    t_opcao opcao = OPCAO_1;
    
    switch(opcao) {
    case OPCAO_1: return 1;
    case OPCAO_2: return 2;
    case OPCAO_3: return 3;
    case OPCAO_4: return 4;
    } // Sem caso default para que o compilador nos avise de possiveis casos não tratados
}
```

__Motivação__: Se os valores avaliados pelo `switch` forem valores de uma enumeração, o
compilador pode avisar-nos de possíveis casos não tratados. Assim, se novos valores forem
adicionados ao `enum` e nos esquecermos de os tratar no `switch`, podemos aperceber-nos no
_compile time_.

Contudo, é importante que o código tome as devidas precauções para os casos que não são
tratados. 

No nosso caso, como todos os valores dos menus provêm de uma função que faz a verificação
prévia dos valores inseridos pelo utilizador, não precisamos de nos preocupar com isso.

#### Organização do código fonte

- O código de implementação de funções deve ir para os respetivos ficheiros `.c` e não para os
ficheiros `.h` correspondentes.
- Usar apenas caracteres alfanuméricos minúsculos (`a-z0-9`) e _underscore_ (`_`) para nomes de ficheiros e diretórios.
- Todos os ficheiros `.c` e `.h` devem incluir (`#include`) todos os _headers_ que utilizam diretamente, mesmo que
esses _headers_ sejam incluídos indiretamente por outros _headers_.
- Os ficheiros `.h` devem incluir _header guards_ para evitar que sejam incluídos mais do que uma vez:

```c
#ifndef PORJETO_NOME_DO_FICHEIRO_H
#define PORJETO_NOME_DO_FICHEIRO_H

#endif

// Ou

#pragma once    // Não aconselhado
```

## Utilitários

A acompanhar o código fonte do projeto, na pasta `scripts`, encontram-se diversos utilitários que facilitam a interação, gestão e construção do projeto.

Os dois ficheiros principais (`start.sh` ou `start.ps1`) podem ser utilizados para interagir com os utilitários do projeto.

### Utilitários disponíveis

#### Construir ficheiro C único:

Utilizado para construir um único ficheiro __C__ a partir de um projeto com diversos ficheiros. Útil para a criação de um ficheiro `.c` compatível com a ferramenta de deteção de plágio utilizada pela instituição.

Esta ferramenta deteta automáticamente todos os ficheiros `.c` e `.h` do projeto e junta os seus conteúdos corretamente num único ficheiro `.c`, ignorando diretórios comuns de cache gerados automáticamente por alguns IDEs (como o diretório `cmake-build-debug` ou `cmake-cache`).

Adicionalmente a ferramenta deteta a utilização de bibliotecas externas que não se encontram na _standard library_ do __C__ ou dos sistemas operativos __Linux__ e __Windows__, produzindo um comando de compilação que compila o projeto corretamente (incluindo as _linker flags_ para as bibliotecas externas).

O ficheiro `.c` gerado pode ser compilado automáticamente, ou através da execução do comando de compilação construido pela ferramenta.

##### Gerar projeto para o Code::Blocks

Esta ferramenta é utilizada para converter um projeto com diversos ficheiros, num projeto passível de ser aberto no IDE __Code::Blocks__.

Todos os ficheiros do projeto (`.c` e `.h`) são adicionados ao contexto do projeto e movidos para um único diretório.

Por fim, o ficheiro `.cdb` com as configurações do projeto em `xml` é gerado, permitindo a abertura do projeto no __Code::Blocks__.

O __Code::Blocks__ guarda todos os ficheiros do projeto em diretórios virtuais no ambiente do IDE, diretórios estes definidos automáticamente no ficheiro `.cdb`, com base nos diretórios do projeto original.

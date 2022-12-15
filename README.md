# Projeto de Fundamentos de Programação

## Autores

* [João Matos](https://github.com/JoaoAJMatos)
* [Rúben Lisboa](https://github.com/Lisboa14)

### Contribuir

Para contribuir para o projeto, por favor segue as indicações descritas [aqui](CONTRIBUIR.md)

### Notas de desenvolvimento

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

Contudo, é importante que o código trate tome as devidas precauções para os casos que não são
tratados. No nosso caso, como todos os valores dos menus provêm de uma função que faz a verificação
prévia dos valores inseridos pelo utilizador, não precisamos de nos preocupar com isso.

#### Organização do código fonte

- O código de implementação de funções deve ir para os respetivos ficheiros `.c` e não para os
ficheiros `.h` correspondentes.
- Usar apenas caracteres alfanuméricos minúsculos (`a-z0-9`) e _underscore_ (`_`) para nomes de ficheiros e diretórios.
- Todos os ficheiros `.c` e `.h` devem incluir (`#include`) todos os _headers_ que utilizam diretamente, mesmo que
esses _headers_ sejam incluídos indiretamente por outros _headers_.
- Os ficheiros `.h` devem incluir _header guards_ para evitar que sejam incluídos mais do que uma vez.
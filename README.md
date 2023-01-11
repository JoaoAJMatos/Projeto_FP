# Projeto de Fundamentos de Programação

O presente repositório contém o código fonte do projeto
desenvolvido no âmbito do primeiro semestre do curso __PSI__
para a unidade curricular de __Fundamentos de Programação__, lecionada pelo __professor José Vítor Martins Ramos__.

## Download e compilação

Para fazer o download do projeto, basta clonar o repositório:

``` bash
git clone https://github.com/JoaoAJMatos/Projeto_FP.git
```

Para compilar o projeto, pode fazê-lo manualmente através do comando:

```bash
cd Projeto_FP
gcc main.c -o main.exe # No Windows
./main.exe             # Para executar
```

Ou

``` bash
cd Projeto_FP
gcc main.c -o main.o # Em sistemas POSIX
./main.o             # Para executar
```

## Autores

* [João Matos](https://github.com/JoaoAJMatos) (__2220857__)
* [Rúben Lisboa](https://github.com/Lisboa14) (__2220862__)

### Contribuir (instruções para o grupo)

Para contribuir para o projeto, por favor segue as indicações descritas [aqui](docs/CONTRIBUIR.md)

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

__Motivação__: Se os valores avaliados pelo `switch` forem valores de uma enumeração, o compilador pode avisar-nos de possíveis casos não tratados.

Assim, se novos valores forem adicionados ao `enum` e nos esquecermos de os tratar no `switch`, podemos aperceber-nos no _compile time_.

Contudo, é importante que o código tome as devidas precauções para os casos que não são tratados. 

<hr>

- Sem early `return`:

Devido a restrições impostas as funções apenas podem conter um return no fim da função. Desta forma, código como o seguinte:

```c
int numero_par(int numero) {
    if (numero % 2 == 0) return 1;
    return 0;
}
```

Passaria a ser escrito assim:

```c
int numero_par(int numero) {
    int par = 0;
    if (numero % 2 == 0) par = 1;
    return par;
}
```

<br>
<br>

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/4.0/"><img alt="Licença Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/4.0/88x31.png" /></a>
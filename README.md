# Compilador

## Integrantes

GRUPO: <br/>
 Geovane Schmitz <br/>
 Gian Carlo Ferrari <br/>
 Guilherme Adenilson <br/>
 Gustavo Konescki <br/>

## Requerimentos

Para o presente projeto, é necessário a instalação do cmake e clang

### Fedora:

```bash
    sudo dnf install cmake clang
```

### Ubuntu:
```bash
    sudo apt install cmake clang
```

## Compilação

### Usando script

Apenas faça o comando abaixo na pasta Compilador:
```bash
    ./make.sh
```

### Fazendo manualmente

Primeiro faça:
```bash
    cmake .
```

Então faça:
```bash
    make
```

## Execução

Para executar o compilador, deve-se fazer o seguinte:
```bash
    ./bin/main filepath/file.xpp
```

Os arquivos prontos para testes estão na pasta xpp, segue alguns exemplos de arquivos com 100 ou mais linhas:
```bash
   ./bin/main xpp/arquivo2.xpp
```
```bash
   ./bin/main xpp/arquivo3.xpp
```
```bash
   ./bin/main xpp/funcoes.xpp
```

## Resultados

Um log para cada parte do compilador pode ser vista na pasta /log.

## Observação

As declarações da gramática em LL1, juntamente com uma SDD e uma SDT para expressões numéricas, estão na pasta *docs*. Apenas as tarefas AL e AS estão implementadas totalmente.
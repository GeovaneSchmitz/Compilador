# Compilador

## Integrantes

GRUPO: <br/>
 Geovane Schmitz <br/>
 Gian Carlo Ferrari <br/>
 Guilherme Adenilson <br/>
 Gustavo Konescki <br/>

## Requirementos

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

Os arquivos prontos para testes estão na pasta xpp, segue um exemplo de comando:
```bash
   ./bin/main xpp/funcoes.xpp
```

## Resultados

O log de cada parte do compilador pode ser vista na pasta /log.

### Observação

As declarações da gramática LL1, SDDs e SDTs estão expressas na pasta /docs.
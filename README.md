# Compilador

## Integrantes

GRUPO: <br/>
 Geovane Schmitz <br/>
 Gian Carlo Ferrari <br/>
 Guilherme Adenilson <br/>
 Gustavo Konescki <br/>

## Requerimentos

## Compilação

Para o presente projeto, basta rodar **make** na pasta Compilador.

## Execução

Para executar o compilador, deve-se fazer o seguinte:
```bash
    ./bin/compilador filepath/file.xpp
```

Os arquivos prontos para testes estão na pasta xpp, segue alguns exemplos de arquivos com 100 ou mais linhas:
```bash
   ./bin/compilador xpp/arquivo1.xpp
```
```bash
   ./bin/compilador xpp/arquivo2.xpp
```
```bash
   ./bin/compilador xpp/arquivo3.xpp
```

## Resultados

Um log para cada parte do compilador pode ser vista na pasta /log.

## Observação

As declarações da gramática em LL1, juntamente com uma SDD e uma SDT para expressões numéricas, estão na pasta *docs*. Apenas as tarefas AL e AS estão implementadas totalmente.
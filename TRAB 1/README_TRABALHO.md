# Trabalho 1 — Desempenho de ordenação interna

## Arquivos

- `ordena_final.c`: implementação C adaptada a partir do `ordena.c` fornecido.
- `resultados.csv`: resultados medidos pelo programa.
- `gera_graficos.py`: script que lê o CSV e gera os seis gráficos.
- `graficos/`: seis imagens PNG, uma para cada grupo e tipo de entrada.
- `relatorio.pdf`: relatório com metodologia, resultados e gráficos.
- `ordena.c`: arquivo original fornecido para referência.

## Compilar e executar

Em Linux com GCC:

```bash
gcc -O3 -std=c11 -Wall -Wextra -D_POSIX_C_SOURCE=199309L ordena_final.c -o ordena
./ordena
```

A execução cria/sobrescreve `resultados.csv` no diretório atual.

Para gerar novamente os gráficos (requer Python 3 e Matplotlib):

```bash
python3 -m pip install matplotlib
python3 gera_graficos.py
```

## O que foi implementado

- Algoritmos simples: bolha otimizada (`ordena_bolha3`), inserção, seleção e Shell.
- Algoritmos rápidos: heap, merge e quick.
- Três entradas: crescente, decrescente e aleatória.
- Tamanhos dobrados a partir de 8 elementos.
- Cada repetição recebe um vetor novo, preparado fora do intervalo cronometrado.
- O tempo considerado é o menor tempo observado.
- A busca termina quando as duas menores medidas diferem em no máximo 1%, após duas medidas consecutivas sem alterar os dois melhores tempos.
- Se a primeira medida ultrapassa 5 segundos, não se busca convergência. Se o melhor tempo ultrapassa 5 segundos, não são testados tamanhos maiores para aquela combinação de algoritmo e entrada.
- A alocação é verificada e o vetor é validado após a ordenação.
- O Quick Sort usa o elemento central como pivô, evitando a partição degenerada do pivô inicial nos vetores crescente e decrescente.

## Limites adotados

Para manter a execução prática e evitar consumo excessivo de tempo/memória:

- Algoritmos simples: máximo de 32.768 elementos.
- Algoritmos rápidos: máximo de 131.072 elementos.

Os limites são definidos por `MAX_N_SIMPLES` e `MAX_N` no início da seção de execução de `ordena_final.c`.

## Observações sobre os gráficos

Os eixos X e Y estão em escala logarítmica. O eixo X representa o número de elementos do vetor e o eixo Y representa o melhor tempo medido, em segundos. Os pontos de cada linha correspondem aos tamanhos efetivamente testados; por isso, uma linha pode terminar antes de outra.

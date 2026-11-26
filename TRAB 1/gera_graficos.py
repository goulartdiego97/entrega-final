#!/usr/bin/env python3
"""Gera os seis graficos do Trabalho 1 a partir de resultados.csv."""
import csv
from pathlib import Path
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
CSV = BASE / 'resultados.csv'
OUT = BASE / 'graficos'
OUT.mkdir(exist_ok=True)

with CSV.open(newline='', encoding='utf-8') as f:
    dados = list(csv.DictReader(f))

configuracoes = [
    ('simples', 'ordenado', 'Algoritmos simples — vetor ordenado', 'simples_ordenado.png'),
    ('simples', 'inverso', 'Algoritmos simples — vetor em ordem inversa', 'simples_inverso.png'),
    ('simples', 'aleatorio', 'Algoritmos simples — vetor aleatório', 'simples_aleatorio.png'),
    ('rapidos', 'ordenado', 'Algoritmos rápidos — vetor ordenado', 'rapidos_ordenado.png'),
    ('rapidos', 'inverso', 'Algoritmos rápidos — vetor em ordem inversa', 'rapidos_inverso.png'),
    ('rapidos', 'aleatorio', 'Algoritmos rápidos — vetor aleatório', 'rapidos_aleatorio.png'),
]

for grupo, entrada, titulo, arquivo in configuracoes:
    fig, ax = plt.subplots(figsize=(10, 6))
    subset = [r for r in dados if r['grupo'] == grupo and r['entrada'] == entrada]
    algoritmos = sorted({r['algoritmo'] for r in subset})
    for algoritmo in algoritmos:
        serie = sorted(
            (r for r in subset if r['algoritmo'] == algoritmo),
            key=lambda r: int(r['tamanho'])
        )
        x = [int(r['tamanho']) for r in serie]
        y = [float(r['tempo_segundos']) for r in serie]
        ax.plot(x, y, marker='o', linewidth=1.8, markersize=4, label=algoritmo)
    ax.set_title(titulo)
    ax.set_xlabel('Tamanho do vetor (número de elementos)')
    ax.set_ylabel('Tempo de execução (segundos)')
    ax.set_xscale('log', base=2)
    ax.set_yscale('log')
    ax.grid(True, which='both', linestyle=':', alpha=0.55)
    ax.legend(title='Algoritmo')
    fig.tight_layout()
    fig.savefig(OUT / arquivo, dpi=180, bbox_inches='tight')
    plt.close(fig)

print(f'{len(configuracoes)} gráficos salvos em {OUT}')

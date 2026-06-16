import pandas as pd
import matplotlib.pyplot as plt
import os

CSV_PATH = "results/resultados.csv"
OUTPUT_DIR = "results"

os.makedirs(OUTPUT_DIR, exist_ok=True)

# Leitura dos dados
df = pd.read_csv(CSV_PATH)

print("\nDados carregados:")
print(df.head())

# Gáfico 1 - Tempo de Execução
plt.figure(figsize=(10, 6))

for algoritmo in df["algoritmo"].unique():

    dados = df[df["algoritmo"] == algoritmo]

    plt.plot(
        dados["n_itens"],
        dados["tempo_ms"],
        marker="o",
        linewidth=2,
        label=algoritmo
    )

plt.title("Tempo de Execução dos Algoritmos")
plt.xlabel("Número de Itens")
plt.ylabel("Tempo (ms)")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig(f"{OUTPUT_DIR}/tempo.png")
plt.close()

print("tempo.png gerado")

# Gáfico 2 - Qualidade das Soluções
pivot_valor = df.pivot(
    index="n_itens",
    columns="algoritmo",
    values="valor"
)

plt.figure(figsize=(10, 6))

for coluna in pivot_valor.columns:

    plt.plot(
        pivot_valor.index,
        pivot_valor[coluna],
        marker="o",
        linewidth=2,
        label=coluna
    )

plt.title("Qualidade das Soluções")
plt.xlabel("Número de Itens")
plt.ylabel("Valor Obtido")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig(f"{OUTPUT_DIR}/qualidade.png")
plt.close()

print("qualidade.png gerado")

# Gáfico 3 - Comparação das Soluções
plt.figure(figsize=(10, 6))

pivot_valor.plot(
    kind="bar",
    ax=plt.gca()
)

plt.title("Comparação das Soluções")
plt.xlabel("Número de Itens")
plt.ylabel("Valor da Mochila")

plt.tight_layout()
plt.savefig(f"{OUTPUT_DIR}/comparacao.png")
plt.close()

print("comparacao.png gerado")

# Gáfico 4 - Erro Percentual da Solução Gulosa
if "Exata" in df["algoritmo"].values and \
   "Gulosa" in df["algoritmo"].values:

    exata = (
        df[df["algoritmo"] == "Exata"]
        .sort_values("n_itens")
    )

    gulosa = (
        df[df["algoritmo"] == "Gulosa"]
        .sort_values("n_itens")
    )

    erro = (
        (
            exata["valor"].values
            -
            gulosa["valor"].values
        )
        /
        exata["valor"].values
    ) * 100

    plt.figure(figsize=(10, 6))

    plt.plot(
        exata["n_itens"],
        erro,
        marker="o",
        linewidth=2
    )

    plt.title("Erro Percentual da Solução Gulosa")
    plt.xlabel("Número de Itens")
    plt.ylabel("Erro (%)")

    plt.grid(True)

    plt.tight_layout()
    plt.savefig(f"{OUTPUT_DIR}/erro_gulosa.png")
    plt.close()

    print("erro_gulosa.png gerado")

# Gáfico 5 - Escalabilidade dos Algoritmos
pivot_tempo = df.pivot(
    index="n_itens",
    columns="algoritmo",
    values="tempo_ms"
)

plt.figure(figsize=(10, 6))

for coluna in pivot_tempo.columns:

    plt.plot(
        pivot_tempo.index,
        pivot_tempo[coluna],
        marker="o",
        linewidth=2,
        label=coluna
    )

plt.title("Escalabilidade dos Algoritmos")
plt.xlabel("Número de Itens")
plt.ylabel("Tempo (ms)")
plt.yscale("log")  # destaca diferenças grandes
plt.grid(True, which="both")
plt.legend()

plt.tight_layout()
plt.savefig(f"{OUTPUT_DIR}/escalabilidade.png")
plt.close()

print("escalabilidade.png gerado")

# Resumo estatístico
print("\nResumo estatístico:")
print(
    df.groupby("algoritmo")
      .agg({
          "tempo_ms": ["mean", "max"],
          "valor": ["mean", "max"]
      })
)

print("\nTodos os gráficos foram gerados com sucesso.")
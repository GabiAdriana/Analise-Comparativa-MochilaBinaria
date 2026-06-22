# Analise-Comparativa-MochilaBinaria

Este repositório contém o trabalho prático desenvolvido para a disciplina de **Projeto e Análise de Algoritmos (PAA)** na **PUC Minas**. O objetivo principal é estudar, implementar e analisar o comportamento prático e teórico de três abordagens distintas para a resolução do **Problema da Mochila 0/1**.

## 🚀 Estratégias Implementadas

1. **Abordagem Exata (Força Bruta):** Implementação exponencial usando máscaras de bits (*bitmask*) para testar exaustivamente todas as $`2^n`$ combinações possíveis. Complexidade: $`O(2^n \cdot n)`$.
2. **Programação Dinâmica:** Algoritmo exato baseado na lei de recorrência *bottom-up* com memorização em matriz. Complexidade: $`O(n \cdot L)`$.
3. **Heurística Gulosa Modificada (1/2-aproximação):** Algoritmo aproximado que ordena os elementos pela densidade de utilidade:
   $$\text{Razão} = \frac{u_i}{p_i}$$
   O algoritmo escolhe o melhor valor entre a solução gulosa convencional e o item mais valioso de forma isolada. Complexidade: $`O(n \log n)`$.

---

## 📊 Estrutura do Projeto

* `main.c`: Código-fonte principal em C que executa os testes experimentais e calcula os tempos médios.
* `results/resultados.csv`: Arquivo gerado automaticamente com os dados colhidos de tempo e valor.
* `graficos.py`: Script em Python responsável por ler o arquivo CSV e gerar os gráficos de escalabilidade e erro.
* `relatorio.pdf`: Relatório técnico acadêmico produzido em LaTeX (padrão SBC).

---

## ⚙️ Como Executar o Projeto

### Prerrequisitos

Certifique-se de ter o compilador GCC e as bibliotecas do Python instaladas, você pode instalar rodando:

```bash
pip install pandas matplotlib

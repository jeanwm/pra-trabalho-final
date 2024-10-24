import pandas as pd
import matplotlib.pyplot as plt

# Carrega os dados do arquivo CSV e pula a primeira linha
df = pd.read_csv("output.csv", skiprows = 1)

# Remove espaços em branco nos nomes das colunas
df.columns = df.columns.str.strip()

# Adiciona manualmente os nomes das colunas
df.columns = ['Tamanho', 'ArvoreBinariaAdicao', 'ArvoreRubroNegraAdicao', 'ArvoreBAdicao', 'ArvoreBinariaRemocao', 'ArvoreRubroNegraRemocao', 'ArvoreBRemocao']

# Imprime as colunas disponíveis que foram representadas no gráfico
print(df.columns)

# Plota os dados
fig, ax = plt.subplots(figsize=(10, 6))

# Adiciona uma linha para cada método de ordenação
ax.plot(df['Tamanho'], df['ArvoreBinariaAdicao'], label='Árvore binária (adição)')
ax.plot(df['Tamanho'], df['ArvoreRubroNegraAdicao'], label='Árvore rubro-negra (adição)')
ax.plot(df['Tamanho'], df['ArvoreBAdicao'], label='Árvore B (adição)')
ax.plot(df['Tamanho'], df['ArvoreBinariaRemocao'], label='Árvore binária (remoção)')
ax.plot(df['Tamanho'], df['ArvoreRubroNegraRemocao'], label='Árvore rubro-negra (remoção)')
ax.plot(df['Tamanho'], df['ArvoreBRemocao'], label='Árvore B (remoção)')

# Adiciona rótulos e título
ax.set_title('Comparação de Árvores nas Operações de Adição e Remoção de Nós')
ax.set_xlabel('Tamanho do Conjunto de Dados')
ax.set_ylabel('Esforço Computacional/Número de Iterações')
ax.legend()
plt.yscale('log')

# Exibe o gráfico
plt.show()
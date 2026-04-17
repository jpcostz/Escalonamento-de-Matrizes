# Escalonamento de Matrizes (Matrizes Esparsas) 🧮

Este projeto é uma implementação em **C** de algoritmos clássicos de Álgebra Linear para escalonamento de matrizes. Desenvolvido como Trabalho Prático, o programa permite a resolução e simplificação de sistemas lineares utilizando métodos de eliminação, otimizados através do uso de estruturas de dados para matrizes esparsas.

## 🚀 Funcionalidades

O programa roda via interface de linha de comando (CLI) e oferece as seguintes opções:

- **Inicialização Dinâmica:** Criação de matrizes de qualquer dimensão.
- **Dois Métodos de Inserção:**
  - *Simples:* Preenchimento intuitivo linha por linha.
  - *Avançado:* Inserção direta por coordenadas (linha, coluna, valor).
- **Eliminação de Gauss (Simples):** Transforma a matriz em sua forma escalonada (Triangular Superior).
- **Eliminação de Gauss-Jordan:** Transforma a matriz em sua forma escalonada reduzida (RREF - *Reduced Row Echelon Form*).
- **Visualização Formatada:** Exibição em grade com tratamento de zeros absolutos.
- **Histórico em Memória:** Permite armazenar e visualizar até 100 matrizes simultaneamente.

## 🧠 Diferenciais Técnicos

Para garantir eficiência de memória e precisão matemática, este projeto implementa:

1. **Estrutura de Matriz Esparsa (Formato COO):** Em vez de alocar uma grade $N \times M$ inteira na memória (o que desperdiça espaço com zeros), o programa armazena apenas os elementos não-nulos utilizando uma *Lista de Coordenadas*. O array interno cresce dinamicamente com o uso de `realloc`.
2. **Pivoteamento Parcial:** Durante as eliminações de Gauss e Gauss-Jordan, o algoritmo não escolhe o primeiro pivô disponível, mas sim o **maior pivô em valor absoluto** da coluna. Isso mitiga severamente erros de arredondamento de ponto flutuante, garantindo estabilidade numérica.
3. **Tolerância de Ponto Flutuante:** Uso de precisão $\epsilon = 1 \times 10^{-6}$ para evitar falhas de comparação com zeros residuais (`-0.00`).

## 🛠️ Como Compilar e Executar

Este projeto requer apenas um compilador C padrão (como `gcc`).

1. Clone o repositório:
```bash
git clone https://github.com/jpcostz/escalonamento-de-matrizes.git
cd escalonamento-de-matrizes
```
2. Compile o código:
```bash
gcc Trabalho1.c -o matrizes -lm
```
(Nota: a flag -lm é necessária em alguns ambientes Linux para linkar a biblioteca <math.h>)

3. Execute o programa:
```bash
./matrizes
```

## 💻 Exemplo de Uso
Ao iniciar o programa, você verá o menu principal:

```text
=== Menu ===
 1. Inicializar nova Matriz
 2. Visualizar Matriz
 3. Escalonar Matriz (Gauss - Simples)
 4. Escalonar Matriz (Gauss-Jordan - Reduzida)
 5. Sair
Escolha uma opcao:
```

Ao escalonar uma matriz, o programa preserva a versão original e cria uma nova cópia na memória, permitindo que você visualize o "antes e depois".

## 👨‍💻 Autor
[João Paulo Costa](https://github.com/jpcostz) - Desenvolvimento e Estruturação

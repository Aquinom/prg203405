# Regicida - Jogo de Cartas Cooperativo

Um jogo de cartas cooperativo implementado em C++ usando Qt, baseado no jogo de cartas Regicide.

## Sobre o Jogo

Regicide é um jogo cooperativo onde os jogadores trabalham juntos para derrotar 12 inimigos poderosos. Os jogadores se revezam jogando cartas para atacar o inimigo, e quando dano suficiente é causado, o inimigo é derrotado. Os jogadores vencem quando o último Rei é derrotado. Mas cuidado! A cada turno o inimigo contra-ataca.

## Objetivo

Derrote todos os 12 inimigos do Castelo:

- 4 Valetes (20 HP, 10 de ataque)
- 4 Rainhas (30 HP, 15 de ataque)
- 4 Reis (40 HP, 20 de ataque)

## Estrutura do Código

### Classes Principais

- **Card**: Representa uma carta do jogo com naipe e valor

  - Naipes: Copas (♥), Ouros (♦), Paus (♣), Espadas (♠)
  - Valores: 2-10, Valete, Rainha, Rei, Animal Companion, Bobo

- **Deck**: Gerencia um baralho de cartas

  - `createTavernDeck()`: Cria o baralho da Taverna (cartas 2-10 + especiais)
  - `createCastleDeck()`: Cria o baralho do Castelo (Valetes, Rainhas, Reis)

- **Player**: Representa um jogador com mão de cartas

  - Gerencia a mão de cartas
  - Tamanho máximo da mão varia por número de jogadores

- **Enemy**: Representa um inimigo (Valete, Rainha ou Rei)

  - HP, ataque, imunidade a naipes
  - Sistema de escudo (cartas de Espadas)

- **GameState**: Gerencia o estado completo do jogo

  - Turnos, fases, jogadores
  - Lógica de combate e poderes das cartas
  - Validação de combos

- **GameWindow**: Interface gráfica Qt
  - Exibição do inimigo atual
  - Mão de cartas do jogador
  - Botões de ação (jogar, desistir, descartar)
  - Log do jogo

## Poderes dos Naipes

- **♥ Copas**: Cura - embaralha a pilha de descarte e move cartas para o fundo do baralho da Taverna
- **♦ Ouros**: Comprar cartas - jogadores compram cartas em ordem
- **♣ Paus**: Dano dobrado - o dano causado por Paus conta em dobro
- **♠ Espadas**: Escudo - reduz o ataque do inimigo

## Mecânicas Especiais

### Combos

- 2-4 cartas do mesmo valor podem ser jogadas juntas
- Valor total não pode exceder 10
- Todos os poderes dos naipes são ativados

### Animal Companions

- Podem ser pareados com uma outra carta
- Contam como 1 de ataque
- Seu poder de naipe também é aplicado

### Bobo (Jester)

- Sempre jogado sozinho
- Cancela a imunidade do inimigo
- Jogador escolhe quem joga em seguida
- Permite comunicação limitada

### Imunidade de Inimigos

- Cada inimigo é imune ao poder do naipe correspondente
- Ex: Valete de Copas é imune a poderes de Copas
- O Bobo cancela essa imunidade

## Como Jogar

1. Execute o jogo e selecione o número de jogadores (1-4)
2. Cada turno tem 4 fases:

- **Jogar Carta**: Selecione cartas da sua mão e clique em "Jogar Cartas Selecionadas" ou "Ceder Turno"
- **Ativar Poder**: Poderes dos naipes são ativados automaticamente
- **Causar Dano**: Dano é aplicado ao inimigo
- **Sofrer Dano**: Descarte cartas para cobrir o ataque do inimigo

3. Continue até derrotar todos os 12 inimigos ou até todos os jogadores serem derrotados

## Compilação

### Requisitos

- Qt 6.5 ou superior
- CMake 3.19 ou superior
- Compilador C++17

### Build

```bash
cd code
mkdir build
cd build
cmake ..
cmake --build .
```

## Configuração de Jogadores

| Jogadores | Bobos | Tamanho Max da Mão |
| --------- | ----- | ------------------ |
| 1         | 0     | 8                  |
| 2         | 0     | 7                  |
| 3         | 1     | 6                  |
| 4         | 2     | 5                  |

## Vitória e Derrota

### Vitória

- Derrote o último Rei

### Derrota

- Um jogador não consegue descartar cartas suficientes para cobrir o dano
- Um jogador não pode jogar uma carta nem desistir no seu turno

## Créditos

Jogo baseado em **Regicide** de Paul Abrahams, Luke Badger e Andy Richdale.

Implementação em Qt/C++ por Guilherme Franco.

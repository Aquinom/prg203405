<img src="img/ifsc-logo.png"
     width="30%"
     style="padding: 10px">

# << Regicide >>

## << Descrição breve do domínio do problema >>

<img src="img/ifsc-logo.png"
     width="30%"
     style="padding: 10px">

# Regicide Game

# StartUp
* O game Regicide é um jogo coparticipativo, sendo jogado solo ou em grupos de até 4 pessoas;
     * Para uma quantidade de Players finita, é definido um número de cartas para cada player iniciar;
 
          | Players | Numbers of Card | 
          |----------|----------|
          | 1 | 8 | 
          | 2 | 7 | 
          | 3 | 6 | 
          | 4 | 5 | 

* O Regicide pode ser jogado com um baralho tradicional de 4 naipes de 12 cartas de cada Naipe (A,2,3,4,5,6,7,8,9,J,Q,K) ou utilizar o baralho específico do Regicide;
* Cada participante, recebes cartas sortidas de 2 a 9 como suas cartas guerreiras;
* O Jogo consiste em elimiar todas as cartas J, Q, K dos 4 naipes do monte (totalizando 16 cartas), essas cartas são consideras os monstros do Game;

# Como funciona - Cartas Guerreiras (2 a 9)
* Cada carta Guerreira tem associado um valor de ataque e vida igual ao valor da carta;
     * *A*:
          * Vida: 1Pt, Poder de ataque: 1Pt
     * <...>
     * *9*:
          * Vida: 9Pt, Poder de ataque: 9Pt
        
* O Naipe associado a carta, determina um poder extra que a carta tem:
     * Moles:
          * Permite que se compre um número de cartas igual ao valor da carta, sendo compradas de um por uma por cada player presente em sequência (P1 Compra, P2 Compra ... até atingir o valor nominal).
               * 9 de Moles: Além de sofrer dano e efetuar ataques, pode comprar 9 cartas do deck, dividindo entre os participantes;             
     * Espadas:
          * Reduz o dano de ataque de um monstro em um valor igual ao da carta.
               *  9 de Espadas: Se receber dano 10, ele consegue abstrair 9 unidades desse dano, permanecendo com 9 de Vida.
     * Copas:
          * Reduz o dano de ataque de um monstro em um valor igual ao da carta.
               *  9 de Espadas: Se receber dano 10, ele consegue abstrair 9 unidades desse dano, permanecendo com 8 de Vida.               
     * Paus:
          * Aumenta o dano de ataque em duas vezes o valor nominal da carta. 
               *  9 de Paus: Tem dano de ataque final de 9 + 9 = 18
            

# Como funciona - Cartas Monstros (J, Q, K)
# Como funciona - Cartas Animais (A)
# Como funciona - Cartas Coringas ()

Descrever mais domínio do projeto.
https://www.regicidegame.com/

> Repositório destinado para o desenvolvimento do projeto final da discipliana PRG203405 - Programação orienta a objeto. 
> 
> Alunos: 
> Vilian Nicoladeli Esmeraldino
> Rafael Aquino de Meireles
> Professor: Hugo Marcondes


<p align=center><strong>SUMÁRIO</strong></p>

[**1. ANÁLISE ORIENTADA A OBJETO**](./analise.md)<br>
[**2. PROJETO ORIENTADO A OBJETO**](./projeto.md)<br>
[**3. IMPLEMENTAÇÃO (C++)**](./implementacao.md)<br>
[**4. TESTES**](./testes.md)<br>

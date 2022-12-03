#atividade3

- Atividade 3 da disciplina de Computação Gráfica, ministrada pelo professor Harlen pela Universidade Federal do ABC.

Nomes:  Henrique Augusto Santos Batista
        Paulo Henrique Eiji Hayashida

RAs:    11050714
        11104714

## Atividade 3 - Simulação de Construção de Terreno

## Descrição

- Utilizando malhas 2D composta por triângulos. O programa gera elevações no eixo z, para simular regiões com relevo.

- Através da interação com mouse pode-se rotacionar o modelo 3D para visualização de diferentes ângulos.

## Implementação

- Para o projeto foi criado dois arquivo OBJ, ambos desses são malhas 2D, uma composta por 25 vértices e 32 triângulos e outra composta por 121 vértices e 200 triângulos. No arquivo final descidiu-se utilizar a malha com 200 triângulos, na esperança de gerar elevações mais realisticas.

- O projeto viewer1 foi utilizado como base para implementação da aplicação, nele alterou-se a velocidade de rotação, não permitindo o objeto girar indefinidamente quando o botão do mouse é solto. Permitindo ao usuário uma maior liberdade para visualisar regiões de interesse

- Também se criou um função dentro do arquivo model.cpp que permite a randomização das alturas, utilzando um pseudo-gerador de números aleatórios, e um laço para alterar vértice por vértice os respectivos valores de z.

## Dificultadades Encontradas

- Infelizmente a randomização das alturas só ocorre uma única vez, na inicialização do modelo.

- Tentativas de alterar as alturas utilizando funções dentro da atulização onPaint, bem como o uso de variáveis uniform float com valor u_time dentro do arquivo do shader, foram testadas sem resultados desejados, onde nenhuma alteração visivél foi observada.

## Melhorias para trabalhos futuros

- Além da correção dos problemas apresentados na seção anterior, um função adicinal para melhoria dos formatos seria a implementação de opções para escolha do número de triângulos para cada vértice, suavizando as curvas bruscas encontradas na contrução do modelo.
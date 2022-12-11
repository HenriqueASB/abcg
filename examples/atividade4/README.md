#atividade4

- Atividade 4 da disciplina de Computação Gráfica, ministrada pelo professor Harlen pela Universidade Federal do ABC.

Nomes:  Henrique Augusto Santos Batista
        Paulo Henrique Eiji Hayashida

RAs:    11050714
        11104714

## Atividade 4 - Simulação de Construção Aleatória de Terreno

## Descrição

- Por meio de Malha 2D gerados por código, criou-se os vértices e indices para serem desenhados e alterando-se a altura desses vértices e propriedades de textura, causam uma sensação de movimentação em um terreno.

- Através da interação com mouse pode-se rotacionar o modelo 3D para visualização de diferentes ângulos.

## Implementação

- A ideia da atividade anterior foi aprimorada, descartando o uso de arquivos OBJ, uma vez que não possibilita a alteração dos índices durante a execução do programa. Para a criação de malha usou-se duas funções for para geração dos vértices distribuidos no quadrante positivo, e outras duas funções for para a geração dos índices de cada triângulo. A malha então é normalizada de forma a extender-se pelo plano e ficar centralizada.

- O projeto viewer4 foi utilizado como base para implementação da aplicação. Alterou-se algumas funcionalidades e, por simplicidade, decidiu-se manter alguns trechos de códigos.

- Inspirados o exemplo demonstrado pelo Professor, incluiu-se um variável aleatória com base no tempo dentro do fragmente shader, permitindo usar tais valores de forma a deslocar as unidades de textura, levando a uma impressão de movimento.

- A geração aleatória de relevo aliada a ilusão de movimento, produz uma animação de movimentação por um terreno.

- Uma observação quanto ao framerate da animação, para a aplicação para desktop a animação apresentou um melhor comportamento para um framerate de 3 frames por segundo, enquanto que para versão Web o melhor framerate foi de 30 frames por segundo. Com esses valores há uma maior fluidez na animação.

## Melhorias para trabalhos futuros

- A utilização de outras texturas bem como efeitos que podem ser implementados diretos no shader são ideias interessantes que podem ser exploradas com mais profundidade nos trabalhos posteriores.
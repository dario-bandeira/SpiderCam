# SpiderCam

![spidercam demonstration](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_demonstration.gif)

Este projeto é um protótipo de câmera como <a href="https://youtu.be/wor4BFjbIyI?t=1m32s" target="_blank">esta</a> para Arduino.
Ainda está simples e tedioso, só faz um movimento infinito no eixo X, mas no futuro vou construir uma versão controlada por joystick.

Para construir o seu próprio, você deve considerar 3 coisas:

## 1. Medidas da caixa
![box measures](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_measures.jpg)

Você deve converter as medidas para "passos de motor", isto é, quantos passos seu motor tem que dar para enrolar (por exemplo) 40cm de fio. No meu caso são 4920, isso significa que minha caixa tem 4920 passos de altura.

## 2. Eixos da caixa
![box axes](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_dimensions.jpg)

Você deve definir um ponto zero e começar as medidas a partir dele (em passos).

## 3. A posição da bola
É dada pelas variáveis `x`, `y` e `z`, que são os valores nas 3 dimensões.<br>
**Importante**: para o algoritmo, a posição da bola é onde os fios se encontram, não o centro da bola!


# Os cálculos
![box triangles](https://github.com/dario-bandeira/SpiderCam/blob/master/img/spidercam_triangles.jpg)

Defina um ponto inicial para a bola dentro da caixa antes de começar o movimento.<br>
Para calcular o comprimento do fio neste momento, você calcula a hipotenusa de dois triângulos imaginários (t1 e t2 na figura) usando teorema de Pitágoras.
```
hipotenusa = sqrt( pow(cateto1, 2) + pow(cateto2, 2) );
```
**Triângulo 1:**<br>
cateto1 = x<br>
cateto2 = box width - y


**Triângulo 2:**<br>
cateto1 = a hipotenusa do triângulo 1<br>
cateto2 = z

A hipotenusa do triângulo 2 é o fio.
Repita isto para os 4 fios.

# Movimento
Defina um ponto imaginário dentro da caixa pra onde você quer que a bola vá. Então você repete o cálculo acima considerando o novo ponto.
Agora que você sabe os comprimentos atuais dos fios e os comprimentos que eles tem que ter para segurar a bola na posição desejada, então faça os motores fazerem o movimento.

Para cada fio você faz `novoComprimento - atualComprimento`, se der um valor positivo você libera este fio, se der um valor negativo você puxa este fio.

Exemplo:<br>
```
atualComprimento = 3460;
novoComprimento = 2940;

novoComprimento - atualComprimento = -520

Então este fio deve ser puxado 520 passos.
```

**Cuidado:** garanta que os motores vão mover sincronizados para evitar sobrepeso nos fios.

###### Como fazer isto?

Suponhamos que os motores tem que dar esta quantidade de passos para alcançar o ponto desejado:

motor 0: 200 passos<br>
motor 1: 100 passos<br>
motor 2: 60 passos<br>
motor 3: 50 passos

O movimento inteiro é dividido em **ciclos**. Para descobrir a quantidade de passos que cada motor tem que dar **em um único ciclo**, divida seu número de passos pelo menor deles.<br>
Por exemplo:

```
motor 0: 200 passos
motor 3: 50 passos (o menor valor de todos)

200/50 = 4
```

Então o 'motor 0' tem que dar 4 passos em cada ciclo.<br>
Repita este cálculo para todos os motores.

```
Passos no primeiro ciclo:

motor 0: 4 passos
motor 1: 2 passos
motor 2: 1,2 passos
motor 3: 1 passos
```

Note que o motor 2 tem um valor decimal de passos, não é possível fazer isto num motor de passos. Então você deve armazenar a parte decimal e aplicar no próximo ciclo para evitar que o fio fique apertado ou frouxo.

```
Depois de separar a parte decimal:

motor 0: 4 passos
motor 1: 2 passos
motor 2: 1 passo
motor 3: 1 passo

sobra 0: 0
sobra 1: 0
sobra 2: 0,2 (este motor teve uma "sobra" de 0,2 passo)
sobra 3: 0
```

Faça o ciclo, aplique a sobra no próximo ciclo e armazene as sobras de novo, se houver.

```
O segundo ciclo será:

motor 0: 4 passos
motor 1: 2 passos
motor 2: 1 passo
motor 3: 1 passo

sobra 0: 0
sobra 1: 0
sobra 2: 0,4 <-- (houve mais 0,2 passo de sobra, somada à sobra anterior ficou 0,4)
sobra 3: 0
```

Continue fazendo isto até que as sobras alcancem um valor inteiro. Quando isto acontecer, você tem 1 passo a mais para dar para este motor no próximo ciclo.

Este processo levará a bola para o ponto desejado.<br>
**Nota:** o número total de ciclos é o mesmo que o menor valor de passos. Neste caso, 50.

### Para as próximas versões
Um joystick (ou um teclado) dirá as direções e a velocidade do movimento.

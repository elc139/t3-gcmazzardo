# T3: Scheduling com OpenMP 

Nome: Gabriel Cortez Mazzardo

Disciplina: Programação Paralela

Link para o programa OpenMPDemoABC.cpp desenvolvido: .


## Casos de testes

**Caso de teste 1 - Scheduling static sem especificação do chunk:**

<img src="https://i.imgur.com/8dpEI0t.png" width="400">

Neste cenário de escalonamento, observou-se que as 60 iterações de preenchimento do array foram divididas igualmente entre as 3 threads, em 3 fatias contínuas de 20 iterações. Com isso, logicamente, cada thread executou um número igual de iterações e, logo, os caracteres 'A', 'B' e 'C' sempre aparecem em mesma quantidade (20 vezes cada um) no array preenchido. Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').

**Caso de teste 2 - Scheduling static com especificação do chunk (chunk == 10):**

<img src="https://i.imgur.com/goNMTzb.png" width="400">

Diferentemente do primeiro caso de teste, especificou-se o chunk para o valor 10. PARA O VALOR DE CHUNK ESCOLHIDO, assim como no primeiro caso, 20 iterações são atribuídas para cada thread, pois as 60 iterações são divididas em 6 fatias iguais de 10, e cada uma das 3 threads fica responvável por executar 2 das 6 fatias, totalizando 20 cada. Também vale ressaltar que, com a especificação do chunk, as iterações não são mais simplesmente divididas em 3 fatias de 20 entre as threads, e sim dadas em "fatias" de 10 em 10 para as threads 0, 1 e 2, nesta ordem, até que todas as fatias tenham sido atribuídas a uma das threads. Para diversos outros valores de chunk, porém, o número de iterações não permanece 20/20/20, pois não consegue-se dividir as iterações igualmente entre as threads devido ao tamanho da "fatia". Um exemplo onde isso acontece é com chunk==11, onde as threads 0 e 1 conseguiriam receber as duas fatias de 11 iterações (totalizando 22 para cada uma delas), porém a thread 2 não conseguiria receber as mesmas 11 iterações na segunda fatia, pois só restariam 5 iterações do total de 60, ficando assim o número de iterações em 22/22/16, respectivamente. Como a exclusão mútua continua sendo garantida, nenhuma posição do array fica em branco('-').

**Caso de teste 3 - Scheduling dynamic sem especificação do chunk:**

<img src="https://i.imgur.com/bBlyNuE.png" width="400">

Neste cenário de escalonamento, as iterações são atribuídas dinamicamente para as threads. Quando uma thread termina de executar a iteração atual concedida à ela, é atribuída a próxima iteração que ainda não foi destinada para nenhuma thread. Assim sendo, não há como garantir que todas as threads executem o mesmo número de iterações de preenchimento do vetor (20 cada), pois não é determinístico quantas iterações uma thread conseguirá terminar de executar (e começar outra). Logo, geralmente as threads executam quantidades diferentes de iterações e, portanto, a quantidade de caracteres 'A', 'B' e 'C' do vetor preenchido são muitas vezes diferentes. Também é válido ressaltar que também é indeterministica a ordem de atribuição das iterações a cada uma das threads. Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').

**Caso de teste 4 - Scheduling dynamic com especificação do chunk (chunk==10):**

<img src="https://i.imgur.com/mJ5iTpC.png" width="400">

Assim como no caso 3, como o tipo de scheduling continua sendo dynamic, as iterações continuam sendo atribuídas dinamicamente para as threads. A diferença, neste caso, é que as iterações não são atribuídas uma a uma para as threads, e sim em fatias. Quando uma thread termina de executar a fatia de iterações atual concedida à ela, é atribuída a próxima fatia de iterações que ainda não foi atribuída para nenhuma thread. Com este valor de chunk em específico, observou-se que cada uma das threads executou duas fatias de 10 iterações cada uma (totalizando as 60), assim tornando 20 iterações de preenchimento do vetor para cada thread. Esse resultado de execução poderia ser diferente, porém isso só aconteceria se uma thread X acabasse a sua segunda fatia de iterações sem que uma outra thread Y tivesse ainda acabado sua primeira fatia, o que não aconteceu em nenhuma execução durante os testes. Supondo que isso acontecesse, seria atribuída à thread X sua terceira fatia de iterações, e quando a thread Y finalmente acabasse sua primeira, não haveriam mais iterações para serem atribuídas à ela, fazendo com que a thread X tivesse executado 30 execuções e a Y apenas 10, gerando um possível resultado 30/10/20 iterações entre as threads, por exemplo. Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').


**Caso de teste 5 - Scheduling guided sem especificação do chunk:**

<img src="https://i.imgur.com/dPkqYCi.png" width="400">

Este cenário de escalonamento é semelhante ao scheduling dynamic, com exceção de que o tamanho das fatias atribuídas à cada uma das threads muda automaticamente a medida em que o programa é executado. As threads começam com chunks maiores, mas depois ajustam-se tamanhos menores para o chunk se a carga de trabalho das threads estiver muito diferente. Com isso, também é indeterminístico saber a quantidade de iterações que serão atribuídas a cada uma das 3 threads durante a execução. Assim sendo, geralmente as threads executam quantidades diferentes de iterações e, portanto, a quantidade de caracteres 'A', 'B' e 'C' do vetor preenchido são muitas vezes diferentes. Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').

**Caso de teste 6 - Scheduling guided com especificação do chunk (chunk==5):**

<img src="https://i.imgur.com/D1tLQDU.png" width="400">

Assim como no caso de teste 5, o escalonamento continua sendo guided. A diferença é que, com a especificação do chunk, consegue-se determinar qual é o tamanho da fatia inicial atribuída às threads. O restante do funcionamento é condizente com o caso anterior, uma vez que o chunk continua sendo ajustado automaticamente para tamanhos menores caso a carga de trabalho das threads estiver muito diferente. Logo, o indeterminismo em saber a quantidade das iterações atribuídas a cada uma das 3 threads durante a execução permanece existindo e, assim sendo, a quantidade de iterações que cada uma delas executa, muitas vezes, tende a ser diferente e, portanto, a quantidade de caracteres 'A', 'B' e 'C' do vetor preenchido também são diferentes. Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').

**Caso de teste 7 - Scheduling runtime:**

<img src="https://i.imgur.com/ZVmCBd9.png" width="400">

O escalonamento runtime configura o scheduling utilizando o valor contido na variável de ambiente OMP_SCHEDULE, que pode estar setada para outro tipo de escalonamento (static ou dynamic, por exemplo). Na imagem de exemplo acima, quando este caso de teste foi executado, a variável de ambiente estava configurada como "static,10" (igualmente ao exemplo visto no caso de teste 2, acima), e podemos observar que o resultado obtido na execução foi exatamente o mesmo que naquele caso (20 iterações atribuídas para o preenchimento do vetor a cada uma das threads, divididas em 2 fatias de 10, etc...). Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').


**Caso de teste 8 - Scheduling auto:**

<img src="https://i.imgur.com/2j9HOGC.png" width="400">

No escalonamento auto, a missão de decidir qual política de escalonamento será aplicada é dada ao compilador. No exemplo de execução, logo acima, podemos observar que, no caso da execução do programa OpenMPDemoABC, a decisão foi a de tornar o escalonamento simplesmente "static" (igualmente ao visto no caso de teste 1), ou seja, dividir as iterações de preenchimento do vetor entre as threads em 3 fatias contínuas (20 para cada uma). Como a exclusão mútua foi garantida, nenhuma posição do array ficou em branco('-').

**Caso de teste 9 - Scheduling static sem especificação do chunk E SEM EXCLUSÃO MÚTUA:**

<img src="https://i.imgur.com/5HKIA6N.png" width="400">


Neste caso de teste, repetimos a política de escalonamento vista no caso de teste 1 (static) acima, só que desta vez sem garantir a exclusão mútua. É possível observar na foto do exemplo de execução acima que, apesar de todas as iterações serem atribuídas às threads da mesma forma (20 iterações para cada uma das 3 threads, etc.), 3 das 60 posições do array acabaram ficando em branco, o que não acontecia quando a exclusão mútua era garantida. 





## Referências

- [OpenMP Scheduling](http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf)
- [Tutorial OpenMP](https://computing.llnl.gov/tutorials/openMP/)


<a name="readme-topo"></a>

<h1 align='center'>
  🧮 Algoritmo de Classificação LAC
</h1>

<div align='center'>

[![SO][Ubuntu-badge]][Ubuntu-url]
[![IDE][vscode-badge]][vscode-url]
[![Make][make-badge]][make-url]
[![Linguagem][cpp-badge]][cpp-url]

Algoritmos e Estruturas de Dados I <br>
Engenharia de Computação <br>
Prof. Michel Pires da Silva <br>
CEFET-MG Campus V <br>
2024/1  
</div>

<details>
  <summary>
  <b style='font-size: 15px'>
    📑 Sumário
  </b>
  </summary>
  <ol>
    <li><a href="#-introdução">🔍 Introdução</a></li>
    <li>
      <a href="#-fundamentação-teórica">💡 Fundamentação Teórica</a>
      <ul>
        <li><a href='#%EF%B8%8F%EF%B8%8F-fase-de-treino'>🏋️‍♂️ Fase de Treino</a></li>
        <li><a href='#%EF%B8%8F-fase-de-teste'>🏃‍♂️ Fase de Teste</a></li>
      </ul>
    </li>
    <li>
      <a href="#-objetivos">🎯 Objetivos</a>
      <ul>
        <li><a href='#objetivo-geral'>Objetivo Geral</a></li>
        <li><a href='#objetivos-específicos'>Objetivos Específicos</a></li>
      </ul>
    </li>
    <li>
      <a href="#-modelagem-de-aplicação">🔬 Modelagem de Aplicação</a>
      <ul>
        <li><a href='#-estrutura-de-dados'>Estrutura de Dados</a></li>
        <li><a href='#%EF%B8%8F%EF%B8%8F-otimizações-propostas'>Otimizações Propostas</a></li>
        <ul>
          <li><a href='#quando-realizar-interseções'>Quando Realizar Interseções</a></li>
          <li><a href='#irrelevância-para-classificação'>Irrelevância Para Classificação</a></li>
          <li><a href='#uso-do-unordered-set'>Uso do Unordered_set</a></li>
          <li><a href='#diminuição-de-cardinalidade'>Diminuição de Cardinalidade</a></li>
          <li><a href='#cache-de-interseções'>Cache de Interseções</a></li>
          <li><a href='#grid-search-para-hiperparâmetros'>Grid Search para Hiperparâmetros</a></li>
          <li><a href='#multithreading'>Multithreading</a></li>
          <li><a href='#cache-de-similaridade'>Cache de Similaridade</a></li>
          <ul>
            <li><a href='#similaridade'>Similaridade</a></li>
            <li><a href='#threshold'>THRESHOLD</a></li>
            <li><a href='#cache-de-features-com-similaridade'>Cache de Features Com Similaridade</a></li>
          </ul>
        </ul>
      </ul>
    </li>
    <li>
      <a href="#%EF%B8%8F-metodologia">🗳️ Metodologia</a>
      <ul>
        <li><a href='#-arquivos'>Arquivos</a></li>
        <li><a href='#-bibliotecas'>Bibliotecas</a></li>
        <li><a href='#definições-e-estruturas-usadas'>Definições e Estruturas Usadas</a></li>
        <li><a href='#-funções-implementadas'>Funções Implementadas</a></li>
      </ul>
    </li>
    <li>
      <a href="#-testes-e-análises-dos-resultados">📊 Testes e Análises dos Resultados</a>
    </li>
    <li><a href="#-conclusão">🏁 Conclusão</a></li>
    <li>
      <a href="#-começando">🔨 Começando</a>
      <ul>
        <li><a href="#pré-requisitos">Pré-requisitos</a></li>
        <li><a href="#instalando">Instalando</a></li>
      </ul>
    </li>
    <li><a href="#-ambiente-de-compilação-e-execução">🧪 Ambiente de Compilação e Execução</a></li>
    <li><a href="#-contato">📨 Contato</a></li>
    <li><a href="#referencias">📚 Referências</a></li>
  </ol>
</details>

<details> 
  <summary>
    <b style='font-size: 12px'> Abstract </b>
  </summary>
  Utilizando o algoritmo de classificação LAC, este projeto busca estratégias para aprimorar seu desempenho quando aplicado à classificação do conjunto de dados PokerHand Data-Set. Através do uso de conceitos como sistema de caching de informações, otimizações de interseções, predeterminação de classificações, e tabelas hash LSH, estudamos melhores maneiras e adaptações que permitem uma melhor gestão e um controle mais eficiente das classificações feitas pelo algoritmo. Dessa forma, serão apresentados aqui os resultados provenientes de testes e estudos realizados para averiguar a eficácia da implementação dessas estratégias em conjunto com o algoritmo em questão, bem como outras abordagens adotadas com o objetivo de melhorar a performance em termos de tempo e acurácia ao realizar o procedimento. <br><br>
  🔑 <b>Keywords:</b> Lac, Classificação, PokerHand Data-Set, Caching, Otimização, Hash LSH
<br>
</details>

## 🔍 Introdução

<div align='justify'>

  Este [trabalho][trabalho-url] (Algoritmo de Classificação LAC) foi proposto na disciplina de Algoritmos e Estruturas de Dados I (AEDSI) pelo professor [Michel Pires da Silva][github-prof].

  A partir da base do algoritmo apresentado pelo Prof. Dr. Adriano Veloso em sua tese de doutorado [^1], neste projeto, iremos nos aprofundar em conceitos e práticas que visam melhorar a eficácia de tal algoritmo. O algoritmo apresentado em [^1], utiliza um conceito de treino e teste para classificar determinadas bases de dados. Primeiramente, o algoritmo busca mapear os dados a serem classificados por meio de ocorrências selecionadas, denominadas base de treino. Em seguida, ao ser apresentada a base de dados onde será feita a classificação, chamada base de teste, o algoritmo, tendo acesso às informações coletadas no treino, pode utilizar conceitos como similaridade, confiança e suporte para realizar a classificação de determinado conjunto de dados.

  Neste estudo, iremos aplicar tal algoritmo na classificação do conjunto de dados PokerHand Data-Set [^2]. Este conjunto de dados contém todas as combinações possíveis para uma mão de cinco cartas no jogo de pôquer, bem como a categoria ou classe à qual essa mão pertence. A disposição desse conjunto de dados é feita da seguinte forma: formam-se 11 colunas para cada mão a ser analisada, onde cada coluna representa uma carta ou naipe, de forma que a última coluna representa a classe à qual essa mão pertence. Para cada mão, uma série com 11 colunas (10 cartas e 1 classe), as colunas ímpares, começando por 1, representam os naipes, enquanto as colunas pares representam as cartas. Os dados são codificados da seguinte forma:

  - ***NAIPES:*** 
    - **1**- Copas, **2**- Espadas, **3**- Ouros, **4**- Paus

 - ***CARTAS:***
    - **1**- Ás, **2**- Dois, **3**- Três, **4**- Quatro, **5**- Cinco, **6**- Seis, **7**- Sete, **8**- Oito, **9**- Nove, **10**- Dez, **11**- Valete, **12**- Rainha, **13**- Rei

  - ***CLASSES:***
    - **0**- Nada em mãos, **1**- Um par, **2**- Dois pares, **3**- Trinca, **4**- Sequência, **5**- Flush, **6**- Full House, **7**- Quadra, **8**- Straight Flush, **9**- Royal Flush

  **Exemplo de representação (11D):**
  - **Dados:** 1, 11, 1, 13, 1, 10, 1, 12, 1, 1, 9
  - **Codificação:** Copas-Ás, Copas-10, Copas-Valete, Copas-Dama, Copas-Rei, Royal Flush

  Dentre todas as classes presentes em nossa base de dados, algumas aparecem com mais frequência do que outras. Por exemplo, a probabilidade de um jogador obter uma mão Royal Flush é muito menor do que obter uma mão com apenas um par. Sendo assim, a frequência de cada classe é a seguinte. 
  - **0:** Nada em mãos (49,95202%)  
  - **1:** Um par (42,37905%)
  - **2:** Dois pares (4,82207%)
  - **3:** Trinca (2,05118%)
  - **4:** Sequência (0,37185%)
  - **5:** Flush: 54 ocorrências (0,21591%)
  - **6:** Full House: 36 ocorrências (0,14394%)
  - **7:** Quadra (0,02399%)
  - **8:** Straight Flush (0,01999%)
  - **9:** Royal Flush (0,01999%)

  Todas as combinações de cartas e possíveis mãos no jogo de pôquer totalizam mais de um milhão de possibilidades. Para o treinamento de nosso algoritmo, foram selecionados 25 mil exemplos da base de dados original de forma a fornecer informações suficientes para que possamos classificar as demais amostras.

  Dessa forma, empregando a base do algoritmo LAC [^1] para realizar a classificação desta base de dados, iremos buscar meios de otimizar o procedimento de forma a aprimorar a acurácia obtida e o tempo gasto para classificar toda a base de dados.

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 💡 Fundamentação Teórica

<div align='justify'>

  Neste estudo serão utilizadas apenas as funcionalidades básicas do algoritmo LAC, apesar de sua complexidade adicional devido a várias implementações proposta pelo autor. O conceito base do algoritmo envolve o cálculo de variáveis como suporte e confiança para determinar a qual classe uma determinada sequência de dados pertence. Sendo assim, analisemos como se deu o procedimento base até a classificação final.

  ### 🏋️‍♂️ Fase de Treino
  Durante o processo de treinamento, nosso algoritmo mapeia todas as informações fornecidas da seguinte forma: a partir da posição (coluna) em que um dado está disposto, é criado o conceito de tuplas, estruturas do tipo `<Chave, Valor>`, para mapeá-lo. Essas tuplas são formadas a partir do valor da coluna em que o dado se encontra e do valor presente na mesma. Esse processo é realizado para todas as cartas, deixando de fora as classes de cada linha.

  - **Exemplo de tuplas geradas:**
    - **Dados:** [3, 3, 1, 1, 2, 3, 1]
    - **Mapeamento:** [(1, 3), (2, 3), (3, 1), (4, 1), (5, 2), (6, 3), 1]


  Em conjunto com o mapeamento dos dados, são criadas tabelas invertidas responsáveis por armazenar as tuplas/features mapeadas e as linhas onde elas ocorrem. Dessa forma, são criadas estruturas `<Chave, Valor>`, onde as chaves são as tuplas/features e o valor é um array de inteiros contendo todas as linhas onde essa feature ocorre.

  <div align='center'>
    <img src='./images/tabelaFeatures.png' alt='Exemplo de Tabela Invertida de Features' width='300px'>
    <p>Exemplo de Tabela Invertida de Features</p>
  </div>

  De maneira semelhante, é criada uma tabela invertida responsável por armazenar as classes e as linhas onde elas ocorrem, permitindo o controle sobre a qual classe cada linha do treinamento pertence.

  <div align='center'>
    <img src='./images/tabelaClasses.png' alt='Exemplo de Tabela Invertida de Classes' width='300px'>
    <p>Exemplo de Tabela Invertida de Classes</p>
  </div>

  ### 🏃‍♂️ Fase de Teste
  Após realizar o procedimento de treino e ter salvo em memória todas as features presentes na base utilizada, bem como as linhas nas quais elas têm recorrências, e de forma semelhante para as classes, nosso algoritmo passa para a fase de teste. Nessa etapa, é realizada finalmente a classificação de cada linha (mão de pôquer) presente na base de teste. 

  Para realizar tal procedimento, nosso algoritmo executa as seguintes etapas: Primeiramente, realizamos o mapeamento dos dados de forma semelhante à fase de treino, gerando tuplas/features, estruturas do tipo `<Chave, Valor>`. Semelhante à fase de treino, essas features representam as colunas de cada valor e o valor presente em tal posição. 

  <div align='center'>
    <img src='./images/mapeamentoTeste.png' alt='Exemplo de Mapeamento Fase de Teste' width='400px'>
    <p>Exemplo de Mapeamento Fase de Teste</p>
  </div>

  Realizado o mapeamento, acessamos na tabela invertida de features, criada na fase de treino, o array de inteiros que representa cada linha onde determinada feature aparece. Em seguida, realizamos um procedimento que realiza interseções entre os arrays provenientes de cada tupla, ou seja, comparamos quais e quantas linhas determinadas tuplas têm em comum, de forma a fazer a análise combinatória de todas as interseções possíveis. Durante esse procedimento, ao realizar as interseções entre os arrays referentes a cada tupla, calculamos o suporte e a confiança, variáveis responsáveis por classificar cada mão. Ao calcular as interseções para cada combinação de features, iteramos sobre a matriz invertida de classes, criada durante o treinamento, e, para cada classe, fazemos a interseção entre o array de inteiros que representa as linhas onde a classe aparece e o array resultante da interseção da análise combinatória das features. O nosso valor **confiança** recebe o tamanho do vetor resultante dessa interseção.

  Por fim, na iteração para cada classe, para calcular o suporte, dividimos o valor da **confiança** pela quantidade de features presentes na base de dados construída durante o treinamento. Dessa forma, ao calcular o valor de suporte, este é somado em um array `resultado`, que é responsável por guardar a soma do suporte para cada classe. Após realizar todas as análises combinatórias possíveis, a classe atribuída para a mão será aquela que tiver o maior valor de suporte no array `resultado`.

  Concluímos, assim, o procedimento necessário para realizar a classificação de cada mão/linha da base de dados [^2]. A seguir, veremos as otimizações propostas com o objetivo de aprimorar esse procedimento, buscando alcançar resultados mais satisfatórios em termos de tempo e acurácia, bem como a forma com a qual as mesma foram implementadas.

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🎯 Objetivos

<div align="justify">

  ### Objetivo Geral
  O objetivo geral deste estudo é investigar e aprimorar a eficácia do algoritmo Lazy Associative Classifier (LAC) na classificação de uma grande massa de dados, utilizando o PokerHand Data-Set como um caso de estudo [^2]. Busca-se melhorar a precisão e eficiência do processo de classificação, explorando otimizações para especificamente a fase de teste do algoritmo, buscando reduzir o tempo de execução e aumentar a acurácia das classificações realizadas.

  ### Objetivos Específicos
  - Implementar o algoritmo LAC para classificar o PokerHand Data-Set, utilizando a metodologia baseada em suporte e confiança para determinar as classes das mãos de pôquer.
  - Analisar a precisão e o desempenho do algoritmo LAC com a implementação padrão no conjunto de dados, identificando possíveis limitações ou áreas para melhoria. 
  - Desenvolver e testar técnicas de otimização que possam reduzir o tempo de execução do algoritmo sem comprometer a acurácia da classificação. 
  - Comparar os resultados obtidos após a implementação das otimizações com os resultados da implementação padrão, avaliando melhorias em termos de eficiência computacional e precisão da classificação. 

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🔬 Modelagem de Aplicação 

<div aling='justify'>

  Partindo do conceito apresentado acima, é possível abordar diferentes soluções para o nosso estudo. Nesta seção, apresentaremos a forma com a qual modelamos o problema, bem como as estratégias de otimização empregadas para aumentar o desempenho em termos de acurácia das classificações e tempo gasto nessa tarefa. Para implementar nossa solução, a linguagem escolhida foi C++, uma vez que esta nos oferece um paradigma procedural que demonstra ótimo desempenho ao lidar com o processamento de grandes volumes de instruções e também conta com a vantagem de ter diversas estruturas de dados implementadas que facilitam a modelagem da solução

  ### 📊 Estrutura de Dados

  Compreendendo o procedimento proposto pelo algoritmo [^1], torna-se claro que, em alguns casos, estruturas de dados primárias não são suficientes para construir uma solução ideal. Por exemplo, a construção das tabelas invertidas, tanto na fase de treino quanto na de teste. Sendo assim, torna-se mister o uso de estruturas de dados mais complexas, a fim de obter um código limpo, que mantenha a consistência com o modelo apresentado e apresente bom desempenho.

  A começar pelo exemplo citado acima, definamos como e com quais estruturas de dados foram implementadas as tabelas invertidas. Primeiramente, ao mapear as entradas de dados, antes mesmo de começar a preencher as tabelas invertidas de features e classes, utilizou-se da estrutura `pair`, apresentada em [^3]. Essa estrutura nos permite armazenar dois valores em uma única variável, estabelecendo de forma clara o conceito de tupla, ideal para modelar esses dados. Fica evidente que os valores 
  armazenados em cada `pair` correspondem ao número da coluna e ao valor presente na mesma, sendo, portanto, um `pair` 
  do tipo <int,int>. Essa estrutura foi utilizada ainda em outras partes da aplicação, dada sua eficácia e facilidade de uso.

  Para a criação das tabelas invertidas, primeiramente referente às features, utilizou-se da estrutura `unordered_map`, apresentada em [^4]. Tal estrutura representa uma tabela que, novamente, utiliza-se do conceito <Chave, Valor>. Para modelar nossos dados, definiu-se que a chave para cada `unordered_map` seria uma estrutura do tipo `pair`, citada acima, e o valor contido na mesma seria representado por um `vector`, estrutura apresentada em [^5], uma espécie de array dinâmico, otimizado, de  fácil uso e com pouca necessidade de gerenciamento de memória. Nessas tabelas, as estruturas do tipo `pair` representam as features, e as do tipo `vector` representam as linhas onde cada feature teve recorrência. De forma semelhante, deu-se a implementação da tabela invertida referente às classes, sendo que a única divergência se deu ao fato de que as chaves utilizadas para preencher o `unordered_map` foram variáveis do tipo inteiro, que por sua vez representam as classes em questão, enquanto seus valores, também de forma semelhante, são constituídos por um `vector` que armazena as linhas onde as mesmas apareceram na base de treino. Passemos, agora, para estruturas utilizadas durante o teste. 

  Como exemplificado no tópico sobre o fundamento teórico, durante o mapeamento de dados realizado na fase de teste, é necessário utilizar estruturas para modelar as features presentes nesta base de dados, de forma semelhante às usadas durante o processo de treino. Para tanto, utilizou-se a mesma estrutura `pair`. Continuando na mesma fase da aplicação, utilizou-se novamente a estrutura do tipo `vector` para guardar os resultados das combinações feitas entre as features de cada linha. Dessa vez, o valor armazenado foi um outro `vector`, criando-se o conceito de matriz, onde cada linha guardava a combinação das features, estruturas do tipo `pair`, que posteriormente seriam utilizadas para fazer as interseções e o cálculo do suporte e confiança. Por fim, para armazenar os valores provenientes das interseções feitas durante o processo de análise combinatória, foi utilizada, novamente, a estrutura `vector`, uma vez que esta nos possibilita saber a quantidade de elementos armazenados nela de forma nativa, sem a necessidade de implementar nenhuma função auxiliar.

  É importante destacar a impossibilidade de utilizar determinadas estruturas de dados, como *chave*, em tabelas hash de forma nativa. Por exemplo, a necessidade de usar a estrutura `pair` como *chave* em uma estrutura `unordered_map` é um caso específico. Como a linguagem não oferece suporte nativo para funções de hash para determinadas estruturas, foi necessário criar funções de *hash* que lidam com essas estruturas, assim como funções de *equals* para validar a igualdade entre duas estruturas do mesmo tipo, a fim de utilizá-las como chave em outros contextos. Dessa forma, a limitação de não trabalhar de forma nativa pode impactar a performance do nosso algoritmo, embora o benefício da modelagem proposta supere os possíveis malefícios.

  ### 🏋️‍♂️ Otimizações Propostas 

  Inicialmente, para termos noção de quais partes de nosso algoritmo necessitam de mais otimização, criamos a seguinte divisão de tarefas, onde cada parte representa um procedimento realizado. Nota-se que, para esta divisão, não foram contabilizados os procedimentos realizados na fase de treino, uma vez que esta não é contabilizada no tempo de execução. Os procedimentos aqui apresentados dizem respeito apenas à função de teste:

  **1º Procedimento:** Leitura linha a linha do arquivo.

  **2º Procedimento:** Tokenização/Mapeamento das informações contidas no arquivo para variáveis do tipo `vector`.

  **3º Procedimento:** Análise combinatória das tuplas/features, seguida da interseção entre elas.

  **4º Procedimento:** Cálculo de suporte e confiança para a classificação.

  Para o 1º Procedimento, o custo é linear, aumentando conforme a quantidade de linhas a serem lidas. Contudo, pode-se afirmar que, para a leitura de cada linha, este procedimento tem um custo constante, encontrando-se já em uma faixa de rendimento próximo à ideal. Dessa forma, embora existam possíveis otimizações a serem feitas nesse sentido, elas não impactarão de forma crucial a performance de nosso algoritmo.

  Analisando agora o 2º Procedimento, temos que, de forma semelhante ao 1º Procedimento, seu custo está próximo da otimização, uma vez que o custo para realizá-lo é constante $\Theta(K)$, sendo **K** a quantidade de caracteres presentes em cada linha, que é onze, no caso do PokerHand Data-set. Conclui-se, então, que este procedimento também não surte efeito significativo para nossa aplicação.

  Partindo para o 3º procedimento, onde se encontram as maiores oportunidades de implementar otimizações. Esse procedimento consiste em realizar a análise combinatória entre todas as features e, durante esse processo, realizar a interseção entre elas. O fato é que existem diversas formas de implementar tanto a análise combinatória quanto as interseções entre os vetores. Contudo, tais procedimentos exigem um custo computacional significativamente maior em comparação com todos os demais procedimentos realizados até então, independente da forma como forem implementados. A título de exemplificação, consideremos uma amostra da base de dados para ilustrar:

  **Dados:** 1, 11, 1, 10, 1, 12, 3, 8, 1, 9, 4

  Para este exemplo, temos um total de 1023 combinações possíveis, calculadas pela seguinte somatória:

</div>

$$
\sum_{k=1}^{10} C(10,k) = 1023
$$

<div align='justify'> 

  Isso significa que será necessário, na forma padrão da implementação proposta, realizar 1023 processos de interseção entre vetores, sendo que, em alguns casos, se trata de interseções entre nove ou mais vetores. Para cada interseção entre dois ou mais vetores desordenados, que é o caso de nossa aplicação (uma vez que os vetores representam linhas que são dispostas de maneira não ordenada na tabela de features), o custo computacional é dado por $\Theta(n_{1} \cdot n_{2} \cdot n_{3} \cdots n_{k})$, onde $n$ é o tamanho de cada vetor. Com isso, vemos o quanto esse custo pode se tornar elevado à medida que aumenta a quantidade de elementos a serem intersectados.

  #### Quando Realizar Interseções:
  A partir deste ponto, surgem otimizações que podem ter um grande impacto na eficiência do nosso algoritmo. Inicialmente, considerou-se a necessidade de não realizar interseções quando a análise combinatória envolve apenas uma única feature, uma vez que sua interseção será ela mesma. Assim, ao realizar análises combinatórias de forma individual, basta acessar a tabela de features na chave correspondente e retornar as linhas que pertencem a essa feature. Embora essa melhoria pareça insignificante, não havia sido prevista inicialmente no modelo base apresentado na fundamentação teórica, e ela evita processamentos desnecessários. A partir dessa melhoria, define-se o conceito a ser seguido para as próximas otimizações: evitar ao máximo realizar interseções desnecessárias e, se possível, eliminar o processo de análise combinatória.

  #### Irrelevância para Classificação
  Para a próxima otimização implementada, sua origem se deu ao analisar minuciosamente o procedimento de análise combinatória. Verificou-se que, para determinado conjunto de *features*, ao realizar a interseção entre as mesmas, a dimensão do *array* resultante era ínfima, de modo que, ao utilizar essa dimensão para, posteriormente, calcular a confiança e o suporte para cada classe, obtinha-se um valor também ínfimo para cada classificação. Em um cenário ideal, onde o tempo não impacta na performance, seria necessário realizar todas as interseções, ainda que seu resultado se aproximasse da irrelevância para a classificação dos dados. Contudo, não sendo este o cenário proposto, optou-se por interromper o processo de análise ao atingir um determinado estágio, onde as interseções já não tinham impacto considerável nas classificações. Isso ocorre porque, quanto maior o conjunto de *features* analisadas, menor seria o *array* de interseção e, consequentemente, menor o impacto nas classificações. Com a implementação dessa regra de gerenciamento, o número de interseções desnecessárias foi drasticamente reduzido, assim como o processamento necessário para realizá-las. Observou-se também que, conforme previsto, a acurácia das classificações não sofreu prejuízo relevante, mantendo sua média anterior, com a única vantagem de melhorar o tempo gasto para realizá-las.

  #### Uso do `unordered set`
  Em um contexto geral, buscando ainda outras formas de otimização, ao ler a documentação da linguagem utilizada para desenvolver o LAC, viu-se que haviam determinadas estruturas de dados que possuem melhor performance do que as utilizadas na versão inicial de nosso método. Entre tais estruturas de dados, figura-se principalmente o uso de `unordered set`, apresentado em [^7], no lugar do `vector`, estrutura usada para guardar os valores de recorrência de linhas tanto na fase de treino quanto durante o teste. Dessa forma, uma vez que o tempo de pesquisa fornecido pela estrutura `unordered set` se faz em tempo constante $\Theta(1)$, enquanto para o `vector` tem-se o custo linear $\Theta(n)$, em uma grande quantidade de pesquisas feitas, como no caso da nossa implementação do LAC, obtém-se um grande ganho de performance ao longo da execução até que seja realizada toda a classificação da base de dados em questão. Com isso, finalizamos as implementações que buscaram otimizar o LAC enquanto classificava o PokerHand Data-set. Podemos concluir que amplas foram as abordagens de otimização, variando desde simplificações de processos até o estudo da base de dados para buscar melhores resultados, mostrando que diversos são os meios de conseguir melhorar determinados processos e alcançar ganhos em desempenho.

  #### Diminuição de Cardinalidade
  Passemos para a próxima otimização implementada durante este estudo. Tal implementação foi realizada visando seguir o conceito principal para a otimização do algoritmo: fazer o mínimo de interseções possíveis. Contudo, desta vez, a implementação ocorreu ao analisar os dados que eram submetidos para classificação, suas especificidades e como poderíamos usá-los para melhorar a performance. Dessa forma, pensou-se que, se fosse possível diminuir a quantidade de dados a serem processados, sem perder suas características, ou seja, sem desfigurá-los, surtiria em um grande impacto, uma vez que seria necessário realizar menos operações para classificar os mesmos dados. 

  Com isso, através de um estudo específico para a base PokerHand DataSett, viu-se que seria possível diminuir a cardinalidade de cada linha/mão da seguinte forma: como cada par de colunas seguidas em nossa base de dados representa o naipe e o valor da carta, seria natural implementar um processo que possa juntar cada par de colunas, a fim de reduzir pela metade a quantidade de dados a serem classificados. Dessa forma, nossa base teria sua representação de 0 a 52, sendo cada valor a combinação entre um naipe e um valor, formando, assim, uma carta. Diversas são as formas de implementar processos que façam essa codificação, contudo, desde que não atribuam valores repetidos para combinações diferentes, a forma como ele lida com esse processo não impacta significativamente em nosso algoritmo, embora quanto mais simples for a implementação, melhor será o resultado obtido. A seguir, apresentamos a função responsável por fazer a codificação de cada par de valores.

  **Algoritmo: Codificação de uma carta de baralho** 
  ```pseudo
  Entrada: naipe, valor
  Saída: carta

  Função codificação(naipe, valor):
      carta ← (naipe - 1) * 13 + (valor - 1)
      Retorne carta
  ``` 
  A fim de economizar tempo e melhorar a performance, tal procedimento foi implementado durante o mapeamento das features, tanto na fase de treino quanto na fase de teste. Dessa forma, esta foi uma otimização que também impactou o 2º procedimento, não se limitando apenas a ele. A título de exemplo, mostramos como se deu a transformação dos dados para uma determinada série de cartas:

  **Forma Padrão:** 1, 1, 1, 10, 1, 11, 1, 12, 1, 13, 9 

  **Transformação:** 

</div>

<div align='center'>
  (1, 1) → 0 <br>
  (1, 10) → 9 <br>
  (1, 11) → 10 <br>
  (1, 12) → 11 <br>
  (1, 13) → 12 <br>
  (9) → 9 <br>
</div>

<div align='justify'>

  Dessa forma, ao reduzir pela metade os dados a serem utilizados durante o processo de análise combinatória, a eficiência do nosso algoritmo teve um grande aumento, visto que será necessário realizar apenas 31 combinações ao invés de 1023, quantidade necessária para a análise de 10 valores. Essa redução, aplicada a todas as linhas do arquivo de teste, tem um grande impacto em todo o procedimento de classificação, sendo essa uma das principais otimizações implementadas.
 
  #### Cache de Interseções
  Seguindo à risca esse conceito, apresenta-se a primeira grande otimização em relação ao modelo base apresentado anteriormente. Ao percebermos que, durante o processo de análise combinatória e interseção entre as *features* na fase de teste, muitas interseções eram realizadas repetidamente, resultando em um grande tempo de processamento para repetir procedimentos já realizados, surgiu uma ideia para otimizar esse processo. Pensou-se, então, em seguir a ideia de memória **cache**, presente na maioria dos computadores modernos, para utilizar uma tabela que armazenaria a classificação feita para análises combinatórias já realizadas. Dessa forma, ao encontrar um conjunto de *features* já processado anteriormente, nosso algoritmo realizará apenas uma busca na tabela de **cache**, economizando o processamento das interseções e também do cálculo de suporte e confiança. Para maximizar a quantidade de informações possíveis na memória **cache**, ao realizar a análise de um conjunto de *features* ainda não processado, seu resultado/classificação será salvo em uma tabela, tendo como chave um *array* de *pairs* que representam as *features* da análise. Assim, na próxima vez que surgir a necessidade de processar o mesmo conjunto de *features*, será feita uma busca na tabela; se tal conjunto já for uma chave armazenada, o processo de análise será ignorado, pois já se conhece o seu resultado. Para essa abordagem, quanto maior for a quantidade de dados a serem classificados, maior será a utilização da **memória cache** e, consequentemente, maior o ganho de desempenho.

  #### Grid Search para Hiperparâmetros
  A partir do conceito implementado, de não realizar o processo de análise combinatória a partir de um valor mínimo para a dimensão do **array** de interseções, surgiu a necessidade de um parâmetro que definisse qual seria o limite ideal. A partir desse ponto, se deu a próxima implementação que traria resultados de otimização para o LAC. Usando o conceito apresentado por L. He, Z. Gao, Q. Liu, e Z. Yang em [^6], o método **Grid Search** é uma técnica amplamente utilizada no contexto de aprendizado de máquinas, buscando ajustar hiperparâmetros para melhor desempenho de um modelo. A partir de uma série exaustiva de testes, o método retorna o melhor valor encontrado para ser usado em determinado parâmetro. Dessa forma,  este método foi aplicado no contexto do LAC, mais especificamente em qual seria o valor mínimo ideal para a dimensão do vetor de interseções, para que seja relevante continuar realizando o processo de análise combinatória. Seguindo o conceito apresentado pelo GridSearch, o algoritmo foi executado diversas vezes com valores diferentes para o parâmetro em questão. Como resultado da implementação desse método, chegamos a um parâmetro que apresentou o melhor resultado entre todos os testes realizados, concluindo que a dimensão mínima esperada para continuar a análise seria de 10 unidades . Com isso, a implementação já feita, ponderando quando seria viável continuar com o processo de análise combinatória, teve  um impacto ainda mais significativo, uma vez que obtivemos o parâmetro melhor ajustado para quando parar esta etapa. Para mais informações, sua implementação está disponível na branch [`grid-search`][grid-search].

  #### Multithreading
  Uma vez que todos os procedimentos realizados para classificação, mesmo após a implementação de todas as otimizações já citadas, não são dependentes entre si, podemos implementar multithreading em nosso algoritmo, visando a distribuição de tarefas e execução em paralelo. Cada thread pode ser responsável por uma parte específica da análise combinatória e, se necessário, realizar a interseção entre as features. No que diz respeito à análise combinatória e interseção, cada thread pode processar uma parte da análise, permitindo que as tarefas sejam realizadas simultaneamente. A interseção dos resultados pode ser feita pelas threads conforme necessário, garantindo que cada segmento da análise seja tratado de forma eficiente.

  É crucial garantir que cada thread termine sua execução antes que o resultado final seja computado. Problemas de assincronia podem surgir se uma thread terminar sua execução antes de outras, o que pode levar a resultados incorretos ou inconsistentes. Portanto, a sincronização entre threads é essencial para evitar condições de corrida e inconsistências nos resultados, que devem ser gerenciadas cuidadosamente para garantir a integridade dos dados.
  
  Optamos por usar a biblioteca `pthread` em C++ em vez da biblioteca `thread` por várias razões. A pthread oferece maior controle sobre a criação e o gerenciamento de threads, além de fornecer uma gama mais ampla de funcionalidades para sincronização, como mutexes, semáforos e barreiras. Essas ferramentas são essenciais em contextos onde a precisão e o controle detalhado das operações de multithreading são cruciais. Além disso, pthread é uma biblioteca madura e amplamente utilizada, o que a torna uma escolha confiável para aplicações de alto desempenho.

  Decidimos usar cinco threads, pois esse número apresentou o melhor desempenho nos testes realizados. Após experimentarmos com diferentes quantidades de threads, descobrimos que cinco era o número ideal, equilibrando a carga de trabalho entre as threads e evitando a sobrecarga de gerenciamento que poderia ocorrer com um número maior. Esse ajuste permitiu maximizar a eficiência do processamento paralelo, mantendo o uso de recursos em um nível ótimo.

  A implementação de multithreading gerou ótimos resultados, aumentando significativamente a eficiência e a velocidade do algoritmo ao permitir a execução paralela de tarefas.

  Por fim, para o 4º Procedimento, a busca por métodos que pudessem otimizá-lo não obteve tantos resultados quanto para o terceiro procedimento. Como apresentado na seção de fundamento teórico, o processo de cálculo de suporte e confiança para a classificação de determinada linha/mão se dá apenas realizando cálculos matemáticos, uma vez que a análise combinatória e interseções já foram realizadas. Sendo assim, ainda que a implementação do sistema de **memória cache** surtisse impacto também nesta fase da aplicação, reduzindo a necessidade de realizá-lo, não houve outra implementação que pudesse otimizá-lo, visto que o custo de realizar cálculos matemáticos tem pouco impacto durante a execução de nosso algoritmo, uma vez que tais instruções possuem custo de execução constante.

  #### Cache de Similaridade
  Através da última otimização apresentada, os resultados de tempo de execução e acurácia tiveram um grande impacto positivo, possibilitando classificar massas de dados que antes não eram possíveis devido ao seu tamanho. Sendo assim, com o intuito de, ainda trabalhando com o conceito de memória cache, obter performances ainda melhores, surgiu a próxima otimização, que será uma melhoria do conceito já apresentado. Inicialmente, o benefício que a memória cache trazia para o fluxo de execução era a vantagem de não precisar realizar determinadas interseções, aproveitando as que já foram feitas e armazenadas em cache, evitando processamento desnecessário e ganhando tempo. Contudo, a nova forma de implementar o sistema de cache se deu de maneira diferente, apresentando resultados ainda melhores de tempo e precisão durante as classificações.

  Na primeira versão do cache, embora houvesse vantagens ao reutilizar interseções já realizadas, ainda era necessário realizar uma grande quantidade de interseções, dado que o processo de análise combinatória gera uma quantidade significativa de agrupamentos de features, tornando inviável realizar todas. Assim, buscando reduzir ainda mais a necessidade de realizar interseções desnecessárias, a nova abordagem visa classificar uma linha de forma completa baseada na sua similaridade com linhas já classificadas, sem precisar realizar nenhum processo de análise combinatória e, portanto, nenhuma interseção.

  Embora utilize conceitos diferentes para o reaproveitamento de processamento, a memória cache utilizando similaridade de linhas opera de maneira semelhante à primeira versão, em seu conceito básico. Seguindo o fluxo da aplicação, inicialmente não há como aproveitar nenhum processamento já realizado; porém, de maneira semelhante à primeira versão, quanto mais linhas o algoritmo classificar, maior será o ganho produzido pelo sistema de cache. Uma vez realizada a classificação da primeira linha através do conceito base de interseções, o resultado da classificação é armazenado em uma tabela hash; contudo, agora a chave da tabela será um vector de pair, representando as features formadas pela linha classificada, e o valor armazenado será a classe resultante da classificação realizada.

  A partir da segunda linha, antes mesmo de iniciar o procedimento de análise combinatória, as features geradas através do mapeamento passam por um processo de análise de similaridade com as linhas já classificadas, que constituem as chaves da tabela de memória cache com similaridade. Se houver uma linha já mapeada que apresente similaridade suficiente com a linha a ser mapeada, o valor de classificação atribuído à linha já classificada também será atribuído à linha a ser classificada, uma vez que a probabilidade de ambas pertencerem à mesma classe é muito grande. A partir dessa lógica, a necessidade de se realizar o processo de análise combinatória e interseções diminui ainda mais, resultando em um grande ganho de desempenho. Conforme são feitas as classificações, seus resultados são armazenados na tabela, de forma que chega um determinado ponto da aplicação em que não é mais necessário processar nenhuma nova linha para classificação, uma vez que são grandes as chances de se ter uma linha com coeficiente de similaridade semelhante ao apresentado.

  ##### Similaridade
  A partir do conceito apresentado, surge a necessidade de se definir qual método será empregado para determinar a similaridade entre duas linhas, de forma a calcular se ambas são suficientemente 'parecidas' para que compartilhem da mesma classe. Existem diversos meios de realizar tal processo, contudo, ainda que haja métodos com desempenho semelhante, a similaridade de cossenos foi a escolhida para essa tarefa. Uma vez que o procedimento para calcular a similaridade entre dois vetores espaciais é realizado pelo método de similaridade de cossenos, através de uma série de cálculos matemáticos, sua eficácia é muito alta, assim como o tempo gasto para realizar tal processo. Este foi o principal aspecto considerado para a utilização desse método, pois, uma vez que a similaridade entre duas linhas é calculada diversas vezes durante a execução, outro método de cálculo de similaridade que exigisse mais processamento acabaria por prejudicar o desempenho do processo. Dessa forma, métodos como o cálculo de similaridade de Jaccard não foram empregados, dado o tempo necessário para realizar o cálculo de similaridade em cada operação. 

  ##### THRESHOLD
  O segundo conceito a ser analisado, para a correta implementação da otimização apresentada acima, é o limite necessário de similaridade entre duas linhas, para que a classificação de uma possa ser feita com base na outra. Surge, assim, o conceito de threshold, uma relação mínima de semelhança. Ao utilizar o método de similaridade de cossenos, observou-se que, embora apresente bons resultados, ainda há casos em que o cálculo de similaridade pode gerar ruídos, ou seja, imprecisões aleatórias durante o cálculo, como, por exemplo, aproximar números como 7 e 8 de forma a gerar um valor alto de similaridade, mesmo que tal fato não seja verdadeiro para a base de dados com a qual estamos lidando. Dessa forma, torna-se necessária uma relação de similaridade alta, para que esses ruídos não interfiram no processo e gerem classificações imprecisas. Para que pudéssemos escolher um valor ideal para o threshold, foi necessário realizar uma série de testes, o mesmo que usamos para saber o valor mínimo de interseções necessárias para continuar a análise combinatória, o Grid Search. A partir desses testes, chegamos a um valor de 0.8, que apresentou os melhores resultados para a base de dados com a qual estamos lidando, garantindo que a semelhança entre duas linhas seja suficientemente alta para que sejam agrupadas na mesma classificação. Esta implementação do Grid Search para o threshold está disponível na mesma branch do Grid Search, [`grid-search`][grid-search].

  ##### Cache de Features Com Similaridade
  A otimização referente ao cache de similaridade entre as linhas surtiu uma grande melhoria no desempenho do algoritmo, uma vez que a necessidade de realizar interseções — processo que possui maior custo computacional — diminuiu drasticamente. Com isso, foi possível seguir para a próxima implementação. Através do conceito inicial de cache de features, com o intuito de reaproveitar o processamento de features já realizado, surgiu a ideia de aplicar esse mesmo conceito somado ao cálculo de similaridade entre as features presentes no cache, garantindo melhor aproveitamento dos cálculos já efetuados.

  Uma vez iniciado o processo de análise combinatória, após verificar se existe alguma chave presente no cache de interseções que corresponda à que será processada, caso não exista, nosso algoritmo, ao invés de partir diretamente para a realização de interseções entre as features, busca primeiramente se há alguma chave no cache que possua a similaridade desejada com a nova linha de features a ser analisada. A similaridade, assim como no modelo apresentado anteriormente, é calculada a partir do método de similaridade de cossenos.

  No contexto em que já exista uma chave suficientemente similar, o valor dessa chave — que corresponde ao suporte atribuído a cada classe ao processar esse conjunto de features — será novamente atribuído às classes, pois, dado que a similaridade entre os dois conjuntos de features é notável, é grande a possibilidade de que ambos resultem na mesma classificação.

  O resultado dessa implementação, como já mencionado, foi um uso ainda mais eficiente dos cálculos realizados em features anteriores. Com isso, o desempenho do algoritmo durante as classificações foi ainda mais aprimorado, uma vez que o processo de análise combinatória e de interseções foi otimizado, sendo executado por completo apenas quando não há nenhum cálculo previamente realizado que possa ser reutilizado.

</div>

## 🗳️ Metodologia

<div align="justify">

  As abordagens propostas para otimizar o algoritmo LAC foram implementadas em C++, utilizando a IDE Visual Studio Code para o desenvolvimento do código-fonte. O projeto foi organizado em um diretório principal, contendo subdiretórios para armazenar os arquivos de código-fonte, os datasets utilizados e os arquivos de saída. A implementação do algoritmo LAC foi dividida em duas etapas principais: a fase de treinamento e a fase de teste. Durante a fase de treinamento, o algoritmo mapeia todas as informações fornecidas e cria tabelas invertidas para armazenar as features e as classes presentes na base de treinamento. Na fase de teste, o algoritmo classifica as mãos de pôquer presentes na base de teste, utilizando a metodologia baseada em suporte e confiança para determinar as classes das mãos de pôquer.

  ### 📁 Arquivos 

  Para a implementação do algoritmo LAC, o projeto foi organizado em um diretório principal, contendo subdiretórios para armazenar os arquivos de código-fonte, os datasets utilizados e os arquivos de saída. A seguir, são apresentados os arquivos e diretórios utilizados no projeto:

  - [`datasets/`](datasets): diretório contendo os datasets utilizados para treinamento e teste do algoritmo LAC.
    - [`poker-hand-training.data`](datasets/poker-hand-training.data): arquivo contendo os dados utilizados para treinar o algoritmo LAC, sendo a base de treinamento do PokerHand Data-Set do UCI Machine Learning Repository [^2].
    - [`poker-hand-testing.data`](datasets/poker-hand-testing.data): arquivo contendo os dados utilizados para testar o algoritmo LAC, sendo a base de teste do PokerHand Data-Set do UCI Machine Learning Repository [^2].
    - [`base-avaliacao/poker-hand-training.data`](datasets/base-avaliacao/poker-hand-training.data): arquivo contendo os dados utilizados para treinar o algoritmo LAC, sendo a base de treinamento do PokerHand Data-Set modificada pelo professor [Michel][github-prof].
    - [`base-avaliacao/poker-hand-testing.data`](datasets/base-avaliacao/poker-hand-testing.data): arquivo contendo os dados utilizados para testar o algoritmo LAC, sendo a base de teste do PokerHand Data-Set modificada pelo professor [Michel][github-prof].
    - [`output.txt`](datasets/output.txt): arquivo de saída contendo os resultados da classificação das mãos de pôquer.

  - [`images/`](images): diretório contendo as imagens utilizadas na documentação do projeto.
    - [`tabelaFeatures.png`](images/tabelaFeatures.png): imagem ilustrando um exemplo de tabela invertida de features.
    - [`tabelaClasses.png`](images/tabelaClasses.png): imagem ilustrando um exemplo de tabela invertida de classes.
    - [`mapeamentoTeste.png`](images/mapeamentoTeste.png): imagem ilustrando um exemplo de mapeamento na fase de teste.
    - [`cossenoComReducao.jpeg`](images/cossenoComReducao.jpeg): imagem ilustrando o cálculo de similaridade de cossenos com redução de dimensionalidade.
    - [`cossenoSemReducao.jpeg`](images/cossenoSemReducao.jpeg): imagem ilustrando o cálculo de similaridade de cossenos sem redução de dimensionalidade.
    - [`reducaoCardinalidade.jpeg`](images/reducaoCardinalidade.jpeg): imagem ilustrando a redução da cardinalidade das features.

  - [`src/`](src): diretório contendo os arquivos de código-fonte do projeto.
    - [`main.cpp`](src/main.cpp): arquivo contendo a função principal do programa, responsável por realizar a classificação das mãos de pôquer.
    - [`lac.cpp`](src/lac.cpp): arquivo contendo a implementação das funções do algoritmo LAC.
    - [`lac.hpp`](src/lac.hpp): arquivo contendo a definição das funções do algoritmo LAC.

  - [`.gitignore`](.gitignore): arquivo contendo a lista de arquivos e diretórios a serem ignorados pelo Git.
  - [`make.sh`](make.sh): arquivo de script para compilar o código-fonte do projeto.
  - [`makefile`](makefile): arquivo contendo as regras para compilar o código-fonte do projeto.
  - [`README.md`](README.md): arquivo contendo a documentação do projeto.

  De uma forma compacta e organizada, os arquivos e diretórios estão dispostos da seguinte forma:

  ```.
  |
  ├── datasets
  |   |   ├── base-avaliacao
  │   │   │   ├── poker-hand-testing.data
  │   │   |   └── poker-hand-training.data
  │   │   ├── poker-hand-testing.data
  │   │   ├── poker-hand-training.data
  │   │   └── output.txt
  ├── images
  │   │   ├── tabelaFeatures.png
  │   │   ├── tabelaClasses.png
  │   │   ├── mapeamentoTeste.png
  │   │   ├── cossenoComReducao.jpeg
  │   │   ├── cossenoSemReducao.jpeg
  │   │   └── reducaoCardinalidade.jpeg
  ├── src
  │   │   ├── main.cpp
  │   │   ├── lac.cpp
  │   │   └── lac.hpp
  ├── .gitignore
  ├── make.sh
  ├── makefile
  └── README.md
  ```
  
</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

### 📚 Bibliotecas

<div align="justify">

  As bibliotecas utilizadas na implementação do algoritmo LAC são as seguintes:

  - [`lac.hpp`](src/lac.hpp): biblioteca criada pelos autores do projeto, contendo a definição das funções do algoritmo LAC.
  - [`bits/stdc++.h`](https://www.geeksforgeeks.org/bitsstdc-h-c-include/): biblioteca que inclui todas as bibliotecas padrão da linguagem C++. Veja abaixo as bibliotecas que usamos da `bits/stdc++.h`:
    - [`iostream`](https://www.cplusplus.com/reference/iostream/): biblioteca padrão de entrada e saída de dados.
    - [`fstream`](https://www.cplusplus.com/reference/fstream/): biblioteca para manipulação de arquivos. 
    - [`string`](https://www.cplusplus.com/reference/string/): biblioteca para manipulação de strings.
    - [`cmath`](https://www.cplusplus.com/reference/cmath/): biblioteca para funções matemáticas.
    - [`vector`](https://www.cplusplus.com/reference/vector/): biblioteca para manipulação de vetores.
    - [`unordered_map`](https://www.cplusplus.com/reference/unordered_map/): biblioteca para manipulação de tabelas hash.
    - [`unordered_set`](https://www.cplusplus.com/reference/unordered_set/): biblioteca para manipulação de conjuntos hash.
    - [`pthread.h`](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html): biblioteca para programação paralela.
  
</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

### Definições e Estruturas Usadas

<div align="justify">

  No arquivo [`lac.hpp`](src/lac.hpp), são definidas as estruturas de dados utilizadas no algoritmo LAC, bem como algumas variáveis globais. A seguir, são apresentadas as definições e estruturas utilizadas:

  - **`#define MIN_SUPPORT 0`**: definição do suporte mínimo para a classificação de uma linha.

  - **`#define DECREASE_CARDINALITY 1`**: definição para reduzir a cardinalidade das features.

  - **`#define USE_COSINE_SIMILARITY 1`**: definição para usar a similaridade de cossenos.

  - **`#define THRESHOLD 0.8`**: definição da confiança mínima para a classificação de uma linha.

  - **`#define MAX_COMBS 3`**: definição do número máximo de combinações de features a serem analisadas.

  - **`using namespace std`**: definição do namespace padrão da linguagem C++.

  - **`struct pairHashSimilarity`**: Essa estrutura define uma função hash para um par (`std::pair`). A função hash é uma implementação personalizada que combina os valores hash dos dois elementos do par usando a operação XOR (`^`).
    - `hash<T1>{}(p.first)`: Calcula o valor hash do primeiro elemento do par.
    - `hash<T2>{}(p.second)`: Calcula o valor hash do segundo elemento do par.
    - `hash1 ^ hash2`: Combina os dois valores hash usando XOR.

  - **`template <typename T> void hashCombine(size_t& seed, T const& v)`**: Essa é uma função auxiliar para combinar de forma mais robusta um valor hash existente (`seed`) com um novo valor (`v`), utilizando uma fórmula que inclui deslocamento de bits e uma constante mágica (`0x9e3779b9`), que ajuda a espalhar os bits de forma mais uniforme.
    - `seed ^= hash<T>()(v)`: Combina o valor hash de v com o seed existente usando XOR.
    - `0x9e3779b9`: Constante usada para espalhar bits de forma uniforme, derivada da proporção áurea.
    - (`seed << 6`) e (`seed >> 2`): Operações de deslocamento de bits que ajudam a misturar os bits do seed.

  - **`struct pairHash`**: Essa estrutura define uma função hash para um par (`std::pair`) usando a função hashCombine, que combina de maneira mais robusta os hashes dos dois elementos do par.
    - `retval = hash<T>()(rhs.first)`: Calcula o valor hash do primeiro elemento e o armazena em retval.
    - `hashCombine(retval, rhs.second)`: Combina retval com o hash do segundo elemento usando hashCombine.
    - `return retval`: Retorna o valor hash combinado.

  - **`struct vectorPairHash`**: Essa estrutura define uma função hash para um `vector` de pares de inteiros, utilizando `pairHash` para calcular os hashes dos elementos no vetor e combiná-los.
    - `pairHash{}(p)`: Calcula o hash de cada par no vetor
    - `seed ^= ...`: Combina o hash de cada par com o `seed` existente usando XOR e uma constante mágica.
    - `return seed`: Retorna o hash final para o vetor.

  - **`struct vectorPairEqual`**: Essa estrutura define uma função de igualdade para `vector` de pares de inteiros. Ela compara dois vetores para ver se eles são iguais.
    - `return lhs == rhs`: Retorna verdadeiro se os dois vetores forem iguais, falso caso contrário.

  - **`using cacheKey = unordered_set<pair<int, int>, pairHashSimilarity>`**: Essa linha define um `typedef` ou `using` para `unordered_set` de pares de inteiros com a função hash `pairHashSimilarity`.

  - **`using cacheValue = int`**: Essa linha define um `typedef` ou `using` para um inteiro, que será o valor armazenado na tabela de cache.

  - **`struct ThreadData`**: Essa estrutura define os dados que serão passados para cada thread durante a execução paralela do algoritmo LAC.
    - `vector<unordered_set<pair<int, int>, pairHash>>* combinationsFeatures`: Ponteiro para o vetor de combinações de features.
    - `unordered_map<pair<int, int>, unordered_set<int>, pairHash>* features`: Ponteiro para a tabela de features.
    - `unordered_map<int, unordered_set<int>>* classes`: Ponteiro para a tabela de classes.
    - `unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual>* similarityCache`: Ponteiro para a tabela de cache de similaridade.
    - `bool* shouldStop`: Ponteiro para a variável que indica se o processo de análise deve ser interrompido.
    - `int start`: Índice de início do intervalo de combinações.
    - `int end`: Índice de fim do intervalo de combinações.
    - `double* result`: Ponteiro para a variável que armazena o resultado da classificação.

  - **`class Lac`**: Essa classe define o algoritmo LAC, onde as funções são separadas em `private`e `public`:
    - `private`:
      - `unordered_map<pair<int, int>, unordered_set<int>, pairHash> features`: Tabela de features.
      - `unordered_map<int, unordered_set<int>> classes`: Tabela de classes.

    - `public`:
      - `Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes)`: Construtor da classe Lac.
      - `void training(string path)`: Função de treinamento do algoritmo LAC.
      - `float testing(string path)`: Função de teste do algoritmo LAC.
      - `static unordered_set<int> intersectionAll(vector<unordered_set<int>> list)`: Função para calcular a interseção de todos os conjuntos em uma lista.
      - `int findMaxIndex(double* arr, int size)`: Função para encontrar o índice do maior valor em um array.
      - `vector<int> splitString(string line)`: Função para dividir uma string em um vetor de inteiros.
      - `vector<unordered_set<pair<int, int>, pairHash>> combinations(const vector<pair<int, int>>& c, int k)`: Função para gerar todas as combinações de tamanho k de um vetor de pares de inteiros.
      - `void populateCache(cacheKey lineFeatures, cacheValue classesSupport)`: Função para popular a cache de similaridade.
      - `static pair<vector<double>, double> checkSimilarity(cacheKey lineFeatures)`: Função para verificar a similaridade entre duas linhas.
      - `static double cosineSimilarity(const vector<pair<int, int>>& set1, const vector<pair<int, int>>& set2)`: Função para calcular a similaridade de cossenos entre dois vetores.
      - `static void* threadIntersection(void* arg)`: Função para realizar a interseção em paralelo.
      - `static int INTERSECTION_LIMIT`: Limite de interseção.
      - `static unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual> similarityCache`: Tabela de cache de similaridade.
      - `static pthread_mutex_t mutex`: Mutex para sincronização entre threads.

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

### 📝 Funções Implementadas

<div align="justify">

  A seguir, são apresentadas as funções implementadas no algoritmo LAC, bem como a descrição de suas funcionalidades:

  #### [**`int main()`**](src/main.cpp)
  Esta função é responsável por apenas fazer o gereciamento do fluxo de execução do programa, chamando as funções necessárias para realizar a classificação das mãos de pôquer, medir o tempo e inicialmente escolher se será executado reduzindo a cardinalidade ou não. Além de no final, fazer o cálculo do resultado, relacionando a acurácia e o tempo de execução.

  #### [**`int Lac::INTERSECTION_LIMIT = 0`**](src/lac.hpp)
  Variável estática que define o limite de interseção para a análise combinatória.

  #### [**`pthread_mutex_t Lac::mutex = PTHREAD_MUTEX_INITIALIZER`**](src/lac.hpp)
  Variável estática que define o mutex para sincronização entre threads.

</div>

#### [**`unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual> Lac::similarityCache`**](src/lac.hpp)

<div align='justify'>

  Tabela de cache de similaridade para armazenar os valores de similaridade entre as linhas.

</div>

#### [**`Lac::Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes) )`**](src/lac.cpp)

<div align='justify'>

  Construtor da classe Lac, responsável por inicializar as variáveis necessárias para a execução do algoritmo LAC. Sendo elas a tabela de features e a tabela de classes.

</div>

#### [**`void Lac::trainig(string path)`**](src/lac.cpp)

<div align='justify'>

  Função responsável por realizar a fase de treinamento do algoritmo LAC, mapeando as features e as classes presentes na base de treinamento. Nesta função, é feito a leitura do arquivo de treinamento, mapeamento das features e classes, e a redução da cardinalidade, caso seja escolhido. A redução da cardinalidade é feita pegando os valores de uma carta e um naipe, e transformando em um valor único, de 0 a 51, para cada carta. Caso a redução da cardinalidade não seja escolhida, o algoritmo segue normalmente, mapeando as features e classes da base de treinamento. 

</div>

#### [**`float Lac::testing(string path)`**](src/lac.cpp)

<div align='justify'>

  Essa função é responsável por testar o algoritmo LAC, classificando as mãos de pôquer presentes na base de teste. A função segue os seguintes passos:
  1. **Abertura dos Arquivos**: Abre o arquivo de teste e o arquivo de saída.
  2. **Inicialização das Variáveis**: Inicializa as variáveis necessárias para a classificação. 
  -`int j = 1`: Essa variável é usada para contar as linhas do arquivo de teste (ou seja, os exemplos sendo processados).
  - `int loss = 0`, `accuracy = 0`: Essas variáveis são usadas para contar quantas vezes o classificador errou (loss) e quantas vezes acertou (accuracy).
  - `pthread_mutex_init(&mutex, NULL)`: Inicializa o mutex para sincronização entre threads.
  3. **Processamento Linha a Linha do Arquivo de Teste**: A função lê o arquivo linha por linha utilizando `getline(file, line)`. Para cada linha:
  - *Extração dos Valores*: A função chama `splitString(line)` para dividir a linha em um vetor de inteiros (`values`), representando os atributos da amostra de teste e sua classe esperada (o último valor).
  - *Inicialização de Resultados*: Um array `result[]` é inicializado com zeros. Ele terá o tamanho do número de classes e será preenchido com os valores de confiança (suporte) para cada classe.
  - *Extração das Features*: A linha é convertida em uma lista de pares `lineFeatures`, que são as features (características) extraídas do exemplo de teste. Dependendo do valor de `DECREASE_CARDINALITY`, as features são processadas de duas formas:
    - `DECREASE_CARDINALITY = true`: Os valores são processados aos pares e combinados em índices. Isso parece ser uma maneira de reduzir a cardinalidade dos dados.
    `DECREASE_CARDINALITY = false`: Cada valor da linha é considerado individualmente como uma feature.
  4. **Combinando as Features**: Aqui inicia o processo de combinação das features. Onde: 
  - O vetor `lineFeatures` armazena os atributos (*features*) de cada linha do conjunto de dados.
  - O vetor `combinationsFeatures` é uma estrutura que vai armazenar todas as combinações de atributos geradas para cada exemplo de teste.
  - A função `combinations(lineFeatures, q)` será usada para gerar as combinações de features, onde `q` representa o tamanho das combinações que serão geradas (1 a 1, 2 a 2, e assim por diante até o tamanho máximo `MAX_COMBS`).
  - Também é definida a variável `shouldStop`, que indica se o processo de análise deve ser interrompido, caso a dimensão do vetor de interseções seja menor que um valor mínimo (`INTERSECTION_LIMIT`).

  5. **Processamento Paralelo das Combinações**: Aqui, o algoritmo divide o trabalho de processamento das combinações entre várias threads, para acelerar o processo. Cada thread é responsável por processar um intervalo de combinações.
  - *Inicialização das Threads*:
    - Defini-se número de threads (`numThreads = 5`) que serão usadas para processar as combinações.
    - Cada thread é representada por um objeto `pthread_t`, e o código cria um array de threads.
    - A estrutura `ThreadData` contém os dados que serão passados para cada thread.
    - A variável `chunkSize` armazena o tamanho do intervalo de combinações que cada thread irá processar.

  - *Divisão de Tarefas*: Cada thread recebe uma estrutura de dados `ThreadData`, que contém os seguintes elementos:
    - `combinationsFeatures`: Um ponteiro para o vetor de combinações de features que a thread irá processar.
    - `features`: Um ponteiro para o conjunto completo de features do conjunto de dados.
    - `classes`: Um ponteiro para as classes associadas ao conjunto de dados.
    - `start` e `end`: Definem o intervalo de combinações de features que essa thread irá processar.
      - `start` é calculado como `t * chunkSize`.
      - `end` é calculado como `(t + 1) * chunkSize`, exceto para a última thread, que processa até o fim de `combinationsFeatures`.
    - `result`: Uma referência ao vetor que armazena os resultados das classificações feitas pela thread.
    - `shouldStop`: Um ponteiro para a variável de controle que indica se o processamento deve parar.
    - `similarityCache`: Um ponteiro para o cache de similaridades, que pode ser usado para acelerar o cálculo de interseções, evitando a recomputação de resultados que já foram processados anteriormente.
  
  - *Criação das Threads*: Depois de atribuir os dados, o código cria a thread com a função `pthread_create`. Cada thread executa a função `threadIntersection`, que é responsável por processar as combinações de features e calcular as interseções.

  - *Espera pelo Término das Threads*: Após criar todas as threads, o código aguarda o término de cada uma delas com a função `pthread_join`.

  6. **Classificação das Linhas**: Após o processamento paralelo das combinações, o algoritmo classifica as linhas com base nas interseções de features geradas. A função `classification` é responsável por determinar a classe de cada linha com base nas interseções de features. Logo após, é chamado um `pthread_mutex_destroy(&mutex)` para destruir o mutex utilizado para sincronização entre threads. Depois é escrito o resultado da classificação no arquivo de saída, contendo a linha e a sua classificação.

  7. **Cálculo da Acurácia**: O algoritmo calcula a acurácia da classificação, comparando as classes reais com as classes previstas. A acurácia é o número de acertos dividido pelo número total de exemplos de teste.

  8. **Fechamento dos Arquivos**: Por fim, o algoritmo fecha os arquivos de entrada e saída.

</div>

#### [**`vector<int> Lac::splitString(string line)`**](src/lac.cpp)

<div align='justify'>
  
  Função responsável por dividir uma string em um vetor de inteiros, separando os valores por vírgula.

</div>

#### [**`unordered_set<int> Lac::intersectionAll(vector<unordered_set<int>> lists)`**](src/lac.cpp)

<div align='justify'>
  
  Função responsável por realizar a interseção entre todas as listas de inteiros presentes em um vetor de conjuntos.

</div>

#### [**`int Lac::findMaxIndex(double* arr, int size)`**](src/lac.cpp)

<div align='justify'>

Função responsável por encontrar o índice do maior valor em um vetor de doubles.

</div>

#### [**`vector<unordered_set<pair<int, int>, pairHash>> Lac::combinations(const vector<pair<int, int>>& c, int k)`**](src/lac.cpp)

<div align='justify'>

  Essa função recebe como parâmetros o vetor `c` e o número de elementos na combinação `k`. 
  - Seu principal objetivo é retornar um vetor contendo todas as combinações dos elementos de `c`, cada uma com `k` valores. A função utilizada foi escolhida por ser diferente das que estamos habituados a usar (as com recursão ou similares), justamente pela curiosidade.
  - Ela lança mão de operações bit a bit e shifting para retornar o resultado. De forma muito simplificada, a cada iteração ela mapeia os elementos do vetor `c` com um número binário "comb" para informar as combinações de `c` com `k` valores.
  - Ela inicia sempre das "combinações" de apenas um elemento e vai até a de `k` elementos. Porém, existe uma condicional que adiciona no vetor de resultados apenas as junções de `k` valores.
  - **Exemplo**: Suponhamos que temos um vetor de 4 elementos: `c = {A, B, C, D}` e queremos gerar todas as combinações de 2 elementos (`k = 2`).
    - *Inicialização*:
      - `n = c.size() = 4` // (número de elementos na coleção `c`).
      - `combo = (1 << k) - 1 = (1 << 2) - 1 = 0b11` // (ou seja, 3 em decimal), que representa a combinação inicial `{A, B}` (os dois primeiros bits estão definidos como 1).

    - *Primeira Iteração*:
      - Estado Inicial:
        - `combo = 0b0011` // (representa o primeiro e segundo elementos do vetor `c`: `(A e B)`).
      
      - Inserção: os elementos correspondentes são inseridos no vetor de combinações no "for-loop" interno:
        - Para `i = 0`:
          - `combo >> 0` // `0b0011` (não muda).
          - `(combo >> 0) & 1` // `0b0011 & 0b0001 = 0b0001 → 1` (verdadeiro).
          - `c[i] = A` // inserido em "currentComb".
      E assim sucessivamente até `n`. Após isso, como `k = 2`, nenhum desses valores é aproveitado e o vetor "result" permanece vazio.

    - *Cálculos*:
      - `x = combo & -combo` // `x = 0b0011 & 0b0101 = 0b0001`.
      - `y = combo + x` // `y = 0b0011 + 0b0001 = 0b0100`.
      - `z = combo & ~y` // `z = 0b0011 & ~0b0100 = 0b0011 & 0b1011 = 0b0011`.
      - `combo = z / x` // `combo = 0b0011 / 0b0001 = 0b0011`.
      - `combo >>= 1` // `combo = 0b0011 >> 1 = 0b0001`.
      - `combo |= y` // `combo = 0b0001 | 0b0100 = 0b0101`.

    - *Novo combo*:
      - `combo = 0b0101`.

    E o processo se repete até acabar as combinações de `k` elementos.

</div>

#### [**`void Lac::populateCache(cacheKey lineFeatures, cacheValue classesSupport)`**](src/lac.cpp)

<div align='justify'>

  Função responsável por popular a cache de similaridade, armazenando os valores de similaridade entre as features de uma linha e as classes associadas a ela.

</div>

#### [**`pair<vector<double>, double> Lac::checkSimilarity(cacheKey lineFeatures)`**](src/lac.cpp)

<div align='justify'>

  Função responsável por verificar a similaridade entre as features de uma linha e as features presentes no cache de similaridade.
  - Nessa função, é feito um loop para percorrer todas as linhas presentes no cache de similaridade, chamando a função `cosineSimilarity` para calcular a similaridade entre as features da linha e as features presentes no cache. Caso a similaridade seja maior que o threshold, a função retorna um par contendo a classe da linha e a similaridade calculada.

</div>

#### [**`double Lac::cosineSimilarity(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2)`**](src/lac.cpp)

<div align='justify'>

  Essa função calcula a similaridade de cosseno entre dois vetores de pares de inteiros. Ela mede a similaridade entre dois vetores num espaço de n-dimensões, independentemente de sua magnitude. A similaridade de cosseno é amplamente utilizada em algoritmos de classificação e recuperação de informações para medir a semelhança entre dois conjuntos de dados. Quanto maior o cosseno entre dois vetores, menor o ângulo entre eles e, consequentemente, maior a similaridade. Ela funciona da seguinte forma:
  - *Produto Escalar*: O produto escalar (`dotProduct`) é calculado somando os produtos dos segundos valores de cada par (os valores `second` de `vec1` e `vec2`).
  - *Magnitude dos Vetores*: As magnitudes dos vetores são calculadas somando os quadrados dos valores do segundo elemento de cada par em `vec1` e `vec2`. Estas magnitudes são usadas para normalizar o produto escalar, permitindo uma comparação entre vetores de diferentes tamanhos.
  - *Raiz Quadrada das Magnitudes*: Após calcular as somas dos quadrados, a função tira a raiz quadrada para obter as magnitudes reais dos vetores.
  - *Retorno da Similaridade*: A similaridade de cosseno é dada pela divisão do produto escalar pela multiplicação das magnitudes dos dois vetores.
  - O valor retornado estará no intervalo de -1 a 1, onde:
    - 1 significa que os vetores estão perfeitamente alinhados (mesma direção).
    - 0 significa que não há correlação (os vetores são ortogonais).
    - -1 significa que os vetores estão em direções opostas.

Em resumo similaridade de cossenos é calculada pela fórmula:

$$
cos(\theta) = \frac{A \cdot B}{||A|| \cdot ||B||}
$$

</div>

#### [**`void* Lac::threadIntersection(void* arg)`**](src/lac.cpp)

<div align='justify'>

  Essa função é o núcleo da execução paralela no código que trabalha com combinações de features para realizar classificações. Ela é executada por cada thread criada, e cada uma processa uma parte do conjunto de combinações de features para calcular a interseção entre elas e as classes.
  1. **Conversão do Argumento**: O argumento da função (`arg`) é convertido de `void*` para `ThreadData*`. Isso porque a função `pthread_create` passa o dado como um ponteiro genérico, e é necessário convertê-lo de volta para o tipo específico.
  2. **Laço de Processamento das Combinações**: A função processa as combinações de features no intervalo entre `start` e `end` definido para a thread. Converte-se a combinação atual (um `unordered_set` de pares) para um vetor de pares chamado `combinacoesCacheVec`.
  3. **Verificação no Cache de Similaridade**: Se a combinação já existe no cache de similaridades (`similarityCache`), os resultados armazenados no cache são diretamente somados ao vetor `result`, evitando o recálculo. 
  4. **Cáculo da Similaridade de Cossenos**: Se a similaridade cosseno for ativada (`USE_COSINE_SIMILARITY`), a função tentará calcular a similaridade entre as combinações e as classes. É feita a inicialização de `pthread_mutex_lock(&mutex)` para garantir a exclusão mútua entre threads. Caso o resultado seja superior a um certo limiar (`THRESHOLD`), ele será usado para incrementar o `result`. Depois é feita a liberação do mutex com `pthread_mutex_unlock(&mutex)`.
  5. **Interseção de Linhas**: Caso a combinação ainda não tenha sido processada, a função realiza a interseção das linhas das features associadas a essa combinação.
  - Se a combinação contiver apenas uma *feature*, a interseção é simplesmente o conjunto de linhas dessa *feature*.
  - Caso contrário, a função `intersectionAll` é chamada para calcular a interseção entre todos os conjuntos de linhas.
  6. **Limite de Interseção**: Se o número de elementos na interseção for inferior a um certo limite (`INTERSECTION_LIMIT`), o processamento é interrompido para essa thread.
  7. **Verificação de Interseção por Classe**: Para cada classe, o código verifica se os elementos da interseção pertencem a essa classe. Caso positivo, calcula a confiança (número de elementos na interseção) e o suporte, que é a proporção de elementos da interseção em relação ao total de features.
  - Se a confiança for maior que um limite mínimo (`MIN_SUPORTE`), o suporte é somado ao vetor `result` para a classe correspondente.
  - Se a similaridade cosseno estiver ativada, o suporte também é adicionado ao `similarityCache`.
  8. **Atualização do Cache de Similaridade**: Se o uso de similaridade cosseno estiver ativo, o suporte para a combinação é armazenado no cache, para evitar recalcular no futuro.
  10. **Retorno dos Resultados**: Após processar todas as combinações, a função retorna `NULL`.

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🏁 Conclusão

<div  align="justify">

  O desenvolvimento e aprimoramento do algoritmo LAC, como apresentado neste trabalho, demonstram a eficácia e a flexibilidade dessa abordagem na resolução de problemas complexos de classificação. As melhorias implementadas, incluindo a utilização de técnicas de multithreading e otimizações específicas, permitiram um ganho significativo de desempenho, tornando o algoritmo mais eficiente em termos de tempo de execução e utilização de recursos.

  Além disso, a aplicação de um cache de similaridade e a escolha criteriosa dos métodos de cálculo, como a similaridade de cossenos, reforçam a robustez do LAC em diferentes cenários de classificação, especialmente em conjuntos de dados de alta dimensionalidade e com características complexas.

  Os resultados obtidos confirmam que o LAC, aliado às otimizações propostas, é uma ferramenta poderosa para a classificação de grandes volumes de dados, com potencial para ser aplicado em diversas áreas, desde a mineração de dados até o aprendizado de máquina. O uso de cinco threads, conforme determinado pelos testes, mostrou-se a escolha mais eficaz, equilibrando a carga de trabalho e maximizando a eficiência do algoritmo.

  Este trabalho abre portas para futuras pesquisas e melhorias adicionais, como a exploração de novas técnicas de otimização e o estudo do impacto de diferentes estratégias de paralelismo. A contínua evolução do LAC poderá contribuir para sua aplicação em contextos ainda mais desafiadores, mantendo sua relevância no campo da ciência de dados.

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🔨 Começando

<div align="justify">

  Nesta seção estão exemplificados os meios através dos quais se tornam possíveis a compilação e execução do programa apresentado.

</div>

### Pré-requisitos

<div align="justify">

  Inicialmente, algumas considerações importantes sobre como se deve preparar o ambiente para compilar e executar o programa:

</div>

> [!NOTE]
> Recomenda-se usar uma distribuição de sistema operacional Linux ou o Windows Subsystem for Linux (WSL), pois os comandos no [`makefile`][makefile] foram selecionados para funcionar em um ambiente [_shell/bash_][bash-url].

<div align="justify">

  Considerando um ambiente _shell_, garanta que os seguintes comandos já foram executados:
  - Atualize os pacotes antes da instalação dos compiladores:
  ```console
  sudo apt update
  ```
  - Instale a coleção de compiladores ___GNU/g++___ e o ___make___:
  ```console
  sudo apt install build-essential
  ```
  - Se necessário, instale o ___make___ individualmente:
  ```console
  sudo apt install make
  ```

</div>

### Instalando

<div align="justify">
  Com o ambiente preparado, os seguintes passos são para a instalação, compilação e execução do programa localmente:

  1. Clone o repositório no diretório desejado:
  ```console
  git clone https://github.com/alvarengazv/lac-algorithm.git
  cd lac-algorithm
  ```
  2. Compile o programa com o ___make___, o que gera a pasta `build`, que contém arquivos de objeto e um arquivo executável:
  ```console
  make
  ```
  3. Execute o programa da pasta `build` após a compilação:
  ```console
  make run
  ```

  4. Se necessário, apague a última compilação da pasta `build`:
  ```console
  make clean
  ```

  O programa estará pronto para ser testado. Veja a tabela abaixo com alguns comandos do makefile:

</div>

<div align="center">

  | Comando      | **Descrição**                           |
  |--------------|-----------------------------------------|
  | `make`       | Compila o programa.                     |
  | `make run`   | Executa o programa com o arquivo de entrada fornecido. |
  | `make clean` | Remove os arquivos compilados.          |

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🧪 Ambiente de Compilação e Execução

<div align="justify">

  O trabalho foi desenvolvido e testado em várias configurações de hardware. Podemos destacar algumas configurações de Sistema Operacional e Compilador, pois as demais configurações não influenciam diretamente no desempenho do programa.

</div>

<div align='center'>

![Ubuntu][ubuntu-badge]
![GCC][gcc-badge]
![Make][make-badge] 

SO | Compilador 
--- | --- 
Ubuntu 24.04.4 LTS | g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0 

</div>

> [!IMPORTANT] 
> Para que os testes tenham validade, considere as especificações
> do ambiente de compilação e execução do programa.

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 📨 Contato

<div align="center">
  <br><br>
     <i>Guilherme Alvarenga de Azevedo - Graduando - 3º Período de Engenharia de Computação @ CEFET-MG</i>
  <br><br>
  
  [![Gmail][gmail-badge]][gmail-autor1]
  [![Linkedin][linkedin-badge]][linkedin-autor1]
  [![Telegram][telegram-badge]][telegram-autor1]
  
  <br><br>
     <i>João Paulo da Cunha Faria - Graduando - 3º Período de Engenharia de Computação @ CEFET-MG</i>
  <br><br>
  
  [![Gmail][gmail-badge]][gmail-autor2]
  [![Linkedin][linkedin-badge]][linkedin-autor2]
  
  <br><br>
     <i>Maria Eduarda Teixeira Souza - Graduando - 3º Período de Engenharia de Computação @ CEFET-MG</i>
  <br><br>
  
  [![Gmail][gmail-badge]][gmail-autor3]
  [![Linkedin][linkedin-badge]][linkedin-autor3]
  [![Telegram][telegram-badge]][telegram-autor3]
  
  <br><br>
     <i>Matheus Emanuel da Silva - Graduando - 3º Período de Engenharia de Computação @ CEFET-MG</i>
  <br><br>
  
  [![Gmail][gmail-badge]][gmail-autor4]
  [![Linkedin][linkedin-badge]][linkedin-autor4]
  [![Telegram][telegram-badge]][telegram-autor4]

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

<a name="referencias">📚 Referências</a>

[^1]: A. A. Veloso, "Classificação associativa sob demanda," Ph.D. dissertação, Departamento de Ciência da Computação, Universidade Federal de Minas Gerais, Belo Horizonte, Brasil, 2009.

[^2]: R. Cattral and F. Oppacher, *Poker Hand*, UCI Machine Learning Repository, 2007. [Online]. Available: https://doi.org/10.24432/C5KW38.

[^3]: Microsoft, "pair structure," Microsoft Learn, 2024. [Online]. Available: https://learn.microsoft.com/pt-br/cpp/standard-library/pair-structure?view=msvc-170. [Accessed: Aug. 30, 2024].

[^4]: Microsoft, "unordered_map class," Microsoft Learn, [Online]. Available: https://learn.microsoft.com/pt-br/cpp/standard-library/unordered-map-class?view=msvc-170. [Accessed: Aug. 30, 2024].

[^5]: Microsoft, "vector class," Microsoft Learn, [Online]. Available: https://learn.microsoft.com/pt-br/cpp/standard-library/vector-class?view=msvc-170. [Accessed: Aug. 30, 2024].

[^6]: L. He, Z. Gao, Q. Liu, e Z. Yang, "An Improved Grid Search Algorithm for Parameters Optimization on SVM," Applied Mechanics and Materials, vol. 644-650, pp. 2216-2221, 2014. DOI: 10.4028/www.scientific.net/AMM.644-650.2216.

[^7]: "unordered_set Class | Microsoft Learn," Microsoft, [Online]. Available: https://learn.microsoft.com/pt-br/cpp/standard-library/unordered-set-class?view=msvc-170. [Accessed: 29-Aug-2024].


[vscode-badge]: https://img.shields.io/badge/Visual%20Studio%20Code-0078d7.svg?style=for-the-badge&logo=visual-studio-code&logoColor=white
[vscode-url]: https://code.visualstudio.com/docs/?dv=linux64_deb
[make-badge]: https://img.shields.io/badge/_-MAKEFILE-427819.svg?style=for-the-badge
[make-url]: https://www.gnu.org/software/make/manual/make.html
[cpp-badge]: https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white
[cpp-url]: https://en.cppreference.com/w/cpp
[github-prof]: https://github.com/mpiress
[main-ref]: src/main.cpp
[branchAMM-url]: https://github.com/alvarengazv/trabalhosAEDS1/tree/AlgoritmosMinMax
[makefile]: ./makefile
[bash-url]: https://www.hostgator.com.br/blog/o-que-e-bash/
[lenovo-badge]: https://img.shields.io/badge/lenovo%20laptop-E2231A?style=for-the-badge&logo=lenovo&logoColor=white
[ubuntu-badge]: https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white
[Ubuntu-url]: https://ubuntu.com/
[ryzen5500-badge]: https://img.shields.io/badge/AMD%20Ryzen_5_5500U-ED1C24?style=for-the-badge&logo=amd&logoColor=white
[ryzen3500-badge]: https://img.shields.io/badge/AMD%20Ryzen_5_3500X-ED1C24?style=for-the-badge&logo=amd&logoColor=white
[windows-badge]: https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white
[gcc-badge]: https://img.shields.io/badge/GCC-5C6EB8?style=for-the-badge&logo=gnu&logoColor=white

[trabalho-url]:https://drive.google.com/file/d/11tvEmPrVSYZFsJzXbcc15Ags8CLRJByU/view?usp=sharing
[grid-search]: https://github.com/alvarengazv/lac-algorithm/tree/grid-search

[linkedin-autor1]: https://www.linkedin.com/in/guilherme-alvarenga-de-azevedo-959474201/
[telegram-autor1]: https://t.me/alvarengazv
[gmail-autor1]: mailto:gui.alvarengas234@gmail.com

[linkedin-autor2]: https://www.linkedin.com/in/jo%C3%A3o-paulo-cunha-faria-219584270/
[gmail-autor2]: mailto:joaopaulofaria98@gmail.com

[linkedin-autor3]: https://www.linkedin.com/in/maria-eduarda-teixeira-souza-2a2b3a254   
[telegram-autor3]: https://t.me/dudat_18
[gmail-autor3]: mailto:dudateixeirasouza@gmail.com

[linkedin-autor4]: https://www.linkedin.com/
[telegram-autor4]: https://t.me/
[gmail-autor4]: mailto:memanuel643@gmail.com

[linkedin-badge]: https://img.shields.io/badge/-LinkedIn-0077B5?style=for-the-badge&logo=Linkedin&logoColor=white
[telegram-badge]: https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white
[gmail-badge]: https://img.shields.io/badge/-Gmail-D14836?style=for-the-badge&logo=Gmail&logoColor=white

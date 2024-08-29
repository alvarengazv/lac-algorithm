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
    <li><a href="#-Introdução">🔍 Introdução</a></li>
    <li>
      <a href="#-Fundamentação-Teórica">💡 Fundamentação Teórica</a>
    </li>
    <li>
      <a href="#-Objetivos">🎯 Objetivos</a>
      <ul>
        <li><a href='#Objetivo-Geral'>Objetivo Geral</a></li>
        <li><a href='#Objetivos-Específicos'>Objetivos Específicos</a></li>
      </ul>
    </li>
    <li>
      <a href="#-Metodologia">🔬 Metodologia</a>
      <ul>
        <li><a href='#Arquivos'>Arquivos</a></li>
        <li><a href='#Bibliotecas'>Bibliotecas</a></li>
        <li><a href='#Funções'>Funções</a></li>
        <li><a href='#Detalhes-de-Implementação'>Detalhes de Implementação</a></li>
      </ul>
    </li>
    <li>
      <a href="#-Testes-e-Análises-dos-Resultados">📊 Testes e Análises dos Resultados</a>
    </li>
    <li><a href="#-Conclusão">🏁 Conclusão</a></li>
    <li>
      <a href="#-Começando">🔨 Começando</a>
      <ul>
        <li><a href="#Pré-requisitos">Pré-requisitos</a></li>
        <li><a href="#Instalando">Instalando</a></li>
      </ul>
    </li>
    <li><a href="#-Ambiente-de-Compilação-e-Execução">🧪 Ambiente de Compilação e Execução</a></li>
    <li><a href="#-Referências">📚 Referências</a></li>
    <li><a href="#-Contato">📨 Contato</a></li>
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

  <!-- Falar sobre a base de treino -->
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
  <img src='./images/mapeamentoTeste.png' alt='Exemplo de Mapeamento Fase de Teste' width='300px'>
  <p>Exemplo de Mapeamento Fase de Teste</p>
</div>

Realizado o mapeamento, acessamos na tabela invertida de features, criada na fase de treino, o array de inteiros que representa cada linha onde determinada feature aparece. Em seguida, realizamos um procedimento que realiza interseções entre os arrays provenientes de cada tupla, ou seja, comparamos quais e quantas linhas determinadas tuplas têm em comum, de forma a fazer a análise combinatória de todas as interseções possíveis. Durante esse procedimento, ao realizar as interseções entre os arrays referentes a cada tupla, calculamos o suporte e a confiança, variáveis responsáveis por classificar cada mão. Ao calcular as interseções para cada combinação de features, iteramos sobre a matriz invertida de classes, criada durante o treinamento, e, para cada classe, fazemos a interseção entre o array de inteiros que representa as linhas onde a classe aparece e o array resultante da interseção da análise combinatória das features. O nosso valor **confiança** recebe o tamanho do vetor resultante dessa interseção.

Por fim, na iteração para cada classe, para calcular o suporte, dividimos o valor da **confiança** pela quantidade de features presentes na base de dados construída durante o treinamento. Dessa forma, ao calcular o valor de suporte, este é somado em um array **resultado**, que é responsável por guardar a soma do suporte para cada classe. Após realizar todas as análises combinatórias possíveis, a classe atribuída para a mão será aquela que tiver o maior valor de suporte no array **resultado**.

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

## 🔬 Metodologia

### Arquivos

<div align="justify">

  ### Bibliotecas

  ### Funções

  ### Detalhes de Implementação

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 📂 Estrutura de Diretórios

O projeto é organizado da seguinte forma:

```.
|
├── datasets
│   │   ├── poker-hand-testing.data
│   │   ├── poker-hand-training.data
│   │   └── output.txt
├── images
│   │   ├── mapeamentoTeste.png
│   │   ├── tabelaClsses.png
│   │   └── tabelaFeatures.png
├── src
│   │   ├── main.cpp
│   │   ├── lac.cpp
│   │   └── lac.hpp
├── .gitignore
├── make.sh
├── makefile
└── README.md
```

Esta estrutura de diretórios facilita a organização do projeto e a localização dos arquivos necessários para compilar o código-fonte, executar o programa e visualizar os resultados.
<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 📊 Testes e Análises dos Resultados

<div  align="justify">

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🏁 Conclusão

<div  align="justify">

</div>

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🔨 Começando

Nesta seção estão exemplificados os meios através dos quais se tornam possíveis a compilação e execução do programa apresentado.

### Pré-requisitos

Inicialmente, algumas considerações importantes sobre como se deve preparar o ambiente para compilar e executar o programa:

> [!NOTE]
> Recomenda-se usar uma distribuição de sistema operacional Linux ou o Windows Subsystem for Linux (WSL), pois os comandos no [`makefile`][makefile] foram selecionados para funcionar em um ambiente [_shell/bash_][bash-url].

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

### Instalando

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

O programa estará pronto para ser testado.

<p align="right">(<a href="#readme-topo">voltar ao topo</a>)</p>

## 🧪 Ambiente de Compilação e Execução

o trabalho foi desenvolvido e testado em várias configurações de hardware. Podemos destacar algumas configurações de Sistema Operacional e Compilador, pois as demais configurações não influenciam diretamente no desempenho do programa.

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

## ⛏ Makefile

O Makefile é um utilitário que automatiza o processo de compilação e execução de programas. Aqui estão os principais comandos do Makefile para este projeto:

| Comando      | **Descrição**                           |
|--------------|-----------------------------------------|
| `make`       | Compila o programa.                     |
| `make run`   | Executa o programa com o arquivo de entrada fornecido. |
| `make clean` | Remove os arquivos compilados.          |

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

📚 Referências

[^1]: A. A. Veloso, "Classificação associativa sob demanda," Ph.D. dissertação, Departamento de Ciência da Computação, Universidade Federal de Minas Gerais, Belo Horizonte, Brasil, 2009.

[^2]: R. Cattral and F. Oppacher, *Poker Hand*, UCI Machine Learning Repository, 2007. [Online]. Available: https://doi.org/10.24432/C5KW38.

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

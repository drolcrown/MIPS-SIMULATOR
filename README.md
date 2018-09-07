# MIPS-SIMULATOR


ORGANIZAÇÃO E ARQUITETURA DE COMPUTADORES 

# LoadAndSaveMIPS

**PROBLEMA**

Nesse primeiro trabalho inicia-se a implementação de um simulador MIPS em C. Os dois primeiros problemas a serem abordados são:

1. Como simular a carga de elementos na memória;

2. Funções de acesso a memória, leitura e escrita;

**FUNÇÕES**

Para tentar solucionar os problemas, a memória foi simulada com um vetor de 4096 palavras de 32bits, em que a primeira metade da memória é reservada para os códigos das instruções do programa e a segunda metade para os dados.

Para carregar os elementos e realizar as instruções de acesso e leitura na memória, implementou-se as funções listadas abaixo:

1. dump_mem

A função recebe de parâmetros dois valores inteiro de 32 bits sem sinal representando a posição de inicio e fim do armazenamento na memória e imprime na tela os espaços de memoria indicados e seus respectivos valores.


2. printResult

A função recebe de parâmetros um valor inteiro de 32 bits com sinal representando o dado a ser impresso na tela, um valor inteiro que representa o numero de caracteres que devem ser mostrados e um valor booleano que representa se um valor hexadecimal será impresso.

A função imprime na tela um texto pré-definido de acordo com os parâmetros passados.

3. isMultiple

A função recebe de parâmetros um valor inteiro que representa o dado que será avaliado e outro valor inteiro que representa o divisor que será usado.

A função retorna um valor booleano que representa se o dado enviado é multiplo do divisor.


4. moveData

A função recebe de parâmetros um valor inteiro de 32 bits com sinal representando o dado a ser deslocado, um valor inteiro que representa o quantos bytes serão deslocados pelo dado e um valor booleano que representa a direção do deslocamento(true = direita, false = esquerda).

A função retorna o dado deslocado.


5. masks 

A função recebe de parâmetros um valor inteiro representando o dado a ser mascarado, um valor inteiro que representa o tipo de dado, um inteiro de 8 ou 16 bits e um valor booleano que representa 
se a mascara sera aplicada em um dado ou na memoria.

A função retorna o valor já mascarado.


6. exception

A função recebe de parâmetros um valor inteiro de 32 bits sem sinal representando um endereço na memoria, um valor inteiro de 16 bits com sinal que representa uma constante, um vetor de caracteres    que representa a exceção que será lançada na tela e um valor inteiro que representa o numero de bytes que são usados pra definir o tipo de verificação que será testada.

A função retorna um dado verificado e imprime uma erro na tela, caso o dado não passe no teste de verificação.


7. startSave

A função não recebe parâmetros e salva na memória os dados carregados nela.


8. startLoad

A função não recebe parâmetros e carrega os dados salvos em memoria.


	As funções de load (lw, lh e lb) apresentam os mesmos parâmetros, um endereço de memória (32 bits sem sinal) e uma constante de deslocamento (16 bits com sinal).

1. lw

Para a função de load word os únicos endereços acessáveis são os múltiplos de 4, se a soma dos parâmetros não cumprir esse requisito ou o de ser menor que o tamanho da memória, uma mensagem de erro é impressa e o programa é finalizado. Caso contrário, a palavra na devida posição de memória é retornada.

2. lh

Para a função de load half-word os únicos endereços acessáveis são os múltiplos de 2, se a soma dos parâmetros não cumprir esse requisito ou o de ser menor que o tamanho da memória, uma mensagem de erro é impressa e o programa é finalizado. Caso contrário, a meia palavra (16 bits) na devida posição de memória é retornada na parte menos significante de uma palavra de 32 bits, mantendo seu valor – extensão do bit de sinal.

3. lhu

Para a função de load half-word unsigned é semelhante a função load half-word, porém o valor de retornado por ela é um inteiro de 32 bits sem sinal.

4. lb

Para a função de load byte qualquer endereço de memória é acessáveis. Logo, apenas se a soma dos parâmetros não for menor que o tamanho da memória, uma mensagem de erro é impressa e o programa é finalizado. Nos demais casos, o byte na devida posição de memória é retornada na parte menos significante de uma palavra de 32 bits, mantendo seu valor – extensão do bit de sinal.

5. lbu

Para a função de load byte unsigned é semelhante a função load byte, porém o valor de retornado por ela é um inteiro de 32 bits sem sinal.


As funções de store (sw, sh e sb) apresentam parâmetros parecidos, um endereço de memória (32 bits sem sinal), uma constante de deslocamento (16 bits com sinal) e o dado a ser escrito na memória (sw – 32bits, sh – 16 bits e sb – 8 bits).


1. sw

O alinhamento do endereço é testado – ou seja, se o endereço é múltiplo de 4 – e caso o endereço não seja alinha, ou seja maior que o tamanho da memória, o programa é finalizado após imprimir uma mensagem de erro. Caso contrário, a posição de memória fornecida recebe o dado.

1. sh

O alinhamento do endereço é testado – ou seja, se o endereço é múltiplo de 2 – e caso o endereço não seja alinha, ou seja maior que o tamanho da memória, o programa é finalizado após imprimir uma mensagem de erro. Caso contrário, a posição de memória fornecida recebe o dado.

2. sb

No caso do endereço ser maior que o tamanho da memória o programa é finalizado após impressão de uma mensagem de erro. Nos demais casos, a posição de memória fornecida recebe o dado.

	O acesso ao vetor de memória nas funções de half-word e byte (lh, sh, lb e sb) é feito com um cast de ponteiro para o formato da informação que está sendo acessado, permitindo assim acessar as posições não alinhadas com as palavras de 32bits.

**TESTES**

Para testar o correto funcionamento das funções, elas foram chamadas  com parâmetros diferentes e seus resultados foram comparados com os esperados. Chamadas com parâmetros dos casos de erro foram executados um a um, para garantir o correto funcionamento.

Casos de erro previstos e com tratamento:


1. Intervalo de impressão da memória invertido, endereço final menor que o inicial;

2. Acesso a endereços não alinhados;

3. Acesso a endereços maiores que o tamanho da memória.

**RESULTADOS**

Todas as funções apresentaram o comportamento esperado. E todos os casos de erros previstos estão com o tratamento funcionando corretamente.



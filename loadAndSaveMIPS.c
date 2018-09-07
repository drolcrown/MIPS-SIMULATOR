/* -------  Rafael Oliveira de Souza -----------
    ------- Simulador do MIPS I ------- 
    ------- Sistema: Linux - Ubuntu 16.04 LTS ------- 
    -------  IDE: VisualCode --------    
    ------- Compilador: GCC - Compilado Via terminal  ------- 


   Para compilar e executar o arquivo:
    -> Acesse o diretorio do arquivo .c pelo terminal
    -> Compile usando o comando: gcc loadAndSaveMIPS.c -o mips
    -> Execute usando o comando: ./mips
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>

#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

void printResult(int32_t valor, int numCaracter, bool printHexa){
    if(printHexa){
        switch(numCaracter){
        case 2:
            printf("\33[0;33mLoad Byte em Hexa: \33[0m%02X", (uint8_t)valor);
            break;
        case 4:
            printf("\33[0;33mLoad Byte em Hexa: \33[0m%04X", (uint16_t)valor);
            break;
        case 8:
            printf("\33[0;33mLoad Byte em Hexa: \33[0m%08X", (uint32_t)valor);
            break;
        }
    }
    printf("\33[0;33m -> Load Byte em Decimal: \33[0m%d\n", valor);
}

/*Verificar se um tamanho é multiplo de um divisor*/
bool isMultiple(int valor, int divisor){
    if(valor % divisor == 0){
        return true;
    }
    return false;
}

/*Define um conjunto de mascaras pra cada tipo de dado*/
unsigned int moveData(int32_t dado, bool direcao, int deslocamento){
    /*Direcao == true - deslocamento a direita*/
    if(direcao){
        /*Verifica-se se o deslocamento é pela direita e maior que 16 bytes,
            pois esse tipo de deslocamento preenche o campo com F
         */
        return (deslocamento > 16 ? ((dado >> deslocamento) & 0X000000FF) : (dado >> deslocamento) ) ;
    }
    return (dado << deslocamento);
} 

/*Define um conjunto de mascaras pra cada tipo de dado*/
unsigned int masks(int valor, int tipo, bool memoriaOuDado){
    /*memoriaOuDado é true se a mascara for aplicada em um dado
        e false se a mascara for aplicada em uma memoria
    */
    unsigned int int16[4] = {0x0000ffff, 0x0000ffff, 0xffff0000, 0xffff0000};
    unsigned int int8Mem[4] = {0xffffff00, 0xffff00ff, 0xff00ffff, 0x00ffffff};
    unsigned int int8Dado[4] = {0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};

    switch (tipo){
    case 16:
        valor = int16[valor];
        break;
    case 8:
        valor = (memoriaOuDado ? int8Dado[valor] : int8Mem[valor]);
        break;
    }
    return valor;
}

bool exception(uint32_t address, int16_t kte, char *function, int tipoVerificacao){
    bool valor;
    /*verifica se a instrução foi executada corretamente
        é necessario saber se a instrução verifica uma meia palavra, pois nesse caso 
        a kte tambem é verificada para ser um multiplo de 2
    */
    if(address + kte >= MEM_SIZE){
        printf("\33[0;33mAddresses that are larger than the memory size\n\33[0m");
        return false;
    }
   if(tipoVerificacao == 32){
       valor = (!isMultiple((address + kte), 4));
    }
    if(tipoVerificacao == 16){
        valor = (!isMultiple(address, 4) || !isMultiple(kte, 2));
    }
    if(tipoVerificacao == 8){
        valor = (!isMultiple(address, 4));
    }
    if(valor){
        printf("\33[0;33mInvalid Address in %s\n\33[0m", function);
    }
    return valor;
}

/*Load Word - Carregar uma palavra inteira da memoria*/
int32_t lw(uint32_t address, int16_t kte){
    int32_t valor = 0;
    /*Para não dar erro na soma do endereço e da constante é necessario fazer um cast em kte*/
    kte = (uint32_t)kte;

    /*Necessario checar se a soma do endereço e a constante são multiplos de 4*/
    if(exception(address, kte, "LW", 32)){
        return 0;
    }else{ 
    /*Já que o endereço é valido, só é necessario transforma-lo pra um
        endereço compativel em C dividindo esse endereço por 4*/
        address = (address + kte) / 4;    
        valor = mem[address];
    }
    return (int32_t)valor;
}

/*Load Half-Word-Unsigned - Carregar uma meia palavra da memoria sem sinal*/
uint32_t lhu(uint32_t address, int16_t kte){
    int32_t valor = 0, resto;
    kte = (uint32_t)kte;
    /*Necessario checar se a soma do endereço é multiplo de 4 e kte é multiplo de 2*/
    if(exception(address, kte, "LH", 16)){
       return 0;
    } else{
        resto = (address + kte) % 4;
        address = (address + kte) / 4;
        if(resto == 0 || resto == 1){
             /*Zera na memoria os bytes 2 e 3 da palavra 4 bytes - Mascara 0x0000ffff */
            valor = mem[address] & masks(resto, 16, true);
        }
        if(resto == 2 || resto == 3){
            /*Desloca o dado da memoria 16 bytes a direita */
            valor = moveData(mem[address], true, 16);
        }
    }

    return valor;
}

/*Load Half-Word - Carregar uma meia palavra da memoria*/
int32_t lh(uint32_t address, int16_t kte){
    return (int16_t)lhu(address, kte);
}

/*Load Byte Unsigned- Carregar um byte de uma palavra da memoria e retorna um valor sem sinal*/
int32_t lbu(uint32_t address, int16_t kte){
    int32_t resto, valor = 0;
    kte = (int32_t)kte;  
    if(exception(address, kte, "LB", 8)){ 
        return 0;
    }else{
        resto = (address + kte) % 4;
        address = (address + kte) / 4;
        /*Efetua a mascara em um valor da memoria*/
        valor = mem[address] & masks(resto, 8, true);
        /*Desloca o dado para a direita e concatena o endereço e o novo dado*/
        /*Como o dado representa 8 bytes o restó é multiplicado por 8 na hora do deslocamento*/
        valor =  moveData(valor, true, (resto * 8));
    }
    return valor;
}

/*Load Byte - Carregar um byte de uma palavra da memoria e retorna um valor com sinal*/
int32_t lb(uint32_t address, int16_t kte){
    return (int8_t)lbu(address, kte);
}

/*Save Word - Salvar palavra inteira na memoria*/
void sw(uint32_t address, int16_t kte, int32_t dado){
    /*Para não dar erro na soma do endereço e da constante é necessario fazer um cast em kte*/
    kte = (uint32_t)kte;

    /*Necessario checar se a soma do endereço e a constante são multiplos de 4*/
    if(exception(address, kte, "SW", 32)){
        return;
    }else{
        mem[((address+kte) / 4)] = dado;
    }
}

/*Save HalfWord - Salvar meia palavra na memoria*/
void sh(uint32_t address, int16_t kte, int16_t dado){
    uint32_t resto, dadoMascarado;
    /*Para não dar erro na soma do endereço e da constante é necessario fazer um cast em kte*/
    kte = (uint32_t)kte;
    dado = (int32_t)dado;

    /*Necessario checar se a soma do endereço e a constante são multiplos de 4*/
    if(exception(address, kte, "SH", 16)){ 
        return;
    }else{
        resto = (address + kte) % 4;
        address = (address + kte) / 4;
        // printf("resto = %d, adress = %d\n", resto , address);
        if(resto == 0 || resto == 1){
            /*Zera na memoria os bytes 0 e 1 da palavra 4 bytes - Mascara 0xffff0000*/
            mem[address] = mem[address] & masks((resto + 2), 16, false);
            /*Zera os bytes 2 e 3 do dado recebido - Mascara 0x0000ffff*/
            dadoMascarado = dado & masks(resto, 16, true);
        }
        if(resto == 2 || resto == 3){
            /*Zera na memoria os bytes 2 e 3 da palavra 4 bytes - Mascara 0x0000ffff */
            mem[address] = mem[address] & masks((resto - 2) , 16, false);
            /*Move o dado recebido 16 bytes para a esquerda -> Ex: 00008CDF =  8CDF0000 */
            dadoMascarado = moveData(dado, false, 16);
        }
        /*Concatena o dadoMascarado com o novo adress*/
        mem[address] = mem[address] | dadoMascarado;
    }
}

/*Save Byte - Salvar um byte na memoria*/
void sb(uint32_t address, int16_t kte, int8_t dado){
    int32_t resto;
    dado = (int32_t)dado;
    kte = (int32_t)kte;  
    if(exception(address, kte, "SB", 8)){ 
        return;
    }else{
        resto = (address + kte) % 4;
        address = (address + kte) / 4;
        /*Limpa o espaço na memoria onde o dado sera salvo*/
        mem[address] = mem[address] & masks(resto, 8, false);
        /*Desloca o dado pra posicao desejada e concatena o endereço e o novo dado*/
        /*Como o dado representa 8 bytes o restó é multiplicado por 8 na hora do deslocamento*/
        mem[address] = mem[address] | moveData(dado, false, resto * 8);
    }
}

void start_save(){
   printf("\nArmazenar dados na memória:\n\n");  
   sb(0, 0, 0x04); sb(0, 1, 0x03); sb(0, 2, 0x02); sb(0, 3, 0x01);
   sb(4, 0, 0xFF);sb(4, 1, 0xFE); sb(4, 2, 0xFD); sb(4, 3, 0xFC);
   sh(8, 0, 0xFFF0);
   sh(8, 2, 0x8C);
   sw(12, 0, 0xFF);
   sw(16, 0, 0xFFFF);
   sw(20, 0, 0xFFFFFFFF);
   sw(24, 0, 0x80000000);
}

void start_load(){
    //lb deve ser impresso apenas com dois dígitos em hexa, e lh() apenas com 4.
    printf("Teste da função Load Byte - byte por byte: \n\n"); /*imprimir em hexa e decimal*/
    printResult(lb(0,0), 2, true);
    printResult(lb(0, 1), 2, true);
    printResult(lb(0, 2), 2, true);
    printResult(lb(0, 3), 2, true);
    printResult(lb(4,0), 2, true);
    printResult(lb(4, 1), 2, true);
    printResult(lb(4, 2), 2, true);
    printResult(lb(4, 3), 2, true);
    printf("\nTeste da função Load Byte Unsigned - byte por byte: \n\n"); /*imprimir em decimal*/
    printResult(lbu(4, 0), 2, false);
    printResult(lbu(4, 1), 2, false);
    printResult(lbu(4, 2), 2, false);
    printResult(lbu(4, 3), 2, false);
    printf("\nTeste da função Load Half Word - Meia Palavra: \n\n"); /*imprimir em hexa e decimal*/
    printResult(lh(8, 0), 4, true);
    printResult(lh(8, 2), 4, true);
    printf("\nTeste da função Load Half Word Unsigned - Meia Palavra: \n\n"); /*imprimir em decimal*/
    printResult(lhu(8, 0), 4, false);
    printResult(lhu(8, 2), 4, false);
    printf("\nTeste da função Load Word - Palavra Inteira: \n\n"); /*imprimir em hexa e decimal*/
    printResult(lw(12, 0), 8, true);
    printResult(lw(16, 0), 8, true);
    printResult(lw(20, 0), 8, true);
    /*Testando exceptions - Todos os testes abaixo devem gerar uma exceção*/
    printf("\nTestando valores que geram erro de instrução na memoria:\n\n");
    sb(4099, 0, 0xFC);
    sb(4092, 6, 0xFC);
    sb(3, 3, 0xFC);
    sh(2, 2, 0x8C);
    sw(25, 0, 0xFF);
    lw(3, 0);
    lh(5, 2);
    lhu(1, 0);
    lb(9, 2);
    lbu(3, 1);
}

/* imprime o conteúdo da memória no formato hexa, palavra por palavra.*/
void dump_mem(uint32_t add, uint32_t size){
    int i;
    /* Os endereços de memoria em mips são definidos em valores multiplos de 4,
       por isso é necessario verificar se size é multiplo de 4
        se sim, size = size / 4 
        se não, size = 0
    */ 
    size = (isMultiple(size, 4) == true ? (size / 4) : 0);
    for (i = add; i < size; i++){
        printf("\33[0;33mMem[%d]\33[0m = %08X\n", i, mem[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]){
 uint32_t u32;
 uint16_t u16;
 uint8_t u8;

 printf("sizeof(u32) = %ld Bytes\n", sizeof(u32));
 printf("sizeof(u16) = %ld Bytes\n", sizeof(u16));
 printf("sizeof(u8) = %ld Bytes\n", sizeof(u8));

 start_save();
 dump_mem(0, 28);
 start_load();

 return 0;
}
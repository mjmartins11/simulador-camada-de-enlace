#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2
#define TAMANHO_CRC 32
#define TAMANHO_GERADOR TAMANHO_CRC + 1

/*!< Dúvidas
* Qual o formato da mensagem de entrada? É algo como "Hello World" ou "11001100110"? 
* A mensagem tem tamanho máximo? É necessário dividir o quadro se os dados possuem mais de 1500 bytes?
* Qual quadro deve ser construido: Ethernet (http://deptal.estgp.pt:9090/cisco/ccna1/course/module5/5.1.2.3/5.1.2.3.html), IEEE 802.1Q (https://www.cisco.com/c/pt_br/support/docs/lan-switching/8021q/17056-741-4.html), PPP, etc...?
* O quadro é um vetor de int que contém binário? 
* O quadro, por exemplo, da função CamadaEnlanceDadosTransmissoraControleDeErro é um quadro só com a mensagem ou possui o endereço de destino, origem, preambulo, etc...?
* Podemos usar bitset? Então, por exemplo, a mensagem seria transformada em um vetor de bitset<8> onde cada posição é um caractere (https://stackoverflow.com/a/10184269/13274909)?
* No caso de ser Ethernet, ao escolher um controle de erro (paridade par, impar ou CRC), essa informação irá no quadro na última posição sempre? Ou seja, onde supostamente deveria ir o CRC também vai a paridade se for a escolha?
* Para os algoritmos de paridade, vamos utilizar paridade unidimensional ou bidimensional?
* O quadro é formado pela CamadaDeAplicacaoTransmissora e enviado feito para CamadaEnlaceDadosTransmissora? Se sim, qual a função da camada de enlace? Apenas detecção de erros? Ou é necessário fazer controle de fluxo, etc...?
* O endereço MAC é adicionado na função CamadaDeAplicacaoTransmissora quando o quadro é montado? Qual o endereço MAC que deve ser utilizado para origem e destino?
* Devemos utilizar socket? Está descrito em "o que deve ser entrega".
* A probabilidade de erro possui algum embasamento, ou pode ser um valor arbitrario?
* Na função MeioDeComunicação, o que seria o Fluxo Bruto de Bits?
*/

void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(int quadro[]);

void MeioDeComunicacao(int fluxoBrutoDeBits[]);

void CamadaEnlaceDadosReceptora(int quadro[]);
void CamadaDeAplicacaoReceptora(int quadro[]);
void AplicacaoReceptora(string mensagem);

//
void CamadaEnlanceDadosTransmissoraControleDeErro(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]);

int tamanhoMensagemQuadro(vector<int> quadro);
vector<int> mensagemQuadro(vector<int> quadro);
vector<int> mensagemErroQuadro(vector<int> quadro);
vector<int> erroQuadro(vector<int> quadro);

/*!< Estrutura do Quadro
* É um vetor de inteiros onde cada posição representa um bit;
* Os 8 primeiros bits (inteiros) são reservados para o tamanho do quadro. Esse tamanho é a quantidade de caracteres que a mensagem possui (ou seja, a quantidade de bits será dada por esse valor * 8);
* Os [tamanho do quadro * 8] bits seguintes representarão a mensagem do quadro. Note que para cada caractere, serão utilizados 8 bits. Além disso, o bit mais significativo estará na posição a frente;
* Os últimos x bits serão reservados para o erro.
*/

//     0123456
//72 : 0100100       
//     ^

void main(void) {
    AplicacaoTransmissora();
} 

void AplicacaoTransmissora(void) {
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    cin >> mensagem;
    CamadaDeAplicacaoTransmissora(mensagem);
} 
   
void CamadaDeAplicacaoTransmissora(string mensagem) {
    vector<int> quadro;

    //Adicionar tamanho da mensagem no inicio

    for (int k = 0; k < mensagem.length(); k++) {
        /*!< Convertendo para ASCII */
        int ascii = int(mensagem[k]);

        /*!< Convertendo ASCII para binário */
        int binarioInvertido[8];
        int i = 0;
        int n = ascii;
        while (n > 0) {
            binarioInvertido[i] = n % 2; //O bit MAIS significativo estará na posição 0
            n = n / 2;
            i++;
        }
        for(int j = 7; j >= 0; j--) {
            quadro.push_back(binarioInvertido[j]); //O bit MAIS significativo estará na posição 0
        }
    }

    CamadaEnlaceDadosTransmissora(quadro);
}

void CamadaEnlaceDadosTransmissora(vector<int> quadro) {
    int fluxoBrutoDeBits[];

    CamadaEnlanceDadosTransmissoraControleDeErro(quadro);

    MeioDeComunicacao(fluxoBrutoDeBits);
}

void MeioDeComunicacao(int fluxoBrutoDeBits[]) {
    // OBS: trabalhar com BITS e nao com BYTES!
    int erro, porcentagemDeErros;
    int fluxoBrutoDeBitsPontoA[0], fluxoBrutoDeBitsPontoB[0];

    porcentagemDeErros = 0; // 10%, 20%, 30%, ... 100%
    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;

    while(fluxoBrutoDeBitsPontoB.lenght != fluxoBrutoDeBitsPontoA){
        if((rand() % == ...)) // fazer a probabilidade do erro
            fluxoBrutoDeBitsPontoB += fluxoBrutoDeBitsPontoA;
        else { // ERRO! INVERTER (usa condicao ternaria)
            fluxoBrutoDeBitsPontoB == 0) ?
            fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB++;
            fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB--;
        }

    }

    int quadro[];
    CamadaEnlaceDadosReceptora(quadro);
}

void CamadaEnlaceDadosReceptora(vector<int> quadro) {

    CamadaDeAplicacaoReceptora(quadro);
}

void CamadaDeAplicacaoReceptora(vector<int> quadro) {
    string mensagem = quadro[]; // estava trabalhando com bits
    // chama proxima camada

    AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(string mensagem) {
    cout << "A mensagem recebida foi:" << mensagem << endl;
} 

//*********************************** CALCULO DE ERRO ***********************************

void CamadaEnlanceDadosTransmissoraControleDeErro(vector<int> quadro) {
    int tipoDeControleDeErro = 0; // alterar de acordo com o teste
    
    switch(tipoDeControleDeErro) {
        case BIT_PARIDADE_PAR: 
            CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
            break;
        case BIT_PARIDADE_IMPAR: 
            CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(quadro);
            break;
        case CRC:
            CamadaEnlaceDadosTransmissoraControleDeErroErroCRC(quadro);
            break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadro) {
    int tamMensagem, bitParidade, qtdBit1;
    vector<int>mensagem;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);
    bitParidade = erroQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == BIT_PARIDADE_IMPAR)
            qtdBit1++;
    }

    if(((qtdBit1%2 == 0) && (bitParidade[0] == 0)) || ((qtdBit1%2 != 0) && (bitParidade[0] == 1))){
        cout << "paridade par nao apresentou erros" << endl;
    } else{
        cout << "paridade par apresentou erros";
    }
    
    return;
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(vector<int> quadro) {
    int tamMensagem, bitParidade, qtdBit1;
    vector<int>mensagem;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);
    bitParidade = erroQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == BIT_PARIDADE_IMPAR)
            qtdBit1++;
    }

    if(((qtdBit1%2 != 0) && (bitParidade[0] == 0)) || ((qtdBit1%2 == 0) && (bitParidade[0] == 1))){
        cout << "paridade impar nao apresentou erros" << endl;
    } else {
        // o codigo
        cout << "paridade impar apresentou erros";
    }
    
    // o codigo
}

void CamadaEnlaceDadosTransmissoraControleDeErroErroCRC(vector<int> quadro) {
    // usar polinomio CRC-32(IEEE 802)
    int tamanho_mensagem;
    vector<int> gerador = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    vector<int> mensagem = mensagemErroQuadro(quadro);
    vector<int> crc = erroQuadro(mensagem); 

    tamanho_mensagem = tamanhoMensagemQuadro(quadro) + TAMANHO_GERADOR;

    for (int i = 0; i < TAMANHO_GERADOR; i++)
        mensagem.push_back(0);

    vector<int> dividendo;
    vector<int> resto;

    for (int i = 0; i < TAMANHO_GERADOR; i++)
        resto.push_back(mensagem[i]);

    for (int i = TAMANHO_GERADOR; i < tamanho_mensagem; i) {
        if (resto[0] != 0)
            for (int j = 0; j < TAMANHO_GERADOR; j++)
                if (resto[j] == gerador[j])
                    resto[j] = 0;
                else 
                    resto[j] = 1;
        resto[TAMANHO_CRC] = mensagem[i];
    }
}

vector<int> XOR(vector<int> resto, vector<int> gerador) {
    for (int j = 0; j < TAMANHO_GERADOR; j++)
        if (resto[j] == gerador[j])
            resto[j] = 0;
        else 
            resto[j] = 1;
    return resto;
}

//*********************************** MANIPULAÇÃO DO QUADRO ***********************************


//Retorna a quantidade de bits
int tamanhoMensagemQuadro(vector<int> quadro) {
    //O tamanho do quadro será armazenado com o bit mais significativo na primeira posição
    return (((quadro[0] * pow(2, 7)) 
            + (quadro[1] * pow(2, 6)) 
            + (quadro[2] * pow(2, 5)) 
            + (quadro[3] * pow(2, 4)) 
            + (quadro[4] * pow(2, 3)) 
            + (quadro[5] * pow(2, 2)) 
            + (quadro[6] * pow(2, 1)) 
            + (quadro[7] * pow(2, 0))) * 8); 
}

vector<int> mensagemQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamanhoMensagemEmBits = 6;
    vector<int> mensagemQuadro(quadro.begin()+tamanhoMensagemEmBits, quadro.begin() + tamanho_mensagem);

    return mensagemQuadro;
}

vector<int> mensagemErroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamanhoMensagemEmBits = 6;
    vector<int> mensagemErroQuadro(quadro.begin()+tamanhoMensagemEmBits, quadro.end());

    return mensagemErroQuadro;
}

vector<int> erroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamanhoMensagemEmBits = 6;
    vector<int> erroQuadro(quadro.begin()+tamanhoMensagemEmBits+tamanho_mensagem, quadro.end());
    
    return erroQuadro;
}
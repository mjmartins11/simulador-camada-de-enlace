#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2

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

void main(void) {
    AplicacaoTransmissora();
} 

void AplicacaoTransmissora(void) {
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    cin >> mensagem;
    
    CamadaDeAplicacaoTransmissora(mensagem); // em um exemplo mais realistico, aqui seria dado um SEND do SO
} 

void CamadaDeAplicacaoTransmissora(string mensagem) {
    int quadro[] = mensagem;
    
    CamadaEnlaceDadosTransmissora(quadro);
}

void CamadaEnlaceDadosTransmissora(int quadro[]) {
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

void CamadaEnlaceDadosReceptora(int quadro[]) {

    CamadaDeAplicacaoReceptora(quadro);
}

void CamadaDeAplicacaoReceptora(int quadro[]) {
    string mensagem = quadro[]; // estava trabalhando com bits
    // chama proxima camada

    AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(string mensagem) {
    cout << "A mensagem recebida foi:" << mensagem << endl;
} 

// Funções auxiliares

void CamadaEnlanceDadosTransmissoraControleDeErro(int quadro[]) {
    int tipoDeControleDeErro = 0; // alterar de acordo com o teste
    
    switch(tipoDeControleDeErro) {
        case BIT_PARIDADE_PAR: 
            // codigo
            break;
        case BIT_PARIDADE_IMPAR: 
            // codigo
            break;
        case CRC:
            // codigo
            break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]) {
    // implementacao do algoritmo
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]) {
    // implementacao do algoritmo
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]) {
    // implementacao do algoritmo
    // usar polinomio CRC-32(IEEE 802)
}
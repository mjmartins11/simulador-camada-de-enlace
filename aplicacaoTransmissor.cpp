#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]);
void CamadaEnlanceDadosTransmissoraControleDeErro(int quadro[]);
void CamadaEnlaceDadosTrasnmissora(int quadro[]);
void MeioDeComunicacao(int fluxoBrutoDeBits[]);
void AplicacaoReceptora(string mensagem);
void CamadaDeAplicacaoReceptora(int quadro[]);
void CamadaDeAplicacaoTransmissora(string mensagem);
void AplicacaoTransmissora(void);

void main(void) {
    AplicacaoTransmissora();
} 

void AplicacaoTransmissora(void) {
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    cin >> mensagem;
    // chama a proxima camada
    CamadaDeAplicacaoTransmissora(mensagem); // em um exemplo mais realistico, aqui seria dado um SEND do SO
} 

void CamadaDeAplicacaoTransmissora(string mensagem) {
    int quadro[] = mensagem; // trabalhar com bits!!
    // chama a proxima camada

    CamadaEnlaceDadosTransmissora(quadro);
}

void CamadaEnlaceDadosTransmissora(int quadro[]) {
    CamadaEnlanceDadosTransmissoraControleDeErro(quadro);
    // chama proxima camada
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
}

void CamadaDeAplicacaoReceptora(int quadro[]) {
    // string mensagem = quadro[]; // estava trabalhando com bits
    // chama proxima camada
}

void AplicacaoReceptora(string mensagem) {
    cout << "A mensagem recebida foi:" << mensagem << endl;
} 


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
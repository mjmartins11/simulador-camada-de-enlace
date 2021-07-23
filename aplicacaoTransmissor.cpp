#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

#define BIT_PARIDADE_PAR 1
#define BIT_PARIDADE_IMPAR 2
#define CRC 3

/*!< Funções que simulam o fluxo do dado */
void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(int quadro[]);
void MeioDeComunicacao(int fluxoBrutoDeBits[]);
void CamadaEnlaceDadosReceptora(int quadro[]);
void CamadaDeAplicacaoReceptora(int quadro[]);
void AplicacaoReceptora(string mensagem);

/*!< Funções de erro */
void CamadaEnlanceDadosTransmissoraControleDeErro(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(int quadro[]);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(int quadro[]);

/*!< Funções de recuperação de informação do quadro */
int tamanhoMensagemQuadro(vector<int> quadro);
vector<int> mensagemQuadro(vector<int> quadro);
vector<int> mensagemErroQuadro(vector<int> quadro);
int erroQuadro(vector<int> quadro);

/*!< Funções de manipulação de binário */
void transformarEmBinario(int binario[8], int n);
vector<int> transformarEmBinarioInverso(vector<int> quadro, int n);

/*!< Estrutura do Quadro
* É um vetor de inteiros onde cada posição representa um bit;
* Os 8 primeiros bits (inteiros) são reservados para o tamanho do quadro. Esse tamanho é a quantidade de caracteres que a mensagem possui (ou seja, a quantidade de bits será dada por esse valor * 8);
* Os [tamanho do quadro * 8] bits seguintes representarão a mensagem do quadro. Note que para cada caractere, serão utilizados 8 bits. Além disso, o bit mais significativo estará na posição 0 (a frente);
* Os últimos x bits serão reservados para o erro.
*/

void main(void) {
    AplicacaoTransmissora();
} 

int funcaoErro = -1;
int porcentagemErro = -1;

bool definirErroPorcentagem() {
    int funcaoErroTemporaria = 0;
    cout << "Escolha a função de erro: " << endl;
    cout << BIT_PARIDADE_PAR << " - Paridade Par" << endl;
    cout << BIT_PARIDADE_IMPAR << " - Paridade Impar" << endl;
    cout << CRC << " - CRC" << endl;
    cin >> funcaoErroTemporaria;
    cout << endl;
    
    if(funcaoErroTemporaria != BIT_PARIDADE_PAR && funcaoErroTemporaria != BIT_PARIDADE_IMPAR && funcaoErroTemporaria != CRC) {
        cout << "Operação inválida" << endl << endl;
        return false;
    }

    int porcentagemErroTemporaria = 0;
    cout << "Defina a porcentagem do erro: " << endl;
    cin >> porcentagemErroTemporaria;
    cout << endl;
    
    funcaoErro = funcaoErroTemporaria;
    porcentagemErro = porcentagemErroTemporaria;

    return true;
}

void AplicacaoTransmissora(void) {
    cout << "SIMULADOR DA CAMADA DE ENLACE" << endl << endl; 
    string mensagem;

    while(true) {
        int sair = 0;
        cout << "Deseja finalizar a aplicação?" << endl;
        cout << "0 - Não" << endl;
        cout << "1 - Sim" << endl;
        cin >> sair;
        if(sair == 1) 
            break;
        cout << endl;
        
        char c; while ((c = getchar()) != '\n' && c != EOF);
        cout << "Digite uma mensagem:" << endl;
        std::getline(cin, mensagem);
        cout << mensagem << endl;
        cout << endl;

        if(!definirErroPorcentagem()) {
            funcaoErro = -1;
            porcentagemErro = -1;
            continue;
        }

        CamadaDeAplicacaoTransmissora(mensagem);
    }

    return;
} 

void CamadaDeAplicacaoTransmissora(string mensagem) {
    vector<int> quadro;

    /*!< Transformando o tamanho da mensagem (quantidade de caracteres) em binário */
    int tamanhoMensagem = mensagem.size();
    quadro = transformarEmBinarioInverso(quadro, tamanhoMensagem);

    /*!< Transformando a mensagem em binario com base no ASCII */
    for (int k = 0; k < mensagem.length(); k++) {
        /*!< Convertendo para ASCII */
        int ascii = int(mensagem[k]);

        /*!< Convertendo ASCII para binário */
        quadro = transformarEmBinarioInverso(quadro, ascii);
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

/*!< *********************************** CALCULO DE ERRO *********************************** */

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
    vector<int> mensagem, crc, gerador;
    int tamanho_mensagem, tamanho_crc, tamanho_gerador;

    tamanho_crc = 5;    // pode ser de outro tamanho
    tamanho_gerador = tamanho_crc + 1;
    tamanho_mensagem = tamanhoMensagemQuadro(quadro) + tamanho_gerador;

    crc = erroQuadro(mensagem);
    mensagem = mensagemErroQuadro(quadro);

    for (int i = 0; i < tamanho_gerador; i++)
        mensagem.push_back(0);

    for (int i = tamanho_gerador; i < tamanho_mensagem; i++) {
        
    }
    


}

/*!< *********************************** MANIPULAÇÃO DO QUADRO *********************************** */

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
    vector<int> mensagem;
    return mensagem;
}

vector<int> mensagemErroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    vector<int> mensagem;
    return mensagem;
}

vector<int> erroQuadro(vector<int> quadro) {
    vector<int> erro;
    return erro;
}

/*!< *********************************** MANIPULAÇÃO DE BINÁRIO *********************************** */
   
void transformarEmBinario(int binario[8], int n) {
    int i = 0;
    while (n > 0) {                                                                       //Indice: 76543210  
        binario[i] = n % 2; /*!< O bit MENOS significativo estará na posição 0, exemplo do valor 6: 00000110 */
        n = n / 2;
        i++;
    }
}

vector<int> transformarEmBinarioInverso(vector<int> quadro, int n) {
    int binario[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    transformarEmBinario(binario, n);
    for(int j = 7; j >= 0; j--) {                                                                  //Indice: 01234567 
        quadro.push_back(binario[j]); /*!< O bit MAIS significativo estará na posição 0, exemplo do valor 6: 00000110 */
    }
    return quadro;
}
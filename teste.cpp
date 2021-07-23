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
#define INICIO_MENSAGEM_NO_QUADRO 8

int tamanhoMensagemQuadro(vector<int> quadro) {
    //O tamanho do quadro será armazenado com o bit mais significativo na primeira posição
    return  (((quadro[0] * pow(2, 7)) 
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
    std::size_t const tamMsgBit = 8;
    std::size_t const tamErroBit = 8;
    std::vector<int> msgQuadro(quadro.begin() + tamMsgBit, quadro.end() - tamErroBit + 1);

    return msgQuadro;
}

vector<int> mensagemErroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    std::size_t const tamMsgBit = 8;
    std::vector<int> msgErroQuadro(quadro.begin() + tamMsgBit, quadro.end());

    return msgErroQuadro;
}

vector<int> erroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    std::size_t const tamMsgBit = 8;
    std::vector<int> errQuadro(quadro.begin() + tamanho_mensagem + tamMsgBit, quadro.end());
    
    return errQuadro;
}

int main(void){

    vector<int> lines;
    int aux;
    int metade = lines.size()/2;
    
    for(int i = 0; i< 88;i++){
        cin >> aux;
        lines.push_back(aux);
    }
        
    // for(int i = 0; i <  lines.size(); i++){
    //     cout << lines[i];
    // }
    // cout << endl;

    // vector<int> msgQuadro = mensagemQuadro(lines);
    // for(int i = 0; i < msgQuadro.size(); i++){
    //     cout << msgQuadro[i];
    // }
    // cout << endl;

    // vector<int> msgErroQuadro = mensagemErroQuadro(lines);
    // for(int i = 0; i < msgErroQuadro.size(); i++){
    //     cout << msgErroQuadro[i];
    // }
    // cout << endl;

    // vector<int> errQuadro = erroQuadro(lines);
    // for(int i = 0; i < errQuadro.size(); i++){
    //     cout << errQuadro[i];
    // }
    // cout << endl;

    size_t const tamMsgBit = INICIO_MENSAGEM_NO_QUADRO;
    int tamanho_mensagem = tamanhoMensagemQuadro(lines);
    int tamMsgETamMsgBit = tamanho_mensagem + tamMsgBit;
    unsigned long int errInteiroQuadro = 0;

    cout << tamMsgETamMsgBit << endl;
    
    for(int indice = tamMsgETamMsgBit, pot2 = TAMANHO_CRC-1; indice < tamMsgETamMsgBit + TAMANHO_CRC; indice++, pot2--){
        errInteiroQuadro += (lines[indice] * pow(2, pot2));
        cout << errInteiroQuadro << endl;
    }

    cout << errInteiroQuadro;
    
    return 0;
}
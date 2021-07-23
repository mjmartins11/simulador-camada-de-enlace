#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2
#define TAMANHO_CRC 31

int tamanhoMensagemQuadro(vector<int> quadro) {
    //O tamanho do quadro será armazenado com o bit mais significativo na primeira posição
    return (((quadro[0] * pow(2, 7)) 
            + (quadro[1] * pow(2, 6)) 
            + (quadro[2] * pow(2, 5)) 
            + (quadro[3] * pow(2, 4)) 
            + (quadro[4] * pow(2, 3)) 
            + (quadro[5] * pow(2, 2))) * 8); 
}

vector<int> mensagemQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    std::size_t const tamMsgBit = 6;
    std::size_t const tamErroBit = 8;
    std::vector<int> msgQuadro(quadro.begin() + tamMsgBit, quadro.end() - tamErroBit);

    return msgQuadro;
}

vector<int> mensagemErroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    std::size_t const tamMsgBit = 6;
    std::vector<int> msgErroQuadro(quadro.begin() + tamMsgBit, quadro.end());

    return msgErroQuadro;
}

vector<int> erroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    std::size_t const tamMsgBit = 6;
    std::vector<int> msgErroQuadro(quadro.begin() + tamanho_mensagem, quadro.end());
    
    return errQuadro;
}

int main(void){

    vector<int> lines;
    int aux;
    int metade = lines.size()/2;
    
    for(int i = 0; i<63;i++){
        cin >> aux;
        lines.push_back(aux);
    }
        
    for(int i = 0; i <  lines.size(); i++){
        cout << lines[i];
    }
    cout << endl;

    vector<int> msgQuadro = mensagemQuadro(lines);
    for(int i = 0; i < msgQuadro.size(); i++){
        cout << msgQuadro[i];
    }
    cout << endl;

    vector<int> msgErroQuadro = mensagemErroQuadro(lines);
    for(int i = 0; i < msgErroQuadro.size(); i++){
        cout << msgErroQuadro[i];
    }

    vector<int> errQuadro = erroQuadro(lines);
    for(int i = 0; i < errQuadro.size(); i++){
        cout << errQuadro[i];
    }
    cout << endl;
    
    return 0;
}
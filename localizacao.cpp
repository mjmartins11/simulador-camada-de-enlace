#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <locale>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2
#define TAMANHO_CRC 32
#define TAMANHO_GERADOR TAMANHO_CRC + 1
#define INICIO_MENSAGEM_NO_QUADRO 8

#define NUMERO_ALEATORIO_MIN 0
#define NUMERO_ALEATORIO_MAX 99

int porcentagemErro = 100;

/*!< Retorna a quantidade de bits que a mensagem possui */
/*!< Com base no exemplo, retorna o valor 6*8 = 48 */
int tamanhoMensagemQuadro(vector<int> quadro) {
    /*!< O tamanho do quadro sera armazenado com o bit mais significativo na primeira posicao */
    return  (((quadro[0] * pow(2, 7)) 
            + (quadro[1] * pow(2, 6)) 
            + (quadro[2] * pow(2, 5)) 
            + (quadro[3] * pow(2, 4)) 
            + (quadro[4] * pow(2, 3)) 
            + (quadro[5] * pow(2, 2))
            + (quadro[6] * pow(2, 1))
            + (quadro[7] * pow(2, 0))) * 8); 
}

void transformarEmBinario(int binario[8], int n) {
    int i = 0;
    while (n > 0) {                                                                       //Indice: 76543210  
        binario[i] = n % 2; /*!< O bit MENOS significativo estarï¿½ na posiï¿½ï¿½o 0, exemplo do valor 6: 00000110 */
        n = n / 2;
        i++;
    }
}

vector<int> transformarEmBinarioInverso(vector<int> quadro, int n) {
    int binario[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    transformarEmBinario(binario, n);
    for(int j = 7; j >= 0; j--) {                                                                  //Indice: 01234567 
        quadro.push_back(binario[j]); /*!< O bit MAIS significativo estarï¿½ na posiï¿½ï¿½o 0, exemplo do valor 6: 00000110 */
    }
    return quadro;
}

void AplicacaoReceptora(wstring mensagem) {
    // std::wstring st = L"á";
    // std::wcout << st << endl; 
    // wcout << (int) mensagem[0] << endl;
    // wcout << (int) mensagem[1] << endl;
    // wcout << (int) mensagem[2] << endl;
    // wcout << (int) mensagem[3] << endl;
    // wcout << (int) mensagem[4] << endl;
    wcout << "A mensagem recebida foi: " << mensagem << endl;
} 

/*!< Transforma o quadro em mensagem string */
void CamadaDeAplicacaoReceptora(vector<int> quadro) {
    wstring mensagem;
    int tamanhoMensagem = tamanhoMensagemQuadro(quadro);

              /*!< Pulando bits de tamanho */
    for(int i = INICIO_MENSAGEM_NO_QUADRO; i < INICIO_MENSAGEM_NO_QUADRO+tamanhoMensagem; i+=8) {
        int unicode_numerico =  (quadro[i+0] * pow(2, 7)) 
                            + (quadro[i+1] * pow(2, 6)) 
                            + (quadro[i+2] * pow(2, 5))
                            + (quadro[i+3] * pow(2, 4)) 
                            + (quadro[i+4] * pow(2, 3)) 
                            + (quadro[i+5] * pow(2, 2)) 
                            + (quadro[i+6] * pow(2, 1)) 
                            + (quadro[i+7] * pow(2, 0));

        wchar_t unicode = 63; /*!< ? */
        /*!< Os valores no intervalo [0, 32] e [127, 160] não são visisveis no terminal e por isso são substituidos pelo simbolo de interrogação */
        if(!((unicode_numerico >= 0 && unicode_numerico <= 32) || (unicode_numerico >= 127 && unicode_numerico <= 160))) {
            unicode = (wchar_t) unicode_numerico;
        }

        mensagem.push_back(unicode);
    }

    AplicacaoReceptora(mensagem);
}

void MeioDeComunicacao(vector<int> quadro) { 
    vector<int> quadroPosMeioDeComunicacao;
    
    /*!< Inserindo o tamanho da mensagem no novo quadro */
    for(int i = 0; i < INICIO_MENSAGEM_NO_QUADRO; i++) {
        quadroPosMeioDeComunicacao.push_back(quadro[i]);
    }
    
    srand(time(nullptr));
            /*!< Pulando bits de tamanho, eles nao serao alterados */
    for(int i = INICIO_MENSAGEM_NO_QUADRO; i < quadro.size(); i++) {
        int numeroAleatorio = NUMERO_ALEATORIO_MIN + (rand()) / (RAND_MAX/(NUMERO_ALEATORIO_MAX - NUMERO_ALEATORIO_MIN));
        
        if(numeroAleatorio < porcentagemErro) { /*!< Acontece a inversao do bit */
            quadroPosMeioDeComunicacao.push_back(!quadro[i]);
        } else { /*!< Nao acontece a inversao do bit */
            quadroPosMeioDeComunicacao.push_back(quadro[i]);
        }
    }
    
    CamadaDeAplicacaoReceptora(quadroPosMeioDeComunicacao);
}

/*!< Cria o quadro contendo o tamanho da mensagem + mensagem (em binario) */
void CamadaDeAplicacaoTransmissora(wstring mensagem) {
    vector<int> quadro;

    /*!< Transformando o tamanho da mensagem (quantidade de caracteres) em binario */
    int tamanhoMensagem = mensagem.size();
    quadro = transformarEmBinarioInverso(quadro, tamanhoMensagem);

    /*!< Transformando a mensagem em binario com base no ASCII */
    for (int k = 0; k < mensagem.length(); k++) {
        /*!< Convertendo para ASCII */
        int ascii = int(mensagem[k]);

        /*!< Convertendo ASCII para binario */
        quadro = transformarEmBinarioInverso(quadro, ascii);
    }

    MeioDeComunicacao(quadro);
}


//UNICODE: https://unicode-table.com/en/#00A9
//Wide Char: https://www.geeksforgeeks.org/wide-char-and-library-functions-in-c/
//String vs WString: https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
int main() {
    locale::global(locale(""));

    // Imprimir tabela UNICODE
    // for(int i = 0; i < 256; i++) {
    //     int ascii_numerico = i;
    //     wchar_t ascii = (wchar_t)ascii_numerico;
    //     std::wstring mensagem;
    //     mensagem.push_back(ascii);
    //     wcout << i << " : " << mensagem << endl;
    // }

    wstring mensagem;
    getline(wcin, mensagem);

    CamadaDeAplicacaoTransmissora(mensagem);
    return 0;
}


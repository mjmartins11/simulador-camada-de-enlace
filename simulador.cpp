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
#define GRAU_GERADOR 32
#define INICIO_MENSAGEM_NO_QUADRO 8

#define NUMERO_ALEATORIO_MIN 0
#define NUMERO_ALEATORIO_MAX 99

/*!< Funções que simulam o fluxo do dado */
void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(wstring mensagem);
void CamadaEnlaceDadosTransmissora(vector<int> quadro);
void MeioDeComunicacao(vector<int> quadro);
void CamadaEnlaceDadosReceptora(vector<int> quadro);
void CamadaDeAplicacaoReceptora(vector<int> quadro);
void AplicacaoReceptora(wstring mensagem);

/*!< Funções de erro */
vector<int> CamadaEnlanceDadosTransmissoraControleDeErro(vector<int> quadro);
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(vector<int> quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> &quadro);
bool CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> &quadro);
bool CamadaEnlanceDadosReceptoraControleDeErro(vector<int> quadro);
bool CamadaEnlaceDadosReceptoraControleDeErroBitParidade(vector<int> quadro);
bool CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(int qtdBit1, vector<int>bitParidade);
bool CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(int qtdBit1, vector<int>bitParidade);

/*!< Funcoes auxiliares CRC */
void adicionarNoFinalDoResto(vector<int> &resto, int valor);
void XOR(vector<int> &resto, vector<int> gerador);

/*!< Funções de recuperação de informação do quadro */
int tamanhoMensagemQuadro(vector<int> quadro);
vector<int> mensagemQuadro(vector<int> quadro);
vector<int> mensagemErroQuadro(vector<int> quadro);
vector<int> erroQuadro(vector<int> quadro);
int erroInteiroQuadro(vector<int> quadro);
vector<int> inserirCampoErroQuadro(vector<int> quadro);

/*!< Funções de manipulação de binário */
void transformarEmBinario(int binario[8], int n);
vector<int> transformarEmBinarioInverso(vector<int> quadro, int n);

/*!< Função de menu */
bool definirErroPorcentagem();

/*!< Estrutura do Quadro
* É um vetor de inteiros onde cada posição representa um bit;
* Os 8 primeiros bits (inteiros) são reservados para o tamanho do quadro. Esse tamanho é a quantidade de caracteres que a mensagem possui (ou seja, a quantidade de bits será dada por esse valor * 8);
* Os [tamanho do quadro * 8] bits seguintes representarão a mensagem do quadro. Note que para cada caractere, serão utilizados 8 bits. Além disso, o bit mais significativo estará na posição 0 (a frente);
* Os últimos 32 bits serão reservados para o erro. Caso o erro seja de paridade impar ou par, o bit de paridade sera o primeiro dos 32 alocados.
*/

/*!< Útil
* UNICODE: https://unicode-table.com/en/#00A9
* wide char: https://www.geeksforgeeks.org/wide-char-and-library-functions-in-c/
* string vs wstring: https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
*/

int funcaoErro = -1;
int porcentagemErro = -1;

int main(void) {
    locale::global(locale(""));

    /*!< Imprimir tabela UNICODE */
    // for(int i = 0; i < 256; i++) {
    //     int ascii_numerico = i;
    //     wchar_t ascii = (wchar_t)ascii_numerico;
    //     std::wstring mensagem;
    //     mensagem.push_back(ascii);
    //     wcout << i << " : " << mensagem << endl;
    // }

    AplicacaoTransmissora();

    return 0;
} 

void AplicacaoTransmissora(void) {
    wstring mensagem;

    while(true) {
        wcout << ">>>>> SIMULADOR DA CAMADA DE ENLACE" << endl; 
        int sair = 0;
        wcout << "Deseja finalizar a aplicacao?" << endl;
        wcout << "0 - Nao" << endl;
        wcout << "1 - Sim" << endl;
        wcin >> sair; 
        wcin.ignore(); /*!< Limpando o buffer */   
        if(sair == 1) 
            break;
        wcout << endl;
        
        wcout << "Digite uma mensagem: ";        
        getline(wcin, mensagem);
        wcout << "Mensagem: " << mensagem << endl;
        wcout << endl;

        porcentagemErro = 1;

        if(!definirErroPorcentagem()) {
            funcaoErro = -1;
            porcentagemErro = -1;
            continue;
        }

        CamadaDeAplicacaoTransmissora(mensagem);
    }
    
    return;
} 

/*!< Cria o quadro contendo o tamanho da mensagem + mensagem (em binário) */
void CamadaDeAplicacaoTransmissora(wstring mensagem) {
    vector<int> quadro;

    /*!< Transformando o tamanho da mensagem (quantidade de caracteres) em binário */
    int tamanhoMensagem = mensagem.size();
    quadro = transformarEmBinarioInverso(quadro, tamanhoMensagem);

    /*!< Transformando a mensagem em binario com base no UNICODE */
    for (int k = 0; k < mensagem.length(); k++) {
        /*!< Convertendo para UNICODE */
        int unicode = int(mensagem[k]);

        /*!< Convertendo ASCII para binário */
        quadro = transformarEmBinarioInverso(quadro, unicode);
    }

    CamadaEnlaceDadosTransmissora(quadro);
}

/*!< Cria o quadro contendo o tamanho da mensagem + mensagem + erro (em binário) */
void CamadaEnlaceDadosTransmissora(vector<int> quadro) {
    /*! Incluindo o campo de erro ao quadro */
    quadro = CamadaEnlanceDadosTransmissoraControleDeErro(quadro);
    /*! Simulacao de passagem pelo meio de comunicacao */
    MeioDeComunicacao(quadro);
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
    
    CamadaEnlaceDadosReceptora(quadroPosMeioDeComunicacao);
}

/*!< Verifica se ocorreu erro e cria o quadro removendo o erro e ficando com tamanho da mensagem + mensagem (em binário) */
void CamadaEnlaceDadosReceptora(vector<int> quadro) {
    /*! Verificacao de erros na mensagem recebida */
    if(CamadaEnlanceDadosReceptoraControleDeErro(quadro)){
        wcout << "O mecanismo de deteccao de erros encontrou erro(s) na mensagem recebida!" << endl;
    }

    CamadaDeAplicacaoReceptora(quadro);
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
        /*!< Os valores no intervalo [0, 31] e [127, 160] não são visisveis no terminal e por isso são substituidos pelo simbolo de interrogação */
        if(!((unicode_numerico >= 0 && unicode_numerico <= 31) || (unicode_numerico >= 127 && unicode_numerico <= 160))) {
            unicode = (wchar_t) unicode_numerico;
        }

        mensagem.push_back(unicode);
    }

    AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(wstring mensagem) {
    wcout << "A mensagem recebida foi: " << mensagem << endl << endl;
} 

/*!< *********************************** CALCULO DE ERRO *********************************** */

vector<int> CamadaEnlanceDadosTransmissoraControleDeErro(vector<int> quadro) {
    /*!< Insere 32 bits ao quadro (campo de erro que pode ser utilizado pelo CRC ou pelo metodo de paridade) */
    quadro = inserirCampoErroQuadro(quadro);

    switch(funcaoErro) {
        case BIT_PARIDADE_PAR: 
            quadro = CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(quadro);
            break;
        case BIT_PARIDADE_IMPAR: 
            quadro = CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(quadro);
            break;
        case CRC:
            CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro);
            break;
    }

    return quadro;
}

bool CamadaEnlanceDadosReceptoraControleDeErro(vector<int> quadro) {
    bool erro;
    switch(funcaoErro) {
        case BIT_PARIDADE_PAR: 
            erro = CamadaEnlaceDadosReceptoraControleDeErroBitParidade(quadro);
            break;
        case BIT_PARIDADE_IMPAR: 
            erro = CamadaEnlaceDadosReceptoraControleDeErroBitParidade(quadro);
            break;
        case CRC:
            erro = CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
            break;
    }

    return erro;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(vector<int> quadro) {
    int tamMensagem, qtdBit1;
    vector<int>mensagem;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == 1)
            qtdBit1++;
    }

    if((qtdBit1%2 == 1 && funcaoErro == BIT_PARIDADE_PAR) || (qtdBit1%2 == 0 && funcaoErro == BIT_PARIDADE_IMPAR)){
        quadro[tamMensagem + INICIO_MENSAGEM_NO_QUADRO] = 1;
    } else{
        quadro[tamMensagem + INICIO_MENSAGEM_NO_QUADRO] = 0;
    }
    return quadro;
}

bool CamadaEnlaceDadosReceptoraControleDeErroBitParidade(vector<int> quadro) {
    int tamMensagem, qtdBit1;
    vector<int>mensagem;
    vector<int>bitParidade;
    bool erro;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);
    bitParidade = erroQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == 1)
            qtdBit1++;
    }

    wcout << "Para funcao do tipo " << funcaoErro << ", ha " << qtdBit1 << " bits 1 na mensagem com bit paridade " << bitParidade[0] << endl; 

    if(funcaoErro == BIT_PARIDADE_PAR){
        erro = CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(qtdBit1, bitParidade);
    } else{
        erro = CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(qtdBit1, bitParidade);
    }
    return erro;
}

bool CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(int qtdBit1, vector<int>bitParidade) {
    if(((qtdBit1%2 == 0) && (bitParidade[0] == 0)) || ((qtdBit1%2 == 1) && (bitParidade[0] == 1))){
        return false;
    } else{
        return true;
    }
}

bool CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(int qtdBit1, vector<int>bitParidade) {
    if(((qtdBit1%2 == 0) && (bitParidade[0] == 1)) || ((qtdBit1%2 == 1) && (bitParidade[0] == 0))){
        return false;
    } else {
        return true;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> &quadro) {
    // polinomio CRC-32(IEEE 802)
    vector<int> gerador = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1};
    vector<int> mensagem = mensagemErroQuadro(quadro);
    vector<int> crc;
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro)-8;

    for (int i = 0; i < GRAU_GERADOR; i++) {
        crc.push_back(mensagem[i]);
    }
    crc.push_back(mensagem[GRAU_GERADOR]);

    for (int i = TAMANHO_GERADOR; i < tamanho_mensagem + GRAU_GERADOR; i++) {
        if (crc[0] != 0)
            XOR(crc, gerador);
        adicionarNoFinalDoResto(crc, mensagem[i]);
    }

    for (int j = 1, i = tamanho_mensagem + 8; j < 33; j++, i++)
        quadro[i] = crc[j];

}

bool CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> &quadro) {
    // polinomio CRC-32(IEEE 802)
    vector<int> gerador = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1};
    vector<int> mensagem = mensagemErroQuadro(quadro);
    vector<int> resto; 
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro)-8;
    bool erro = false;

    for (int i = 0; i < TAMANHO_GERADOR; i++)
        resto.push_back(mensagem[i]);

    for (int i = TAMANHO_GERADOR; i < tamanho_mensagem+TAMANHO_CRC; i++) {
        if (resto[0] != 0)
            XOR(resto, gerador);
        adicionarNoFinalDoResto(resto, mensagem[i]);
    }

    for (int i = 1; i < TAMANHO_GERADOR; i++) {
        if (resto[i] != 0) {
            erro = true;
            break;
        }
    }

    return erro;
}

void XOR(vector<int> &resto, vector<int> gerador) {
    for (int i = 0; i < TAMANHO_GERADOR; i++)
        if (resto[i] == gerador[i])
            resto[i] = 0;
        else 
            resto[i] = 1;
}

void adicionarNoFinalDoResto(vector<int> &resto, int valor) {
    for (int i = 0; i < TAMANHO_GERADOR-1; i++)
        resto[i] = resto[i+1];
    resto[TAMANHO_GERADOR-1] = valor;
}

vector<int> inserirCampoErroQuadro(vector<int> quadro){
    for(int i = 0; i < TAMANHO_CRC; i++){
        quadro.push_back(0);
    }

    return quadro;
}

/*!< *********************************** MANIPULAÇÃO DO QUADRO *********************************** */

/*!< Para exemplificação das funções abaixo, considere o seguinte quadro com TAMANHO + MENSAGEM + ERRO 
*     00000101        01001000 01100101 01101100 01101100 01101111      00000000 00000000 00000000 00000001
*    Tamanho = 5       H       e        l        l        o                            Erro
*/

/*!< Retorna a quantidade de bits que a mensagem possui */
/*!< Com base no exemplo, retorna o valor 5*8 = 40 */
int tamanhoMensagemQuadro(vector<int> quadro) {
    /*!< O tamanho do quadro será armazenado com o bit mais significativo na primeira posição */
    return  (((quadro[0] * pow(2, 7)) 
            + (quadro[1] * pow(2, 6)) 
            + (quadro[2] * pow(2, 5)) 
            + (quadro[3] * pow(2, 4)) 
            + (quadro[4] * pow(2, 3)) 
            + (quadro[5] * pow(2, 2))
            + (quadro[6] * pow(2, 1))
            + (quadro[7] * pow(2, 0))) * 8); 
}

/*!< Retorna apenas os bits que representam a mensagem no quadro */
/*!< Com base no exemplo, retorna: 01001000 01100101 01101100 01101100 01101111 */
vector<int> mensagemQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamMsgBit = INICIO_MENSAGEM_NO_QUADRO;
    size_t const tamErroBit = INICIO_MENSAGEM_NO_QUADRO;
    vector<int> msgQuadro(quadro.begin() + tamMsgBit, quadro.end() - tamErroBit + 1);

    return msgQuadro;
}

/*!< Retorna os bits que representam a mensagem e o erro no quadro */
/*!< Com base no exemplo, retorna: 01001000 01100101 01101100 01101100 01101111 00000000 00000000 00000000 00000001 */
vector<int> mensagemErroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamMsgBit = INICIO_MENSAGEM_NO_QUADRO;
    vector<int> msgErroQuadro(quadro.begin() + tamMsgBit, quadro.end());

    return msgErroQuadro;
}

/*!< Retorna os bits que representam o erro no quadro */
/*!< Com base no exemplo, retorna: 00000000 00000000 00000000 00000001 */
vector<int> erroQuadro(vector<int> quadro) {
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    size_t const tamMsgBit = INICIO_MENSAGEM_NO_QUADRO;
    vector<int> errQuadro(quadro.begin() + tamanho_mensagem + tamMsgBit, quadro.end());
    
    return errQuadro;
}

/*!< Retorna os bits que representam o erro no quadro */
/*!< Com base no exemplo, retorna: 1 */
int erroInteiroQuadro(vector<int> quadro) {
    size_t const tamMsgBit = INICIO_MENSAGEM_NO_QUADRO;
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro);
    int tamMsgETamMsgBit = tamanho_mensagem + tamMsgBit;
    unsigned long int errInteiroQuadro = 0;
    
    for(int indice = tamMsgETamMsgBit, pot2 = TAMANHO_CRC-1; indice < tamMsgETamMsgBit + TAMANHO_CRC; indice++, pot2--){
        errInteiroQuadro += (quadro[indice] * pow(2, pot2));
        wcout << errInteiroQuadro << endl;
    }

    return  errInteiroQuadro;
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

/*!< *********************************** MENU *********************************** */

bool definirErroPorcentagem() {
    int funcaoErroTemporaria = 0;
    wcout << "Seja" << endl;
    wcout << BIT_PARIDADE_PAR << " - Paridade Par" << endl;
    wcout << BIT_PARIDADE_IMPAR << " - Paridade Impar" << endl;
    wcout << CRC << " - CRC" << endl;
    wcout << "Escolha a funcao de erro: ";
    wcin >> funcaoErroTemporaria;

    if(funcaoErroTemporaria != BIT_PARIDADE_PAR && funcaoErroTemporaria != BIT_PARIDADE_IMPAR && funcaoErroTemporaria != CRC) {
        wcout << "Operacao invalida" << endl << endl;
        return false;
    }
    wcout << "Funcao de erro: " << funcaoErroTemporaria << endl;
    wcout << endl;

    int porcentagemErroTemporaria = 0;
    wcout << "Defina a porcentagem de dar erro (valor inteiro entre 0 a 100): ";
    wcin >> porcentagemErroTemporaria;

    if(porcentagemErroTemporaria < 0 || porcentagemErroTemporaria > 100) {
        wcout << "Porcentagem invalida" << endl << endl;
        return false;
    }
    wcout << "Porcentagem: " << porcentagemErroTemporaria << endl;
    wcout << endl;
    
    funcaoErro = funcaoErroTemporaria;
    porcentagemErro = porcentagemErroTemporaria;

    return true;
}
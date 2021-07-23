#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

#define BIT_PARIDADE_PAR 0
#define BIT_PARIDADE_IMPAR 1
#define CRC 2
#define TAMANHO_CRC 32
#define TAMANHO_GERADOR TAMANHO_CRC + 1
#define INICIO_MENSAGEM_NO_QUADRO 8

#define NUMERO_ALEATORIO_MIN 0
#define NUMERO_ALEATORIO_MAX 99

/*!< Funções que simulam o fluxo do dado */
void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<int> quadro);
void MeioDeComunicacao(vector<int> quadro);
void CamadaEnlaceDadosReceptora(vector<int> quadro);
void CamadaDeAplicacaoReceptora(vector<int> quadro);
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
vector<int> erroQuadro(vector<int> quadro);
int erroInteiroQuadro(vector<int> quadro);

/*!< Funções de manipulação de binário */
void transformarEmBinario(int binario[8], int n);
vector<int> transformarEmBinarioInverso(vector<int> quadro, int n);

/*!< Estrutura do Quadro
* É um vetor de inteiros onde cada posição representa um bit;
* Os 8 primeiros bits (inteiros) são reservados para o tamanho do quadro. Esse tamanho é a quantidade de caracteres que a mensagem possui (ou seja, a quantidade de bits será dada por esse valor * 8);
* Os [tamanho do quadro * 8] bits seguintes representarão a mensagem do quadro. Note que para cada caractere, serão utilizados 8 bits. Além disso, o bit mais significativo estará na posição 0 (a frente);
* Os últimos 32 bits serão reservados para o erro. Caso o erro seja de paridade impar ou par, o bit de paridade sera o primeiro dos 32 alocados.
*/

/*!< Dúvida
 * É preciso criar o código CRC antes de enviar a mensagem ou apenas recebe de entrada do usuário?
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
    cout << "Defina a porcentagem de dar erro (valor entre 0 a 100): " << endl;
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
        getline(cin, mensagem);
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

/*!< Cria o quadro contendo o tamanho da mensagem + mensagem (em binário) */
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

/*!< Cria o quadro contendo o tamanho da mensagem + mensagem + erro (em binário) */
void CamadaEnlaceDadosTransmissora(vector<int> quadro) {
    CamadaEnlanceDadosTransmissoraControleDeErro(quadro);
    MeioDeComunicacao(quadro);
}

void MeioDeComunicacao(vector<int> quadro) {
    vector<int> quadroPosMeioDeComunicacao;

    srand(time(nullptr));
    for(int i = 0; i < quadro.size(); i++) {
        int numeroAleatorio = NUMERO_ALEATORIO_MIN + (rand()) / (RAND_MAX/(NUMERO_ALEATORIO_MAX - NUMERO_ALEATORIO_MIN));

        if(numeroAleatorio < porcentagemErro) { /*!< Acontece a inversão do bit */
            quadroPosMeioDeComunicacao.push_back(!quadro[i]);
        } else { /*!< Não acontece a inversão do bit */
            quadroPosMeioDeComunicacao.push_back(quadro[i]);
        }
    }

    CamadaEnlaceDadosReceptora(quadroPosMeioDeComunicacao);
}

/*!< Verifica se ocorreu erro e cria o quadro removendo o erro e ficando com tamanho da mensagem + mensagem (em binário) */
void CamadaEnlaceDadosReceptora(vector<int> quadro) {

    CamadaDeAplicacaoReceptora(quadro);
}

/*!< Transforma o quadro em mensagem string */
void CamadaDeAplicacaoReceptora(vector<int> quadro) {
    string mensagem;
    int tamanhoMensagem = tamanhoMensagemQuadro(quadro);

              /*!< Pulando bits de tamanho */
    for(int i = INICIO_MENSAGEM_NO_QUADRO; i < INICIO_MENSAGEM_NO_QUADRO+tamanhoMensagem; i+=8) {
        int ascii_numerico =  (quadro[i+0] * pow(2, 7)) 
                            + (quadro[i+1] * pow(2, 6)) 
                            + (quadro[i+2] * pow(2, 5))
                            + (quadro[i+3] * pow(2, 4)) 
                            + (quadro[i+4] * pow(2, 3)) 
                            + (quadro[i+5] * pow(2, 2)) 
                            + (quadro[i+6] * pow(2, 1)) 
                            + (quadro[i+7] * pow(2, 0));

        char ascii = (char) ascii_numerico;
        mensagem += ascii;
    }

    AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(string mensagem) {
    cout << "A mensagem recebida foi: " << mensagem << endl;
} 

/*!< *********************************** CALCULO DE ERRO *********************************** */

void CamadaEnlanceDadosTransmissoraControleDeErro(vector<int> quadro) {
    switch(funcaoErro) {
        case BIT_PARIDADE_PAR: 
            CamadaEnlaceDadosReceptoraControleDeErroBitParidade(quadro);
            break;
        case BIT_PARIDADE_IMPAR: 
            CamadaEnlaceDadosReceptoraControleDeErroBitParidade(quadro);
            break;
        case CRC:
            CamadaEnlaceDadosTransmissoraControleDeErroErroCRC(quadro);
            break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidade(vector<int> quadro) {
    int tamMensagem, qtdBit1;
    vector<int>mensagem;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == BIT_PARIDADE_IMPAR)
            qtdBit1++;
    }

    if((qtdBit1%2 == 1 && funcaoErro == BIT_PARIDADE_PAR) || (qtdBit1%2 == 0 && funcaoErro == BIT_PARIDADE_IMPAR)){
        quadro[tamMensagem + INICIO_MENSAGEM_NO_QUADRO] = 1;
    } else{
        quadro[tamMensagem + INICIO_MENSAGEM_NO_QUADRO] = 0;
    }
    return;
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidade(vector<int> quadro) {
    int tamMensagem, qtdBit1;
    vector<int>mensagem;
    vector<int>bitParidade;
    
    mensagem = mensagemQuadro(quadro);
    tamMensagem = tamanhoMensagemQuadro(quadro);
    bitParidade = erroQuadro(quadro);

    for(int i = 0; i < tamMensagem; i++){
        if(mensagem[i] == BIT_PARIDADE_IMPAR)
            qtdBit1++;
    }

    if(funcaoErro == BIT_PARIDADE_PAR){
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(qtdBit1, bitParidade);
    } else{
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(qtdBit1, bitParidade);
    }
    return;
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(int qtdBit1, vector<int>bitParidade) {
    if(((qtdBit1%2 == 0) && (bitParidade[0] == 0)) || ((qtdBit1%2 != 0) && (bitParidade[0] == 1))){
        cout << "paridade par nao apresentou erros" << endl;
    } else{
        cout << "paridade par apresentou erros";
    }
    return;
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(int qtdBit1, vector<int>bitParidade) {
    if(((qtdBit1%2 != 0) && (bitParidade[0] == 0)) || ((qtdBit1%2 == 0) && (bitParidade[0] == 1))){
        cout << "paridade impar nao apresentou erros" << endl;
    } else {
        cout << "paridade impar apresentou erros";
    }
    return;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro) {
    // polinomio CRC-32(IEEE 802)
    vector<int> gerador = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    vector<int> mensagem = mensagemErroQuadro(quadro);
    vector<int> crc;
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro) + TAMANHO_GERADOR;

    for (int i = 0; i < TAMANHO_GERADOR; i++) {
        mensagem.push_back(0);
        crc.push_back(mensagem[i]);
    }

    for (int i = TAMANHO_GERADOR; i < tamanho_mensagem; i++) {
        if (crc[0] != 0)
            crc = XOR(crc, gerador);
        crc = adicionarNoFinalDoResto(crc, mensagem[i]);
    }

    // é preciso inserir o crc no quadro ainda dentro da função e não retornar o crc

    crc.pop_front();

    return crc;
}

bool CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro) {
    // polinomio CRC-32(IEEE 802)
    vector<int> gerador = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    vector<int> mensagem = mensagemErroQuadro(quadro);
    vector<int> crc = erroQuadro(mensagem);
    vector<int> resto; 
    int tamanho_mensagem = tamanhoMensagemQuadro(quadro) + TAMANHO_GERADOR;
    bool erro = false;

    for (int i = 0; i < TAMANHO_GERADOR; i++)
        resto.push_back(mensagem[i]);

    for (int i = TAMANHO_GERADOR; i < tamanho_mensagem; i++) {
        if (resto[0] != 0)
            resto = XOR(resto, gerador);
        resto = adicionarNoFinalDoResto(resto, mensagem[i]);
    }

    for (int i = 0; i < TAMANHO_GERADOR; i++) {
        if (resto[i] != 0) {
            erro = true;
            break;
        }
    }

    return erro;
}

vector<int> XOR(vector<int> resto, vector<int> gerador) {
    for (int i = 0; i < TAMANHO_GERADOR; i++)
        if (resto[i] == gerador[i])
            resto[i] = 0;
        else 
            resto[i] = 1;
    return resto;
}

vector<int> adicionarNoFinalDoResto(vector<int> resto, int valor) {
    for (int i = 0; i < TAMANHO_GERADOR-1; i++)
        resto[i] = resto[i+1];
    resto[TAMANHO_GERADOR-1] = valor;
    return resto;
}

/*!< *********************************** MANIPULAÇÃO DO QUADRO *********************************** */

/*!< Para exemplificação das funções abaixo, considere o seguinte quadro com TAMANHO + MENSAGEM + ERRO 
*     00000110        01001000 01100101 01101100 01101100 01101111      00000000 00000000 00000000 00000001
*    Tamanho = 6       H       e        l         l        o                           Erro
*/

/*!< Retorna a quantidade de bits que a mensagem possui */
/*!< Com base no exemplo, retorna o valor 6*8 = 48 */
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
        cout << errInteiroQuadro << endl;
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
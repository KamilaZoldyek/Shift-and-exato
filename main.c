/** BUSCA POR CASAMENTO DE PADRAO - SHIFT-AND EXATO
 *
 *  Camila Cristina de Araujo - 10.2.8093
 *  CEA429 - 22
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//CONSTANTES
#define MAX_PATTERN_SIZE 100
#define MAX_TEXT_SIZE 1000


//NOMES DOS ARQUIVOS
#define TEXTO "text.txt"
#define PATTERN "pattern.txt"
#define RESULT "result.txt" //resultado do shift-and
#define REPLACE "replacepattern.txt" //palavra para trocar no texto
#define REPLACEDTEXT "replacedtext.txt" //texto depois de trocar o padrão buscado

char text[MAX_TEXT_SIZE+1];
char pattern[MAX_PATTERN_SIZE+1];
char replacepattern[MAX_PATTERN_SIZE+1];
int result[MAX_TEXT_SIZE+1];

//Declaração das funções
void shift_and(char *text, int text_size, char *pattern, int pattern_size);
void replace(char *text, char *replacepattern, int *result, int pattern_size);
void clearfile();
void clearfile2();
void abreTexto(FILE *arquivo, char *array);
void gravaArquivo(char c);
void abreInt(FILE *arquivo, int *array);
void gravaTexto(char c);

int main(){
    FILE *texto, *padrao, *padraoTroca;
    texto = fopen(TEXTO, "r");
    padrao = fopen(PATTERN, "r");
    padraoTroca = fopen(REPLACE, "r");


    while(1){

        int i;
        printf("\n\nOpcoes:\n1-Busca de padrao - Shift-and\n2-Substituicao de padrao\n3-sair\n");
        scanf("%d", &i);


        switch (i){
        case 1:
            abreTexto(texto, text);
            abreTexto(padrao, pattern);
            int text_size = strlen(text);
            int pattern_size = strlen(pattern);
            shift_and(text, text_size, pattern, pattern_size);

            break;

        case 2:


            abreTexto(texto, text);
            FILE *resultado;
            resultado = fopen(RESULT, "r");
            abreTexto(padrao, pattern);
            abreTexto(padraoTroca, replacepattern);
            abreInt(resultado, result);

            text_size = strlen(text);
            pattern_size = strlen(pattern);
            replace(text, replacepattern, result, pattern_size);

            break;

        case 3:
            exit(0);

        default:

            printf("insira uma posicao valida");
            break;
            }
        }

    return 0;
}

void clearfile(){
        FILE *resultado;
        resultado = fopen(RESULT, "w+");
        fclose(resultado);
}

void clearfile2(){
        FILE *resultado;
        resultado = fopen(REPLACEDTEXT, "w+");
        fclose(resultado);
}

void abreTexto(FILE *arquivo, char *array){
	int i=0;
	char ch;
	if (arquivo==NULL){
	    printf("Nao foi possivel ler o arquivo\n");
	    exit(1);
	}else{
        while(fscanf(arquivo, "%c", &ch)>0){
            array[i] = ch;
            i++;
        }
	}


	fclose(arquivo);
}

void abreInt(FILE *arquivo, int *array){
	int i=0;
	int ch;
	if (arquivo==NULL){
	    printf("Nao foi possivel ler o arquivo\n");
	    exit(1);
	}else{
        while(fscanf(arquivo, "%i", &ch)>0){
            array[i] = ch;
            i++;
        }
	}
	fclose(arquivo);
}

void gravaArquivo(char c){
    FILE *texto;
    texto = fopen(RESULT, "a+");
    fprintf(texto, "%i\n", c);
	fclose(texto);
}

void gravaTexto(char c){
    FILE *texto;
    texto = fopen(REPLACEDTEXT, "a+");
    fprintf(texto, "%c", c);
	fclose(texto);
}

void shift_and(char *text, int text_size, char *pattern, int pattern_size){
    int temp = 0;
    int pos;
    char posicoes[MAX_TEXT_SIZE] = { 0 };
    clearfile();

    printf("\n=====\nPadrao a ser buscado: ");
    for (int a=0; a<pattern_size; a++)
        printf("%c", pattern[a]);

    //máscara de bits:
    long alfabeto['z'-'a'+1] = { 0 }; //começa a mascara de bits com todo o alfabeto em 0
    for (int i = 0; i<pattern_size; i++){
        alfabeto[pattern[i]-'a'] |= 1 << i; //aqui faz cada letra do pattern ter sua máscara de bits
                                            //recebe 0 ou 1 de acordo com sua posicao na array da pattern
                                            //0 se a letra não tá naquela posição do array
                                            //1 se tá naquela posicao do array

    }

    long d = text[0] == pattern[0];     //d é a variavel de decisão.
                                        //a primeira letra do texto é igual a primeira letra da pattern? se sim, recebe 1.
                                        //essa é tipo a primeira iteração.
                                        //se eu assumir que d é 0, assume que na primeira posicao do texto
                                        //não tem o que a gente procura, o que é uma afirmação equivocada.

    for (int i = 1; i < text_size; i++){
        d = (d << 1 | 1);               //shift: desloca d em um bit e faz OR com 1

        d &= alfabeto[text[i]-'a'];     //and: compara d com a mascara daquela letra no alfabeto

        if (d & 1 << (pattern_size-1)){ //achou a pattern?
                                        //Se d & última letra do pattern(que no caso é a primeira
                                        //porque lê de trás pra frente) = 1, achou o pattern
            temp++;
            posicoes[i] = (i+(2-pattern_size)); //posições[i] deve ser o número da coluna, não a posicao no vetor
        }
    }

    //if pro caso de só a primeira letra der match, ou a última der match:
    if (d & 1 << (pattern_size-1)){
        pos = text_size-pattern_size+1;
        posicoes[text_size-1] = pos;
        }

    gravaArquivo(temp);
    printf("\n=====\nNumero de ocorrencias: %d\n", temp);
    for (int i = 0; i<=text_size; i++){
            if(posicoes[i]>0 && posicoes[i]<text_size){
            gravaArquivo(posicoes[i]);
            printf("Coluna: %d\n", posicoes[i]);
            }
    }
    printf("\nGravacao bem-sucedida\n");
}

void replace(char *text, char *replacepattern, int *result, int pattern_size){

    printf("\n=====\nPalavra para substituir o padrao buscado: ");
    for (int a=0; a<pattern_size; a++)
        printf("%c", replacepattern[a]);

    printf("\n=====\nTexto antes da substituicao:\n");
    for(int l=0; l<=strlen(text); l++){
        printf("%c", text[l]);
    }


    int ocorrencia = result[0]; //a primeira posição do vetor é o numero de ocorrencias
                                //que é também o numero de iteracoes para troca que vai fazer



    for(int j=1; j<ocorrencia+1; j++){ //pega a posicao onde tá a primeira ocorrencia do padrao no texto
                                        //(isso tá em result[1], por isso o for começa em 1

        for(int k=-1; k<=pattern_size; k++){    //substitui letra do texto pela letra do padrao
                                                //repete até o padrao acabar
                                                //k começa em -1 porque result[] marca o numero da coluna
                                                //que é diferente do numero da posicao no vetor

            if(pattern[k+1]!=0){                //if pra não pegar lixo de memoria
                text[result[j]+k] = replacepattern[k+1]; //troca
                                                        //o algoritmo vai truncar replacepattern para o tamanho de pattern
                                                        //porque só substitui, não insere
            }
        }
    }

    clearfile2();
    for(int i=0; i<=strlen(text); i++){
        gravaTexto(text[i]);
    }

    printf("\n=====\nTexto depois da substituicao:\n");
    for(int m=0; m<=strlen(text); m++){
        printf("%c", text[m]);
    }
    printf("\n=====\nGravacao bem-sucedida\n");
}

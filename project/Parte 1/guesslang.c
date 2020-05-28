#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*O programa le um arquivo de texto dado por argv e identifica o idioma em
que ele esta escrito baseado em textos de alguns idiomas. Abra no terminal
digitando o nome do executavel e depois o nome do arquivo de texto a ser 
analisado com a extensao*/

//calcula e armazena as frequencias de cada letra da stream
void letterFreq(FILE *stream, double lFreq[26]){
    int i, total=0;
    char letter;
    rewind(stream);

    for(i=0; i<26; i++){
            lFreq[i]=0;
        }
    //identifica as letras e conta elas e o numero total de letras
    while ((letter = fgetc(stream)) != EOF){
        for(i=0; i<26; i++){
            if (letter==65+i){
                (lFreq[i])++;
                total++;
                break;
            }
            if (letter==97+i){
                (lFreq[i])++;
                total++;
                break;
            }
        }
    }
    //calcula a frequencia com a qual cada letra aparece e armazena em um vetor
    for(i=0; i<26; i++)
        lFreq[i]=lFreq[i]/total;
}

//calcula a distancia euclidiana
float Dist(double fText[26], double freq[26]){
    float soma=0;
    int i;

    for(i=0; i<26; i++)
        soma += pow(fText[i] - freq[i], 2);

    return sqrt(soma);
}


int main(int argc, char **argv){
    FILE *fp, *conf, *ref;
    char language[20], txt[20], path[40], pLanguage[20];
    double freq[26], fText[26], dist, pDist=1;

    printf("%d\b",argc);

    fp = fopen(argv[1], "r");

    if (fp == NULL){
          printf ("O arquivo nao foi aberto\n");
          exit (1);
        }

    letterFreq(fp, fText);

    fclose(fp);

    conf = fopen("guesslang/guesslang.conf", "r");

//le o arquivo conf e recebe o idioma e o arquivo m que esta o texto para comparacao
    while (fscanf(conf, "%s : %s", language, txt) != EOF){

        strcpy(path, "guesslang/");
        strcat(path, txt);

        ref = fopen(path, "r");

        letterFreq(ref, freq);

        fclose(ref);

        dist = Dist(fText, freq);

        //armazena a menor distancia e o idioma que possui essa distancia
        if (dist<pDist){
            pDist=dist;
            strcpy(pLanguage, language);
        }
    }

    fclose(conf);

    if (pDist>0.06)
        printf("Nao foi possivel identificar o idioma");

    if (pDist<=0.06 && pDist>=0)
        printf("o texto esta em %s", pLanguage);

    return 0;
}

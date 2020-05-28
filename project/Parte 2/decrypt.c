#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*O programa identifica a menor distancia euclidiana entre o texto em argv e os textos
base e cria um arquivo texto descriptografado com a possivel chave identificada. Abra
no terminal digitando o nome do executavel e o nome do arquivo a ser analisado com a 
extensao*/

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

//calcula a distancia euclidiana com as frequencias alteradas com uma chave
float Dist(double fText[26], double freq[26], int key){
    double soma=0;
    int i, j;

    for(i=0; i<26; i++){
        j=i;
        j+=key;
        j=j%26;
        soma += pow(fText[j] - freq[i], 2);
    }

    return sqrt(soma);
}

//descriptografa o texto usando a chave encontrada
void decrypt(FILE *fr, FILE *fw, int pKey){
    int i;
    int letter;

    rewind(fr);
    rewind(fw);

    while ((letter = fgetc(fr)) != EOF){
        for(i=0; i<26; i++){
            if (letter=='a'+i){
                //tiramos a chave usada para criptografar o texto paa descriptografa-lo
                letter-=pKey;
                if(letter<97)
                    letter+=26;
                    break;
            }
            if (letter=='A'+i){
                letter-=pKey;
                if(letter<65)
                    letter+=26;
                    break;
            }
        }
        fputc(letter, fw);
    }
}

double compare(FILE *fr, FILE *ref, int *chave, double fText[26], double freq[26]){
    int i, a=0;
    double d, pD;

    letterFreq(ref, freq);
    letterFreq(fr, fText);

    for (i=0; i<26; i++){
        d = Dist(fText, freq, i);
        if(a==0){
            pD=d;
            *chave=i;
            a=1;
        }
        if (d<pD){
            pD=d;
            *chave=i;
        }
    }
    return pD;
}

int main(int argc, char **argv){
    FILE *fr, *fw, *conf, *ref;
    int i=0, *chave, pKey, c=0, b=0;
    char language[20], txt[20], path[40], pLanguage[40], file_name[50];
    double fText[26], freq[26], dist, pDist;
    
     printf("%d\b",argc);

    chave=&c;

    fr = fopen(argv[1], "r");

    if (fr == NULL){
        printf ("O arquivo nao foi aberto\n");
        exit (1);
    }

    conf = fopen("guesslang/guesslang.conf", "r");

    while (fscanf(conf, "%s : %s", language, txt) != EOF){

        strcpy(path, "guesslang/");
        strcat(path, txt);

        ref = fopen(path, "r");

        dist = compare(fr, ref, chave, fText, freq);

        if(b==0)
            pDist=dist;
            b=1;

        if (dist<pDist){
            pDist=dist;
            pKey=*chave;
            strcpy(pLanguage, language);
        }

        fclose(ref);
        }

    printf("A chave eh %d\n", pKey);
    printf("O idioma eh %s\n", pLanguage);
    //conta o numero de caracteres de argv
    while(argv[1][i] != 0){
        i++;
    }

    i-=4;
    //substitui a extensao do arquivo por .raw
    strcpy(file_name, argv[1]);
    strcpy((file_name+i), ".raw");
    fw = fopen(file_name, "w");

    decrypt(fr, fw, pKey);
	
	printf("O texto foi descriptografado");
    return 0;
}

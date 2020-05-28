#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*O programa recebe um arquivo de texto e o criptografa utilizando uma chave
da cifra de cesar dada por argv. Abra no terminal digitando o nome do 
executavel, o nome do arquivo a ser analisado com a extensao e a chave que sera usada
para criptografar*/

//substitui cada letra substituindo-a pela letra que esta um numero de casas do alfabeto a frente dado pelo valor da chave
void crypt(FILE *fr, FILE *fw, int chave){
    int i;
    int letter;

    //para cada letra adiciona a chave e se passar de z volta para o a
    while ((letter = fgetc(fr)) != EOF){
        for(i=0; i<26; i++){
            if (letter=='a'+i){
                letter+=chave;
                letter=letter%123;
                if(letter<26)
                    letter+=97;
                    break;
            }
            if (letter=='A'+i){
                letter+=chave;
                letter=letter%91;
                if(letter<26)
                    letter+=65;
                    break;
            }
        }
        fputc(letter, fw);
    }
}

int main(int argc, char **argv){
    FILE *fr, *fw;
    int chave, i=0;
    char file_name[50];
    
     printf("%d\b",argc);

    //o modulo do valor da chave por 26 equivale ao valor da chave
    chave=atoi(argv[2])%26;

    fr = fopen(argv[1], "r");

    if (fr == NULL){
          printf ("O arquivo nao foi aberto\n");
          exit (1);
        }

    //conta o numero de caracteres em argv
    while(argv[1][i] != 0){
        i++;
    }
    i-=4;

    //substitui a extensao por .csr e cria o arqivo para escrita
    strcpy(file_name,argv[1]);
    strcpy((file_name+i), ".csr");
    fw = fopen(file_name, "w");

    crypt(fr, fw, chave);

    fclose(fw);
    fclose(fr);
    
    printf("O texto foi criptografado utilizando a chave %s", argv[2]);

    return 0;
}

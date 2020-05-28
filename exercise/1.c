#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//this function returns the size in bytes of the text
int byte_count (FILE *fp){
    int len;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);

    return len;
}

//returns the size in kilobytes knowing the size in bytes
int size_in_kilobytes ( int size_in_bytes ){

    size_in_bytes = size_in_bytes/1024;

    return size_in_bytes;
}

//returns the number of lines of the text
int n_lines (FILE *fp, int n_bytes){
    int i=0, n_lines=0, j;
    char a[100];
    rewind(fp);

    while(i<n_bytes){
        fgets(a, 100, fp);
        j = 0;
        n_lines++;
        while(a[j]!=0){
            i++;
            j++;
        }
    }
    return n_lines;
}

//returns the number of letters of the txt
float letter_count (FILE *fp, int n_lines){
    int i, j, n_letters=0;
    char a[100];
    rewind(fp);

    for(i=0; i<n_lines; i++){
        fgets(a, 100, fp);
        j = 0;
        while (a[j]!=0){
            if (('a'<=a[j] && a[j]<='z') || ('A'<=a[j] && a[j]<='Z'))
                n_letters++;
            j++;
        }
    }
    return n_letters;
}

//returns the number of words in the txt
float word_count (FILE *fp, int n_lines){
    int i, j, n_words=0;
    char a[100];
    rewind(fp);

    for(i=0; i<n_lines; i++){
        fgets(a, 100, fp);
        j = 0;
        while (a[j]!=0){
            if ((a[j]!=' ') && ((a[j+1]==' ') || (a[j+1]=='\n')))
                n_words++;
            j++;
        }
    }
    //ver final do texto

    return n_words;

}

//returns the number of times a character speaks
int speech_count (FILE *fp, int n_lines, char *name){
    int i, n_speech;
    char a[100];
    rewind(fp);
    n_speech=0;

    for(i=0; i<n_lines; i++){
        fgets(a, 100, fp);
        if (strstr(a, name)!=NULL && a[15]==' ')
            n_speech++;
    }
    return n_speech;
}

//returns the most talkative of the four characters(Vader, Leia, Luke and C-3PO)
int most_speeches(int sc[4]){
    int maior, i;

    maior=sc[0];
    for(i=0; i<4; i++){
        if(sc[i]>maior)
            maior=sc[i];
    }
    return maior;
}

//returns the number of times each letter appears
void alpha_count (FILE *fp, int n_lines, int freq[26]){
    int i, j, k;
    char a[100];
    rewind(fp);

    for(i=0; i<26; i++)
        freq[i]=0;

    for(i=0; i<n_lines; i++){
        fgets(a, 100, fp);
        j = 0;
        while (a[j]!=0){
            for(k=0; k<26; k++){
                if(a[j]==(65+k) || a[j]==(97+k))
                freq[k]++;
            }
            j++;
        }
    }
}

//sorts the letters by frequency
int sort_by_frequency (int freq[26], char sorted[26]){
    int i, j, k, M, m, r, s;

    M=freq[0];
    for(i=0; i<26; i++){
        if (freq[i]>=M){
            M=freq[i];
            sorted[0]=(97+i);
        }
    }

    m=freq[0];
    for(i=0; i<26; i++){
        if (freq[i]<=m){
            m=freq[i];
            sorted[25]=(97+i);
            s=i;
        }
    }

    for(i=1; i<26; i++){
        for(j=0; j<26; j++){
            if (freq[j]<M && freq[j]>=m){
                sorted[i]=(97+j);
                m=freq[j];
                r=j;
            }
        }
        M=freq[r];
        m=s;
    }

    for(k=0; k<26; k++)
        printf("%c ",sorted[k]);
}

//calculates the average and deviation of word size
void statistics (FILE *fp, float *average, float *deviation, int lines){
    int i, j, k;
    float n_symbols, s;
    char a[100];
    n_symbols=0;
    rewind(fp);

    for(i=0; i<lines; i++){
        fgets(a, 100, fp);
        j = 0;
        while (a[j]!=0){
            if ((a[j]!=' ') && (a[j]!='\n'))
                n_symbols++;
            j++;
        }
    }

    *average = n_symbols/word_count(fp, lines);

    rewind(fp);

    for(i=0; i<lines; i++){
        fgets(a, 100, fp);
        j = 0;
        while (a[j]!=0){
            if ((a[j]==' ') && (a[j+1]!='\n' && a[j+1]!=' ')){
                n_symbols=0;
                k=0;
                while (a[j+k]!='\n' && a[j+k]!=' '){
                n_symbols++;
                k++;
                }
            s=s+pow(n_symbols-*(average), 2);
            }
            j++;
        }
    }
    *deviation=sqrt(s/(word_count(fp, lines)));
    }

//prints the 10 most repeated letters and its frequency
void show_top_10 (FILE *fp, int freq[26], char sorted[26], int n_lines){
    int i, j, M;
    float k;

    M=letter_count(fp, n_lines);
    for(i=0; i<10; i++){
        k=freq[0];
        for(j=0; j<26; j++){
            if (freq[j]>k && freq[j]<M){
                k=freq[j];
            }
        }
        M=k;
        printf("%c = %f\n", sorted[i], k/letter_count(fp, n_lines));
    }
}

//copy the txt in another file with all letters uppercased
void uppercase(FILE *fp, FILE *fpw){

    char copy;

    while (1) {
            copy = fgetc(fp);

            if (copy == EOF)
                break;

            if (copy >= 'a' && copy <= 'z'){
                copy = copy-32;
                putc(copy, fpw);
            }

            else
                putc(copy, fpw);
       }
    printf("\nuppercase.txt has been writed and saved");
}

int main(){
    FILE *fp, *fpw;
    int n_bytes, lines, n_letters, n_words, speeche[4], sc, freq[26];
    float average, deviation;
    char a, sorted[26];

    fp = fopen("input.txt", "r");

    //indicates that the file didn't open
    if (fp == NULL)
    {
      printf ("O arquivo nao foi aberto\n");
      exit (1);
    }

    n_bytes = byte_count (fp);
    n_bytes = byte_count (fp);
    printf ("This file is %d bytes long\n", n_bytes);

    printf ("Fize size is %d\n\n",  size_in_kilobytes (n_bytes));

    lines = n_lines(fp, n_bytes);

    n_letters = letter_count (fp, lines);
	printf ("The text has %d letters\n", n_letters);

    n_words = word_count (fp, lines);
	printf ("The text has %d words\n\n", n_words);

    speeche[0] = speech_count(fp, lines, "VADER");
    speeche[1] = speech_count(fp, lines, "LEIA");
    speeche[2] = speech_count(fp, lines, "LUKE");
    speeche[3] = speech_count(fp, lines, "THREEPIO");

	printf ("Darth Vader has %d speeches.\n", speeche[0]);
	printf ("Princess Leia has %d speeches.\n", speeche[1]);
	printf ("Luke Skywalker has %d speeches.\n", speeche[2]);
	printf ("C-3PO has %d speeches.\n\n", speeche[3]);

    sc = most_speeches(speeche);
    if (sc == speeche[0])
    printf ("The most talking character is Darth Vader\n");
    if (sc == speeche[1])
    printf ("The most talking character is Princess Leia\n");
    if (sc == speeche[2])
    printf ("The most talking character is Luke Skywalker\n");
    if (sc == speeche[3])
    printf ("The most talking character is C-3PO\n");

    printf("\n");

    alpha_count (fp, lines, freq);
    sort_by_frequency (freq, sorted);

    statistics (fp, &average, &deviation, lines);
    printf("\n\nThe average length of a word is %f, with standard deviation %f.\n\n", average, deviation);

    show_top_10 (fp, freq, sorted, lines);

    rewind(fp);

    fpw = fopen("uppercase.txt", "w");

    uppercase(fp, fpw);

    fclose(fp);
    fclose(fpw);

    return 0;
}

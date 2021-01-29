#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<dirent.h>

static char name[] = {"Aluno: Ian Mendes Badan"};
static char filesPath[] = {"./Files/"};
static char SortedPath[] = {"./Sorted/"};
static char separator = ',';

typedef struct Number{
    unsigned long int integer;
    unsigned long long int decimal;
    int qtdDecimal;
} number;

void quickSort(number array[],long int first,long int last);

number* createArray(unsigned long int *lenght, char *path);

void createSortedFile(number *array, unsigned long int lenght, int timeSec,unsigned long int timeNano, char *path);


int main(){
    setlocale(LC_ALL, "portuguese-brazilian");
    DIR *filesFolder;
    struct dirent *entry;
    filesFolder = opendir(filesPath);
    if(filesPath == NULL){
        perror("Unable to read directory");
    }
    entry = readdir(filesFolder);// to exclude . path
    entry = readdir(filesFolder);// to exclude .. path

    while(( entry = readdir(filesFolder))){
        char path[200];
        unsigned long int lenght = 0, timeSpentNano;
        int timeSpentSec;
        number *array;
        //Getting the numbers in the file
        printf("Creating array\n");
        sprintf(path, "%s%s", filesPath, entry->d_name);
        array = createArray(&lenght, path);
        printf("Array created\n");
        printf("Array lenght = %lu\n", lenght);

        //Sorting number and measuring time
        printf("Starting sorting\n");
        struct timespec timeBegin, timeEnd;
        timeSpentSec = 0;
        timeSpentNano = 0;
        clock_gettime(CLOCK_MONOTONIC, &timeBegin);
        quickSort(array, 0, lenght-1);
        clock_gettime(CLOCK_MONOTONIC, &timeEnd);
        timeSpentSec = (timeEnd.tv_sec - timeBegin.tv_sec);
        timeSpentNano = (timeEnd.tv_nsec - timeBegin.tv_nsec);
        printf("Numbers Sorted\n");

        //Creating file with sorted files
        printf("Creating file of sorted number\n");
        sprintf(path, "%s%s", SortedPath, entry->d_name);
        createSortedFile(array, lenght, timeSpentSec, timeSpentNano, path);
        printf("File Created\nEverything done\n\n");
    }
    closedir(filesFolder);
    free(filesFolder);
    free(entry);
    filesFolder = NULL;
    entry = NULL;
    return 0;
}

number* createArray(unsigned long int *lenght, char *path){
    number *array;
    FILE *file;
    if((file = fopen(path, "r")) == NULL){
        printf("Problem at opening file\n");
    } else{
        char numInt[10], numDec[20], numLenght[10], c;
        int contInt = 0, contDec = 0, contArray, hasFoundComma;

        while((c = fgetc(file)) != '\n'){
            //valores na tabela ASCII dos numeros entre 0 e 9
            if(c >= 48 && c <= 57){
                numLenght[contInt] = c;
                contInt++;
            }
        }
        *lenght = atoi(numLenght);
        array = malloc(sizeof(number)*(*lenght));
        contInt = contArray = hasFoundComma = 0;

        while(1){
            c = fgetc(file);
            if(c == ';' || c == EOF){
                 char *stop;
                numInt[contInt] = '\0';
                numDec[contDec] = '\0';
                
                array[contArray].integer = strtoul(numInt, &stop, 10);
                array[contArray].decimal = strtoull(numDec, &stop, 10);
                array[contArray].qtdDecimal = strlen(numDec);

                numInt[0] = '\0'; //simple way to clean up a string
                numDec[0] = '\0';
                contArray++;
                contInt = contDec = hasFoundComma = 0;

                if(c == EOF){
                    break;
                }
            } else{
                if(c == ','){
                    hasFoundComma = 1;
                    continue;
                }
                if(hasFoundComma == 0){
                    numInt[contInt] = c;
                    contInt++;

                } else if(hasFoundComma == 1){
                    numDec[contDec] = c;
                    contDec++;

                }
            }
        }

    }

    fclose(file);
    free(file);
    file = NULL;
    return array;
}

void createSortedFile(number *array, unsigned long int lenght, int timeSec,unsigned long int timeNano, char * path){
    int i, cont, flag=0, timeLenght = 0;
    FILE *fileSorted;
    char time[10];

    //spliting time nano to separated variables
    int nano=0, micro=0, milli=0;
    
    if(timeNano >= 1000000000){
        nano = (timeNano%10000);
        nano = nano/10;
        micro = ((timeNano/10000)%1000);
        milli = ((timeNano/10000)/1000);
        
    } else{
        nano = (timeNano%1000);
        micro = ((timeNano/1000)%1000);
        milli = ((timeNano/1000)/1000);
    }
    fileSorted = fopen(path, "wt");
    fprintf(fileSorted, "%s\n", name);
    fprintf(fileSorted, "Time to Sort:");
    if(timeSec > 0){
        fprintf(fileSorted, " %ld seconds",timeSec);
    }
    if(milli > 0){
        fprintf(fileSorted, " %d milliseconds",milli);
    }
    if(micro > 0){
        fprintf(fileSorted, " %d microseconds",micro);
    }
    if(nano > 0){
        fprintf(fileSorted, " %d nanoseconds", nano);
    }
    fprintf(fileSorted, "!\n");
    for(i = 0; i < lenght; i++){
            char temp[30];
            sprintf(temp, "%lu%c%llu;", array[i].integer, separator, array[i].decimal);
        if(fprintf(fileSorted, temp) == EOF) printf("Error while writing data");
    }

    fclose(fileSorted);
    free(fileSorted);
    fileSorted = NULL;
}

void quickSort(number array[],long int first,long int last){
    int i, j, pivot;
    number temp;
    if(first<last){
        pivot=first;
        i=first;
        j=last;
        while(i<j){
            while((array[i].integer<=array[pivot].integer || (array[i].integer==array[pivot].integer && ((array[i].decimal/(pow(10, (array[i].qtdDecimal - 7))))<=(array[pivot].decimal/(pow(10, (array[pivot].qtdDecimal - 7))))))) && i<last)
            i++;
            while(array[j].integer>array[pivot].integer || (array[j].integer==array[pivot].integer && (array[j].decimal/(pow(10, (array[j].qtdDecimal -7))))>(array[pivot].decimal/(pow(10, (array[pivot].qtdDecimal - 7))))))
            j--;
            if(i<j){
                temp=array[i];
                array[i]=array[j];
                array[j]=temp;
            }
        }
        temp=array[pivot];
        array[pivot]=array[j];
        array[j]=temp;
        quickSort(array,first,j-1);
        quickSort(array,j+1,last);
    }
}

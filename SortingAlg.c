#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<math.h>
#include<time.h>

static char name[] = {"Aluno: Ian Mendes Badan"};

static char path[] = {"./Files/file_100000_numbers.txt"};
//static char path[] = {"file_100_numbers_example.txt"};

static int adjustNumber = 10000;

typedef struct Number{
    double number;
    int decimal_qtd;
} number;

void quickSort(number array[],int first,int last);

number* createArray(int *lenght);

void createSortedFile(number *array, int lenght, long int timeSec, long int timeNano);

int main(){
    setlocale(LC_ALL, "portuguese-brazilian");
    //Primeiro passo encontrar todos os arquivos na pasta
    int lenght = 0;
    number *array;
    printf("Creating array\n");
    array = createArray(&lenght);
    printf("Array created\n");
    printf("Array lenght = %d\n", lenght);
      // Function Call 
    printf("Starting sorting\n");

    struct timespec timeBegin, timeEnd;
    long int timeSpentSec=0, timeSpentNano;
    clock_gettime(CLOCK_MONOTONIC, &timeBegin);
    quickSort(array, 0, lenght-1);
    clock_gettime(CLOCK_MONOTONIC, &timeEnd);
    timeSpentSec = (timeEnd.tv_sec - timeBegin.tv_sec);
    timeSpentNano= (timeEnd.tv_nsec - timeBegin.tv_nsec);
    printf("Numbers Sorted, time spent %ld %ld\n", timeSpentSec, timeSpentNano);

    //for(int i = 0; i < lenght; i++){
       // printf("Value: %.16Lf\n", array[i].number);
    //}
    printf("Creating file\n");
    createSortedFile(array, lenght, timeSpentSec, timeSpentNano);
    printf("Everything done\n");
    return 0; 

}

number* createArray(int *lenght){
    number *array;
    FILE *file;

    if((file = fopen(path, "r")) == NULL){
        printf("Problem at opening file\n");
    } else{
        char num[30], c;
        int contNum = 0, contArray, contDecimal, hasFoundComma;

        while((c = fgetc(file)) != '\n'){
            //valores na tabela ASCII dos numeros entre 0 e 9
            if(c >= 48 && c <= 57){
                num[contNum] = c;
                contNum++;
            }
        }

        *lenght = atoi(num);
        num[0] = '\0';
        array = malloc(sizeof(number)*(*lenght));
        contNum = contArray = hasFoundComma = contDecimal = 0;

        while(1){
            c = fgetc(file);
            //valores na tabela ASCII dos numeros entre 0 e 9
            if(c == ';' || c == EOF){
                char *end;
                array[contArray].decimal_qtd = contDecimal;

                array[contArray].number = strtod (num, end); //converting a string to double
                
                num[0] = '\0'; //simple way to clean up a string
                contArray++;
                contNum = contDecimal = hasFoundComma = 0;

                if(c == EOF){
                    break;
                }
            } else{
                if(hasFoundComma == 1) contDecimal++;
                if(c == ',') hasFoundComma = 1;
                num[contNum] = c;
                contNum++;
            }
        }
        
    }

    fclose(file);
    free(file);
    file = NULL;

    return array;
}

void createSortedFile(number *array, int lenght, long int timeSec, long int timeNano){
    int i;
    FILE *fileSorted;

    fileSorted = fopen("./Sorted/ArqGrav02.txt", "wt");
    fprintf(fileSorted, "%s\n", name);
    for(i = 0; i < lenght; i++){
            //printf(" Numero em double: %.16lf casa decimais: %d ", array[contArray].number, array[contArray].decimal_qtd);
            char temp[30], temp2[10];
            sprintf(temp2, "%%.%dLf;", array[i].decimal_qtd);
            sprintf(temp, temp2, (array[i].number));
        if(fprintf(fileSorted,temp2, temp) == EOF) printf("Error while writing data");
    }

    fclose(fileSorted);
    free(fileSorted);
    fileSorted = NULL;
}

void quickSort(number array[],int first,int last){
    int i, j, pivot;
    number temp;
    if(first<last){
        pivot=first;
        i=first;
        j=last;
        while(i<j){
            while(array[i].number<=array[pivot].number && i<last)
            i++;
            while(array[j].number>array[pivot].number)
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

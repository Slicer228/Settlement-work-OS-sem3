#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR() do{puts("ERROR IN MATRIX");exit(1);}while(0)
#define LERROR() do{puts("ERROR IN LAUNCH");exit(1);}while(0)

typedef struct{
	int rc;
	int rl;
	double buf[256][256];
}matrix;

void parseMatrix(matrix *m, char *filepath){
	char buffer[1024];
	int rc = 0, rl = 0;
	double num;
	char *er;
	char *tempEr;
	FILE *file = fopen(filepath,"r");
	if(file == NULL){
		puts("ERROR WHEN READING FILE");
		exit(1);
	}
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		tempEr = buffer;
		while(1){
			num = strtod(tempEr,&er);
			if(tempEr == er) break;
			m->buf[rc][rl] = num;
			rl++;
			tempEr = ++er;
		}
		rc++;
		if(m->rl == 0){
			m->rl = rl;
		}else if(m->rl != rl){fclose(file);ERROR();}
		rl = 0;
    }
	fclose(file);
	if(rc != m->rl){
		ERROR();
	}else m->rc = rc;
	if(m->rc == 0 || m->rl == 0) ERROR();
	
}
 

void getCofactor(double *mat, double *temp, int p, int q, int n, int N){
    int i = 0, j = 0;
 
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++) 
        {
            // Copying into temporary matrix 
            // only those element which are 
            // not in given row and column
            if (row != p && col != q) 
            {
                *((temp+i*N)+j) = *((mat+row*N)+col);
				j++;
 
                // Row is filled, so increase row 
                // index and reset col index
                if (j == n - 1) 
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

double determinantOfMatrix(double *mat, int n, int N){
    // Initialize result
    double D = 0; 
 
    //  Base case : if matrix contains 
    // single element
    if (n == 1)
        return *mat;
 
    // To store cofactors
    double temp[N][N]; 
 
    // To store sign multiplier
    int sign = 1; 
 
    // Iterate for each element of 
    // first row
    for (int f = 0; f < n; f++) 
    {
        // Getting Cofactor of mat[0][f]
        getCofactor((double *)mat, (double *)temp, 0, f, n, N);
        D += sign * (*(mat+f)) * determinantOfMatrix((double *)temp, n - 1, N);
 
        // Terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}
 
void printMatrix(double *arr,int n){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%g ",*((arr + i*n) + j));
		}
		puts("");
	}
}

void getMatrixFromBuf(matrix* m, double *dest){
	
	int i,j;
	for(i = 0;i < m->rc;i++){
		for(j = 0; j < m->rl;j++){
			*((dest + i*(m->rc))+j) = m->buf[i][j];
		}
	}
	
}

void printBorder(int rl){
	int i;
	for(i = 0;i < rl * 4;i++) printf("-");
	puts("");
}

void print_help() {
	
    FILE *help_file = fopen("readme.txt", "r");
    if (help_file == NULL) {
        puts("Could not open readme.txt\n");
        return;
    }
	puts("HELP BLOCK ->");
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), help_file) != NULL) {
        printf("%s", buffer);
    }
	puts("\n<-");
    fclose(help_file);
}

int main(int argc, char *argv[])
{

	char *path = NULL;
	if(argc < 2 || argc > 3) LERROR();
	argv++;
	if(argc == 2){
		if(!strcmp(*argv,"-help")){
			print_help();
			return 0;
		}else{
			path = *argv;
		}
	}else{
		if(strcmp(*argv,"-help")){
			path = *argv;
			if(strcmp(*(argv+1),"-help")) LERROR();
			print_help();
		}else{
			path = *(argv+1);
			print_help();
		}
	}
	
	if(path != NULL){
		matrix m;//определяем объект для хранения вводных
		parseMatrix(&m,path);//спарсим входной файл
		double mat[m.rc][m.rc];//создадим массив с матрицей подходящих размеров
		getMatrixFromBuf(&m,(double *)mat);//перекинем данные в новыйы массив
		printBorder(m.rl);//для красоты
		printMatrix((double *)mat,m.rc);//выводим матрицу
		printBorder(m.rl);//для красоты
		printf("Determinant of the matrix is : %g\n",determinantOfMatrix((double *)mat, m.rc, m.rc));//получаем определитель
		return 0;
	}else LERROR();
}
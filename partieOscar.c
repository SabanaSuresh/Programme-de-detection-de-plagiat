#include <stdio.h>

#include <stdlib.h>

#define A 0 //0 for the default method, 1 for the Kuhn-Munkres method


/* *********************************** */
/* ******* FONCTIONS PROTOTYPES ****** */
/* *********************************** */
typedef struct{
    int a;
    int b;
}Couple;


Couple* couple_init(int a, int b);

void display_couple(Couple *C);

void copy(double **matrix, double **matrix_copy, int n, int m);

void display(double **matrix, int n, int m);  //à retirer

void replace_row_column(double **matrix, int n, int m, Couple *C);

void display_couple(Couple *C); //à retirer

void generate_PGM_file(char *file_name, double **matrix, int n, int m);

int in_array(int i, int j, Couple **couple_array, int couple_counter);

Couple *find_min(double **matrix, int n, int m, Couple **couple_array, int couple_counter);

Couple **minimal_coupling(double **matrix, int n, int m, int *couple_counter);

void post_filtering(double **matrix, double **filtrated_matrix,int n, int m);

double convolution(double **matrix, int n, int m, int i, int j);

double compute_distance(double **matrix, int n, int m);

int following_steps(double **matrix, int n, int m);

void bonus_display(double **matrix, int n, int m);



//part on the Kuhn-Munkres's algorithm


void display_double_matrix(double **matrix, int n, int m);

void step_0(double **matrix, int n);

int count_zeros(double **matrix, int **crossed_zeros, int **labeled_zeros, int i, int n);

int count_zeros_matrix(double **matrix, int **crossed_zeros, int **labeled_zeros, int n);

void step_1(double **matrix, int **crossed_zeros, int **labeled_zeros, int n);

int count_labeled_zeros(int **labeled_zeros, int i, int n);

void step_2(double **matrix, int **crossed_zeros, int **labeled_zeros, int *marked_rows, int *marked_columns, int *covered_rows, int *covered_columns, int n);

void step_3(double **matrix, int *covered_rows, int *covered_columns, int n);

int count_labeled_zeros_matrix(int **matrix, int n);

void fill_with_zeros_matrix(int **matrix, int n);

void fill_with_zeros_array(int *matrix, int n);

void completion(double **matrix, int n, int m, int k);

int algorithme_hongrois(double **original_matrix, int n, int m);


/* *********************************** */
/* *********************************** */


/* ************************ */
/* *** STRUCTURE COUPLE *** */
/* ************************ */
Couple* couple_init(int a, int b){
    Couple *C = malloc(sizeof(Couple));
    C->a = a;
    C->b = b;

    return C;
}


void display_couple(Couple *C){
    printf("%d %d\n", C->a, C->b);
}

/* ************************ */
/* ************************ */



/* ************************ */
/* ********** 4 *********** */
/* ************************ */

/* ************************ */
/* ******* FONCTIONS ****** */
/* ************************ */


void copy(double **matrix, double **matrix_copy, int n, int m){

    for (int i = 0; i < n; i++){

        for (int j = 0; j < m; j++)

            matrix_copy[i][j] = matrix[i][j];
    }
}


int in_array(int i, int j, Couple **couple_array, int couple_counter){

    Couple *C;

    for (int k = 0; k < couple_counter; k++){

        C = couple_array[k];
        if (C->a == i && C->b == j)
            return 1;
    }

    return 0;
}

Couple *find_min(double **matrix, int n, int m, Couple **couple_array, int couple_counter){ //(n, m) dimension de la matrix

    if (n == 0 && m == 0) return NULL;
    int i, j;

    //we initialise a value out of the range covered by the matrix
    //it is licit because of the initialisation of the matrix the other .c file
    Couple *loc_min = couple_init(n, m);
    matrix[n][m] = 2.0; //arbitrary high value such that the entire matrix is strictly smaller than it

    for (i = 0; i < n; i++){

        for (j = 0; j < m; j++){

            //we verify if the current couple has a smaller value than the current local minimum
            //if it's the case, we also verify if this couple is already in the couple list
            if ( matrix[i][j] < matrix[loc_min->a][loc_min->b] && !in_array(i, j, couple_array, couple_counter) ){
                loc_min->a = i;
                loc_min->b = j;
            }
        }
    }

    return loc_min;
}

void display(double **matrix, int n, int m){

    for (int i = 0; i < n; i++){

        for (int j = 0; j < m; j++){
            printf("%4lf ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}




void replace_row_column(double **matrix, int n, int m, Couple *C){

    for (int j = 0; j < m; j++){
        if (j != C->b)
            matrix[C->a][j] = 1.0;
    }

    for (int i = 0; i < n; i++){
        if (i != C->a)
            matrix[i][C->b] = 1.0;
    }
}


Couple **minimal_coupling(double **matrix, int n, int m, int *couple_counter){
    int min = (n > m)? m : n;
    int max = (n > m)? n : m;
    Couple *C;
    Couple **couple_array = malloc(max*sizeof(Couple));

    while (*couple_counter < min){
        C = find_min(matrix, n, m, couple_array, *couple_counter);
        replace_row_column(matrix, n, m, C);
        couple_array[*couple_counter] = C;
        (*(couple_counter))++;
    }

    return couple_array;
}

void generate_PGM_file(char file_name[1000], double **matrix, int n, int m){
    FILE *file = fopen(file_name, "w");

    fprintf(file, "P2\n%d %d\n255\n", m, n);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            fprintf(file, "%d ", 255 - (int) (matrix[i][j]*255));
        }
    }

    fclose(file);
}
/* ************************ */
/* ************************ */



/* ************************ */
/* ********** 5 *********** */
/* ************************ */
/* ******* FONCTIONS ****** */
/* ************************ */
double convolution(double **matrix, int n, int m, int i, int j){
    double o = 0.0;
    int counter = 0;

    for (int k = -2; k <= 2; k++){

        if (i+k >= 0 && i+k < n && j+k >= 0 && j+k < m){

            o = o + matrix[i+k][j+k];
            counter++;
        }
    }
    o /= counter;
    o = (o >= 0.7) ? 1 : o;

    return o;
}

void post_filtering(double **matrix, double **filtrated_matrix,int n, int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            filtrated_matrix[i][j] = convolution(matrix, n, m, i, j);
        }
    }
}
/* ************************ */
/* ************************ */


/* ************************ */
/* ********** 6 *********** */
/* ************************ */

/* ************************ */
/* ******* FONCTIONS ****** */
/* ************************ */
double compute_distance(double **matrix, int n, int m){
    double o;

   for (int i = 0; i <  n; i++){
        for (int j = 0; j < m; j++){
            o += (1 - matrix[i][j]);
        }
    }
    o /= (n > m ? m : n);
    o = 1 - o;
    return o;
}

/* ************************ */
/* ************************ */

void bonus_display(double **matrix, int n, int m){
    int cpt = 0;
    printf("\nDisplaying plagiarised lines\nHere is the signification of the colors :\n\t- \033[0;31mRed \033[0;37m for the plagiarised lines (dice distance = 0)\n\t- \033[0;33mYellow \033[0;37m for the ones in (0, 0.5]\n\t- the others are not displayed\n");
    for (int i = 0; i < n; i++){

        for (int j = 0; j < m; j++){
		if (matrix[i][j] <= 0.5){
		    printf("\n +--------------+--------------+\n");
		    cpt++;

		    if (matrix[i][j] == 0){
		        printf(" \033[0;37m| \033[0;31msegment %4d \033[0;37m| \033[0;31msegment %4d \033[0;37m|", i, j);
		    }

		    else {
		        printf(" \033[0;37m| \033[0;33msegment %4d \033[0;37m| \033[0;33msegment %4d \033[0;37m|", i, j);
		    }
		}
        }
    }

    if (cpt > 0){
	printf("\n +--------------+--------------+\n\n");
    } 
    else {
        printf("No segments are close enough to be displayed\n");
    }
}


/* ************************ */

/* ********MUNKRES********* */

/* ************************ */


//cf http://optimisons.free.fr/Cours%20M%C3%A9thode%20Hongroise.pdf for the explenation of the method and the steps followed


/* ****** */
/* STEP 0 */
/* ****** */
void step_0(double **matrix, int n){
    int i = 0, j = 0;
    double min_loc;

    /* *** lignes *** */
    for (i = 0; i < n ; i++){
        j = 0;
        min_loc = matrix[i][j];

        for (j = 1; j < n; j++){

            min_loc = (matrix[i][j] < min_loc) ? matrix[i][j] : min_loc;
        }


        for (j = 0; j < n; j++){

            matrix[i][j] -= min_loc;
        }
    }

    /* *** columns *** */
    i = 0;

    for (j = 0; j < n ; j++){

        i = 0;
        min_loc = matrix[i][j];

        for (i = 0; i < n; i++){

            min_loc = (matrix[i][j] < min_loc) ? matrix[i][j] : min_loc;
        }


        for (i = 0; i < n; i++){

            matrix[i][j] -= min_loc;
        }
    }
}
/* ****** */




/* ****** */
/* STEP 1 */
/* ****** */
int count_zeros(double **matrix, int **crossed_zeros, int **labeled_zeros, int i, int n){
    int counter = 0;

    for (int j = 0; j < n; j++){
        counter += (matrix[i][j] == 0.0 && crossed_zeros[i][j] != 1 && labeled_zeros[i][j] != 1) ? 1 : 0;
    }

    //if counter = 0, this implies that no zero is on this line
    //so, we return an arbitrary high value to let the program know that the i-th row isn't the one with a minimal amount of zeros
    if (counter == 0) return n;

    //otherwise...
    return counter;
}



//counting the number of "free" zeros
int count_zeros_matrix(double **matrix, int **crossed_zeros, int **labeled_zeros, int n){
    int counter = 0;
    for (int i = 0; i < n; i++){

        for (int j = 0; j < n; j++){
            counter += (matrix[i][j] == 0.0 && crossed_zeros[i][j] != 1 && labeled_zeros[i][j] != 1) ? 1 : 0;
        }
    }
    return counter;
}




void step_1(double **matrix, int **crossed_zeros, int **labeled_zeros, int n){

    int row_labeled_zero = 0, counter_min_zeros, counter_current_zeros; //at this point we assume that the first row contains the minimal amount of zeros
    int i=0, j=0;

    while (count_zeros_matrix(matrix, crossed_zeros, labeled_zeros, n) > 0){ //we count the number of zeros which aren t crossed or labeled

        counter_min_zeros = n;

        for (i = 0; i < n; i++){

            counter_current_zeros = count_zeros(matrix, crossed_zeros, labeled_zeros, i, n); //number of zeros on the i-th row

            if (counter_current_zeros < counter_min_zeros){
                counter_min_zeros = counter_current_zeros;
                row_labeled_zero = i;
            }
        }

        /* the i-th row is such that i is minimal and the number of zeros on this line is also minimised */

        i = row_labeled_zero;

        for (j = 0; j < n; j++){

            if (matrix[i][j] == 0.0 && labeled_zeros[i][j] != 1 && crossed_zeros[i][j] != 1){

                labeled_zeros[i][j] = 1;
                break;
            }
        }


        /* we cross the non-labeled zeros which are on the same row or column */

        for (int k = 0; k < n; k++){

            if (matrix[i][k] == 0.0 && labeled_zeros[i][k] != 1){

                crossed_zeros[i][k] = 1;
            }
        }

        for (int k = 0; k < n; k++){

            if (matrix[k][j] == 0.0 && labeled_zeros[k][j] != 1){

                crossed_zeros[k][j] = 1;
            }
        }
    }
}
/* ******* */


/* ****** */
/* STEP 2 */
/* ****** */
int count_labeled_zeros(int **labeled_zeros, int i, int n){
    int counter = 0;
    for (int j = 0; j < n; j++)
        counter += labeled_zeros[i][j];

    return counter;
}






void step_2(double **matrix, int **crossed_zeros, int **labeled_zeros, int *marked_rows, int *marked_columns, int *covered_rows, int *covered_columns, int n){

    int counter = 1, i = 0, j = 0;

    /* step a */

    for (i = 0; i < n; i++){

        if (count_labeled_zeros(labeled_zeros, i, n) == 0)
            marked_rows[i] = 1;
    }



    /* step b */

    while(counter > 0){

        counter = 0;

        for (i = 0; i < n; i++){

            for (j = 0; j < n; j++){

                if (crossed_zeros[i][j] && marked_rows[i] && marked_columns[j]==0){

                    marked_columns[j] = 1;
                    counter++;

                }
            }
        }

        /* step c */
        for (i = 0; i < n; i++){

            for (j = 0; j < n; j++){

                if (labeled_zeros[i][j] && marked_columns[j] && marked_rows[i]==0){

                    marked_rows[i] = 1;
                    counter++;
                }
            }
        }
    }

    for (i = 0; i < n; i++){
        covered_rows[i] = 1-marked_rows[i];
    }

    for (j = 0; j < n; j++){
        covered_columns[j] = marked_columns[j];
    }
}
/* ******* */


/* ****** */
/* STEP 3 */
/* ****** */

void step_3(double **matrix, int *covered_rows, int *covered_columns, int n){

    int i, j;
    double min = 2; // initialising an arbitrary high value such that for all {i,j}, matrix[i][j] < min

    /* Searching for the minimum value of the partiel matrix */
    for (i = 0; i < n; i++){

        for (j = 0; j < n; j++){

            if (covered_rows[i] != 1 && covered_columns[j] != 1 && matrix[i][j] < min)

                min = matrix[i][j];
        }
    }

    /* We go through the partial matrix to substract the minimum */
    for (i = 0; i < n; i++){

        for (j = 0; j < n; j++){

            if (covered_rows[i] != 1 && covered_columns[j] != 1)

                matrix[i][j] -= min;
        }
    }

    /* we add the value to the cells on a position covered by two lines */
    /* matrix[i][j] such that covered_rows[i]=1 and covered_columns[j]=1*/
    for (i = 0; i < n; i++){

        for (j = 0; j < n; j++){

            if (covered_rows[i] == 1 && covered_columns[j] == 1)

                matrix[i][j] += min;

        }
    }
}


/* ******* */

int count_labeled_zeros_matrix(int **matrix, int n){
    int counter=0;

    for (int i = 0; i < n; i++){

        for (int j = 0; j < n; j++){

            counter += matrix[i][j];

        }
    }

    return counter;
}


void fill_with_zeros_matrix(int **matrix, int n){

    for (int i = 0; i < n; i++){

            for (int j = 0; j < n; j++){

                matrix[i][j] = 0;

            }
    }
}


void fill_with_zeros_array(int *matrix, int n){

    for (int i = 0; i < n; i++){

            matrix[i] = 0;

        }
}



void completion(double **matrix, int n, int m, int k){

    for (int i = 0; i < k; i++){

        for (int j = 0; j < k; j++){

            if (i >= n || j >= m)
                matrix[i][j] = 2; //we complete the matrix with an arbitrary high value

        }
    }
}







int Kuhn_algorithm(double **original_matrix, int n, int m){


    int max = (n > m) ? n : m;

    int counter = 0;

    int check_error = 0;

    double **matrix = (double **) malloc(n*sizeof(double *));
    for (int i = 0; i < m; i++) {
            matrix[i] = malloc(m*sizeof(double));
    }



    int **crossed_zeros = (int **) malloc(n*sizeof(int *));
    for (int i = 0; i < n; i++) {
            crossed_zeros[i] = calloc(m, sizeof(int));
    }



    int **labeled_zeros = (int **) malloc(n*sizeof(int *));
    for (int i = 0; i < n; i++) {
            labeled_zeros[i] = calloc(m, sizeof(int));
    }


    int *marked_rows = calloc(n, sizeof(int));     



    int *marked_columns = calloc(m, sizeof(int));   



    int *covered_rows = calloc(n, sizeof(int));    



    int *covered_columns = calloc(m, sizeof(int));  


    copy(original_matrix, matrix, max, max);
  
	

    step_0(matrix, max);



    while (count_labeled_zeros_matrix(labeled_zeros, max) < max && counter++ < 2000){

        fill_with_zeros_matrix(crossed_zeros, max);

        fill_with_zeros_matrix(labeled_zeros, max);

        fill_with_zeros_array(covered_rows, max);

        fill_with_zeros_array(covered_columns, max);

        fill_with_zeros_array(marked_rows, max);

        fill_with_zeros_array(marked_columns, max);


        step_1(matrix, crossed_zeros, labeled_zeros, max);

        step_2(matrix, crossed_zeros, labeled_zeros, marked_rows, marked_columns, covered_rows, covered_columns, max);

        step_3(matrix, covered_rows, covered_columns, max);



    }





    //replacing the values which aren t considered after the coupling process
    for (int i = 0; i < n; i++){



        for (int j = 0; j < m; j++){



            if (!(labeled_zeros[i][j] == 1)){



                original_matrix[i][j] = 1.0;

            }

        }



    }

    //Memory clearance
    for (int i = 0; i < max; i++) {
        free(matrix[i]);
        free(crossed_zeros[i]);
        free(labeled_zeros[i]);
    }
    free(matrix);
    free(crossed_zeros);
    free(labeled_zeros);
    free(covered_rows);
    free(marked_rows);
    free(covered_columns);
    free(marked_columns);

    check_error = (counter > 2000) ? 1 : 0;

    return check_error;
}






/* ********************************* */
/* **********MAIN FONCTION********** */
/* ********************************* */



int following_steps(double **matrix, int n, int m){

    double **filtrated_matrix = (double **) malloc(n*sizeof(double *));
    for (int i = 0; i < n; i++) {
            filtrated_matrix[i] = malloc( m*sizeof(double));
    }

    int max = ((n > m)? n : m);
    double distance;

    int check_error = 0;

    //storing the indexes of the coupled segments in an array
    Couple **couple_array;

    int *couple_counter = malloc(sizeof(int));
    *couple_counter = 0;

    printf("\nCoupling and filtering process..\n\n");

    switch(A){
    case 0:
        couple_array = minimal_coupling(matrix, n, m, couple_counter);
        break;

    case 1:
     	completion(matrix, n, m, max); //we complete the original matrix to make it square-shaped
        check_error = Kuhn_algorithm(matrix, max, max);
        break;
    }

    if (check_error == 1) {
	printf("\nThe Kuhn-Munkres algorithm generates an infinite loop... More than 2000 loop iterations. Check if the files have the same dimensions\n");	
    }

    else{

    	post_filtering(matrix, filtrated_matrix, n, m);

    	generate_PGM_file("coupling.pgm", matrix, n, m);

    	generate_PGM_file("post_filtering.pgm", filtrated_matrix, n, m);

    	printf("Your images coupling.pgm and post_filtering.pgm have been created!\n");

    	distance = compute_distance(filtrated_matrix, n , m);

    	bonus_display(filtrated_matrix, n, m);

    	printf("\n\nThe final distance\n%.2lf\n", distance);

    }

    printf("\n..Goodbye !\n");

    //...the memory is free !
    if (A == 0){
        for (int i = 0;i < *couple_counter; i++){
            free(couple_array[i]);
        }
        free(couple_array);
    }

    free(couple_counter);
    for (int i = 0; i < n; i++){
        free(filtrated_matrix[i]);
    }
    free(filtrated_matrix);


    return 1;
}

/* ************************ */
/* ************************ */






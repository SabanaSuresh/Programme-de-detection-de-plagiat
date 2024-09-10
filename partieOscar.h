#ifndef PARTIE_OSCAR_INCLUDED

#define PARTIE_OSCAR_INCLUDED


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
//we followed this article to implement the hungarian method http://optimisons.free.fr/Cours%20M%C3%A9thode%20Hongroise.pdf
//please refer to this to understand the functions below


void display_double_matrix(double **matrix, int n, int m);

void step_0(double **matrix, int n);

int count_zeros(double **matrix, int **crossed_zeros, int **labeled_zeros, int i, int n);

int count_zeros_matrix(double **matrix, int **crossed_zeros, int **labeled_zeros, int n);

void step_1(double **matrix, int **crossed_zeros, int **labeled_zeros, int n);

int count_labeled_zeros(int **labeled_zeros, int i, int n); // on the i-th line

void step_2(double **matrix, int **crossed_zeros, int **labeled_zeros, int *marked_rows, int *marked_columns, int *covered_rows, int *covered_columns, int n);

void step_3(double **matrix, int *covered_rows, int *covered_columns, int n);

int count_labeled_zeros_matrix(int **matrix, int n);

void fill_with_zeros_matrix(int **matrix, int n);

void fill_with_zeros_array(int *matrix, int n);

void copy(double **matrix, double **copied_matrix, int n, int m);

void completion(double **matrix, int n, int m, int k);

int Kuhn_algorithm(double **original_matrix, int n, int m);






#endif // PARTIE_OSCAR_INCLUDED


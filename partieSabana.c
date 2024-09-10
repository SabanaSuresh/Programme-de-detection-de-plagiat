/****************************************************************************/
/****************************************************************************/
/*******************                                      *******************/
/*******************   C PROJECT                          *******************/
/*******************                                      *******************/
/*******************   Authors : SURESH Sabana            *******************/
/*******************             PERIANAYAGASSAMY Oscar   *******************/
/*******************   TD 6                               *******************/
/*******************                                      *******************/
/****************************************************************************/
/****************************************************************************/


#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include <ctype.h>

#include "partieOscar.h"

#define A 0


////////// functions  ////////////
// Structure for linked list
typedef struct Line {
char *data;
struct Line *next;
} Line;

typedef struct LinkedList {
    Line*  head;
}LinkedList;

Line* init_Line(char* line);

LinkedList* init_linked_list();

int in(char *word, char **list_word);

int count(char *word, char **list, int i);

void digramme(char *segmant, char **digramme_list);

int digramme_verif(char **digramme_list1, char **digramme_list2);

int compute_common_digramme(char **digramme_list1, char **digramme_list2);

int liberate_digramme(char **list);

double distance_dice(char *word1, char *word2);

void start();

int Test(char *file_name);

void removecomments(char *File1);

void lettersTOw();

void removeW();

void removeEmptyLines(char *File1);

int segmant(char *File1,char *File2);


/* ************************ */
/* *********MAIN*********** */
/* ************************ */

int main(int argc, char **argv){

    start(argc, argv);

    return 0;
}


/* ************************ */
/* *******FONCTIONS******** */
/* ************************ */
void start(int argc, char **argv){

	if (argc < 3) return;

    char* file1 = argv[1];

	if (Test(file1) == 0)
		return;


	printf("The First File: %s\n", file1);

	printf("\tRemoving unwanted caracters..");
	removecomments(file1);
	printf("Done\n");

	printf("\tReplacing all words with 'w'..");
	lettersTOw();
    	removeW();
	printf("Done\n");

	printf("\tRemoving empty lines..");
	removeEmptyLines("File1FinalOutput.c");
	printf("Done\n");


	char* file2 = argv[2];

	if (Test(file2) == 0)
		return;

	printf("The Second File: \n");

	printf("\tRemoving unwanted caracters..");
	removecomments(file2);
	printf("Done\n");

	printf("\tReplacing all words with 'w'..");
	lettersTOw();
	removeW();
	printf("Done\n");

	printf("\tRemoving empty lines..");
	removeEmptyLines("File2FinalOutput.c");
	printf("Done\n");

	segmant("File1FinalOutput.c","File2FinalOutput.c");
}




int Test(char *file_name){
	//Testing if the file_name is licit
	FILE* tester = NULL;
	fflush(stdin);
	tester = fopen(file_name, "r");

	if (tester == NULL){
		return 0;

	}
	return 1;
}




void removecomments(char *File1){

	FILE* in = fopen(File1, "r");
	FILE* out = fopen("OutputAfterRemovingComments.c", "w");
	char c;

	while((c = fgetc(in)) != EOF){
    		if (c == '"')
		{
      			while(((c = fgetc(in)) != '"'));

      			fputc('"',out);
    		}
    
		if(c=='/'){

			int flag = 0;
			c = fgetc(in);

			if(c == '/'){
				flag = 1;
				while((c = fgetc(in)) != '\n');

			}else if(c == '*'){

				flag = 1;
				while((c = fgetc(in)) != EOF){

					if((c = fgetc(in)) == '*' && (c = fgetc(in)) == '/'){

						c='\n';
		  				break;
					}

				}

			}


      			if(flag == 0) fputs("/",out);

		}

		if (c != ' ' && c != '\t')
    		{
      			fputc(c,out);
    		}
  	}

	fclose(in);
	fclose(out);
}















void lettersTOw(){

	FILE* in = fopen("OutputAfterRemovingComments.c", "r");
	FILE* out = fopen("OutputAfterReplacingCaractersWithW.c", "w");

	char c;

	while((c = fgetc(in)) != EOF ){
		if(isalnum(c)){

			fputs("w",out);

		} else{

			fputc(c,out);

		}
	}

	fclose(in);
	fclose(out);

	remove("OutputAfterRemovingComments.c");

}




void removeW(){

	FILE* in = fopen("OutputAfterReplacingCaractersWithW.c", "r");
	FILE* out = fopen("OutputAfterRemovingWs.c", "w");

	char c;
	int compt=1;

	while((c = fgetc(in)) != EOF){

		if(c=='w'){

			if (compt == 1){

			        fputc(c,out);
			}

			compt = 2;

		}else if (c!='w'){

			fputc(c,out);

			compt = 1;
		}

	}

	fclose(in);
	fclose(out);

	remove("OutputAfterReplacingCaractersWithW.c");

}





int isEmpty(const char *str)  // to check if line is empty
{
	char ch;

	do
	{

		ch = *(str++);

        	// Check non whitespace characters
        	if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            		return 0;

	} while (ch != '\0');

	return 1;
}







void removeEmptyLines(char *File1)
{


	FILE* in = fopen("OutputAfterRemovingWs.c", "r");
  	FILE* out = fopen(File1, "w");
  	char line[1000];

  	while ((fgets(line, 1000, in)) != NULL)

  	{

      	// If current line is not empty then write to temporary file

      	if(!isEmpty(line))

          	fputs(line, out);
	}

	fclose(in);
	fclose(out);

	remove("OutputAfterRemovingWs.c");

}











int segmant(char *File1,char *File2){

	int *l1 = malloc(sizeof(int));

	int *l2 = malloc(sizeof(int));

	//splitting File1 in seg1[][]

	printf("\nSplitting first file into segments..\n");

	FILE* in = fopen(File1, "r");
	
	int LinesFile1 = 1;

	LinkedList *seg1 = init_linked_list();

  	char *line = malloc(1000);
  	Line* temp;

  	fgets(line, 1000, in);
  	seg1->head = init_Line(line);


  	temp = seg1->head;

	while ((fgets(line, 1000, in)) != NULL)
  	{


    	temp->next = init_Line(line);

    	temp = temp->next;

	LinesFile1 = LinesFile1+1;
  	}

  	temp->next = seg1->head;


	//splitting File2 in seg2[][]
	
	printf("Splitting second file into segments..\n");

  	FILE* in2 = fopen(File2, "r");

  	int LinesFile2 = 1;

	LinkedList *seg2 = init_linked_list();

  	fgets(line, 1000, in2);
  	seg2->head = init_Line(line);

  	temp = seg2->head;


	while ((fgets(line, 1000, in2)) != NULL)
  	{

    	temp->next = init_Line(line);

    	temp = temp->next;

    	LinesFile2 = LinesFile2+1;
  	}

    	temp->next = seg2->head;

	printf("Calculating Dice Distance & Storing data in image..\n");

	printf("\n");

	int j,k;


	//to compare seg1 and seg2,
	//we  can just compare the lengh of shortest one.


    	*l1 = LinesFile1; // needed for
    	*l2 = LinesFile2; // following_steps()

    	double **D;
    	int max_dim;

    	if (A == 0){
        	D = (double **) malloc(((*l1)+1)*sizeof(double *));
        	for (int i = 0; i < (*l1)+1; i++) {
                	D[i] = malloc( ((*l2)+1)*sizeof(double));
        	}
    	}

    	else{
        	max_dim = (*l1 > *l2) ? *l1 +1 : *l2 +1;
        	D = (double **) malloc(max_dim*sizeof(double *));
        	
		for (int i = 0; i < max_dim; i++) {
	                D[i] = malloc(max_dim*sizeof(double));
        	}
    	}



    	Line* temp1 = seg1->head;
    	Line* temp2 = seg2->head;


  	for (k = 0; k < *l1; k++)
	{
    		temp2 = seg2->head;
   		for (j = 0; j < *l2; j++){

			D[k][j] = distance_dice(temp1->data, temp2->data);
      			temp2 = temp2->next;
    		}
    		temp1 = temp1->next;
  	}

	generate_PGM_file("dice.pgm", D, *l1, *l2);

  	printf("Your image dice.pgm has been created! Thank you for using our program :) \n");

	fclose(in2);

	fclose(in);

	remove("File1FinalOutput.c");

	remove("File2FinalOutput.c");


	following_steps(D, *l1, *l2); //i.e. coupling and post filtrating



  	//...the memory wants to break free !
  	if (A==0){
    	for (int i = 0; i < (*l1)+1; i++) free(D[i]);
  	}

  	else{
    	for (int i = 0; i < max_dim; i++) free(D[i]);
  	}

  	free(D);



	temp1 = seg1->head->next;

  	while (temp1 != seg1->head){

    		temp2 = temp1;
    		temp1 = temp1->next;
    		free(temp2->data);
    		free(temp2);

 	}

  	free(seg1->head->data);
  	free(seg1->head);
  	free(seg1);



  	temp1 = seg2->head->next;

  	while (temp1 != seg2->head){

    		temp2 = temp1;
    		temp1 = temp1->next;
    		free(temp2->data);
    		free(temp2);

  	}

  	free(seg2->head->data);
  	free(seg2->head);
  	free(seg2);

  	free(line);

  	free(l1);
  	
	free(l2);

 	return 1;

}





LinkedList* init_linked_list(){

	LinkedList *linked_list = malloc(sizeof(LinkedList));
    linked_list->head = NULL;
    return linked_list;

}




Line* init_Line(char* line){

    	Line *new_line = malloc(sizeof(Line));
    	char *new_data = malloc(strlen(line)+1) ;
    	strcpy(new_data, line);
    	new_line->data = new_data;
    	new_line->next = NULL;
    	return new_line;

}




int in(char *word, char **list_word) {

  	int i = 0;

  	while (strcmp(list_word[i], "NULL")) {
    		
		if (strcmp(word, list_word[i]) == 0) {

      			return 1;
    		}

    		i++;

  	}
  	return 0;
}





int count(char *word, char **list, int i) {

	int j, cpt = 0;

  	for (j = i; strcmp(list[j], "NULL") != 0; j++) {
   
		cpt += (strcmp(word, list[j]) == 0) ? 1 : 0;

  	}

  	return cpt;
}


void digramme(char *segmant, char **digramme_list) {

  int i = 0;

  char *digramme;

  char *temp = malloc(3 * sizeof(char));

  strcpy(temp, "tp");

  digramme_list[0] = temp;



  while (segmant[i] != 0 && segmant[i + 1] != 0) {

    digramme = malloc(3 * sizeof(char));

    digramme[0] = segmant[i];

    digramme[1] = segmant[i + 1];

    digramme[2] = '\0';

    digramme_list[i++] = digramme;

  }

  free(temp);

  digramme_list[i] = "NULL";

}



int digramme_verif(char **digramme_list1, char **digramme_list2) {

  int i, output = 0;

  for (i = 0; strcmp(digramme_list1[i], "NULL"); i++) {

    if (in(digramme_list1[i], digramme_list2))

      output++;

  }

  return output;

}



int compute_common_digramme(char **digramme_list1, char **digramme_list2) {

  int output = 0;

  int n = 0, m = 0;

  while (strcmp(digramme_list1[n++], "NULL"))

    ;

  n--;

  while (strcmp(digramme_list2[m++], "NULL"))

    ;

  m--;



  if (m > n) {

    output = digramme_verif(digramme_list1, digramme_list2);

  } else {

    output = digramme_verif(digramme_list2, digramme_list1);

  }

  return output;

}



int liberate_digramme(char **list) {

  int i = 0;

  while (strcmp(list[i], "NULL")) {

    free(list[i]);

    i++;

  }

  free(list);

  return 1;

}



int length(char *word){

    int i = 0;

    while (word[i++]);

    return i;

}



double distance_dice(char *word1, char *word2) {

  char **list1 = malloc(200 * sizeof(char *));

  char **list2 = malloc(200 * sizeof(char *));



  if (word1[1] == 0 && word2[1] == 0)

    return 0.0;



  digramme(word1, list1);

  digramme(word2, list2);



  int nx = compute_common_digramme(list1, list1);

  int ny = compute_common_digramme(list2, list2);

  int nc = compute_common_digramme(list1, list2);

  double d = 1 - ((double)(2 * nc)) / ((double)(nx + ny));



  liberate_digramme(list1);

  liberate_digramme(list2);



  return d;

}


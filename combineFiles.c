#include "defenitions.h"
#define NUM_OF_DIG 7
#define SIZE_OF_LINE_IN_OB_DATA 20

/*this function get chars until space or \n*/
static char *inputUntilSpaceOrEnter(FILE *file)
{
	int i = 0, c;
	char *word = malloc(SIZE_OF_LINE_IN_OB_DATA);
	c = fgetc(file);
	if(c == EOF)/*the first letter is not \n nor space 
	so we do'nt need to check the other conditions*/
	{
		/*if we got to this point it means the file is empty*/
		free(word);
		return NULL;
	}
	word[i++] = c;
	while((c = fgetc(file)) != ' ' && c != EOF && c != '\n')
	{
		word[i++] = c;
	}
	return word;
}

/*this function combines two files into one*/
void combineFiles(FILE *finalFile, FILE *firstFile, FILE *secondFile, int IC)
{
	
	char *word, c;
	int num, i = 1;
	rewind(firstFile);
	rewind(secondFile);
	rewind(finalFile);
	while((c = fgetc(firstFile)) == ' ')
		fprintf(finalFile," ");

	if((word = inputUntilSpaceOrEnter(firstFile)) != NULL)/*check that the files are not empty*/
	{
		fprintf(finalFile,"%c%s ",c,word);
	}
	if((word = inputUntilSpaceOrEnter(firstFile)) != NULL)/*check that the files are not empty*/
	{
		fprintf(finalFile,"%s\n",word);
	}
	
	/*this loop prints the first file into the final file*/
	while((word = inputUntilSpaceOrEnter(firstFile)) != NULL)
	{
		if(i%2 == 0)
			fprintf(finalFile,"%s\n",word);
		else
		{/*the part that we need to add zeros to*/
			fprintf(finalFile,"%s ",word);
			
		}
		i++;
	}
	i = 1;
	
	/*this loop prints the second file into the final file*/
	while((word = inputUntilSpaceOrEnter(secondFile)) != NULL)
	{

		if(i%2 == 0)
			fprintf(finalFile,"%s\n",word);
		else/*the part that we need to add zeros to*/
		{

			num = *strtoint(word);
			num += IC;
			fprintf(finalFile,"%07d ",num);
			
		}
		i++;
	}
}

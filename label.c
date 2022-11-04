#include "defenitions.h"


extern command commands[];

/*this function get the label from the line (if exist, if not returns NULLs)*/
char *hasLabel(char* line)
{
	char *label = malloc(SIZE_OF_LABEL+1);
	int i = 0, j = 0, flag = 1;
	/*i represents the place in the line, j represents the place in the label*/
	while(line[i] == '\t' || line[i] == ' ')/*skips the white chars in the start of the line*/
		i++;
	while(line[i] != 0 && line[i] != ':' && line[i]!= ' ' && line[i]!= '\t')
	{/* the loop stops if we got to the end of the line or we got to the end
		of the label or the label is too long*/
		if(j <= SIZE_OF_LABEL && flag)
			label[j++] = line[i++];
		else/*returns an empty string*/
		{
			free(label);
			label = malloc(sizeof(char));
			label[0] = 0;
			flag = 0;
			i++; 
		}
	}
	if(line[i] == ':' && !flag)
	{/*the label has 0 in the start of it*/
		return label;
	}
	if(line[i] != ':') /* there is no label in this line */
	{
		free(label);
		return NULL;
	}
	label[j] = 0;
	return label;
}

/*this function check if the given label is legal,
 if is return 1 otherwise return 0*/
int isLabelLegal(char *label)
{
	int i = 1;
	if(label == NULL)/*there is no label*/
		return 1;
	if(!((label[0] >= 'A' && label[0] <= 'Z') ||
	 (label[0] >= 'a' && label[0] <= 'z')))
		return 0; /*check if the first char in the label is legal*/
	while(label[i] != 0)
	{
		if(!((label[i] >= 'A' && label[i] <= 'Z') || 
			(label[i] >= 'a' && label[i] <= 'z') ||
			 (label[i] >= '0' && label[i] <= '9')))
		{/*check if the current char is legal (if not returns 0)*/
			return 0;
		}
		i++;
	}
	i = 0;
	while(strcmp(commands[i].cmd,"0"))/*check if the label is not a saved word*/
	{
		if(!strcmp(commands[i].cmd,label))
			return 0;
		i++;
	}
	if(isreg(label))
		return 0;
	return 1;
}

/*this function removes the label from the line*/
char *rmLabel(char *oldLine)
{
	int i = 0;
	char *newLine = malloc(SIZE_OF_LINE+1);
	while(oldLine[i++] != ':');
	/* i is the place after the label*/
	strncpy(newLine, oldLine+i, strlen(oldLine)-i);
	free(oldLine);
	return newLine;
}

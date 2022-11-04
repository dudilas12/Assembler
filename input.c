#include "defenitions.h"

extern command commands[];
extern int IC;
extern int DC;
extern list labellist;
extern int linec;
static char *rmWhiteUntilNonWhiteEndToStart(char *line, int *i);
static char *rmWhiteUntilNonWhiteStartToEnd(char *line, int i);

/*this function get the first line to be converted*/
char * getLine(FILE *file)
{
	int c;
	char* line = malloc(SIZE_OF_LINE + 2);
	int i;
	c = fgetc(file); /*get the first char*/
	if(c == EOF)/*check if we got to EOF*/
	{
		free(line);
		return NULL;
	}
	else if(c == '\n')
	{
		free(line);
		line = malloc(sizeof(char));
		line[0] = 0;
		return line;
	}
	for(i = 0;i < SIZE_OF_LINE+1 && c != '\n' && c != EOF;i++)
	/*the end of the input is when there is too many word in the line or
	we got to the end of the line or we got to EOF*/
	{
		line[i] = (char)c;
		c = fgetc(file);
	}
	if(i == SIZE_OF_LINE + 1)/*the line is longer than 80 chars*/
	{
		while(c != '\n')
			c = fgetc(file);
		line[SIZE_OF_LINE+1] = 0;
		return line;
	}
	line[i] = 0;
	return line;
}
/*this function gets the new line after we remove the white chars*/
char *rmWhiteChars(char *oldLine)
{
	char *newLine = malloc(SIZE_OF_LINE+1);
	int i = 0, *j = malloc(sizeof(int));
	newLine = rmWhiteUntilNonWhiteStartToEnd(oldLine,0);
	while(newLine[i] != 0)
	{
		if(newLine[i] == '\"')/*skip parts that are between ""*/
		{
			i++;
			while(newLine[i] != '\"' && newLine[i] != 0)
				i++;
			if(newLine[i] == '\"')
				i++;
			else
				break;/*got to the end of the string*/
		}
		if(newLine[i] == ' ' || newLine[i] == '\t' || newLine[i] == ',')/*keep one space and remove others*/
		{
			i++;
			newLine = rmWhiteUntilNonWhiteStartToEnd(newLine, i);
		}
		else
			i++;
	}
	*j = strlen(newLine)-1;
	while((*j)>0)
	{
		newLine = rmWhiteUntilNonWhiteEndToStart(newLine,j);
		if(newLine[*j] == '\"')/*skip parts that are between ""*/
		{
			(*j)--;
			while(newLine[*j] != '\"' && *j != 0)
				(*j)--;
			if(newLine[*j] == '\"')
				(*j)--;
		}
		if((*j)>0)
		{
			while(newLine[*j] != ',' && *j != 0)/*after removing spaces we go to the next "," or the start of the str*/
				(*j)--;
			if(newLine[*j] == ',')
				(*j)--;
		}
	}
	free(j);
	return newLine;
}
/*this function gets the line and the place to start removing white chars, it goes
backwords and removes any white chars until non white chars*/
static char *rmWhiteUntilNonWhiteEndToStart(char *line, int *i)
{/* *i is the place to start clearing spaces*/
	char *newLine = malloc(SIZE_OF_LINE+1);
	int j = *i+1, x;
	while(line[*i] == '\t' || line[*i] == ' ')/*skip the spaces*/
	{
		(*i)--;
	}
	for(x=0;x<=*i;x++)/*copy the first *i chars to the new str*/
	{
		newLine[x] = line[x];
	}
	for(;j<strlen(line);j++)/*copy the remaining chars to the str*/
	{
		newLine[x++] = line[j];
	}
	newLine[x] = 0;
	free(line);
	return newLine;
}
/*this function gets the line and the place to start removing white chars, it goes
forwords and removes any white chars until non white chars*/
static char *rmWhiteUntilNonWhiteStartToEnd(char *line, int i)
{/* i is the place to start clearing spaces*/
	char *newLine = malloc(SIZE_OF_LINE+1);
	int j;
	for(j=0;j<i;j++)/*copy the first i chars to the new str*/
	{
		newLine[j] = line[j];
	}
	while(line[i] == '\t' || line[i] == ' ')/*skip the spaces*/
	{
		i++;
	}
	for(;i<strlen(line);i++)/*copy the remaining chars to the str*/
	{
		newLine[j++] = line[i];
	}
	newLine[j] = 0;
	free(line);
	return newLine;
}
/*this function assumes that the line doesn't contain
 a label nor whitechars(except one after the command name)*/
int stateofline(char *line)
{
	char * command = (char *)malloc(SIZE_OF_LINE+1);
	int c, i =0;
	/*gets the commnad of the given line*/
	while((c = line[i])!='\0' && c != ' ' && c != ',')
	{
		command[i++] = c; 
	}
	command[i] = '\0';
	/*returns the state of the given command*/
	return getstate(command);
}
/*this function checks for the command's state*/
int getstate(char * command)
{
	int i = 0;
	/*checks if the command is code */
	while(strcmp(commands[i].cmd,"0"))
	{
		if(!strcmp(commands[i++].cmd,command))
		{
			free(command);
			return CODE;/*code state*/
		}
	}
	/*checks if the command is .data*/
	if(!strcmp(command,DATA_STR))
	{
		return DATA;/*data state*/
		free(command);
	}
	/*checks if the command is .string*/
	if(!strcmp(command,STR_STR))
	{
		return STR;/*string state*/
		free(command);
	}
	/*checks if the command is .entry*/
	if(!strcmp(command,ENT_STR))
	{
		return ENT;/*entry state*/
		free(command);
	}
	/*checks if the command is .extry*/
	if(!strcmp(command,EXT_STR))
	{
		return EXT;/*extern state*/
		free(command);
	}
	free(command);
	return ERROR;/*error state*/


}
/*this function analyzes a given line with a ".data"*/
int handledata(char* line,FILE* datafile)
{
	int cnt;
	int fin = 0;
	int i=0;
	data add;
	data dat;
	int c;
	int j;
	int inum;
	/*allocate data to the num*/
	char * num = (char *)malloc(SIZE_OF_LINE+1);
	/*loops until find the start of the values*/
	while((c = line[i]) != ' ' && c != '\0' && c!= ','&& c != '\t')
	{
		i++;
	}
	/*checks if there is values after the .data ERROR else*/
	if(c == '\0')
	{
		/*handle error*/
		printf("in line number:%d there is no values after .data\n",linec);
		free(num);
		return 0;
	}
	
	i++;
	cnt = 0;
	while(!fin)
	{

		j= 0;
		/*loops until reached to a comma or to the end*/
		while((c= line[i++]) != ',' && c!= '\0')
			num[j++] = c;
		/*reached to the end*/
		if(c == '\0')
		{
			fin = 1;
		}
		num[j] = '\0';
		/*there is a comma not in a right place*/
		if(j == 0)
		{
			/*handle error- comma not in a right place*/
			if(cnt == 0)
				printf("in line number:%d there is a comma in the start\n",linec);
			else if(line[i+1] == '\0')
				printf("in line number:%d there is a comma in the end\n",linec);
			else	
				printf("in line number:%d multiple consecetive commas\n",linec);
			free(num);
			return 0;
		}
		/*checks if it is a number
		if it is adds it prints it to the file
		while increasing DC*/
		if(strtoint(num) != NULL)
		{
			inum = *strtoint(num);
			add.value = DC;
			dat.value = inum;
			fprintf(datafile,"%07d ",add.value);
			fprintf(datafile,"%06x\n",dat.value);
			DC++;
		}
		/*the number is illagel*/
		else
		{
			printf("in line number:%d illegal, %s not number\n",linec,num);
			free(num);
			return 0;
		}
		cnt++;

	}
	free(num);
	return 1;

}
/*this function checks if a given line has only white chars*/
int hasonlywhite(char * line)
{
	int i;
	/*loops for each character in the line, checks if all white chars*/
	for(i = 0; i< strlen(line);i++)
	{
		if(line[i] != ' ' && line[i] != '\t')
			return 0;
	}
	return 1;
}
/*this function analyzes a given line with a command*/
int handlecode(char * line)
{
	
	int i = 0,j = 0,k,c,n;
	int hadcomma = 0;
	/*allocate memory for the command and the parameters*/
	char * command = malloc(SIZE_OF_LINE+1);
	char * prm[MAX_PARM];
	for(i = 0;i<MAX_PARM;i++)
		prm[i] = malloc(SIZE_OF_LINE+1);
	/*increases the IC because it is a code line*/
	IC++;
	i = 0;
	/*finds the command*/
	while((c = line[i]) != '\0' && c != ' ' && c!= ',' && c != '\t')
		command[i++] = c;
	command[i] = '\0';
	i++;
	/*case there is no parameters*/
	if(c == '\0')
	{
		j = -1;
	}
	/*else there is parameters*/
	else
	{
		k = i;
		/*ERROR, comma in the start*/
		if(line[k] == ',')
		{
			printf("in line number:%d there is a comma in the start\n",linec);
			freeparm(prm);
			return 0;
		}
		/*check for multiple consecetive commas in the middle*/
		for(;line[k]!='\0';k++)
		{

			if(line[k] == ',')
			{
				/*ERROR, consecetive commas in the middle*/
				if(hadcomma !=0)
				{
					printf("in line number:%d multiple consecetive commas\n",linec);
					freeparm(prm);
					return 0;
				}
				hadcomma = 1;

			}
			else if(!(line[k] == ' ' || line[k] == '\t'))
				hadcomma = 0;
		}
		k = i;
		/*ckecks for comma in the end*/
		for(;line[k]!='\0';k++)
		{
			if(line[k] == ',')
				hadcomma =1;
			else if(!(line[k] == ' ' || line[k] == '\t'))
				hadcomma = 0;
		}
		/*ERROR, there is a comma in the end*/
		if(hadcomma)
		{
			printf("in line number:%d there is a comma in the end\n",linec);
			freeparm(prm);
				return 0;
		}
	}
	n = 0;
	/*intiallizes the parameters*/
	for(;line[i]!='\0' && j<MAX_PARM;i++)
	{
		if(line[i] == ',')
		{
			j++;
			n = 0;
		}
		else
			prm[j][n++] = line[i];
	}
	if(j == MAX_PARM)
	{
		/*handle the error*/
		printf("in line number:%d too many paramers\n",linec);
		freeparm(prm);
		return 0;
	}
	/*loops until finds the right command*/
	for(i = 0;;i++)
	{
		if(strcmp(command,commands[i].cmd) == 0)
		{
			/*ERROR, not the right amount of parameters*/
			if(commands[i].vars - j !=1)
			{
				/*handle the error*/
				printf("in line number:%d not the right amount of parameters\n",linec);
				freeparm(prm);
				return 0;
			}
			/*loops for the amount of parameters in the line*/
			for(n = 0;n<=j;n++)
			{
				if(strlen(prm[n]) == 1 && prm[n][0] == '#')
				{
					/*handle the error*/
					printf("in line number:%d missing number after #\n",linec);
					freeparm(prm);
					return 0;
				}
				if(strlen(prm[n]) == 1 && prm[n][0] == '&')
				{
					/*handle the error*/
					printf("in line number:%d missing label after &\n",linec);
					freeparm(prm);
					return 0;
				}
				if(strlen(prm[n]) > 1 && prm[n][0] == '#' && !isnum(prm[n]))
				{
					/*handle the error*/
					printf("in line number:%d illegal, %s not a number\n",linec,prm[n]);
					freeparm(prm);
					return 0;
				}
				if(prm[n][0] == '&' && commands[i].isjmp == 0)
				{
					/*handle the error*/
					printf("in line number:%d not a jump function\n",linec);
					freeparm(prm);
					return 0;
				}
				/*if the parameter is not a register, it needs another word of memory*/
				if(!(isreg(prm[n])))
				{
					IC++;
				}
			}
			freeparm(prm);
			return 1;
		}
	}
}
void freeparm(char** prm)
{
	int i = 0;
	for(i = 0; i<MAX_PARM;i++)
	free(prm[i]);
}
/*this function check if the given string is a register
returns the number of the register +1*/
int isreg(char * str)
{
	if(strlen(str) == 2)
	{
		/*checks if the first letter is r and the second letter is number between 0 to 7*/
		if(str[0] == 'r' && '0'<=str[1] && str[1]<='7')
			return str[1] - '0' + 1;
	}
	return 0;
}
/*this function check if the given string is a number*/
int isnum(char * str)
{
	int * x;
	if(strlen(str) > 1)
	{
		/*checks if the first letter is # and the rest is a number*/
		if(str[0] == '#' && (x = strtoint(str+1)))
		{
			free(x);
			return 1;
		}
	}
	return 0;

}
/*checks if the str is a suitable operand fro jmp function*/
int isjmp(char * str)
{
	/*checks if the first letter is & and there is someting after it*/
	if(strlen(str) > 1 && str[0] == '&')
		return 1;
	return 0;
}
/*this function analyzes a given line with a ".string"*/
int handlestring(char* line,FILE* datafile)
{
	int i=0;
	int c;
	data add;
	data dat;
	/*loops until finds the start of the string*/
	while((c = line[i]) != ' ' && c != '\0'&& c != ',' && c != '\t')
		i++
		;
	/*didnt find a string*/
	if(c == '\0')
	{
		printf("in line number:%d missing string after .string \n",linec);
		/*handle error*/
		return 0;
	}
	i++;
	c = line[i++];
	if(c != '"')
	{
		printf("in line number:%d illegal, not a string\n",linec);
		/*handle error*/
		return 0;
	}
	/*loops for each char in the string*/
	while((c = line[i])!= '"' && c!= '\0')
	{
			/*prints each parameter to the datafile while increasing DC each time*/
			add.value = DC;
			dat.value = c;
			fprintf(datafile,"%07d ",add.value);
			fprintf(datafile,"%06x\n",dat.value);
			i++;
			DC++;

	}
	/*adds also zero to the file*/
	add.value = DC;
	dat.value = 0;
	fprintf(datafile,"%07d ",add.value);
	fprintf(datafile,"%06x\n",dat.value);
	DC++;

	if(line[i+1] != '\0')
	{
		printf("in line number:%d extra characters after string\n",linec);
		return 0;
	}
	if(line[i] == '\0')
	{
		printf("in line number:%d illegal, not a string\n",linec);
		return 0;
	}
	return 1;

}
/*this function convert a given string to an integer
(if the string has zeros at the end, the function ignore them)*/
int * strtoint(char *str)
{
	
	int i =0;
	int x;
	int * p = (int *)malloc(sizeof(int));
	if(str[0]=='+' || str[0] == '-')
		i++;
	while(str[i]=='0')
		i++;
	if(str[i]=='\0')
	{
		*p = 0;
		return p;
	}
	
	
	else if((x = atoi(str)) != 0)
	{
		*p = x;
		return p;
	}
	free(p);
	return NULL;
}
/*this function analyzes a given line with a ".extern" command*/
int handleextern(char* line,FILE* datafile)
{
	int i = 0;
	int c;
	/*allocate memory for the name*/
	char * name = malloc(SIZE_OF_LINE +1);
	/*loops until finds the start of the name*/
	while((c =line[i++]) != ' '&& c !='\0' && c != '\t' && c!= ',')
		;
	/*there is no name after .extern, ERROR*/
	if(c == '\0')
	{
		/*handle error*/
		printf("in line number:%d label missing after .extern\n",linec);
		free(name);
		return 0;
	}
	strcpy(name, line + i);
	/*the name is illagel*/
	if(c == ',')
	{
		/*handle error*/
		printf("in line number:%d %s illegal label\n",linec,name);
		free(name);
		return 0;
	}
	if(!isLabelLegal(line +i))
	{
		/*handle error*/
		printf("in line number:%d %s illegal label\n",linec,name);
		free(name);
		return 0;
	}
	/*the name is illagel*/
	if(exist(&labellist,name,DONT_CARE))
		{
				printf("in line number:%d label %s already exist\n",linec,name);
				free(name);
				return 0;
		}
	else
		/* if all right adds to the list*/
		addtolist(name,0,EXT,&labellist);
	return 1;

}
/*this function analyzes a given line with a "entry" command*/
int handleentry(char* line,FILE* file)
{
	int i = 0;
	int c;
	cp label;
	/*allocate memory for name*/
	char * name = malloc(SIZE_OF_LINE +1);
	/*loops until finds the start of the name*/
	while((c =line[i++]) != ' '&& c !='\0' && c != '\t')
		;
	/*there is no name, ERROR*/
	if(c == '\0')
	{
		/*handle error*/
		free(name);
		return 0;
	}
	/*the label is illagel*/
	if(!isLabelLegal(line +i))
	{
		/*handle error*/
		printf("in line number:%d  label %s for .entry instruction not found.\n",linec,line+i);
		free(name);
		return 0;
	}
	strcpy(name, line + i);
	/*checks if already used as ent*/
	if(isent(&labellist,name))
	{
		printf("in line number:%d %s already used as .entry\n",linec,name);
		return 0;
	}
	/*checks if the label exist in the labellist*/
	else if((label = exist(&labellist,name,CODE)) || (label = exist(&labellist,name,DATA)) || (label = exist(&labellist,name,STR)))
	{
		label->ent = 1;
		fprintf(file,"%s %07d\n",name,label->address);
	}
	/*didnt found a label in the label list*/
	else
	{
		printf("in line number:%d  label %s for .entry instruction not found.\n",linec,name);
		return 0;
	}	
	return 1;
}




		








































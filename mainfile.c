#include "defenitions.h"


command commands[] = {{"mov",0,0,2,0},{"cmp",1,0,2,0},{"add",2,1,2,0},{"sub",2,2,2,0},{"lea",4,0,2,0},{"clr",5,1,1,0},{"not",5,2,1,0},{"inc",5,3,1,0},{"dec",5,4,1,0},
{"jmp",9,1,1,1},{"bne",9,2,1,1},{"jsr",9,3,1,1},{"red",12,0,1,0},{"prn",13,0,1,0},{"rts",14,0,0,0},{"stop",15,0,0,0},{"0",0,0,0,0}};
int IC = BASE_IC;
int DC = BASE_DC;
int ICF;
int DCF;
int linec = START_LINE;
list labellist;

int main(int argc, char *argv[])
{
	FILE *inFile;
	int i;
	char * filename = malloc(MAX_FILE_NAME);
	for(i = 1;i<argc;i++)
	{
		strcpy(filename,argv[i]);
		strcat(filename,".as");
		inFile = fopen(filename,"r");
		if(inFile)
			readfile(inFile,argv[i]);
		else
			printf("%s: file doesnt exist\n",filename);
	}
	/*readfile(stdin,"nonsense");*/
	return 0;
}

/*this function reads the given file and makes a ob,ent,ext file from him*/
void readfile(FILE* file,char* filename)
{

	/*declaration of file*/
	/*REMINDED: get the file*/
	int isEOF = 0;
	char *curline;
	char *curlabel;
	int curstate;
	int founderror = 0;
	/*we assum that non of these files exist(we can also put a complicated
	 name to each file to make sure it does not exist
	  but we prefered keeping it more underastandable*/
	/*file decleration and opening*/
	FILE *datafile,*obfile,*entfile,*extfile,*finalfile;
	char * finaleFile = malloc(MAX_FILE_NAME);
	char * entFile = malloc(MAX_FILE_NAME);
	char * extFile = malloc(MAX_FILE_NAME);
	strcpy(finaleFile,filename);
	strcat(finaleFile,".ob");
	strcpy(entFile,filename);
	strcat(entFile,".ent");
	strcpy(extFile,filename);
	strcat(extFile,".ext");
	datafile = fopen("datafile","w+");
	obfile = fopen("obfile", "w+");
	entfile = fopen(entFile, "w+");
	extfile = fopen(extFile, "w+");
	finalfile = fopen(finaleFile, "w+");
	linec = START_LINE-1;
	IC = BASE_IC;
	DC = BASE_DC;
	/*free the label list to make it empty*/
	freelist(&labellist);
	/*prints a message to let know starting*/
	printf("IN FILE %s.as:\n",filename);
	/*mainloop for fist passing*/
	while(!isEOF)
	{
		/*increases the line number every operation*/
		linec++;
		/*gets the line*/
		curline = getLine(file);
		/*if it is a NULL pointer, means found EOF*/
		if(curline == NULL)
		{
			isEOF = 1;
			continue;	
		}
		/*if the length of the line is over SIZE_OF_LINE means ERROR*/
		else if(strlen(curline)>SIZE_OF_LINE)
		{
			/*manage the error*/
			/*adds the line to the array*/
			addCell(linec);
			/*prints the ERROR message*/
			printf("in line number:%d line length is over %d\n",linec,SIZE_OF_LINE);
			founderror = 1;
			continue;
		}
		/*checks if the line is empty or has a comment*/
		if(hasonlywhite(curline) || curline[0] == ';')
			continue;
		/*gets the label if exist*/
		curlabel = hasLabel(curline);
		if(curlabel != NULL)
		{
			/*if the label is empty means its to long ERROR */
			if(curlabel[0] == '\0')
			{
				addCell(linec);
				printf("in line number:%d label is to long\n",linec);
				founderror = 1;
				continue;
			}
			/*checks if  the label is legal else ERROR*/
			else if(!isLabelLegal(curlabel))
			{
				/*label name is illegal manage the ERROR*/
				/*adds the line number to the array*/
				addCell(linec);
				/*prints a message of the ERROR*/
				printf("in line number:%d label %s name is illegal\n",linec,curlabel);
				founderror = 1;
				continue;
			}
		}
		/*removes the label from the line if exists*/
		if(curlabel != NULL)
			curline = rmLabel(curline);
		/*checks if there is text after the label,else ERROR*/
		if(strlen(curline) == 0)
		{
			/*prints a message of the ERROR*/
			printf("in line number:%d there is nothing after the label %s\n",linec,curlabel);
			/*adds the line to the array*/
			addCell(linec);
			founderror = 1;
			continue;
		}
		/*removes white chars from the line to make it easy to analize*/
		curline = rmWhiteChars(curline);
		/*gets the state of line
		-1 = ERROR, 0=CODE, 1=DATA, 2=STRING, 3 = ENTRY, 4 = EXTERN*/
		curstate = stateofline(curline);
		/*if didnt found a state of the line means ERROR*/
		if(curstate == ERROR)
		{
			/*manage the error*/
			/*adds the line to the array*/
			addCell(linec);
			/*prints a message of the ERROR*/
			printf("in line number:%d undefined command name\n",linec);
			founderror = 1;
			continue;
		}
		/*code*/
		else if(curstate == CODE)
		{
			/*if there is a label in the line, adds it to the label list*/
			if(curlabel!= NULL)
			{
				/*add label to list*/
				/*if label already exist ERROR*/
				if(exist(&labellist,curlabel,DONT_CARE))
				{
					/*add the line to the array*/
					addCell(linec);
					/*prints a message of the ERROR*/
					printf("in line number:%d label %s already exist\n",linec,curlabel);
					continue;
				}
				else
					/*adds the label to the label list*/
					addtolist(curlabel,IC,CODE,&labellist);
			}
			/*handles the code, if found ERROR returns 0 else 1*/
			if(!handlecode(curline))
			{
				/*found ERROR in the line*/
				addCell(linec);
				founderror = 1;
			}
		}
		/*data:data*/
		else if(curstate == DATA)
		{	
			/*if there is a label in the line, adds it to the label list*/
			if(curlabel!= NULL)
			{
				/*add label to list*/
				/*if label already exist ERROR*/
				if(exist(&labellist,curlabel,-1))
				{
					/*add the line to the array*/
					addCell(linec);
					/*prints a message of the ERROR*/
					printf("in line number:%d label %s already exist\n",linec,curlabel);
					continue;
				}
				else
					/*adds the label to the label list*/
					addtolist(curlabel,DC,DATA,&labellist);
			}
			/*handles the .data, if found ERROR returns 0 else 1*/
			if(!handledata(curline, datafile))
			{
				addCell(linec);
				founderror = 1;
			}
		}
		/*data:string*/
		else if(curstate == STR)
		{
			/*if there is a label in the line, adds it to the label list*/
			if(curlabel!= NULL)
			{
				/*add label to list*/
				/*if label already exist ERROR*/
				if(exist(&labellist,curlabel,-1))
				{
					/*add the line to the array*/
					addCell(linec);
					/*prints a message of the ERROR*/
					printf("in line number:%d label %s already exist\n",linec,curlabel);
					continue;
				}
				else
					/*adds the label to the label list*/
					addtolist(curlabel,DC,STR,&labellist);
			}
			/*handles the .string, if found ERROR returns 0 else 1*/
			if(!handlestring(curline, datafile))
			{
				addCell(linec);
				founderror = 1;
			}
		}
		/*entry*/
		else if(curstate == ENT)
		{

			/*skip dont need this currently*/
		}
		/*extern*/
		else if(curstate == EXT)
		{
			/*if label exist before .extern, ERROR*/
			if(curlabel!= NULL)
			{
				/*manage the error*/
				/*add line to the array*/
				addCell(linec);
				/*prints a message of the ERROR*/
				printf("in line number:%d label %s is illegal before .extern\n",linec,curlabel);
				founderror = 1;
			}
			else
			{
				/*handles .extern, return 0 if found error, 1 else*/
				if(!handleextern(curline,datafile))
				{
					addCell(linec);
					founderror = 1;
				}
			}	
		}
	}
	/*intiallize the final IC and DC to their values*/
	ICF = IC;
	DCF = DC;
	/*adds to each data and string the IC*/
	addic(&labellist,ICF);
	/*inttialize the IC and DC to their DEFULT*/
	IC = BASE_IC;
	DC = BASE_DC;
	linec = START_LINE -1;
	isEOF = 0;
	/*read from the start again*/
	rewind(file);
	fprintf(obfile,"%7d %d\n",ICF - BASE_IC,DCF);
	/*main loop for second pass*/
	while(!isEOF)
	{
		/*increases the line each operation*/
		linec++;
		/*gets the line*/
		curline = getLine(file);
		/*return NULL if found EOF*/
		if(curline == NULL)
		{
				isEOF = 1;
				continue;	
		}
		/*if already know there is error in the line skips it's analize*/
		if(contentInFirstCell()!= linec)
		{
			/*if has only white ignores the line*/
			if(hasonlywhite(curline) || curline[0] == ';')
				continue;
			/*gets the label if exist, NULL else*/
			curlabel = hasLabel(curline);
			if(curlabel != NULL)
				/*removes the label*/
				curline = rmLabel(curline);
			/*removes white chars from the line*/
			curline = rmWhiteChars(curline);
			/*gets the state of line
			-1 = ERROR, 0=CODE, 1=DATA, 2=STRING, 3 = ENTRY, 4 = EXTERN*/
			curstate = stateofline(curline);
			/*code*/
			if(curstate == CODE)
			{
				/*habdles the code for the second time, this time decode it and prints to the file*/
				if(!handlecode2(curline,obfile,extfile))
				{
					founderror = 1;
					continue;
				}
			}
			/*data*/
			else if(curstate == DATA)
			{
				/*already been handled*/
				continue;
			}
			/*string*/
			else if(curstate == STR)
			{
				/*already been handled*/
				continue;
			}
			/*entry*/
			else if(curstate == ENT)
			{
				/*if there is a label before .entry, ERROR*/
				if(curlabel!= NULL)
				{
					/*manage the error*/
					/*prints a message of the ERROR*/
					printf("in line number:%d label %s is illegal before .extern\n",linec,curlabel);
					founderror = 1;
				}
				else
				{
					/*handles the entry, if ERROR returns 0 else 1*/
					if(!handleentry(curline,entfile))
					{
						founderror = 1;
					}
				}	
			}
			/*extern*/
			else if(curstate == EXT)
			{
				/*already been handled*/
				/*handles the rest in the handle code2*/
			}
		}
		else
		{
			/*removes the line in found it already*/
			rmFirstCell();
		}
	}
	/*combines the files to one and deletes them*/
	combineFiles(finalfile,obfile,datafile, ICF);
	remove("datafile");
	remove("obfile");
	/*if founderror is zero didnt find a error, else found*/
	if(!founderror)
	{
		printf("succses!\n");
		/*fclose(finaleFile);
		fclose(entFile);
		fclose(extFile);*/
	}	
	else
	{
		remove(finaleFile);
		remove(entFile);
		remove(extFile);
	}
}
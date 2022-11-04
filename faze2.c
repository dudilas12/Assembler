#include "defenitions.h"
#define A 4
#define R 2
#define E 1

extern int IC;
extern int linec;
extern list labellist;
extern command commands[];


/*this function gets a code line and analize it, and also decode the code and prints it to the file*/
int handlecode2(char * line, FILE * file, FILE * extfile)
{

	int i = 0,j = 0,c,n,k,reg,d1 = 0,d2 = 0;
	cp ext1 = NULL,ext2 = NULL;
	cp label;
	char * prm[MAX_PARM];
	codebitfield code;
	databitfield data1,data2;
	/*allocate memory for command and parameters*/
	char * command = malloc(SIZE_OF_LINE+1);
	for(i = 0;i<MAX_PARM;i++)
		prm[i] = malloc(SIZE_OF_LINE+1);
	i = 0;
	/*gets the command*/
	while((c = line[i]) != ' ' && c!='\0')
		command[i++] = c;
	command[i++] = '\0';
	/*there are no parameters*/
	if(c == '\0')
	{
		j = -1;
	}
	/*gets the parameters to prm*/
	else
	{
		k = i;
		n = 0;
		/*loops until a end of line,gets the parameter until comma reached, after reached reads the next parameter*/
		for(;line[k]!= '\0' && j<MAX_PARM;k++)
		{
			if(line[k] == ',')
			{
				prm[j][n] = '\0';
				n = 0;
				j++;	
			} 
			else
			{
				prm[j][n++] = line[k];
			}
		}
	}
	i = 0;
	/*loops until finds the right command*/
	while(strcmp(commands[i].cmd,"0"))
	{
		/*if the command is found in the array*/
		if(strcmp(commands[i].cmd,command) == 0)
		{
			/*intiallize the opcode, the funct and the ARE to their values*/
			code.opcode = commands[i].code;
			code.funct = commands[i].funct;
			code.ARE = A;
			n =0;
			/*if the number of parameters is 2*/
			if(commands[i].vars == 2)
			{
				/*-------------------------first parameter analyze-------------------------*/
				/*if reached a lea operator*/
				if(!strcmp(command,"lea"))
				{
					/*the source operand must be a code label in lea function*/
					if(isreg(prm[0]) || isnum(prm[0])|| isjmp(prm[0]))
					{
						printf("in line number:%d source operand in lea func must be a label\n",linec);
						freeparm(prm);
						return 0;
					}
					/*found a data or string label*/
					else if(((label = exist(&labellist,prm[0],DATA)) || (label = exist(&labellist,prm[0],STR))))
					{
						/*makes d1 1 to let know needed another word of memory*/
						d1 = 1;
						data1.value = label->address;
						data1.ARE = R;
						code.rs = 0;
						code.os = LAB_OFF;
					}
					/*found a extern label*/
					else if((label = exist(&labellist,prm[0],4)))
					{
						ext1 = label;
						/*makes d1 1 to let know needed another word of memory*/
						d1 = 1;
						data1.value = 0;
						data1.ARE = E;
						code.rs = 0;
						code.os = LAB_OFF;	
					}
					/*couldent find a label, ERROR*/
					else
					{
						printf("in line number:%d label %s not found\n",linec,prm[0]);
						freeparm(prm);
						return 0;
					}
				}	
				/*didnt reached a lea function, reached other function*/
				else
				{
					/*the first parameter is a register*/
					if((reg = isreg(prm[0])))
					{
						code.rs = reg-1;
						code.os = REG_OFF;
					}
					/*the first parameter is a number*/
					else if(isnum(prm[0]))
					{
						/*makes d1 1 to let know needed another word of memory*/
						d1 = 1;
						data1.value = *strtoint(prm[0]+1);
						data1.ARE = A;
						code.rs = 0;
						code.os = DIR_OFF;
					}
					/*the first parameter starts with a &, ERROR*/
					else if(isjmp(prm[0]))
					{
						printf("in line number:%d not a jump func\n",linec);
						freeparm(prm);
						return 0;
					}
					/*found a data or string label that fits the first parameter*/
					else if((label = exist(&labellist,prm[0],DATA)) || (label = exist(&labellist,prm[0],STR)))
					{
						/*makes d1 1 to let know needed another word of memory*/
						d1 = 1;
						data1.value = label->address;
						data1.ARE = R;
						code.rs = 0;
						code.os = LAB_OFF;
					}
					/*found an extern label that fits the first parameter*/
					else if((label = exist(&labellist,prm[0],4)))
					{
						ext1 = label;
						/*makes d1 1 to let know needed another word of memory*/
						d1 = 1;
						data1.value = 0;
						data1.ARE = E;
						code.rs = 0;
						code.os = LAB_OFF;	
					}
					/*didnt find a label*/
					else
					{
						printf("in line number:%d label %s not found\n",linec,prm[0]);
						freeparm(prm);
						return 0;
					}
				}

				/*---------------------second parameter analize -----------------------*/
				/*the only time the destination parameter can be a number is when the operator is cmp*/
				if((!(strcmp(command,"cmp"))) &&    isnum(prm[1]))
				{
						/*makes d2 1 to let know needed another word of memory*/
						d2 = 1;
						data2.value = *strtoint(prm[1] + 1);
						data2.ARE = A;
						code.rd = 0;
						code.od = DIR_OFF;
				}
				else
				{
					/*the source operator cant be a number*/
					if(isnum(prm[1]))
					{
						printf("in line number:%d number cant be a destination operand\n",linec);
						freeparm(prm);
						return 0;
					}
					/*the second parameter is a register*/
					else if((reg = isreg(prm[1])))
					{
						code.rd = reg-1;
						code.od = REG_OFF;
					}
					/*the second parameter starts with a &, ERROR*/
					else if(isjmp(prm[1]))
					{
						printf("in line number:%d not a jump fucntion\n",linec);
						freeparm(prm);
						return 0;
					}
					/*found a data or string label that fits to the second parameter*/
					else if((label = exist(&labellist,prm[1],DATA)) || (label = exist(&labellist,prm[1],STR)))
					{
						/*makes d2 1 to let know needed another word of memory*/
						d2 = 1;
						data2.value = label->address;
						data2.ARE = R;
						code.rd = 0;
						code.od = LAB_OFF;
					}
					/*found an extern label that fits to the second parameter*/
					else if((label = exist(&labellist,prm[1],EXT)))
					{
						ext2 = label;
						/*makes d2 1 to let know needed another word of memory*/
						d2 = 1;
						data2.value = 0;
						data2.ARE = E;
						code.rd = 0;
						code.od = LAB_OFF;	
					}
					/*didnt find a label*/
					else
					{
						printf("in line number:%d label %s not found\n",linec,prm[1]);
						freeparm(prm);
						return 0;
					}
				}
			}
			/* the command only contains one parameter*/
			else if(commands[i].vars == 1)
			{
				/*checks if the command is a jmp function*/
				if(commands[i].isjmp)
				{
					/*parameter is rgister,cant jump to register*/
					if(isreg(prm[0]))
					{
						printf("in line number:%d cant jump to a register\n",linec);
						freeparm(prm);
						return 0;
					}
					/*parameter is number,cant jump to register*/
					if(isnum(prm[0]))
					{
						printf("in line number:%d cant jump to a number\n",linec);
						freeparm(prm);
						return 0;
					}
					/*parameter starts with a &*/
					if(isjmp(prm[0]))
					{
						/*ckecks if the label exist*/
						if((label = exist(&labellist,prm[0]+1,CODE)))
						{
							/*makes d1 1 to let know needed another word of memory*/
							d1 = 1;
							data1.value = label->address - IC;
							data1.ARE = A;
							code.rd = 0;
							code.od = REL_OFF;
							code.rs = 0;
							code.os = DIR_OFF;
						}
						/*didnt find a label*/
						else
						{
							printf("in line number:%d label %s not found\n",linec,prm[0]+1);
							freeparm(prm);
							return 0;
						}
					}
					else
					{
						/*checks if a code label is found*/
						if((label = exist(&labellist,prm[0],CODE)))
						{
							/*makes d1 1 to let know needed another word of memory*/
							d1 = 1;
							data1.value = label->address;
							data1.ARE = R;
							code.rd = 0;
							code.od = LAB_OFF;
							code.rs = 0;
							code.os = DIR_OFF;
						}	
						/*didnt find a code label*/
						else
						{
							printf("in line number:%d label %s not found\n",linec,prm[0]);
							freeparm(prm);
							return 0;
						}
					}
				}
				else 
				{
					/*the parameter is a register*/
					if((reg = isreg(prm[0])))
					{
						code.rd = reg -1;
						code.od = REG_OFF;
						code.rs = 0;
						code.os = DIR_OFF;
					}
					/*the parameter is a number*/
					else if(isnum(prm[0]))
					{
						/*if the command is prn the number is legal*/
						if(!strcmp(command,"prn"))
						{
							/*makes d1 1 to let know needed another word of memory*/
							d1 = 1;
							data1.value = *strtoint(prm[0] + 1);
							data1.ARE = A;
							code.rd = 0;
							code.od = DIR_OFF;
							code.rs = 0;
							code.os = DIR_OFF;
						}
						/*the command is not prn, the number is not legal*/
						else
						{
						printf("in line number:%d number cant be a destination operand\n",linec);
						freeparm(prm);
						return 0;
						}

					}
					/*parameter starts with a & but not a jmp function*/
					else if(isjmp(prm[0]))
					{
						printf("in line number:%d not a jump function\n",linec);
						freeparm(prm);
						return 0;
					}
					else
					{
						/*checks for data or string label*/
						if((label = exist(&labellist,prm[0],DATA)) || (label = exist(&labellist,prm[0],STR)))
						{
							/*makes d1 1 to let know needed another word of memory*/
							d1 = 1;
							data1.value = label->address;
							data1.ARE = R;
							code.rd = 0;
							code.od = LAB_OFF;
							code.rs = 0;
							code.os = DIR_OFF;
						}	
						/*checks for extern label*/
						else if((label = exist(&labellist,prm[0],EXT)))
						{
							ext1 = label;
							/*makes d1 1 to let know needed another word of memory*/
							d1 = 1;
							data1.value = 0;
							data1.ARE = E;
							code.rd = 0;
							code.od = LAB_OFF;
							code.rs = 0;
							code.os = DIR_OFF;
						}
						/*didnt find a label*/
						else
						{
							printf("in line number:%d label %s not found\n",linec,prm[0]);
							freeparm(prm);
							return 0;
						}
					}
				}
			/*the number of parameters is 0*/
			}
			else if(commands[i].vars == 0)
			{
				code.rd = 0;
				code.od = DIR_OFF;
				code.rs = 0;
				code.os = DIR_OFF;
			}
		}
		i++;
	}
	/*prints the encoded code to the file*/
	fprintf(file,"%07d ",IC);
	fprintf(file,"%06x\n",codeBitFieldToInt(code));
	IC++;
	if(d1)
	{
		/*prints the encoded data1 if exist*/
		fprintf(file,"%07d ",IC);
		fprintf(file,"%06x\n",DataBitFieldToInt(data1));
		if(ext1 != NULL)
		{
			fprintf(extfile,"%s %07d\n",ext1->name,IC);
		}
		IC++;
	}
	if(d2)
	{
		/*prints the encoded data2 if exist*/
		fprintf(file,"%07d ",IC);
		fprintf(file,"%06x\n",DataBitFieldToInt(data2));
		if(ext2 != NULL)
		{
			fprintf(extfile,"%s %07d\n",ext2->name,IC);
		}
		IC++;
	}
	freeparm(prm);
	return 1;

}

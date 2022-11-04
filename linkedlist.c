/*this file creates a linkes list to store the labels and variables 
that reached*/

#include "defenitions.h"



/*declerations of the functions*/

/*this function gets the attributes of a new cell:
 a name, an address
  and also gets a list
 and adds the new cell into the end of the given list*/
cp addtolist(char * name, int address,int type, list* l)
{
	/*allocating the memory for the new cell to add*/
	cp newcell = (cp)malloc(sizeof(cell));
	cp cur = ((*l).head);

	/*defining the new cell*/
	newcell->name = name;
	newcell->address = address;
	newcell->type = type;
	newcell->ent = 0;
	/*defines the current cell to be the head(starter cell)*/
	/*if the cell(in this point equals to head) is empty(null)
	defines the head as  the newcell*/
	if (cur == 0)
	{
		(*l).head = newcell;
		return newcell;
	}
	if (cur->next == 0)
	{
		((*l).head)->next = newcell;
		return newcell;
	}
	cur = ((*l).head)->next;
	/*if the head already exist,loops until finds a cell
	that his next field is empty(null)*/
	while((cur->next)!= 0)
		cur = cur->next;
	/*puts the new cell into the next field*/
	cur->next = newcell;
	return newcell;
}

/*this functions prints the cells of a given cell*/
list * printlist(list* l)
{
	/*defines the current cell to be the head*/
	cp cur = (*l).head;
	/*loops to print all the cels*/
	while(cur!=0)
	{
		/*calls the function that prints the cell*/
		printcell(cur);
		cur = cur->next;
	}
	return l;


}

/*this fucntion prints a given cell*/
void printcell(cp cellp)
{
	/*prints the cell*/
	printf("name: %s address: %d type: %d\n",cellp->name,cellp->address,cellp->type);

}

/*this function fins if a given name of cell is found inside a given list 
if found returns a pointer to the cell else returns null*/
cp exist(list *l,char * name,int type)
{
	cp cur = (*l).head;
	/*loop that exemins each cell*/
	while(cur != 0)
	{
		/*checks if found a matching name*/
		/*type -1 means dont care about the type*/
		if(strcmp(cur->name,name)==0 && (cur->type == type|| type == -1))
			return cur;
		cur = cur->next;
	}
	return 0;

}

/*this function gets a name and searches in the list if there is a entery label with this name*/
int isent(list *l, char * name)
{
	cp cur = (*l).head;
	/*loop that exemins each cell*/
	while(cur != 0)
	{
		/*checks if found a matching name*/
		if(strcmp(cur->name,name)==0)
		{
			if(cur->ent)
				return 1;
		}
			
		
		cur = cur->next;
	}
	return 0;
}

/*this function gets the list and free's the cells*/
void freelist(list* l)
{
	freelistrec((*l).head);
	(*l).head = 0;
}


/*this function gets the head of the list and free's the cells
by recursion
helping function to freelist*/
void freelistrec(cp head)
{
	if(head == 0)
		return;
	freelistrec(head->next);
	free(head);
}
/*this function creates a new list and returns it*/
list createlist()
{
	list l;
	l.head = 0;
	return l;

}

/*this function adds to the DATA and STRING labels in the label list the value v*/
void addic(list* l,int v)
{
	cp cur = (*l).head;

	while(cur!= NULL)
	{
	if(cur->type == DATA || cur->type == STR)
	{
		cur->address += v;
	}
		cur = cur->next;
	}
}



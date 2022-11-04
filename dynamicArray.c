#include "defenitions.h"

/*this is the file where we arrange the dynamic array to keep all the
 number of the lines which have errors*/
typedef struct 
{
	int *arr;
	int sizeOfArr;
}dynamicArray;
static dynamicArray dynamicArr;

static int curcell = 0;/*the current place in the array*/
static int hasCreated = 0;/*this is the flag which indicates if the dynamic array was
used (if it was than the value of hasCreated would change to 1), we need to know
if the array was used to know when to put 0 in sizeOfArr*/

/*this function returns the content of the first used cell*/
int contentInFirstCell()
{
	if(!hasCreated)/*check if the size of the array was already got the value 0*/
	{
		dynamicArr.sizeOfArr = 0;
		hasCreated = 1;
	}
	if(dynamicArr.sizeOfArr-1 >= curcell)
	 	return dynamicArr.arr[curcell];/*the first used cell is the second cell*/
	return 0;/*curcell is more than the size of the array*/
}

/*this function add one cell to the dinamic array*/
void addCell(int n)
{
	int *temp;
	if(!hasCreated)/*check if the size of the array was already got the value 0*/
	{
		dynamicArr.sizeOfArr = 0;
		hasCreated = 1;
	}
	temp = realloc(dynamicArr.arr, sizeof(int)*(dynamicArr.sizeOfArr + 1));/*add one place for one cell*/
	if(temp == NULL)/*check if the realloc did not work, we do not wont to use the dynamic array directly because if the 
	realloc function is not succesful we do not want the array to become NULL*/
	{
		printf("could not add any more space to array\n");
		exit(0);
	}
	else
	{
		dynamicArr.arr = temp;
		dynamicArr.sizeOfArr++;/*add one to the counter of cells in the array*/
		dynamicArr.arr[dynamicArr.sizeOfArr-1] = n;/*add the number of line into the last cell in the array*/
	}
}

/*this function make us not use the first used cell anymore by increasing
curcell by one*/
void rmFirstCell()
{

	if(!hasCreated)/*check if the size of the array was already got the value 0*/
	{
		dynamicArr.sizeOfArr = 0;
		hasCreated = 1;
	}
	if(curcell < dynamicArr.sizeOfArr)/*check if there is no used cells in the array*/
	{
		curcell++;
	}
	else 
	printf("cant remove a cell\n");

}

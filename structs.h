/*defines the pointer to the cell*/
typedef struct item* cp;

/*defines the cell of the list*/
typedef struct item
{
	char * name;
	int address;
	int type;
	cp next;
	int ent;

}cell;

/*defines the list*/
typedef struct
{
	cp head;

}list;

typedef struct
{
	char * cmd;
	int code;
	int funct;
	int vars;
	int isjmp;

}command;

typedef struct
{
	unsigned int ARE : 3;
	unsigned int funct : 5;
	unsigned int rd : 3;
	unsigned int od : 2;
	unsigned int rs : 3;
	unsigned int os: 2;
	unsigned int opcode : 6;


}codebitfield;

typedef struct
{
	unsigned int ARE : 3;
	unsigned int value : 21;

}databitfield;

typedef struct
{
	unsigned int value :24;
}data;

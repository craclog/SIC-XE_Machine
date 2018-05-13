#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define MEMORY_SIZE 1048576
/* structure for history using linked list */
typedef struct _node{
	char data[100];
	struct _node* link;
} node;

/* structure for history linked list */
typedef struct{
	node* head;
	node* tail;
	int cnt;
} linkedList;

/* structure for hash map using linked list */
typedef struct _hnode{
	char mne[8];
	int opcode;
	char format[4];
	struct _hnode* link;
} hash_node;
typedef hash_node* hptr;

/* structure for symbol hash table using linked list */
typedef struct _snode{
	char sym_name[30];
	int value;
	//int code;
	struct _snode* link;
}snode;

/* structure for assembly file */
typedef struct _asmline{
	int line;			// 5, 10, 15 ...
	int loc;			
	char symbol[30];	// LOOP1 ...
	char opcode[30];	// ADD, ...
	int format;			// 1,2,3,4
	char addrmode; 		// "","#","@"
	char operand1[30];
	char operand2[30];
	int idx;			// 1 or -1
	int objcode;		// objcode (int)
	int isComment;		// 1 or 0
	int isDirective; 	// RESW, ...
	char oritxt[200]; 	// Original text
	char cobjcode[30];	// objcode (string)
	struct _asmline* link;
}asmline;

/* structure for external symbol */
typedef struct _exsymbolNode{
	char control_section[10];
	char exsymbol[10];
	int addr;
	int length;
	int refNum;
	struct _exsymbolNode* link;
} exsymbolNode;

/* Global Variable */
unsigned char mem[65536][16];	/* 1MB memory */
hptr hash_head[20];				/* opcode table */
snode* symtab[200];				/* symbol table */
int program_addr = 0;			/* Program load address */
int program_length = 0;			/* Program length */
int bps[500];					/* break points */
int bp_num = 0;					/* Number of break points */
exsymbolNode* extab[200];		/* External symbol table */
int register_cell[10];			/* Registers */

/* Util functions for manufacture inputs */
int valid_arg_check(char* str, int arg_idx);
void print_err(int err_code);
int str2int(char *str, int arg_idx);
void copystr(char *arg, char *buffer, int s, int e);
void trimEdge(char *str, char *des_str);
int valid_addr_check(char* str, int arg_idx);
int tokenizing(char* buffer, char* command, char arg[][20]);

/* functions for hash map using linked list */
void makeList(linkedList* list);
void insertNode(linkedList* list, char* str);
void printList(linkedList* list);
void freeList(linkedList* list);
void print_help();
void print_dir();

/* functions for memory */
void reset_mem();
int edit_mem(char arg[][20]);
int fill_mem(char arg[][20]);
int print_dump(char arg[][20], int arg_num, int* last_addr);

/* functions for opcode hash table */
void init_hash();
int get_idx(char *str);
void insert_hash(int idx, char* mne, int opcode, char* format);
void make_hash();
int print_opcode(char *str);
void print_oplist();
void free_hash();

/* functions for symbol hash table */
void init_symtab();
int get_symkey(char* str);
int get_symvalue(char* str);
void insert_symtab(char* sym_name, int value);
void free_symtab();
void print_symbol();
int print_file(char arg[][20]);

/* functions for assemble file */
void comma_with_space(char* str);
int get_opcode(char* str);
int get_format(char* str);
int is_register(char* str);
int is_directive(char* str);
int read_asm(char filename[], asmline* asminfo);
int asm_pass1(asmline* asminfo);
int is_const(char* str);
int is_hex(char* str);
int asm_pass2(asmline* asminfo);
void free_asminfo(asmline* asminfo);
void write_lst(char filename[], asmline* asminfo);
void write_obj(char filename[], asmline* asminfo);

/* functions for Run */
int set_progaddr(char arg[20], int arg_num);
void insert_extab(char* cs, char* sym_name, int addr, int length, int refNum);
int get_address_from_extab(char* str, int is_sym);
int is_duplicated_extab_symbol(char* cs, char* sym_name);
void free_extab();
void print_loadmap();
int link_load_pass1(char arg[4][20], int arg_num, int* exec_addr);
int link_load_pass2(char arg[4][20], int arg_num);
int breakpoint(char arg[20], int arg_num);
void calc_ope(int op, int format, int* arg1, int* arg2);
int run(int exec_addr);
#include "sicxe.h"

// Main
int main(){
	int exit = 0, last_addr = 0;	// exit : flag for quit program.   last_addr : save last dump address
	char tmp = 0;	
	char buffer[1000];				// Input buffer
	char command[15];				// store command string
	char arg[4][20];				// store arguments string
	int isValidCmd, isHi, errCode;	// flags for check error
	char front[30], rear[30];
	asmline* asmptr = NULL;			// struct for assemble file
	//Proj3
	int exec_addr = 0;
	//End of proj3
	linkedList* his_list = (linkedList*)malloc(sizeof(linkedList));
	makeList(his_list);
	make_hash();
	reset_mem();
	register_cell[is_register("PC")] = -1;
	// print "sicsim>" and get commands until exit program
	while(!exit)
	{
		isValidCmd = isHi = 0;
		errCode = 1;
		printf("sicsim> ");
		fgets(buffer, sizeof(buffer), stdin);
		if( strlen(buffer) > 0);
		else if(buffer[999] == '\0' && buffer[998] != '\n')
		{
			while(tmp != '\n') scanf("%c",&tmp);	// flush the buffer
			buffer[strlen(buffer)-1] = '\n';
		}
		int arg_num = tokenizing(buffer, command, arg);
		if( arg_num >= 0)
		{
			if( strcmp(command, "h") == 0 || strcmp(command, "help") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else 
				{
					isValidCmd = 1;
					print_help();
				}
			}
			else if( strcmp(command, "d") == 0 || strcmp(command, "dir") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else 
				{
					isValidCmd = 1;
					print_dir();
				}
			}
			else if( strcmp(command, "q") == 0 || strcmp(command, "quit") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else 
				{
					freeList(his_list);
					free_hash();
					return 0;
				}
			}
			else if( strcmp(command, "hi") == 0 || strcmp(command, "history") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else 
				{
					isValidCmd = 1;
					isHi = 1;
					insertNode(his_list, buffer);
					if( his_list->cnt == 1) continue;
					else printList(his_list);
				}
			}
			else if( strcmp(command, "du") == 0 || strcmp(command, "dump") == 0)
			{
				if( arg_num != 0 && arg_num != 1 && arg_num != 2) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = print_dump(arg, arg_num, &last_addr);
				}
			}
			else if( strcmp(command, "e") == 0 || strcmp(command, "edit") == 0)
			{
				if( arg_num != 2) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = edit_mem(arg);
				}
			}
			else if( strcmp(command, "f") == 0 || strcmp(command, "fill") == 0)
			{
				if( arg_num != 3) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = fill_mem(arg);
				}
			}
			else if( strcmp(command, "reset") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else
				{
					isValidCmd = 1;
					reset_mem();
				}
			}
			else if( strcmp(command, "opcode") == 0)
			{
				if( arg_num != 1) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = print_opcode(arg[0]);
				}
			}
			else if( strcmp(command, "opcodelist") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else
				{
					isValidCmd = 1;
					print_oplist();
				}
			}
			else if( strcmp(command, "assemble") == 0)
			{
				if( arg_num != 1) errCode = -6;
				else
				{
					free_symtab();
					init_symtab();
					rear[0] = '\0';
					front[0] = '\0';
					sscanf(arg[0], "%[^.]%*c%s",front, rear);
					if( strcmp(rear, "asm") != 0) errCode = -9;
					else
					{
						isValidCmd = 1;
						if(asmptr != NULL) free_asminfo(asmptr);
						asmptr = (asmline*)malloc(sizeof(asmline));
						asmptr->link = NULL;
						errCode = read_asm(arg[0], asmptr );
						if( errCode == 1)
						{
							errCode = asm_pass1(asmptr);
							if( errCode == 1)
							{
								errCode = asm_pass2(asmptr);
								if( errCode == 1)
								{
									printf("\toutput file : [%s.lst], [%s.obj]\n", front, front);
									write_lst(arg[0],asmptr);
									write_obj(arg[0],asmptr);
								}
							}
						}
						//else errCode = -10;
						if(errCode != 1) free_symtab();
					}
				}
			}
			else if (strcmp(command, "type") == 0)
			{
				if( arg_num != 1) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = print_file(arg);
				}
			}
			else if( strcmp(command, "symbol") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else
				{
					isValidCmd = 1;
					print_symbol();
				}
			}
			else if( strcmp(command, "progaddr") == 0)
			{
				if( arg_num != 0 && arg_num != 1) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = set_progaddr(arg[0], arg_num);
				}
			}
			else if( strcmp(command, "loader") == 0)
			{
				if( arg_num < 1 || arg_num > 3 ) errCode = -6;
				else
				{
					isValidCmd = 1;
					free_extab();
					errCode = link_load_pass1(arg, arg_num, &exec_addr);
					if( errCode == 1)
					{
						print_loadmap();
						errCode = link_load_pass2(arg, arg_num);
					}
				}
			}
			else if( strcmp(command, "run") == 0)
			{
				if( arg_num != 0) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = run(exec_addr);
				}
			}
			else if( strcmp(command, "bp") == 0)
			{
				if( arg_num != 0 && arg_num != 1) errCode = -6;
				else
				{
					isValidCmd = 1;
					errCode = breakpoint(arg[0], arg_num);
				}
			}
			else print_err(-2);
			if(isValidCmd && !isHi && errCode==1) insertNode(his_list, buffer);
			if(errCode != 1) print_err(errCode);
		}
		else print_err(arg_num);
	}
	return 0;
}
//------------------------------------------------------
//Function: print_err
//Purpose: print out error message
//Parameter: error code
//return: nothing
//------------------------------------------------------
void print_err(int err_code)
{
	if( err_code == -1) return ;
	else if( err_code == -2) 
		printf("Error : Please Input a correct command\n");
	else if( err_code == -3) 
	{
		printf("Error : Invalid argument  Ex) address : 0 ~ FFFFF\n");
		printf("                              value   : 0 ~ FF   \n");  
	}
	else if( err_code == -4) 
		printf("Error : Invalid argument. Start address is larger than End address\n");
	else if( err_code == -5)
		printf("Error : Invalid argument  Ex) COMMAND [arg1, arg2, arg3]\n");
	else if( err_code == -6)
		printf("Error : Invalid number of argument(s)\n");
	else if( err_code == -7)
		printf("Error : Invalid argument  Ex) ADD, COMP, FLOAT, etc...\n");
	else if( err_code == -8)
		printf("Error : File does not exist\n");
	else if( err_code == -9)
		printf("Error : File Extension error\n");
	else if( err_code == -10);
		// Assemble Error;
	else if( err_code == -11)
		printf("Error : Invalid argument(s) Ex) loader [arg1] [arg2] [...]\n");
	puts("");
}
//------------------------------------------------------
//Function: str2int
//Purpose: return char string(Hex) to integer value(decimal)
//Parameter: char string, index of argument
//return: decimal value
//------------------------------------------------------
int str2int(char *str, int arg_idx)
{
	int res = 0;
	int digit = 1;
	// For memory things
	if( arg_idx < 10)
	{
		int errCode = valid_addr_check(str, arg_idx);
		if( errCode != 1) return errCode;
	}
	for(int i=strlen(str)-1; i >= 0; i--)
	{
		if( str[i] >= '0' && str[i] <= '9') res += (str[i] - '0') * digit;
		else if( str[i] >= 'a' && str[i] <= 'f') res += (str[i] - 'a' + 10) * digit;
		else if( str[i] >= 'A' && str[i] <= 'F') res += (str[i] - 'A' + 10) * digit;
		digit *= 16;
	}
	return res;
}
//------------------------------------------------------
//Function: copystr
//Purpose: copy str1 to str2, from index s to index e
//Parameter: source str, dest str, index s,e
//return: nothing
//------------------------------------------------------
void copystr(char *arg, char *buffer, int s, int e)
{
	// copy arg <- buffer[s] ~ buffer[e]
	int i;
	for( i=0; i + s<=e; i++)
		arg[i] = buffer[i+s];
	arg[i] = '\0';
}
//------------------------------------------------------
//Function: trimEdge
//Purpose: Erase all spaces or tabs located at both edges and copy it to dest_string
//Parameter: source str, dest str
//return: nothing
//------------------------------------------------------
void trimEdge(char *str, char *des_str)
{
	int s = 0, i;
	int e = strlen(str)-1;
	if( str[e] == '\n') e--;
	while( str[s] == ' ' || str[s] == '\t') s++;
	while( str[e] == ' ' || str[e] == '\t') e--;
	for(i=0; i+s <= e; i++)
		des_str[i] = str[i+s];
	des_str[i] = '\0';
}
//------------------------------------------------------
//Function: valid_arg_check
//Purpose: Check whether argument is valid
//Parameter: argument string, argument index
//return: error code
//------------------------------------------------------
int valid_arg_check(char* str, int arg_idx)
{
	int len = strlen(str), i;
	if( len == 0) 
	{
		return -5;
	}
	if( arg_idx == 2 && len > 2) return -3;
	for(i=0; i<len; i++)
	{
		if( str[i] == ' ') 
		{
			printf("idx : %d\n", arg_idx);
			return -5;
		}
	}
	return 1;
}
//------------------------------------------------------
//Function: valid_addr_check
//Purpose: Check whether argument is valid for address
//Parameter: argument string, argument index
//return: error code
//------------------------------------------------------
int valid_addr_check(char* str, int arg_idx)
{
	int len = strlen(str), i;
	if( arg_idx == 2 && len > 2) return -3; // for value check
	if( len > 5) return -3;
	for(i=0; i<len; i++)
	{
		if( !(str[i] >= 'A' && str[i] <= 'F')
				&& !(str[i] >= 'a' && str[i] <= 'f')
				&& !(str[i] >= '0' && str[i] <= '9'))
			return -3;	
	}
	return 1;
}
//------------------------------------------------------
//Function: tokenizing
//Purpose: Slice input string to command and arguments.
//Parameter: Input string, command string, arguments string
//return: number of arguments
//------------------------------------------------------
int tokenizing(char* buffer, char* command, char arg[][20])
{
	int arg_idx = 0;
	int start = 0, end=0;
	char n_buffer[1000];
	int isvalid = 0;
	if( strlen(buffer) <= 1) return -1; // buffer is empty
	trimEdge(buffer, n_buffer);
	if( strlen(n_buffer) == 0) return -1;
	while(n_buffer[end] != ' ' && n_buffer[end] != '\t' && n_buffer[end] != '\0') end++;
	if( end > 15 ) return -2; // command unvalid

	copystr(command, n_buffer, 0, end-1);
	// Find all args
	if( strcmp(command, "loader") == 0)
	{
		int isvalid = sscanf(n_buffer, "%*s%s%s%s%s", arg[0],arg[1],arg[2],arg[3]);
		if( isvalid == 4) return -11;
		if( isvalid == -1) isvalid = 0;
		for(int i=0; i<isvalid; i++)
		{
			int len = strlen(arg[i]);
			for(int j=0; j<len; j++)
				if( arg[i][j] == ' ' ) isvalid = -11;
		}
		return isvalid;
	}
	while(1)
	{
		if( n_buffer[end] == '\0') break; // no arg
		end++;
		start = end;	
		while(n_buffer[end] != ',' && n_buffer[end] != '\0') end++;
		copystr(arg[arg_idx], n_buffer, start, end-1);
		trimEdge(arg[arg_idx], arg[arg_idx]);
		isvalid = valid_arg_check(arg[arg_idx], arg_idx);
		if( isvalid < 1) return isvalid;
		arg_idx++;
		if( arg_idx == 3) break;
	}
	return arg_idx;	
}
//------------------------------------------------------
//Function: makeList
//Purpose: Make linked list for history. put dummy node in.
//Parameter: list
//return: nothing
//------------------------------------------------------
void makeList(linkedList* list)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->link = NULL;
	newNode->data[0] = '\0';

	list->cnt = 0;
	list->head = list->tail = newNode;
}
//------------------------------------------------------
//Function: insertNode
//Purpose: insert new history to history list
//Parameter: list, new history string
//return: nothing
//------------------------------------------------------
void insertNode(linkedList* list, char* str)
{
	node* newNode = (node*)malloc(sizeof(node));
	copystr(newNode->data, str, 0, strlen(str));
	newNode->link = NULL;

	list->tail->link = newNode;
	list->tail = newNode;
	(list->cnt)++;
}
//------------------------------------------------------
//Function: printList
//Purpose: Print out history list
//Parameter: list
//return: nothing
//------------------------------------------------------
void printList(linkedList* list)
{
	int cnt = 1;
	if(list->cnt == 0) return ;
	for(node* tmp = list->head->link; tmp != NULL; tmp = tmp->link)
	{
		int len = strlen(tmp->data);
		printf("        %-3d  ", cnt);
		for(int i=0; i<len; i++)
			printf("%c", tmp->data[i]);
		cnt++;
	}
	puts("");
}
//------------------------------------------------------
//Function: freeList
//Purpose: deallocate history list
//Parameter: list
//return: nothing
//------------------------------------------------------
void freeList(linkedList* list)
{
	node* tmp;
    node* cur;
	for(cur = list->head; cur != NULL;)
	{
		tmp = cur;
		cur = cur->link;
		free(tmp);
	}
	free(list);
}
//------------------------------------------------------
//Function: print_help
//Purpose: Print out available commands
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void print_help()
{
	puts("\t");
	puts("\th[elp]");
	puts("\td[ir]");
	puts("\tq[uit]");
	puts("\thi[story]");
	puts("\tdu[mp] [start, end]");
	puts("\te[dit] address, value");
	puts("\tf[ill] start, end, value");
	puts("\treset");
	puts("\topcode mnemonic");
	puts("\topcodelist");
	puts("\tassemble filename");
	puts("\ttype filename");
	puts("\tsymbol");
	puts("\tprogaddr [address]");
	puts("\tloader [object filename1] [object filename2] [...]");
	puts("\trun");
	puts("\tbp [address]");
	puts("");
}
//------------------------------------------------------
//Function: print_dir
//Purpose: Print out directories and files in current path
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void print_dir()
{
	int cnt = 0;
	DIR* dir;
	struct dirent *ent;
	struct stat info;
	char t_str[20];
	dir = opendir("./");
	if( dir != NULL)
	{
		printf("\t");
		while((ent = readdir(dir)) != NULL)
		{
			if( strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
			lstat(ent->d_name, &info);
			strcpy(t_str, ent->d_name);
			if( S_ISDIR(info.st_mode) == 1) 
			{
				printf("%c[1;34m",27);
				strcat(t_str, "/");
			}
			else if( info.st_mode & S_IXUSR )
			{
				printf("%c[1;32m",27);
				strcat(t_str, "*");
			}
			else strcat(t_str, " ");
			printf("%-20s", t_str);
			printf("%c[0m",27);
			if( cnt % 3 == 2) printf("\n\t");
			cnt++;
		}
	}
	closedir(dir);
	puts("");
}
//------------------------------------------------------
//Function: reset_mem
//Purpose: Reset memories to 0
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void reset_mem()
{
	for(int i=0; i<65536; i++)
		for(int j=0; j<16; j++)
			mem[i][j] = 0;
}
//------------------------------------------------------
//Function: edit_mem
//Purpose: Edit value of memory
//Parameter: arguments including address and value
//return: error code
//------------------------------------------------------
int edit_mem(char arg[][20])
{
	int isErr = valid_arg_check(arg[1], 2);
	if( isErr != 1) return isErr;
	int addr = str2int(arg[0], 0);
	int value = str2int(arg[1], 2);
	if( addr < 0 || value < 0) return -3;
	mem[addr/16][addr%16] = value;
	return 1;
}
//------------------------------------------------------
//Function: fill_mem
//Purpose: Fill memories to value argument
//Parameter: arguments including address and value
//return: error code
//------------------------------------------------------
int fill_mem(char arg[][20])
{
	int start = str2int(arg[0], 0);
	int end = str2int(arg[1], 1);
	int value = str2int(arg[2], 2);
	if( start < 0 || end < 0 || value < 0) return -3;
	if( start > end) return -4;
	for(int i=start; i<= end; i++)
		mem[i/16][i%16] = value;
	return 1;
}
//------------------------------------------------------
//Function: print_dump
//Purpose: print out dump memories
//Parameter: arguments including address and value
//return: error code
//------------------------------------------------------
int print_dump(char arg[][20], int arg_num, int* last_addr)
{
	int start, end, last;
	int cur, idx=0;
	char dump_str[17];
	dump_str[16] = '\0';
	if( (*last_addr) == 0) (*last_addr)--;
	if( arg_num == 0)
	{
		start = (*last_addr)+1;
		end = start + 159;
	}
	else if( arg_num == 1)
	{
		start = str2int(arg[0], 0);
		end = start + 159;
	}	
	else // arg_num == 2
	{
		start = str2int(arg[0], 0);
		end = str2int(arg[1], 1);
	}
	if( start < 0 || end < 0 ) return -3;
	if( start> end) return -4;
	cur = (start/16) * 16;
	end = end < 0xFFFFF ? end : 0xFFFFF;
	last = end - end%16 + 15;
	while(1)
	{
		if( cur % 16 == 0)
		{
			printf("%c[1;34m",27);
			printf("%05X ", cur);
			printf("%c[0m",27);
		}
		if( cur >= start && cur <= end) 
		{
			if( mem[cur/16][cur%16] != 0) printf("%c[1;33m",27);
			printf("%02X ", mem[cur/16][cur%16]);
			printf("%c[0m",27);
			if(mem[cur/16][cur%16] < 0x20 || mem[cur/16][cur%16] > 0x7F) dump_str[idx++] = '.'; 
			else dump_str[idx++] = mem[cur/16][cur%16];
		}
		else
		{
			printf("   ");
			dump_str[idx++] = '.';
		}
		if( cur % 16 == 15)
		{
			printf(";");
			printf("%c[1;32m",27);
			printf(" %s\n", dump_str);
			printf("%c[0m",27);
			idx = 0;
		}
		cur++;
		if( cur == last+1) break;
	}
	puts("");
	*last_addr = end;
	if( *last_addr == 0xFFFFF ) *last_addr = 0;
	return 1;
}
//------------------------------------------------------
//Function: init_hash
//Purpose: initialize hash map
//Parameter: hash map
//return: nothing
//------------------------------------------------------
void init_hash()
{
	for(int i=0; i<20; i++)
		hash_head[i] = NULL;
}
//------------------------------------------------------
//Function: get_idx
//Purpose: get hash index from string
//Parameter: mnemonic string
//return: hash index(0~19)
//------------------------------------------------------
int get_idx(char *str)
{
	int sum = 0;
	int len = strlen(str);
	for(int i=0; i<len; i++)
	{
		sum += str[i];
	}
	return sum % 20;
}
//------------------------------------------------------
//Function: insert_hash
//Purpose: Insert new node to hash map using linked list
//Parameter: hash map, index, mnemonic string, opcode, format
//return: nothing
//------------------------------------------------------
void insert_hash(int idx, char* mne, int opcode, char* format)
{
	hptr newNode = (hptr)malloc(sizeof(hash_node));
	strcpy(newNode->mne, mne);
	newNode->opcode = opcode;
	strcpy(newNode->format, format);
	newNode->link = NULL;
	if( hash_head[idx] == NULL )
		hash_head[idx] = newNode;
	else
	{
		newNode->link = hash_head[idx];
		hash_head[idx] = newNode;
	}
}
//------------------------------------------------------
//Function: make_hash
//Purpose: Read opcode.txt and Make hash map for mnemonics and opcodes using linked list
//Parameter: hash map
//return: nothing
//------------------------------------------------------
void make_hash()
{
	FILE *fp;
	char buffer[100];
	char fmne[8], fformat[4];
	int fopcode, idx;
	
	fp = fopen("opcode.txt", "r");
	if( fp != NULL)
	{
		init_hash();
		while( fgets(buffer, 100, fp))
		{
			sscanf(buffer, "%X %s %s", &fopcode, fmne, fformat);
			idx = get_idx(fmne);
			insert_hash(idx, fmne, fopcode, fformat);
		}
		fclose(fp);
	}
	else puts("Cannot find opcode.txt file");
}
//------------------------------------------------------
//Function: print_opcode
//Purpose: print out opcode for mnemonic using hash map
//Parameter: hash map, mnemonic string
//return: error code
//------------------------------------------------------
int print_opcode(char *str)
{
	int idx = get_idx(str);
	int opcode = -1;
	hptr cur;

	for(cur = hash_head[idx]; cur; cur = cur->link)
	{
		if( strcmp(str, cur->mne) == 0)
		{
			opcode = cur->opcode;
			break;
		}
	}
	if( opcode == -1) return -7;
	printf("opcode is %X\n", opcode);
	puts("");
	return 1;
}
//------------------------------------------------------
//Function: print_oplist
//Purpose: print out opcode list stored in hash map
//Parameter: hash map
//return: nothing
//------------------------------------------------------
void print_oplist()
{
	hptr cur;
	for(int i=0; i<20; i++)
	{
		printf("%d : ", i);
		for(cur = hash_head[i]; cur; cur = cur->link)
		{
			printf("%c[1;33m",27);
			printf("[%s, %X]", cur->mne, cur->opcode);
			printf("%c[0m",27);
			if( cur->link ) printf(" -> ");
		}
		printf("\n");
	}
	puts("");
}
//------------------------------------------------------
//Function: free_hash
//Purpose: Deallocate hash map
//Parameter: hash map
//return: nothing
//------------------------------------------------------
void free_hash()
{
	hptr cur;
	hptr tmp;
	for(int i=0; i<20; i++)
	{
		for(cur = hash_head[i]; cur != NULL;)
		{
			tmp = cur;
			cur = cur->link;
			free(tmp);
		}
		hash_head[i] = NULL;
	}
}
//------------------------------------------------------
//Function: print_file
//Purpose: Print contents of file
//Parameter: filename
//return: error code
//------------------------------------------------------
int print_file(char arg[][20])
{
	FILE* fp;
	DIR* dir;
	struct dirent *ent;
	char buffer[1000];
	int found = 0;
	dir = opendir("./");
	if( dir != NULL)
	{
		while( !found && (ent = readdir(dir)) != NULL )
		{
			if( strcmp(ent->d_name, arg[0]) == 0 )
			{
				found = 1;
				fp = fopen(arg[0], "r");
				if( fp == NULL) return -8;
				while( fgets(buffer, 990, fp) )
				{
					printf("%s", buffer);
				}	
				fclose(fp);
			}
		}
		closedir(dir);
	}
	if( !found ) return -8;
	return 1;	
}
/* Project2 */
//------------------------------------------------------
//Function: init_symtab
//Purpose: Initialize symbol table
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void init_symtab()
{
	for(int i=0; i<200; i++)
		symtab[i] = NULL;
}
//------------------------------------------------------
//Function: get_symkey
//Purpose: Get symbol table key
//Parameter: string for key
//return: symbol table key
//------------------------------------------------------
int get_symkey(char* str)
{
	int len = strlen(str);
	int sum = 0;
	for(int i=0; i<len; i++)
		sum += str[i];
	return sum%200;
}
//------------------------------------------------------
//Function: get_symvalue
//Purpose: Get symbol value
//Parameter: symbol(string)
//return: symbol value
//------------------------------------------------------
int get_symvalue(char* str)
{
	int idx = get_symkey(str);
	int value = -1;
	snode* cur;
	for(cur = symtab[idx]; cur; cur = cur->link)
	{
		if( strcmp(str, cur->sym_name) == 0)
		{
			value = cur->value;
			break;
		}
	}
	return value;
}
//------------------------------------------------------
//Function: insert_symtab
//Purpose: Insert symbol key and value into symbol-table
//Parameter: symbol, symbol-value
//return: nothing
//------------------------------------------------------
void insert_symtab(char* sym_name, int value)
{
	int key;
	snode* newNode = NULL;
	newNode = (snode*)malloc(sizeof(snode));
	if( newNode == NULL) 
	{
		puts("Error : Memory not allocated");
		return ;
	}
	newNode->value = value;
	//newNode->code = code;
	strcpy(newNode->sym_name, sym_name);
	newNode->link = NULL;
	key = get_symkey(sym_name);
	if( symtab[key] == NULL)
		symtab[key] = newNode;
	else
	{
		newNode->link = symtab[key];
		symtab[key] = newNode;
	}
}
//------------------------------------------------------
//Function: free_symtab
//Purpose: Deallocate symbol-table
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void free_symtab()
{
	snode* tmp;
	snode* cur;
	for(int i=0; i<200; i++)
	{
		for(cur = symtab[i]; cur != NULL;)
		{
			tmp = cur;
			cur = cur->link;
			tmp->link = NULL;
			free(tmp);
		}
		symtab[i] = NULL;
	}
}
//------------------------------------------------------
//Function: print_symbol
//Purpose: Print out symbol-table
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void print_symbol()
{
	snode* cur;
	char symbol[200][30];
	char tsym[30];
	int addr[200];
	int taddr;
	int idx = 0;
	for(int i=0; i<200; i++)
	{
		for(cur = symtab[i]; cur != NULL; cur = cur->link)
		{
			strcpy(symbol[idx], cur->sym_name);
			addr[idx] = cur->value;
			idx += 1;
		}
	}
	if( idx == 0)
	{
		printf("Error : There is no symbol table\n\n");
		return ;
	}
	//sort
	for(int i=0; i<idx; i++)
	{
		for(int j=i+1; j<idx; j++)
		{
			if( strcmp(symbol[i], symbol[j]) < 0)
			{
				strcpy(tsym, symbol[i]);
				taddr = addr[i];

				strcpy(symbol[i], symbol[j]);
				addr[i] = addr[j];

				strcpy(symbol[j], tsym);
				addr[j] = taddr;
			}	
		}
	}
	for(int i=0; i<idx; i++)
	{
		printf("\t%s\t%04X\n", symbol[i], addr[i]);
	}
}
//------------------------------------------------------
//Function: is_const
//Purpose: Check whether input is const(Decimal)
//Parameter: string
//return: 1 if input is const. Else 0.
//------------------------------------------------------
int is_const(char* str)
{
	int len = strlen(str);
	int ret = 1;
	for(int i=0; i<len; i++)
		if( str[i] < '0' || str[i] > '9' ) ret = 0;
	return ret;
}
//------------------------------------------------------
//Function: is_hex
//Purpose: Check whether input is Hex
//Parameter: string
//return: 1 if input is Hex. Else 0.
//------------------------------------------------------
int is_hex(char* str)
{
	int len = strlen(str);
	int ret = 1;
	for(int i=0; i<len; i++)
		if( !('0' <= str[i] && str[i] <= '9') && !('A' <= str[i] && str[i] <= 'F') ) ret = 0;
	return ret;
}
//------------------------------------------------------
//Function: comma_with_space
//Purpose: Insert space(' ') both edge of comma(','). To tokenize operands
//Parameter: string with comma
//return: nothing
//------------------------------------------------------
void comma_with_space(char* str)
{
	char tmp[200];
	int len = strlen(str);
	int comma = -1;
	int idx = 0;
	for(int i=0; i<len; i++)
	{
		if( str[i] == ',') 
		{
			comma = i;
			break;
		}
	}
	if( comma == -1) return ;

	for(idx=0; idx<comma; idx++)
		tmp[idx] = str[idx];
	tmp[comma] = ' ';
	tmp[comma+1] = ',';
	tmp[comma+2] = ' ';
	idx = comma+3;
	for( ; idx < len+2; idx++)
		tmp[idx] = str[idx-2];
	tmp[idx] = '\0';
	strcpy(str, tmp);
}
//------------------------------------------------------
//Function: get_opcode
//Purpose: Get opcode from opcode-table
//Parameter: opcode(string)
//return: opcode
//------------------------------------------------------
int get_opcode(char* str)
{
	int idx = get_idx(str);
	int opcode = -1;
	hptr cur;
	for(cur = hash_head[idx]; cur; cur = cur->link)
	{
		if( strcmp(str, cur->mne) == 0)
		{
			opcode = cur->opcode;
			break;
		}
	}
	if( opcode == -1) return -1;
	return opcode;
}
//------------------------------------------------------
//Function: get_format
//Purpose: Get format from opcode-table
//Parameter: opcode(string)
//return: format(1,2,3,4)
//------------------------------------------------------
int get_format(char* str)
{
	int idx = get_idx(str);
	int format = -1;
	hptr cur;
	for(cur = hash_head[idx]; cur; cur = cur->link)
	{
		if( strcmp(str, cur->mne) == 0)
		{
			format = cur->format[0] - '0';
			break;
		}
	}
	return format;
}
//------------------------------------------------------
//Function: is_register
//Purpose: Check whether input is register
//Parameter: string
//return: register number
//------------------------------------------------------
int is_register(char* str)
{
	int ret = -1;
	if( strcmp(str, "A") == 0) ret = 0;
	else if( strcmp(str, "X") == 0) ret = 1;
	else if( strcmp(str, "L") == 0) ret = 2;
	else if( strcmp(str, "B") == 0) ret = 3;
	else if( strcmp(str, "S") == 0) ret = 4;
	else if( strcmp(str, "T") == 0) ret = 5;
	else if( strcmp(str, "F") == 0) ret = 6;
	else if( strcmp(str, "PC") == 0) ret = 8;	
	else if( strcmp(str, "SW") == 0) ret = 9;
	return ret;
}
//------------------------------------------------------
//Function: is_directive
//Purpose: Check whether input is Assembler directive
//Parameter: string
//return: size of assembler directive 
//------------------------------------------------------
int is_directive(char* str)
{
	int ret = 0;
	if( strcmp(str, "RESB") == 0) ret = 1;
	else if( strcmp(str, "RESW") == 0) ret = 3;
	else if( strcmp(str, "BYTE") == 0) ret = 1;
	else if( strcmp(str, "WORD") == 0) ret = 3;
	return ret;
}
//------------------------------------------------------
//Function: free_asminfo
//Purpose: Deallocate memories used for assembler
//Parameter: asmline pointer
//return: nothing
//------------------------------------------------------
void free_asminfo(asmline* asminfo)
{
	asmline* curinfo = asminfo;
	asmline* tmpinfo;
	while(curinfo != NULL)
	{
		tmpinfo = curinfo;
		curinfo = curinfo->link;
		tmpinfo->link = NULL;
		free(tmpinfo);
	}
	asminfo = NULL;
}
//------------------------------------------------------
//Function: read_asm
//Purpose: Read .asm file and save information at asmline structure
//Parameter: filename, asmline pointer
//return: error code
//------------------------------------------------------
int read_asm(char filename[], asmline* asminfo)
{
	FILE* fp = NULL;
	char buffer[200];
	char token[6][30] = {0,};
	int curline = 0;
	int tok_num;
	int isformat4 = 0;
	char tmpstr[30];
	int errCode;
	asmline* curinfo = asminfo;
	fp = fopen(filename, "r");
	if( !fp ) return -8; // file open error
	while( fgets(buffer, sizeof(buffer), fp) )
	{	
		curline += 5;
		isformat4 = 0;
		comma_with_space(buffer);
		for(int i=0; i<6; i++)
			token[i][0] = '\0';
		tok_num = sscanf(buffer, "%s%s%s%s%s%s", token[0], token[1], token[2],token[3],token[4],token[5]);
		asmline* newnode = (asmline*)malloc(sizeof(asmline));
		// Initialize every elements
		newnode->line = curline;
		newnode->loc = -1;
		newnode->symbol[0] = '\0';
		newnode->opcode[0] = '\0';
		newnode->format = -1;
		newnode->addrmode = ' ';
		newnode->operand1[0] = '\0';
		newnode->operand2[0] = '\0';
		newnode->idx = -1;
		newnode->objcode = 0;
		newnode->isComment = 0;
		newnode->isDirective = 0;
		strcpy(newnode->oritxt, buffer);
		newnode->oritxt[strlen(newnode->oritxt)-1] = '\0';
		newnode->cobjcode[0] = '\0';
		newnode->link = NULL;
		if( tok_num == -1) // nothing in line
		{
			continue;
		}
		if( token[0][0] == '.') // is Comment-line
		{
		   	newnode->isComment = 1;
			curinfo->link = newnode;
			curinfo = newnode;
			continue;
		}
		else if( tok_num == 6)
		{
			printf("Error : %s, line %d\n",filename, curline);
			return -10;
		}
		// If there is '+', eliminate it, And set format 4
		if( token[0][0] == '+')
		{
			isformat4 = 1;
			int len = strlen(token[0]);
			for(int i=0; i<len; i++)
				token[0][i] = token[0][i+1];
		}
		else if( token[1][0] == '+')
		{
			isformat4 = 1;
			int len = strlen(token[1]);
			for(int i=0; i<len; i++)
				token[1][i] = token[1][i+1];
		}
		// Case : token[0] is symbol
		// optab에 없고 base아니고 regi 아니면 심볼
		if(get_opcode(token[0]) == -1)
		{
			if(strcmp(token[0], "BASE") == 0)
			{
				strcpy(newnode->opcode, token[0]);
				strcpy(newnode->operand1, token[1]);
				if( token[2][0] != '\0' )
				{
					printf("Error : %s, line %d\n",filename, curline);
					return -10;
				}
			}
			else if( strcmp(token[0], "START") == 0)
			{
				strcpy(newnode->opcode, "START");
				strcpy(newnode->operand1, token[1]);
				if( token[2][0] != '\0' )
				{
					printf("Error : %s, line %d\n",filename, curline);
					return -10;
				}
			}
			else if( strcmp(token[0], "END") == 0)
			{
				strcpy(newnode->opcode, "END");
				strcpy(newnode->operand1, token[1]);
				if( token[2][0] != '\0' )
				{
					printf("Error : %s, line %d\n",filename, curline);
					return -10;
				}
			}
			else if( is_register(token[0]) != -1 )
			{
				printf("Error : %s, line %d\n",filename, curline);
				return -10;
			}
			else // token[0] is symbol!
			{
				if(get_symvalue(token[0]) == -1) 
				{
					if( strcmp(token[1], "START") == 0);
					else insert_symtab(token[0], 0);
					strcpy(newnode->symbol, token[0]);
				}
				else 
				{
					// symbol duplicated
					printf("Error : %s, line %d\n",filename, curline);
					return -10;				
				}
				// for extra input Err check
				strcpy(newnode->operand2, token[3]);

				//token1 이 opcode인지, direct인지, base인지, 
				// Case : token[1] is mnemonic
				if( get_opcode(token[1]) != -1)
				{
					strcpy(newnode->opcode, token[1]);
					newnode->format = get_format(token[1]);
					if( newnode->format == 2)
					{
						strcpy(newnode->operand1, token[2]);
						strcpy(newnode->operand2, token[4]);
					}
					else if( newnode->format == 3 || newnode->format == 4)
					{
						if( token[2][0] == '#' || token[2][0] == '@') 
						{
							newnode->addrmode = token[2][0];
							sscanf(token[2], "%*c%s", tmpstr);
							strcpy(newnode->operand1, tmpstr);
							if( token[3][0] != '\0' )
							{
								printf("Error : %s, line %d\n",filename, curline);
								return -10;
							}
						}
						else 
						{
							strcpy(newnode->operand1, token[2]);
						}
						//idx 있다는 의미로 1, 필요하면 레지 넘버 저장.
						if( tok_num == 5 ) 
						{
							errCode = is_register(token[4]);
							if( errCode < 0 || token[4][0] != 'X')
							{
								printf("Error : %s, line %d\n",filename, curline);
								return -10;
							}
							else newnode->idx = is_register(token[4]);
						}
					}
				}
				else if( strcmp(token[1], "BASE") == 0)
				{
					strcpy(newnode->opcode, "BASE");
					strcpy(newnode->operand1, token[2]);
				}
				else if( is_directive(token[1]))
				{
					strcpy(newnode->opcode, token[1]);
					strcpy(newnode->operand1, token[2]);
					newnode->isDirective = 1;
				}
				else if( strcmp(token[1], "START") == 0)
				{
					strcpy(newnode->opcode, "START");
					strcpy(newnode->operand1, token[2]);
				}
				else if( strcmp(token[1], "END") == 0)
				{
					strcpy(newnode->opcode, "END");
					strcpy(newnode->operand1, token[2]);
				}
				else
				{
					printf("Error : %s, line %d\n", filename, curline);
					return -10;
				}
			}
		}
		// Case : token[0] is opcode ( No symbol )
		else
		{
			strcpy(newnode->opcode, token[0]);
			newnode->format = get_format(token[0]);
			// for extra input Err check
			strcpy(newnode->operand2, token[2]);

			if( newnode->format == 2)
			{
				strcpy(newnode->operand1, token[1]);
				strcpy(newnode->operand2, token[3]);
			}	
			else if( newnode->format == 3 || newnode->format == 4)
			{
				if( token[1][0] == '#' || token[1][0] == '@') 
				{
					newnode->addrmode = token[1][0];
					sscanf(token[1], "%*c%s", tmpstr);
					strcpy(newnode->operand1, tmpstr);
				}
				else strcpy(newnode->operand1, token[1]);
				if( tok_num == 4 ) 
				{
					errCode = is_register(token[3]);
					if( errCode < 0 || token[3][0] != 'X')
					{
						printf("Error : %s, line %d\n",filename, curline);
						return -10; 
					}
					else newnode->idx = is_register(token[3]);
				}
			}
		}
		if( isformat4 ) newnode->format = 4;
		curinfo->link = newnode;
		curinfo = newnode;
	}	
	return 1;
}
//------------------------------------------------------
//Function: asm_pass1
//Purpose: To Assemble file, pass1
//Parameter: asmline pointer
//return: error code
//------------------------------------------------------
int asm_pass1(asmline* asminfo)
{
	int locctr;
	int start_addr;
	snode* cur;
	asmline* curinfo = asminfo->link;
	while(curinfo->isComment == 1) curinfo = curinfo->link;
	if( strcmp(curinfo->opcode, "START") == 0) 
	{
		sscanf(curinfo->operand1, "%X", &locctr);
		curinfo->loc = locctr;
	}
	else locctr = 0;
	start_addr = locctr;
	while(curinfo != NULL)
	{
		if( !( 0 <= locctr && locctr <= 0xFFFFF ) )
		{
			printf("Error : line %d\n", curinfo->line);
			return -10;
		}
		curinfo->loc = locctr;
		// Case : No symbol
		if( strcmp(curinfo->symbol, "") != 0) 
		{
			int key = get_symkey(curinfo->symbol);
			for(cur = symtab[key]; cur != NULL; cur = cur->link)
			{		
				if( strcmp(curinfo->symbol, cur->sym_name) == 0)
					cur->value = locctr;
			}
		}
		if( curinfo->format != -1) 
		{
			locctr += curinfo->format;
		}
		else if( curinfo->isDirective )
		{
			// Case : RESW or RESB
			if( strncmp(curinfo->opcode, "RES", 3) == 0)
			{
				int deci = 0;
				if( !is_const(curinfo->operand1) )
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				sscanf(curinfo->operand1, "%d", &deci);
				locctr += is_directive(curinfo->opcode) * deci;
			}
			// Case : BYTE, WORD
			else
			{
				if( strcmp(curinfo->opcode, "BYTE") == 0)
				{
					if( curinfo->operand1[0] == 'C' )
					{
						locctr += strlen(curinfo->operand1) - 3;
					}
					else if( curinfo->operand1[0] == 'X')
					{
						int len = strlen(curinfo->operand1)-3;
						/*
						//Can be fixed
						if( len % 2 == 1)
						{
							printf("Error : line %d\n", curinfo->line);
						   	return -10;
						}
						else locctr += len/2;
						*/
						locctr += (len+1)/2; // fixed
					}
					else
					{
						printf("Error : line %d\n", curinfo->line);
						return -10;
					}	
				}
				else if( strcmp(curinfo->opcode, "WORD") == 0)
				{
					// 길이 3Byte 넘어가면 Err
					locctr += 3;
				}	
			}
		}
		else if( strcmp(curinfo->opcode, "END") == 0);
		curinfo = curinfo->link;
	}
	asminfo->loc = locctr - start_addr;
	return 1;
}
//------------------------------------------------------
//Function: asm_pass2
//Purpose: To Assemble filem, pass2
//Parameter: asmline pointer
//return: error code
//------------------------------------------------------
int asm_pass2(asmline* asminfo)
{
	asmline* curinfo = asminfo->link;
	int obj;
	int opcode = 0;
	int opevalue;
	int regi1, regi2;
	int nloc = 0;
	int base = 0;			// B register
	int isNobase = 1;		// NOBASE flag
	int isRSUB = 0;			// RSUB flag
	char tmpstr[30];
	while( curinfo != NULL)
	{
		opcode = get_opcode(curinfo->opcode);
		obj = opcode;
		// Not opcode
		if( opcode == -1)
		{
			if( strcmp(curinfo->opcode, "BASE") == 0)
			{
				isNobase = 0;
				if( curinfo->operand1[0] == '\0' )
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				if( is_hex(curinfo->operand1) ) sscanf(curinfo->operand1, "%X", &base);
				else base = get_symvalue(curinfo->operand1);
			}
			else if( strcmp(curinfo->opcode, "NOBASE") == 0) isNobase = 1;
			// Case : Directive
			else if( curinfo->isDirective )
			{
				if( curinfo->operand1[0] == '\0')
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				// Case : RESD, RESW -> Do nothing
				if( strncmp(curinfo->opcode, "RES", 3) == 0 );
				// Case : BYTE, WORD
				else
				{
					if( strcmp(curinfo->opcode, "BYTE") == 0)
					{
						// Case : 'X'
						if( curinfo->operand1[0] == 'X')
						{
							/*
							//Can be fixed
							if( strlen(curinfo->operand1) % 2 == 0) 
							{
								printf("Error : line %d\n", curinfo->line);
								return -10;
							}
							*/
							if( curinfo->operand1[1] == curinfo->operand1[strlen(curinfo->operand1)-1]
									&& curinfo->operand1[1] == '\'')
							{
								sscanf(curinfo->operand1, "%*c%*c%[^']", tmpstr);
								if( strlen(tmpstr) != strlen(curinfo->operand1)-3 )
								{
									printf("Error : line %d\n", curinfo->line);
									return -10;
								}
								if( !is_hex(tmpstr) ) 
								{
									printf("Error : line %d\n", curinfo->line);
									return -10;
								}
								sscanf(tmpstr, "%X", &obj);
							}
							else
							{
								printf("Error : line %d\n", curinfo->line);
								return -10;
							}
						}
						// Case : 'C'
						else if( curinfo->operand1[0] == 'C')
						{
							if( curinfo->operand1[1] == curinfo->operand1[strlen(curinfo->operand1)-1]
								&& curinfo->operand1[1] == '\'')
							{
								sscanf(curinfo->operand1, "%*c%*c%[^']", tmpstr);
								if( strlen(tmpstr) != strlen(curinfo->operand1) - 3 )
								{
									printf("Error : line %d\n", curinfo->line);
									return -10;
								}
								obj = 0;
								for(int i=0; i < strlen(tmpstr); i++)
								{
									obj = obj << 8;
									obj += tmpstr[i];
								}
							}
							else
							{
								printf("Error : line %d\n", curinfo->line);
								return -10;
							}
						}
						// Not a 'X' nor 'C'
						// Can be fixed
						else 
						{
							printf("Error : line %d\n", curinfo->line);
							return -10;
						}
						curinfo->objcode = obj;
					}
					else if( strcmp(curinfo->opcode, "WORD") == 0)
					{
						if( is_const(curinfo->operand1) ) sscanf(curinfo->operand1, "%d", &obj);
						else obj = get_symvalue(curinfo->operand1);
						curinfo->objcode = obj; // Can be fixed
					}
					// Case : Not a BYTE nor WORD
					// Can be fixed
					else
					{
						printf("Error : line %d\n", curinfo->line);
						return -10;
					}
				}	// Not RES*
			}	// Directive endif
			curinfo = curinfo->link;
			continue;	
		}
		// OPCODE
		else if( curinfo->format == 1)
		{
			obj = obj << 2;
			if( curinfo->operand1[0] != '\0' )
			{
				printf("Error : line %d\n", curinfo->line);
				return -10;
			}
		}
		// Not format1 nor RSUB, but no operand -> Err
		else if( strcmp(curinfo->opcode, "RSUB") != 0 && curinfo->operand1[0] == '\0')
		{
			printf("Error : line %d\n", curinfo->line);
			return -10;	
		}
		else if( curinfo->format == 2)
		{			
			obj = obj << 8;
			regi1 = is_register(curinfo->operand1);
			if( strcmp(curinfo->opcode, "CLEAR") == 0
				|| strcmp(curinfo->opcode, "TIXR") == 0
				|| strcmp(curinfo->opcode, "SVC") == 0)
			{
				regi2 = 0;
				curinfo->operand2[0] = '\0';
			}
			else regi2 = is_register(curinfo->operand2);
			
			if( regi1 == -1 || regi2 == -1) 
			{
				printf("Error : line %d\n", curinfo->line);
				return -10;
			}
			obj += regi1 << 4;
			obj += regi2;
		}
		else if( curinfo->format == 3)
		{
			if( curinfo->addrmode == '#' ) obj += 1;
			else if( curinfo->addrmode == '@') obj += 2;
			else obj += 3;
			obj = obj << 16;
			// p, b, 00 check and + index::X
			nloc = curinfo->link->loc;
			if( strcmp(curinfo->opcode, "RSUB") == 0)
			{
				isRSUB = 1;
				//curinfo->operand1[0] = '\0';
				//curinfo->operand2[0] = '\0';
			}
			else if( is_const(curinfo->operand1) ) sscanf(curinfo->operand1, "%d", &opevalue);
			else opevalue = get_symvalue(curinfo->operand1);
			if( !isRSUB && opevalue == -1 )
			{
				printf("Error : line %d\n", curinfo->line);
				return 10;
			}
			// Case : RSUB -> No operands
			if( isRSUB )
			{
				if( curinfo->operand1[0] != '\0')
				{
					printf("Error : line %d\n", curinfo->line);
					return 10;				
				}
			}
			else if( curinfo->addrmode == '#' && is_const(curinfo->operand1) )
			{
				if( !(0 <= opevalue && opevalue <= 0xFFF) )
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				obj += opevalue;
			}
			else if( opevalue - nloc >= -2048 && opevalue - nloc <= 2047) // P
			{
				obj += 1 << 13;
				if( opevalue - nloc < 0) obj += 0x1000 + (opevalue-nloc);
				else obj += opevalue-nloc;
			}
			else if( !isNobase && opevalue - base >= 0 && opevalue - base <= 0xFFF) // B
			{
				obj += 1 << 14;
				obj += opevalue - base;
			}
			// Case : P, B is unavailable, but format is 3.
			else
			{
				printf("Error : line %d\n", curinfo->line);
				return -10;
			}
			isRSUB = 0;
			if( curinfo->idx != -1) obj += 1 << 15;
			// No index
			else if( curinfo->operand2[0] != '\0' )
			{
				printf("Error : line %d\n", curinfo->line);
				return -10;
			}
		}
		else if( curinfo->format == 4)
		{
			if( curinfo->operand2[0] != '\0')
			{
				printf("Error : line %d\n", curinfo->line);
				return -10;
			}
			if( curinfo->addrmode == '#')
			{
				if( curinfo->idx != -1)
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				obj += 1;
				obj = obj << 24;
				if( is_const(curinfo->operand1) ) sscanf(curinfo->operand1, "%d", &opevalue);
				else opevalue = get_symvalue(curinfo->operand1);
				if( opevalue == -1 || !(0 <= opevalue && opevalue <= 0xFFFFF))
				{
					printf("Error : line %d\n", curinfo->line);
					return 10;
				}
				obj += opevalue;
			}
			else if( curinfo->addrmode == '@')
			{
				if( curinfo->idx != -1)
				{
					printf("Error : line %d\n", curinfo->line);
					return -10;
				}
				obj += 2;
				obj = obj << 24;
				if( is_const(curinfo->operand1) ) sscanf(curinfo->operand1, "%d", &opevalue);
				else opevalue = get_symvalue(curinfo->operand1);
				obj += opevalue;
			}
			// Case : Symbol
			// Can be fixed
			else	// 상수 배제
			{
				obj += 3;
				obj = obj << 24;
				opevalue = get_symvalue(curinfo->operand1);
				if( curinfo->idx != -1)
				{
					obj += 1 << 23;
					opevalue += curinfo->idx;	
				}
				obj += opevalue;
			}
			obj += 1 << 20;
		}
		curinfo->objcode = obj;
		curinfo = curinfo->link;
	}
	return 1;
}
//------------------------------------------------------
//Function: write_lst
//Purpose: Write .lst file with asmline information
//Parameter: filename, asmline pointer
//return: nothing
//------------------------------------------------------
void write_lst(char filename[], asmline* asminfo)
{
	FILE* fp = NULL;
	char savename[30];
	char tmpstr[30];
	asmline* curinfo = asminfo->link;
	sscanf(filename, "%[^.]", savename);
	strcat(savename, ".lst");
	
	fp = fopen(savename, "w");
	if( fp == NULL)
	{
		printf("Cannot open file : %s\n",savename);
		return ;
	}
	while(curinfo != NULL)
	{
		fprintf(fp, "%4d  ", curinfo->line);
		if( curinfo->isComment || strcmp(curinfo->opcode, "BASE") == 0
							|| strcmp(curinfo->opcode, "NOBASE") == 0
							|| strcmp(curinfo->opcode, "END") == 0) fprintf(fp, "      ");
		else fprintf(fp, "%04X  ", curinfo->loc);

		if( curinfo->isComment )
		{	
			fprintf(fp, "%s\n", curinfo->oritxt);
			curinfo = curinfo->link;
			continue;
		}
		fprintf(fp, "%-10s  ", curinfo->symbol);
		if( curinfo->format == 4) fprintf(fp, "+");
		else fprintf(fp, " ");
		fprintf(fp, "%-9s  ", curinfo->opcode);
		fprintf(fp, "%c%s", curinfo->addrmode,  curinfo->operand1);
		if( curinfo->format == 2)
		{
			if( strlen(curinfo->operand2) > 0)
				fprintf(fp, ",%2s", curinfo->operand2);
			else fprintf(fp, "   ");
		}
		else if( curinfo->idx != -1)
		{
			fprintf(fp, ", X");
		}
		else fprintf(fp, "   ");
		fprintf(fp, "\t\t");
		
		if( strcmp(curinfo->opcode, "START") == 0
			|| strcmp(curinfo->opcode, "BASE") == 0
			|| strcmp(curinfo->opcode, "NOBASE") == 0
			|| strcmp(curinfo->opcode, "END") == 0
			|| strncmp(curinfo->opcode, "RES", 3) == 0) fprintf(fp, "          \n");
		else if( curinfo->format == 1) fprintf(fp, "%02X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%02X", curinfo->objcode);
		else if( curinfo->format == 2) fprintf(fp, "%04X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%04X", curinfo->objcode);
		else if( curinfo->format == 3) fprintf(fp, "%06X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%06X", curinfo->objcode);
		else if( curinfo->format == 4) fprintf(fp, "%08X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%08X", curinfo->objcode);
		else if( strcmp(curinfo->opcode, "BYTE") == 0)
		{
			int len = 0;
			int tmp = curinfo->objcode;
			for(len=0; tmp != 0; len++)
			{
				tmp = tmp >> 4;
			}
			if( len % 2 == 1) fprintf(fp, "0"), sprintf(curinfo->cobjcode, "0");
			fprintf(fp, "%X\n", curinfo->objcode), sprintf(tmpstr, "%X", curinfo->objcode);
			strcat(curinfo->cobjcode, tmpstr);
		}
		else if( strcmp(curinfo->opcode, "WORD") == 0) fprintf(fp, "%06X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%06X", curinfo->objcode);
		else fprintf(fp, "%X\n", curinfo->objcode), sprintf(curinfo->cobjcode, "%X", curinfo->objcode);
		curinfo = curinfo->link;
	}
	fclose(fp);	
}
//------------------------------------------------------
//Function: write_obj
//Purpose: Write .obj file with asmline information
//Parameter: filename, asmline pointer
//return: nothing
//------------------------------------------------------
void write_obj(char filename[], asmline* asminfo)
{
	FILE* fp = NULL;
	asmline* curinfo = asminfo->link;
	char savename[30];
	char buffer[110];
	int len = 0, nextlen = 0;
	int startAddr = 0;
	int tmp;
	sscanf(filename, "%[^.]", savename);
	strcat(savename, ".obj");
	
	fp = fopen(savename, "w");
	if( fp == NULL)
	{
		printf("Cannot open file : %s\n",savename);
		return ;
	}	
	while(curinfo->opcode[0] == '\0') curinfo = curinfo->link;
	if( strcmp(curinfo->opcode, "START") == 0) 
	{
		fprintf(fp, "H%-6s",curinfo->symbol);
		fprintf(fp, "%06X%06X\n",curinfo->loc, asminfo->loc);
		startAddr = curinfo->loc;
		curinfo = curinfo->link;
	}
	else
	{
		fprintf(fp, "H      ");
		fprintf(fp, "%06X%06X\n",curinfo->loc, asminfo->loc);
		curinfo = curinfo->link;
	}
	buffer[0] = '\0';
	// if 60-length objcode are written OR encounter RES*,
	// to the next line
	while(curinfo != NULL)
	{
		if( curinfo->cobjcode[0] == '\0' )
		{
			curinfo = curinfo->link;
			continue;
		}
		fprintf(fp, "T%06X", curinfo->loc);
		len = 0;
		for(; curinfo != NULL; curinfo = curinfo->link)
		{
			if( strncmp(curinfo->opcode, "RES", 3) == 0)
			{
				break;
			}
			else if( curinfo->cobjcode[0] == '\0' )
			{
				continue;
			}
			strcat(buffer, curinfo->cobjcode);
			
			len += strlen(curinfo->cobjcode)/2;
			if( curinfo->link != NULL)
			{
				nextlen = strlen(curinfo->link->cobjcode)/2;
				if( len + nextlen > 30) break;
			}
		}	
		fprintf(fp, "%02X%s\n", len, buffer);
		buffer[0] = '\0';
		if( curinfo != NULL) curinfo = curinfo->link;
	}
	
	// Modification Record
	curinfo = asminfo;
	while(curinfo != NULL)
	{
		if( strcmp(curinfo->opcode, "END") == 0)
		{
			tmp = get_symvalue(curinfo->operand1);
			if( tmp != -1) startAddr = tmp;
		}
		if( curinfo->format == 4 && curinfo->addrmode != '#')
			fprintf(fp, "M%06X05\n", curinfo->loc + 1);
		curinfo = curinfo->link;
	}	
	fprintf(fp, "E%06X\n", startAddr);
	fclose(fp);
}
//------------------------------------------------------
//Function: set_progaddr
//Purpose: Set program load address
//Parameter: mnemonic, Number of arguments
//return: nothing
//------------------------------------------------------
int set_progaddr(char arg[20], int arg_num)
{
	int tmp = 0;
	if( arg_num == 0) program_addr = 0;
	else
	{
		if( is_hex(arg) ) 
		{
			sscanf(arg, "%X", &tmp);
			if( tmp < 0 || tmp > 0xFFFFF) return -3;
			else program_addr = tmp;
		}
		else return -3;
	}
	return 1;
}
//------------------------------------------------------
//Function: insert_extab
//Purpose: Insert external symbol info in EXTAB
//Parameter: control section, symbol name, address, length of program, Ref number
//return: nothing
//------------------------------------------------------
void insert_extab(char* cs, char* sym_name, int addr, int length, int refNum)
{
	int key;
	exsymbolNode* newNode = NULL;
	newNode = (exsymbolNode*)malloc(sizeof(exsymbolNode));
	if( newNode == NULL) 
	{
		puts("Error : Memory not allocated");
		return ;
	}
	newNode->addr = addr;
	newNode->length = length;
	newNode->refNum = refNum;
	strcpy(newNode->exsymbol, sym_name);
	strcpy(newNode->control_section, cs);
	newNode->link = NULL;
	if( strcmp(sym_name, "") == 0) key = get_symkey(cs);
	else key = get_symkey(sym_name);
	if( extab[key] == NULL)
		extab[key] = newNode;
	else
	{
		newNode->link = extab[key];
		extab[key] = newNode;
	}
}
//------------------------------------------------------
//Function: get_address_from_extab
//Purpose: Get address of symbol from EXTAB
//Parameter: symbol name, is_symbol
//return: address of symbol
//------------------------------------------------------
int get_address_from_extab(char* str, int is_sym)
{
	int key = get_symkey(str);
	exsymbolNode* cur;
	for(cur = extab[key]; cur != NULL; cur = cur->link)
	{
		if( is_sym && strcmp(cur->exsymbol, str) == 0) return cur->addr;	
		else if( !is_sym && strcmp(cur->control_section, str) == 0) return cur->addr;	
	}
	return -1;
}
//------------------------------------------------------
//Function: is_duplicated_extab_symbol
//Purpose: Whether symbol is duplicated external symbol
//Parameter: control section, symbol name
//return: is_duplicated ( duplicated : 1, else : 0 )
//------------------------------------------------------
int is_duplicated_extab_symbol(char* cs, char* sym_name)
{
	int key;
	int is_duplicated = 0;
	exsymbolNode* cur;
	if( strcmp(sym_name, "") == 0)
	{
		key = get_symkey(cs);
		for(cur = extab[key]; cur != NULL; cur = cur->link)
		{
			if( strcmp(cur->exsymbol, cs) == 0) is_duplicated = 1;
			if( strcmp(cur->control_section, cs) == 0) is_duplicated = 1;
		}	
	}
	else 
	{
		key = get_symkey(sym_name);
		for(cur = extab[key]; cur != NULL; cur = cur->link)
		{
			if( strcmp(cur->exsymbol, sym_name) == 0) is_duplicated = 1;
			if( strcmp(cur->control_section, sym_name) == 0) is_duplicated = 1;
		}	
	}
	return is_duplicated;	
}
//------------------------------------------------------
//Function: free_extab
//Purpose: Deallocate external symbol table
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void free_extab()
{
	exsymbolNode* cur;
	exsymbolNode* tmp;
	for(int i=0; i<200; i++)
	{
		for(cur = extab[i]; cur != NULL;)
		{
			tmp = cur;
			cur = cur->link;
			free(tmp);
		}
		extab[i] = NULL;
	}
}
//------------------------------------------------------
//Function: print_loadmap
//Purpose: Print out external symbol table
//Parameter: nothing
//return: nothing
//------------------------------------------------------
void print_loadmap()
{
	char control_section[200][10];
	char symbol_name[200][10];
	char tmp[10];
	int address[200];
	int length[200];
	int idx = 0;
	int itmp;
	int total_length = 0;
	exsymbolNode* cur;
	//Read
	for(int i=0; i<200; i++)
	{
		for(cur = extab[i]; cur != NULL; cur = cur->link)
		{
			strcpy(control_section[idx], cur->control_section);
			strcpy(symbol_name[idx], cur->exsymbol);
			address[idx] = cur->addr;
			length[idx] = cur->length;
			if( length[idx] != -1) total_length += length[idx];
			idx++;
		}
	}	
	//sort
	for(int i=0; i<idx; i++)
	{
		for(int j=i+1; j<idx; j++)
		{
			if( address[i] > address[j])
			{
				strcpy(tmp, control_section[i]);
				strcpy(control_section[i], control_section[j]);
				strcpy(control_section[j], tmp);
				strcpy(tmp, symbol_name[i]);
				strcpy(symbol_name[i], symbol_name[j]);
				strcpy(symbol_name[j], tmp);
				itmp = address[i];
				address[i] = address[j];
				address[j] = itmp;
				itmp = length[i];
				length[i] = length[j];
				length[j] = itmp;
			}
		}
	}
	//print
	printf("\t%-15s%-15s%-15s%-15s\n", "control", "symbol","address","length");	
	printf("\t%-15s%-15s%-15s%-15s\n", "section", "name", "", "");
	printf("\t------------------------------------------------------------\n");
	for(int i=0; i<idx; i++)
	{
		if( strcmp(symbol_name[i], "") == 0)
			printf("\t%-15s%-15s%04X%11s%04X\n", control_section[i], "",address[i],"", length[i]);
		else 
			printf("\t%-15s%-15s%04X\n", "", symbol_name[i], address[i]);
	}
	printf("\t------------------------------------------------------------\n");
	printf("\t%-30s%-15s%04X\n", "", "total length", total_length);
}
//------------------------------------------------------
//Function: link_load_pass1
//Purpose: Linking and Loading - pass1
//Parameter: filenames, number of arguments, exec_addr
//return: errCode
//------------------------------------------------------
int link_load_pass1(char arg[4][20], int arg_num, int* exec_addr)
{
	FILE* fp = NULL;
	char filename[20];
	int	csaddr = program_addr;
	char buffer[1000];
	char csname[10];
	int start_addr, prog_len;
	int refNum = 0;
	int tmp_addr;
	char tmp_symbol[10];
	for(int i=0; i<arg_num; i++)
	{
		refNum = i+1;
		sscanf(arg[i], "%s", filename);
		fp = fopen(filename, "r");
		if( fp == NULL)
		{
			printf("Error : There is no file : %s\n", filename);
			return -1;
		}
		while( fgets(buffer, sizeof(buffer), fp) )
		{
			if( buffer[0] == '.') continue;
			if( buffer[0] == 'T') continue;
			if( buffer[0] == 'H')
			{
				sscanf(buffer, "%*c%6s%6X%6X", csname, &start_addr, &prog_len);
				if( !is_duplicated_extab_symbol(csname, ""))
					insert_extab(csname, "\0", csaddr, prog_len, refNum);	// if start_addr != 0 ????
				else
				{
					printf("Error : Duplicated symbol : %s\n", csname);
					return -1;
				}
			}
			else if( buffer[0] == 'D')
			{
				int len = strlen(buffer) - 1;
				for(int cnt = 0; cnt< len/12; cnt++)
				{
					sscanf(buffer +1 + cnt*12, "%6s%6X", tmp_symbol, &tmp_addr);
					if( !is_duplicated_extab_symbol(csname, tmp_symbol))
						insert_extab(csname, tmp_symbol, csaddr + tmp_addr, -1, refNum);
					else
					{
						printf("Error : Duplicated symbol : (%s,%s)\n", csname, tmp_symbol);
						return -1;
					}
				}
			}
			if( buffer[0] == 'E')
			{
				if( strlen(buffer) > 2)
				{
					sscanf(buffer+1, "%6X", &tmp_addr);
					*exec_addr = csaddr + tmp_addr;
				}
				csaddr += prog_len;
			}
		}
		fclose(fp);
	}
	return 1;
}
//------------------------------------------------------
//Function: link_load_pass2
//Purpose: Linking and Loading - pass2
//Parameter: filenames, number of arguments
//return: errCode
//------------------------------------------------------
int link_load_pass2(char arg[4][20], int arg_num)
{
	FILE* fp = NULL;
	char filename[20];
	char buffer[1000];
	int csaddr = program_addr;
	int ref_record[20] = {0,};	// 2: LISTB ... 
	int record_idx = 0;
	char record_sym[10];
	int prog_len;
	int itmp;
	int t_addr, t_length;
	char stmp[10];

	for(int i=0; i<arg_num; i++)
	{
		sscanf(arg[i], "%s", filename);
		fp = fopen(filename, "r");
		ref_record[1] = csaddr;
		while( fgets(buffer, sizeof(buffer), fp) )
		{
			if( buffer[0] == '.') continue;
			if( buffer[0] == 'H')
			{
				sscanf(buffer, "%*c%*6s%*6X%6X", &prog_len);
			}
			if( buffer[0] == 'D') continue;
			if( buffer[0] == 'R')
			{
				int len = strlen(buffer) - 1;
				len += 7;
				for(int cur = 0; cur < len/8; cur++)
				{
					sscanf(buffer + 1 + cur*8, "%2d%6s", &record_idx, record_sym);
					itmp = get_address_from_extab(record_sym, 1);
					if( itmp < 0 )
					{
						printf("Error : No symbol in EXTAB\n");
						return -1;
					}
					ref_record[record_idx++] = itmp;
				}
			}
			if( buffer[0] == 'T') 
			{
				sscanf(buffer, "%*c%6X%2X", &t_addr, &t_length);
				int cur = csaddr + t_addr;
				for(int idx = 0; idx < t_length*2; idx += 2)
				{
					stmp[0] = buffer[9+idx];
					stmp[1] = buffer[10+idx];
					stmp[2] = '\0';
					sscanf(stmp, "%X", &itmp);
					mem[cur/16][cur%16] = itmp;
					cur++;
				}
			}
			if( buffer[0] == 'M')
			{
				int m_length;
			    char operator;
				int record_num;
				int modified;
				sscanf(buffer, "%*c%6X%2d%c%2d", &t_addr, &m_length, &operator, &record_num);
				itmp = csaddr + t_addr;
				m_length = ((m_length+1)/2) * 2;
				modified = mem[itmp/16][itmp%16] << 16;
				itmp++;
				modified += mem[itmp/16][itmp%16] << 8;
				itmp++;
				modified += mem[itmp/16][itmp%16];

				if( operator == '+') modified += ref_record[record_num];
				else if( operator == '-') modified -= ref_record[record_num];
				modified = modified & 0x00FFFFFF;
				sprintf(stmp, "%06X", modified);
				int cur = csaddr + t_addr;
				for(int idx = 0; idx < m_length; idx += 2)
				{
					sscanf(stmp + idx, "%2X", &itmp);
					mem[cur/16][cur%16] = itmp;
					cur++;
				}
			}
			if( buffer[0] == 'E')
			{
				csaddr += prog_len;
			}
		}
		fclose(fp);
	}
	program_length = prog_len;
	return 1;
}
//------------------------------------------------------
//Function: breakpoint
//Purpose: Set or clear Breaking points for debugging
//Parameter: argument(address), number of arguments
//return: errCode
//------------------------------------------------------
int breakpoint(char arg[20], int arg_num)
{
	int tmp;
	if( arg_num == 1)
	{
		if( strcmp(arg, "clear") == 0) 
		{
			for(int i=0; i<bp_num; i++)
				bps[i] = 0;
			printf("\t\t[ok] clear all breakpoints\n");
			bp_num = 0;
			return 1;
		}

		if( !is_hex(arg) ) return -3;	
		sscanf(arg, "%X", &tmp);
		if( tmp > 0xFFFFF ) return -3;
		bps[bp_num++] = tmp;
		printf("\t\t[ok] create breakpoint %04X\n", tmp);
	}
	else // arg_num == 0
	{
		printf("breakpoint\n");
		printf("----------\n");
		for(int i=0; i< bp_num; i++)
			printf("%04X\n", bps[i]);
	}
	return 1;
}
//------------------------------------------------------
//Function: calc_ope
//Purpose: Deassemble objectcodes
//Parameter: opcode, format, arg1, arg2
//return: errCode
//------------------------------------------------------
void calc_ope(int op, int format, int* arg1, int* arg2)
{
	int tmp_pc = register_cell[is_register("PC")];
	int xbpe,x,b,p,e, ni, n=0, i=0;
	int nega = 0;
	if( format == 2)
	{
		tmp_pc += 1;
		*arg1 = mem[tmp_pc/16][tmp_pc%16] / 0x10;
		*arg2 = mem[tmp_pc/16][tmp_pc%16] % 0x10;	
		register_cell[is_register("PC")] += 2;
	}
	else if( format == 3)
	{
		*arg1 = (mem[(tmp_pc+1)/16][(tmp_pc+1)%16] % 0x10) * 0x100 + mem[(tmp_pc+2)/16][(tmp_pc+2)%16];
		if( *arg1 / 0x100 >= 8) nega = 0x1000;	// To calc negative value

		xbpe = mem[(tmp_pc+1)/16][(tmp_pc+1)%16] / 0x10;
		x = xbpe / 8;
		b = xbpe % 8 / 4;
		p = xbpe % 4 / 2;
		e = xbpe % 2 / 1;
		ni = mem[(tmp_pc)/16][(tmp_pc)%16] % 0x04;
		n = ni / 2;
		i = ni % 2;

		if( x == 1)
			*arg1 += register_cell[is_register("X")];
		//Case e == 1 -> foramt 4
		if( e == 1)
		{
			*arg1 = mem[(tmp_pc+3)/16][(tmp_pc+3)%16] + (*arg1) * 0x100 + program_addr;
			register_cell[is_register("PC")] += 4;
		}
		else register_cell[is_register("PC")] += 3;
		if( b == 0 && p == 1)
		{
			*arg1 = *arg1 + register_cell[is_register("PC")] - nega;
		}
		else if( b == 1 && p == 0)
		{
			*arg1 = *arg1 + register_cell[is_register("B")] - nega;
		}
		else if( b == 0 && p == 0)
		{
			*arg1 -= nega;
		}

		if( n == 1 && i == 0)
		{
			*arg2 = mem[(*arg1)/16][(*arg1)%16] * 0x10000;
			*arg2 += mem[(*arg1+1)/16][(*arg1+1)%16] * 0x100;
			*arg2 += mem[(*arg1+2)/16][(*arg1+2)%16];
			*arg1 = *arg2;
			*arg2 = mem[(*arg1)/16][(*arg1)%16] * 0x10000;
			*arg2 += mem[(*arg1+1)/16][(*arg1+1)%16] * 0x100;
			*arg2 += mem[(*arg1+2)/16][(*arg1+2)%16];
		}
		else if( n == 0 && i == 1)
		{
			*arg2 = *arg1;
		}
		else if( n == 1 && i == 1)
		{
			*arg2 = mem[(*arg1)/16][(*arg1)%16] * 0x10000;
			*arg2 += mem[(*arg1+1)/16][(*arg1+1)%16] * 0x100;
			*arg2 += mem[(*arg1+2)/16][(*arg1+2)%16];	
		}
		
	}
}
//------------------------------------------------------
//Function: run
//Purpose: Run program at memory with exec address
//Parameter: exec_addr
//return: errCode
//------------------------------------------------------
int run(int exec_addr)
{
	int target_addr;
	int target_value;
	int first_regi, second_regi;
	int end_addr = -1;
	int tmp_pc = register_cell[is_register("PC")];
	int isBreak = 0;
	int isNewStart = 1;
	int break_addr;
	if( tmp_pc == -1)	// run first
	{
		for(int i=0; i<10; i++)
			register_cell[i] = 0;
		tmp_pc = register_cell[is_register("PC")] = exec_addr;
		register_cell[is_register("SW")] = 1;
		register_cell[is_register("L")] = exec_addr + program_length;
	}
	else; 				//run continued

	end_addr = exec_addr + program_length;
	while( register_cell[is_register("PC")] < end_addr )
	{
		//bp check
		for(int i=0; i<bp_num; i++)
			if( bps[i] == register_cell[is_register("PC")] )
			{
				isBreak = 1;
				break_addr = bps[i];
				break;
			}
		if( isBreak && !isNewStart ) break;
		if( isNewStart ) isBreak = 0;
		isNewStart = 0;
		int op = mem[register_cell[is_register("PC")]/16][register_cell[is_register("PC")]%16] & 0xFC;
		if( op == 0xB4)			// CLEAR 2
		{
			calc_ope(op, 2, &first_regi, &second_regi);
			register_cell[first_regi] = 0;
		}
		else if( op == 0x28 ) 	// COMP 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			int tmp_a = register_cell[is_register("A")];
			if( tmp_a < target_value) register_cell[is_register("SW")] = -1;
			else if( tmp_a > target_value) register_cell[is_register("SW")] = 1;
			else register_cell[is_register("SW")] = 0;
		}
		else if( op == 0xA0 ) 	// COMPR 2
		{
			calc_ope(op, 2, &first_regi, &second_regi);
			int tmp1 = register_cell[first_regi];
			int tmp2 = register_cell[second_regi];
			if( tmp1 < tmp2 ) register_cell[is_register("SW")] = -1;
			else if( tmp1 > tmp2 ) register_cell[is_register("SW")] = 1;
			else register_cell[is_register("SW")] = 0;
		}
		else if( op == 0x3C ) 	// J 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("PC")] = target_addr;
		}
		else if( op == 0x30 ) 	// JEQ 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			if( register_cell[is_register("SW")] == 0)
				register_cell[is_register("PC")] = target_addr;
		}
		else if( op == 0x38 ) 	// JLT 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			if( register_cell[is_register("SW")] == -1)
				register_cell[is_register("PC")] = target_addr;
		}
		else if( op == 0x48 ) 	// JSUB 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("L")] = register_cell[is_register("PC")];
			register_cell[is_register("PC")] = target_addr;
		}
		else if( op == 0x00 ) 	// LDA 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("A")] = target_value;
		}
		else if( op == 0x68 ) 	// LDB 3
		{
			//break;//
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("B")] = target_value;
			//printf("tv : %X\n", target_value);
		}
		else if( op == 0x50 ) 	// LDCH 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("A")] = register_cell[is_register("A")] - register_cell[is_register("A")] % 0x100 + mem[target_addr/16][target_addr%16];
		}
		else if( op == 0x74 ) 	// LDT 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("T")] = target_value;
		}
		else if( op == 0xD8 ) 	// RD 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			// NO Implementation
		}
		else if( op == 0x4C ) 	// RSUB 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("PC")] = register_cell[is_register("L")];
		}
		else if( op == 0x54 ) 	// STCH 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("A")] = mem[target_addr/16][target_addr%16] % 0x100;
		}
		else if( op == 0x0C ) 	// STA 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			mem[target_addr/16][target_addr%16] = register_cell[is_register("A")] / 0x10000;
			mem[(target_addr+1)/16][(target_addr+1)%16] = register_cell[is_register("A")] % 0x10000 / 0x100;
			mem[(target_addr+2)/16][(target_addr+2)%16] = register_cell[is_register("A")] % 0x100;
		}
		else if( op == 0x14 ) 	// STL 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			mem[target_addr/16][target_addr%16] = register_cell[is_register("L")] / 0x10000;
			mem[(target_addr+1)/16][(target_addr+1)%16] = register_cell[is_register("L")] % 0x10000 / 0x100;
			mem[(target_addr+2)/16][(target_addr+2)%16] = register_cell[is_register("L")] % 0x100;
		}
		else if( op == 0x10 ) 	// STX 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			mem[target_addr/16][target_addr%16] = register_cell[is_register("X")] / 0x10000;
			mem[(target_addr+1)/16][(target_addr+1)%16] = register_cell[is_register("X")] % 0x10000 / 0x100;
			mem[(target_addr+2)/16][(target_addr+2)%16] = register_cell[is_register("X")] % 0x100;
		}
		else if( op == 0xE0 ) 	// TD 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			register_cell[is_register("SW")] = -1;
			// No implementation
		}
		else if( op == 0xB8 )   // TIXR 2
		{
			calc_ope(op, 2, &first_regi, &second_regi);
			register_cell[is_register("X")]++;
			int tmp_x = register_cell[is_register("X")];
			if( tmp_x < register_cell[first_regi] ) register_cell[is_register("SW")] = -1;
			else if( tmp_x > register_cell[first_regi] ) register_cell[is_register("SW")] = 1;
			else register_cell[is_register("SW")] = 0;
		}
		else if( op == 0xDC ) 	// WD 3
		{
			calc_ope(op, 3, &target_addr, &target_value);
			// No implementation
		}
		else
		{
			puts("Error : Undefined opcode");
			return 0;
		}
	}
	if( end_addr == register_cell[is_register("PC")]) end_addr = register_cell[is_register("PC")] = 0;
	printf("\t\tA : %06X  X : %06X\n", register_cell[is_register("A")], register_cell[is_register("X")]);
	printf("\t\tL : %06X  PC: %06X\n", register_cell[is_register("L")], register_cell[is_register("PC")]);
	printf("\t\tB : %06X  S : %06X\n", register_cell[is_register("B")], register_cell[is_register("S")]);
	printf("\t\tT : %06X\n", register_cell[is_register("T")]);
	if( end_addr == register_cell[is_register("PC")]) 
	{
		puts("\tEnd program");
		register_cell[is_register("PC")] = -1;
	}
	else 
	{
		printf("\tStop at checkpoint [%04X]\n", break_addr);
	}
	return 1;
}

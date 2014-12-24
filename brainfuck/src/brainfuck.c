#include <stdio.h>
#include "type.h"

#define BUF_SIZE 30000
byte ds[BUF_SIZE];			/* date segment */
byte cs[BUF_SIZE];			/* code segment */
uint dp;					/* data pointer */
uint cp;					/* code pointer */
typedef struct stack {
	uint sp;				/* stack pointer */
	uint data[BUF_SIZE/2];
} Stack;

/* store loop positions */
Stack loop;

static void init()
{
	int i;
	for (i = 0; i < BUF_SIZE; i++) {
		ds[i] = cs[i] = 0;
	}
	loop.sp = dp = cp = 0;
}

static void push(Stack *s, uint val)
{
	s->data[s->sp++] = val;
}

static int pop(Stack *s)
{
	if (s->sp >0)
		return s->data[--s->sp];
	perror("Stack is overflow");
	return -1;
}

static void parse()
{
	int pos;
	switch (cs[cp]) {
		case '>':
			++dp;
			++cp;
			break;
		case '<':
			--dp;
			++cp;
			break;
		case '+':
			ds[dp] += 1;
			++cp;
			break;
		case '-':
			ds[dp] -= 1;
			++cp;
			break;
		case '.':
			putchar(ds[dp]);
			++cp;
			break;
		case ',':
			ds[dp] = getchar();
			++cp;
			break;
		case '[':
			if (ds[dp]) {
				++cp;
			} else {
				cp = pop(&loop) + 1;
				pop(&loop);
			}
			break;
		case ']':
			if (ds[dp]) {
				pos = pop(&loop);
				cp = pop(&loop);
				push(&loop, cp);
				push(&loop, pos);
			} else {
				++cp;
				pop(&loop);
				pop(&loop);
			}
			break;
		default:
			;
	}
}

static void read_file(char *filename) 
{
	FILE *file = fopen(filename, "r");
	byte code;
	uint i = 0;
	while ((code = fgetc(file)) != EOF && i < BUF_SIZE) {
		if (code == '[' || code == ']') {
			push(&loop, i);
		}
		if (code == '>' || code == '<' || code == '+' || code == '-' ||
			code == '.' || code == ',' || code == '[' || code == ']') {
			cs[i++] = code;
		}
	}
	fclose(file);
}

void run(char *filename)
{
	init();
	read_file(filename);
	while (cp >= 0 && cp < BUF_SIZE && cs[cp]) {
		parse();
	}
}

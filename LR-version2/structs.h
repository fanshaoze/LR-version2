#pragma once

const char key_words[67][10] = { "", "and", "array", "begin", "case",
"const", "div", "do", "downto",
"else", "end", "file", "for",
"function", "goto", "if", "in",
"label", "mod", "nil", "not",
"of", "or", "packed", "procedure",
"program", "record", "repeat", "set",
"then", "to", "type", "until",
"var", "while", "with", "id",
"int", "real_num", "string_c", "plus",
"minus", "multi", "rdiv", "eq",
"lt", "gt", "le", "ge", "ne",
"lr_brac", "rr_brac", "comma",
"p_mark", "f_stop", "range", "colon",
"assign", "semic", "cap", "exp",
"ls_brac", "rs_brac", "q_mark", "integer","real","string" };
const char outkey_words[67][10] = { "", "AND", "ARRAY", "BEGIN", "CASE",
"CONST", "DIV", "DO", "DOWNTO",
"ELSE", "END", "FILE", "FOR",
"FUNCTION", "GOTO", "IF", "IN",
"LABEL", "MOD", "NIL", "NOT",
"OF", "OR", "PACKED", "PROCEDURE",
"PROGRAME", "RECORD", "REPEAT", "SET",
"THEN", "TO", "TYPE", "UNTIL",
"VAR", "WHILE", "WITH", "ID",
"INT", "REAL_NUM", "STRING_C", "PLUS",
"MINUS", "MULTI", "RDIV", "EQ",
"LT", "GT", "LE", "GE", "NE",
"LR_BRAC", "RR_BRAC", "COMMA",
"P_MARK", "F_STOP", "RANGE", "COLON",
"ASSIGN", "SEMIC", "CAP", "EXP",
"LS_BRAC", "RS_BRAC", "Q_MARK","INTEGER","REAL","STRING" };
const char words[67][10] = { "#", "and", "array", "begin", "case",
"const", "div", "do", "downto",
"else", "end", "file", "for",
"function", "goto", "if", "in",
"label", "mod", "nil", "not",
"of", "or", "packed", "proc",
"prog", "record", "repeat", "set",
"then", "to", "type", "until",
"var", "while", "with", "id",
"id", "id", "id", "+",
"-", "*", "/", "=",
"<", ">", "<=", ">=", "<>",
"(", ")", ",",
"、", ".", "..", ":",
":=", ";", "^", "**",
"[", "]", "‘", "integer","real","string" };
union S
{
	int i;
	float f;
	char* c;
};
typedef struct token
{
	int type;
	union S value;

}Token;
typedef struct//产生式结构
{
	char S[10];//产生式左边
	char right[10][10];//产生式右边
	int count;//?????????
}pexp;

typedef struct//项目集中每个产生式的具体的结构体
{
	int exp;
	int state;
}pexp_state;
typedef struct    //产生式状态和展望符
{
	pexp_state state;
	char a[10];//展望符
}item;
typedef struct
{
	item items[50];
	int length;
}projects;
typedef struct
{
	projects pro[200];
	int number;
}entire_projects;
typedef struct
{
	char ch[50][10];
	int length;
}s_char;//定义终结符或者非终结符
typedef struct
{
	char type;
	int value;
}table; //分析表中每一项
typedef struct
{
	char ch[10];
	int j;
}Stack;



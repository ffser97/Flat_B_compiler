%{
#include<bits/stdc++.h>
#include "classes.h"
#include "parser.tab.h"
extern union Node yylval;
%}

%%
"declblock"	{
			return DECL;
		}
"codeblock"	{
			return CODE;
		}
"int" 		{
			return INT;
		}
"if"		{
			return IF;
		}
"else"		{
			return ELSE;
		}
"for"		{
			return FOR;
		}
"while"		{
			return WHILE;
		}
"got"		{
			return GOT;
		}
"goto"		{
			return GOTO;
		}
"print"|"println"		{
			return PRINT;
		}
"read"		{
			return READ;
		}
"break"		{
			return BREAK;
		}
"continue"	{
			return CONTINUE;
		}
"=="		{
			yylval.value = strdup(yytext);
			return E;
		}
"!="		{
			yylval.value = strdup(yytext);
			return NE;
		}
">="		{
			yylval.value = strdup(yytext);
			return GE;
		}
">"		{
			yylval.value = strdup(yytext);
			return GT;
		}
"<="		{
			yylval.value = strdup(yytext);
			return LE;
		}
"<"		{
			yylval.value = strdup(yytext);
			return LT;
		}

"||"		{
			yylval.value = strdup(yytext);
			return OR_COND;
		}
"&&"		{
			yylval.value = strdup(yytext);
			return AND_COND;
		}
\".*\"		{
			yylval.value = strdup(yytext);
			return STRING;
		}
-?[0-9][0-9]*	{
			yylval.number = atoi(yytext);
			return NUMBER;
                }
[a-zA-Z][a-zA-Z0-9]* {
			yylval.value = strdup(yytext);
			return IDENTIFIER;
		     }

"="		{yylval.value = strdup(yytext); return EQ;}
"+"		{yylval.value = strdup(yytext); return ADD;}
"-"		{yylval.value = strdup(yytext); return SUB;}
"*"		{yylval.value = strdup(yytext); return MUL;}
"/"		{yylval.value = strdup(yytext); return DIV;}
":"		{yylval.value = strdup(yytext); return COL;}
"%"		{yylval.value = strdup(yytext); return MOD;}
"^"		{yylval.value = strdup(yytext); return XOR;}
"&"		{yylval.value = strdup(yytext); return AND;}
"|"		{yylval.value = strdup(yytext); return OR;}
";"		return SC;
"("		return OP;
")"		return CP;
"{"		return OB;
"}"		return CB;
","		return COMMA;
"["		return OSB;
"]"		return OCB;

[ \t]		{ /* Do nothing */ }
.		{ 
		  printf("Unexpected token encountered: %s\n", yytext); 
		  return ETOK;
		}

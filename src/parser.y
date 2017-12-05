%{
  #include<bits/stdc++.h>
  #include "classes.h"
  extern FILE *yyin;
  int yylex (void);
  void yyerror (char const *s);
  extern union Node yylval;
  class Prog* start=NULL;
  extern class visitor* v;
%}
/* --------------------tokens--------------*/
%token DECL
%token CODE
%token <number> NUMBER
%token <value> IDENTIFIER
%token ETOK
%token INT
%token IF
%token ELSE
%token FOR
%token WHILE
%token GOT
%token GOTO
%token PRINT
%token READ
%token <value> STRING
%token BREAK
%token CONTINUE
%token <value> OR_COND
%token <value> AND_COND
%token <value> ADD
%token <value> SUB
%token <value> MUL
%token <value> DIV
%token <value> MOD
%token <value> XOR
%token <value> EQ
%token <value> E
%token <value> NE
%token <value> GE
%token <value> GT
%token <value> LE
%token <value> LT
%token <value> OR
%token <value> AND
%token COL
%token OP
%token CP
%token SC
%token OB
%token CB
%token COMMA
%token OSB
%token OCB
/*----------------Left Precedence----------*/
%left OP CP EQ NE
%left AND_COND OR_COND 
%left LT GT LE GE E
%left ADD SUB
%left MUL DIV MOD
%left XOR AND OR

/*----------------Non-Terminals---------- */

%type <prog> Program
%type <decls_block> Decls_B
%type <decls> Decls
%type <decl> Decl
%type <vars> Vars
%type <var> Var
%type <code_block> Code_B
%type <stmnts> Stmnts
%type <stmnt> Stmnt
%type <assign> Assign
%type <expr> Expr
%type <printing> Printing
%type <reading> Reading
%type <var1> Var1
%type <varr> Varr
%type <var2> Var2
%type <forloop> Forloop
%type <ident> Ident
%type <identr> Identr
%type <whileloop> Whileloop
%type <bl> Bool
%type <ifelse> Ifelse
%type <labeling> Labeling
%type <got1> Got1
%type <got2> Got2

%%

Program:  DECL Decls_B CODE Code_B    { $$ = new Prog($2, $4); start = $$;  }

Decls_B:  OB Decls CB  { $$ = new Decls_block($2); }

Decls: 	Decls Decl SC { $$->push_back($2); }
	| {$$ = new Decls();} ;
Decl: INT Vars { $$ = new Decl($2); } 
Vars:   Vars COMMA Var { $$->push_back($3); }
	| Var { $$ = new Vars(); $$->push_back($1); }
Var:   IDENTIFIER { $$ = new Var("Normal", $1);  }
	| IDENTIFIER OSB NUMBER OCB { $$ = new Var("Array", string($1), $3); }

Code_B:   OB Stmnts CB { $$ = new Code_block($2);  }

Stmnts:	Stmnts Stmnt   { $$->push_back($2); }
	| { $$ = new Statements();  } ;

Stmnt:  Assign SC { $$ = new Statement($1);  }
	| Printing SC {$$ = new Statement($1); }
	| Reading SC {$$ = new Statement($1); }
	| Forloop {$$ = new Statement($1);}
	| Whileloop { $$ = new Statement($1); }
	| Ifelse { $$ = new Statement($1); }
	| Got1 SC { $$ = new Statement($1); }
	| Got2 SC {$$ = new Statement($1); }
	| Labeling { $$ = new Statement($1); }

Labeling: IDENTIFIER COL  {$$ = new Labeling($1);} 

Got1:	GOT IDENTIFIER { $$ = new Got1($2); }
Got2:	GOTO IDENTIFIER IF Bool { $$ = new Got2($2, $4); }

Forloop:  FOR IDENTIFIER EQ Var2 COMMA Var2 OB Stmnts CB   { $$ = new Forloop($2, $4, $6, $8); }
        | FOR IDENTIFIER EQ Var2 COMMA Var2 COMMA Var2 OB Stmnts CB  { $$ = new Forloop($2, $4, $6, $8, $10); }

Var2:	NUMBER { $$ = new Var2($1); }
	| IDENTIFIER { $$ = new Var2(string($1));  }
	| IDENTIFIER OSB Expr OCB { $$ = new Var2(string($1), $3); }

Whileloop: WHILE Bool OB Stmnts CB { $$ = new Whileloop($2, $4); }

Ifelse:	  IF Bool OB Stmnts CB { $$ = new Ifelse($2,$4); }
	| IF Bool OB Stmnts CB ELSE OB Stmnts CB { $$ = new Ifelse($2,$4,$8); }

Bool	:  OP Bool CP 	{ $$ = new Bool($2); }
	|  Expr E Expr  { $$ = new Bool($1,string($2),$3);}
	|  Expr NE Expr { $$ = new Bool($1,string($2),$3);}
	|  Expr GT Expr { $$ = new Bool($1,string($2),$3);}
	|  Expr LT Expr { $$ = new Bool($1,string($2),$3);}
	|  Expr GE Expr { $$ = new Bool($1,string($2),$3);}
	|  Expr LE Expr { $$ = new Bool($1,string($2),$3);}

Printing: PRINT Var1 {$$ = new Printing($2);}
Reading:  READ Varr  {$$ = new Reading($2); }

Var1:	Var1 COMMA Ident {$$->push_back($3);}
	| Ident {$$ = new Var1(); $$->push_back($1);}

Varr:	Varr COMMA Identr {$$->push_back($3);}
	| Identr { $$ = new Varr(); $$->push_back($1); }	

Ident:	Expr {$$ = new Ident($1); }
	| STRING {$$ = new Ident(string($1)); }

Identr: IDENTIFIER { $$ = new Identr($1); }
	| IDENTIFIER OSB Expr OCB { $$ = new Identr($1, $3); }

Assign: IDENTIFIER EQ Expr { $$ = new Assign(string($1),$3);   }
	| IDENTIFIER OSB Expr OCB EQ Expr  { $$ = new Assign(string($1),$3,$6);  }


Expr    :       OP Expr CP { $$ = new Expr($2); }
        |       Expr ADD Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr MUL Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr SUB Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr DIV Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr MOD Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr XOR Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr AND Expr { $$ = new Expr($1,string($2), $3); }
        |       Expr OR Expr { $$ = new Expr($1,string($2), $3); }
        |       NUMBER {$$ = new Expr(int($1));}
        |       IDENTIFIER {$$ = new Expr(string($1));}
        |       IDENTIFIER OSB Expr OCB {$$ = new Expr(string($1), $3); }
        ;

%%

void yyerror (char const *s)
{
       fprintf (stderr, "%s\n", s);
}

int main(int argc, char *argv[])
{
	if (argc == 1 ) {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}

	if (argc > 2) {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}

	yyin = fopen(argv[1], "r");

	yyparse();
	if(start != NULL)
	{
		start->accept(v);
		start->Codegen();
		start->generateCode();
	}
}

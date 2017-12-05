#include<bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>
using namespace std;
using namespace llvm;
union Node{
	int number;
	char* value;
	class Prog* prog;
	class Decl* decl;
	class Decls* decls;
	class Decls_block* decls_block;
	class Vars* vars;
	class Var* var;
	class Statements* stmnts;
	class Statement* stmnt;
	class Assign* assign;
	class Expr* expr;
	class Code_block* code_block;
	class Printing* printing;
	class Reading* reading;
	class Var1* var1;
	class Ident* ident;
	class Varr* varr;
	class Identr* identr;
	class Var2* var2;
	class Forloop* forloop;
	class Whileloop* whileloop;
	class Ifelse* ifelse;
	class Bool* bl;
	class Labeling* labeling;
	class Got1* got1;
	class Got2* got2;
	Node(){
		number=0;
		value=NULL;
		prog = NULL;
		decl = NULL;
		decls = NULL;
		decls_block = NULL;
		vars = NULL;
		var = NULL;
		code_block = NULL;
		printing = NULL;
		reading = NULL;
		var1 = NULL;
		ident = NULL;
		varr = NULL;
		identr = NULL;
		var2 = NULL;
		forloop = NULL;
		whileloop = NULL;
		ifelse = NULL;
		bl = NULL;
		labeling = NULL;
		got1 = NULL;
		got2 = NULL;
	}
	~Node(){};
};

typedef union Node YYSTYPE;

#define YYSTYPE_IS_DECLARED 1

class visitor{
public:
        virtual void visit(class Prog*) = 0;
        virtual void visit(class Decl*) = 0;
        virtual void visit(class Decls*) = 0;
        virtual void visit(class Decls_block*) = 0;
        virtual void visit(class Vars*) = 0;
        virtual void visit(class Var*) = 0;
        virtual void visit(class Statements*) = 0;
        virtual void visit(class Statement*) = 0;
        virtual int visit(class Assign*) = 0;
        virtual int visit(class Expr*) = 0;
        virtual void visit(class Code_block*) = 0;
        virtual void visit(class Printing*) = 0;
        virtual void visit(class Reading*) = 0;
        virtual void visit(class Var1*) = 0;
        virtual void visit(class Ident*) = 0;
        virtual void visit(class Varr*) = 0;
        virtual void visit(class Identr*) = 0;
        virtual int visit(class Var2*) = 0;
        virtual void visit(class Forloop*) = 0;
        virtual void visit(class Whileloop*) = 0;
        virtual void visit(class Ifelse*) = 0;
        virtual int visit(class Bool*) = 0;
	virtual void visit(class Got1*) = 0;
	virtual void visit(class Got2*) = 0;
	virtual void visit(class Labeling*) = 0;
};

class interpreter:public visitor{
public:
        void visit(class Prog*);
        void visit(class Decl*);
        void visit(class Decls*);
        void visit(class Decls_block*);
        void visit(class Vars*);
        void visit(class Var*);
        void visit(class Statements*);
        void visit(class Statement*);
        int visit(class Assign*);
        int visit(class Expr*);
        void visit(class Code_block*);
        void visit(class Printing*);
        void visit(class Reading*);
        void visit(class Var1*);
        void visit(class Ident*);
        void visit(class Varr*);
        void visit(class Identr*);
        int visit(class Var2*);
        void visit(class Forloop*);
        void visit(class Whileloop*);
        void visit(class Ifelse*);
        int visit(class Bool*);
	void visit(class Got1*);
	void visit(class Got2*);
	void visit(class Labeling*);
	interpreter(){}
};

class astNode{
};

//DECLARATION BLOCK CLASSES
class Var:public astNode{
private:
public:
	string declType; /* Array or Normal */
	string name; /* Name of the variable */
	unsigned int length; /* if it is an Array then length */
	Var(string,string,unsigned int);
	Var(string,string);
	bool isArray();
	int getLength();
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Vars:public astNode{
private:
public:
	vector<class Var*> vars_list;
	int cnt;
	Vars(){};
	void push_back(class Var*);
	vector<class Var*> getVarList();
	void accept(visitor* V){ V->visit(this); }
};

class Decl:public astNode{
private:
public:
	string datatype;
	vector<class Var*> var_list;
	Decl(class Vars*);
	vector<class Var*> getVarsList();
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Decls:public astNode{
private:
public:
	vector<class Decl*> decl_list;
	Decls(){};
	void push_back(class Decl*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Decls_block:public astNode{
private:
public:
	class Decls* decl_list;
	Decls_block(class Decls*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

//CODEBLOCK CLASSES
class Expr:public astNode{
private:
public:
	class Expr* lhs;
	class Expr* rhs;
	class Expr* exp;
	string identifier;
	string op;
	int num;
	int type;
	Expr(class Expr*, string, class Expr*);
	Expr(class Expr*);
	Expr(int num);
	Expr(string);
	Expr(string, class Expr*);
	int accept(visitor* V){ return V->visit(this); }
	Value* Codegen();
};

class Assign:public astNode{
private:
public:
	string name;
	class Expr *expr1,*expr2;
	Assign(string name,class Expr*);
	Assign(string name,class Expr*,class Expr*);
	int accept(visitor* V){ return V->visit(this); }
	Value* Codegen();
};

class Ident:public astNode{
private:
public:
	string STR;
	class Expr* expr;
	Ident(string);
	Ident(class Expr*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Var1:public astNode{
private:
public:
	vector<class Ident*> idents;
	Var1(){};
	void push_back(class Ident*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Printing:public astNode{
private:
public:
	class Var1* var1;
	Printing(class Var1*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};
class Identr:public astNode{
private:
public:
	string identifier;
	class Expr* expr;
	Identr(string);
	Identr(string,class Expr*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Varr:public astNode{
private:
public:
	vector<class Identr*> identsr;
	Varr(){};
	void push_back(class Identr*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Reading:public astNode{
private:
public:
	class Varr* varr;
	Reading(class Varr*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Var2:public astNode{
private:
public:
	int number;
	string identifier;
	class Expr* expr;
	Var2(int);
	Var2(string identifier);
	Var2(string identifier, class Expr* expr);
	int accept(visitor* V){ return V->visit(this); }
	Value* Codegen();
};

class Forloop:public astNode{
private:
public:
	string identifier;
	class Var2 *start,*end,*step;
	class Statements *stmnts;
	Forloop(string,class Var2*, class Var2*, class Statements*);
	Forloop(string,class Var2*, class Var2*, class Var2*, class Statements*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Bool:public astNode{
private:
public:
	class Expr *lhs,*rhs,*expr;
	class Bool* bl;
	string op;		
	Bool(class Expr*, string,class Expr*);
	Bool(class Bool*);
	bool accept(visitor* V){ return V->visit(this); }
	Value* Codegen();
};

class Whileloop:public astNode{
private:
public:
	class Bool* bl;
	class Statements* stmnts;
	Whileloop(class Bool*,class Statements*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Ifelse:public astNode{
private:
public:
	class Bool* bl;
	class Statements *stmnts_if, *stmnts_else;
	Ifelse(class Bool*,class Statements*);
	Ifelse(class Bool*,class Statements*,class Statements*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Labeling:public astNode{
private:
public:
	string label;
	Labeling(string);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Got1:public astNode{
private:
public:
	string label;
	Got1(string);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Got2:public astNode{
private:
public:
	string label;
	class Bool* bl;
	Got2(string, class Bool*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Statement:public astNode{
private:
public:
	class Assign* assign;
	class Printing* printing;
	class Reading* reading;
	class Forloop* forloop;
	class Whileloop* whileloop;
	class Ifelse* ifelse;
	class Got1* got1;
	class Got2* got2;
	class Labeling* labeling;
	int type;
	Statement(class Assign* assign);
	Statement(class Printing* printing);
	Statement(class Reading* reading);
	Statement(class Forloop* forloop);
	Statement(class Whileloop* whileloop);
	Statement(class Ifelse* ifelse);
	Statement(class Got1* got1);
	Statement(class Got2* got2);
	Statement(class Labeling* labeling);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Statements:public astNode{
private:
public:
	vector<class Statement*> stmnts;
	Statements(){};
	void push_back(class Statement* stmnt);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Code_block:public astNode{
private:
public:
	class Statements* stmnts;		
	Code_block(class Statements* stmnts);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
};

class Prog:public astNode{
private:
public:
	class Decls_block* decls;
	class Code_block* code_block;
	Prog(class Decls_block*, class Code_block*);
	void accept(visitor* V){ V->visit(this); }
	Value* Codegen();
	void generateCode();
};

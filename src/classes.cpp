#include<bits/stdc++.h>
#include "classes.h"
extern union Node yylval;
using namespace std;
using namespace llvm;
map<string,vector<int> > symbol_table;
map<string,int> goto_symbols;
map<string,BasicBlock*> goto_blocks;
int cur_label = -1;
int dum_statements=-1;

static Module *ModuleOb = new Module("Bcomp",llvm::getGlobalContext());
static LLVMContext &Context = getGlobalContext();
static IRBuilder<> Builder(Context);
FunctionType *funcType = FunctionType::get(Builder.getVoidTy(),false);
Function *mainfunc = Function::Create(funcType,Function::ExternalLinkage,"main",ModuleOb);
int cnt = 1;
class visitor *v = new interpreter();

/*--------------Symbol Table Functions-----------------*/
void insert_in_table(string name, int len)
{
	for(int i=0;i<len;i++) symbol_table[name].push_back(0);
}

void print_error(string name)
{
	if(symbol_table[name].size() == 0)
	{
		cout << name << " Not Declared!" << endl;
		exit(1);
	}
}

/* ------------------Constructors ---------------------*/

Var::Var(string declType, string name, unsigned int length)
{
	this->declType  = declType;
	this->name = name;
	this->length = length;
}
Var::Var(string declType, string name)
{
	this->declType = declType;
	this->name = name;
	this->length = 1;
}
Decl::Decl(class Vars* vars)
{
	this->datatype = "int";
	vector<class Var*> v = vars->getVarList();
	for(int i=0;i<v.size();i++)
	{
		this->var_list.push_back(v[i]);
	}
}
Decls_block::Decls_block(class Decls* decls)
{
	this->decl_list = decls;
}

/*------------------------CODE BLOCK--------------------*/
Expr::Expr(class Expr* lhs, string op, class Expr* rhs)
{
	this->lhs = lhs;
	this->op = op;
	this->rhs = rhs;
	this->type = 1;
}
Expr::Expr(class Expr* exp)
{
	this->exp = exp;
	this->type = 2;
}
Expr::Expr(int num)
{
	this->num = num;
	this->type = 3;
}
Expr::Expr(string identifier)
{
	this->identifier = identifier;
	this->type = 4;
}
Expr::Expr(string identifier, class Expr* exp)
{
	this->identifier = identifier;
	this->exp = exp;
	this->type = 5;
}
Assign::Assign(string name, class Expr* expr1)
{
	this->name = name;
	this->expr1 = expr1;
	this->expr2 = NULL;
}
Assign::Assign(string name, class Expr* expr1, class Expr* expr2)
{
	this->name = name;
	this->expr1 = expr1;
	this->expr2 = expr2;
}
Ident::Ident(class Expr* expr)
{
	this->expr = expr;
}
Ident::Ident(string STR)
{
	this->expr = NULL;
	this->STR = STR;
}
Identr::Identr(string identifier)
{
	this->identifier = identifier;
	this->expr = NULL;
}
Identr::Identr(string identifier, class Expr *expr)
{
	this->identifier = identifier;
	this->expr = expr;
}
Printing::Printing(class Var1* var1)
{
	this->var1 = var1;
}
Reading::Reading(class Varr* varr)
{
	this->varr = varr;
}
Var2::Var2(int number) { this->number = number; this->identifier = ""; this->expr = NULL; }
Var2::Var2(string identifier) { this->identifier = identifier; this->expr = NULL; }
Var2::Var2(string identifier, class Expr* expr) { this->identifier = identifier; this->expr = expr;}
Forloop::Forloop(string identifier, class Var2* start, class Var2* end, class Statements* stmnts)
{
	this->identifier = identifier;
	this->start = start;
	this->step = NULL;
	this->end = end;
	this->stmnts = stmnts;
}
Forloop::Forloop(string identifier, class Var2* start,class Var2* end,class Var2* step,class Statements* stmnts)
{
	this->identifier = identifier;
	this->start = start;
	this->step = step;
	this->end = end;
	this->stmnts = stmnts;
}
Bool::Bool(class Expr* lhs, string op, class Expr* rhs)
{
	this->lhs = lhs;
	this->rhs = rhs;
	this->op = op;
}
Bool::Bool(class Bool* bl) {this->bl = bl;}
Whileloop::Whileloop(class Bool* bl, class Statements* stmnts)
{
	this->bl = bl;
	this->stmnts = stmnts;
}
Ifelse::Ifelse(class Bool* bl,class Statements* stmnts_if)
{
	this->bl = bl;
	this->stmnts_if = stmnts_if;
}
Ifelse::Ifelse(class Bool* bl,class Statements* stmnts_if,class Statements* stmnts_else)
{
	this->bl = bl;
	this->stmnts_if = stmnts_if;
	this->stmnts_else = stmnts_else;
}
Labeling::Labeling(string label)
{
	this->label = label;
}
Got1::Got1(string label)
{
	this->label = label;
}
Got2::Got2(string label, class Bool* bl)
{
	this->label = label;
	this->bl = bl;	
}
Statement::Statement(class Assign* assign)
{
	this->assign = assign;
	this->type = 0;
}
Statement::Statement(class Printing* printing)
{
	this->printing = printing;
	this->type = 1;
}
Statement::Statement(class Reading* reading)
{
	this->reading = reading;
	this->type = 2;
}
Statement::Statement(class Forloop* forloop)
{
	this->forloop = forloop;
	this->type = 3;
}
Statement::Statement(class Whileloop* whileloop)
{
	this->whileloop = whileloop;
	this->type = 4;
}
Statement::Statement(class Ifelse* ifelse)
{
	this->ifelse = ifelse;
	this->type = 5;
}
Statement::Statement(class Got1* got1)
{
	this->got1 = got1;
	this->type = 6;
}
Statement::Statement(class Got2* got2)
{
	this->got2 = got2;
	this->type = 7;
}
Statement::Statement(class Labeling* labeling)
{
	this->labeling = labeling;
	this->type = 8;
}
Code_block::Code_block(class Statements* stmnts)
{
	this->stmnts = stmnts;
}
Prog::Prog(class Decls_block* decls, class Code_block* code_b)
{
	this->decls = decls;
	this->code_block = code_b;
}
/*--------------------Interpreter------------------*/
void interpreter::visit(class Prog* prog)
{
	prog->decls->accept(v);
	prog->code_block->accept(v);
}
void interpreter::visit(class Decl* decl)
{
	for(int i=0;i<decl->var_list.size();i++) decl->var_list[i]->accept(v);
}
void interpreter::visit(class Decls* decls)
{
	for(int i=0;i<decls->decl_list.size();i++) decls->decl_list[i]->accept(v);	
}
void interpreter::visit(class Decls_block* decls_block)
{
	decls_block->decl_list->accept(v);
}
void interpreter::visit(class Vars* vars)
{
	for(int i=0;i<vars->vars_list.size();i++) vars->vars_list[i]->accept(v);	
}
void interpreter::visit(class Var* var)
{
	if(symbol_table[var->name].size() > 0) { cout << var->name << " Redeclared!" << endl; exit(1); }
	insert_in_table(var->name, var->length);
}
void interpreter::visit(class Statements* statements)
{
	for(int i=0;i<statements->stmnts.size();i++)
	{
		statements->stmnts[i]->accept(v);
		if(cur_label != -1)
		{
			i = cur_label;
			cur_label = -1;
		}
	}
}
void interpreter::visit(class Statement* statement)
{
	if(statement->type == 0) statement->assign->accept(v);
	if(statement->type == 1) statement->printing->accept(v);
	if(statement->type == 2) statement->reading->accept(v);
	if(statement->type == 3) statement->forloop->accept(v);
	if(statement->type == 4) statement->whileloop->accept(v);
	if(statement->type == 5) statement->ifelse->accept(v);
	if(statement->type == 6) statement->got1->accept(v);
	if(statement->type == 7) statement->got2->accept(v);
	if(statement->type == 8) statement->labeling->accept(v);
}
int interpreter::visit(class Assign* assign)
{
	int val1 = assign->expr1->accept(v), val2;
	if(assign->expr2 != NULL) val2 = assign->expr2->accept(v);
	print_error(assign->name);
	if(assign->expr2 == NULL) symbol_table[assign->name][0] = val1;
	else symbol_table[assign->name][val1] = val2;
}
int interpreter::visit(class Expr* expr)
{
	if(expr->lhs != NULL && expr->rhs != NULL)
	{
		int lval = expr->lhs->accept(v);
		int rval = expr->rhs->accept(v);
		if(expr->op == "+") expr->num = lval + rval;
		if(expr->op == "-") expr->num = lval - rval;
		if(expr->op == "*") expr->num = lval*rval;
		if(expr->op == "/") expr->num = lval/rval;
		if(expr->op == "%") expr->num = lval%rval;
		if(expr->op == "^") expr->num = lval^rval;
		if(expr->op == "&") expr->num = lval&rval;
		if(expr->op == "|") expr->num = lval|rval;
	}
	if(expr->exp != NULL)
	{
		expr->num = expr->exp->accept(v);
		if((expr->identifier).size() > 0) 
		{ 
			print_error(expr->identifier); 
			if(expr->num < symbol_table[expr->identifier].size()) expr->num = symbol_table[expr->identifier][expr->num];
			else { cout << "Array size crossed: " << expr->identifier << endl; exit(1) ; }
		}
	}
	else if((expr->identifier).size() > 0) { print_error(expr->identifier); expr->num = symbol_table[expr->identifier][0];}
	return expr->num;
}
void interpreter::visit(class Code_block* code_block)
{
	code_block->stmnts->accept(v);
}
void interpreter::visit(class Printing* printing)
{
	printing->var1->accept(v);
}
void interpreter::visit(class Reading* reading)
{
	reading->varr->accept(v);
}
void interpreter::visit(class Varr* varr)
{
	for(int i=0;i<(varr->identsr).size();i++)
	{
		varr->identsr[i]->accept(v);
	}
}
void interpreter::visit(class Identr* identr)
{
	print_error(identr->identifier);
	if(identr->expr == NULL) cin >> symbol_table[identr->identifier][0];
	else
	{
		int val = identr->expr->accept(v);
		cin >> symbol_table[identr->identifier][val];
	}
} 
void interpreter::visit(class Var1* var1)
{
	for(int i=0;i<(var1->idents).size();i++)
	{
		var1->idents[i]->accept(v);
	}
	cout << endl;
}
void interpreter::visit(class Ident* ident)
{
	if(ident->expr != NULL) {int num=ident->expr->accept(v); cout << num << " "; }
	else cout << ident->STR << " ";
}
int interpreter::visit(class Var2* var2)
{
	if(var2->expr != NULL) { var2->number = var2->expr->accept(v); var2->number = symbol_table[var2->identifier][var2->number]; }
	else if((var2->identifier).size() > 0) var2->number = symbol_table[var2->identifier][0];
	return var2->number;
}
void interpreter::visit(class Forloop* forloop)
{
	int start=0,end=-1,step=1;
	start = forloop->start->accept(v);
	end = forloop->end->accept(v);
	if(forloop->step != NULL) step = forloop->step->accept(v);
	for(;start<=end;start+=step) forloop->stmnts->accept(v);
}
void interpreter::visit(class Whileloop* whileloop)
{
	while(1)
	{
		int val = whileloop->bl->accept(v);
		if(val == 0) break;
		whileloop->stmnts->accept(v);
	}
}
void interpreter::visit(class Ifelse* ifelse)
{
	int val = ifelse->bl->accept(v);
	if(val) ifelse->stmnts_if->accept(v);
	else if(ifelse->stmnts_else != NULL) ifelse->stmnts_else->accept(v);
}
int interpreter::visit(class Bool* bl)
{
	if(bl->bl != NULL) return bl->bl->accept(v);
	int lval = bl->lhs->accept(v), rval = bl->rhs->accept(v);
	if(bl->op == "==") return (lval == rval);
	if(bl->op == "!=") return (lval != rval);
	if(bl->op == ">") return (lval > rval);
	if(bl->op == "<") return (lval < rval);
	if(bl->op == ">=") return (lval >= rval);
	if(bl->op == "<=") return (lval <= rval);
	if(bl->op == "||") return (lval | rval);
	if(bl->op == "&&") return (lval & rval);
}
void interpreter::visit(class Labeling* label)
{
}
void interpreter::visit(class Got1* got1)
{
	cur_label  = goto_symbols[got1->label];
}
void interpreter::visit(class Got2* got2)
{
	int val = got2->bl->accept(v);
	if(val)
	{
		cur_label = goto_symbols[got2->label];
	}
}

/* -------------------Methods-------------------*/

void Decls::push_back(class Decl* decl)
{
	this->decl_list.push_back(decl);
}
vector<class Var*> Decl::getVarsList()
{
	return this->var_list;
}
void Vars::push_back(class Var* var)
{
	this->vars_list.push_back(var);
}
vector<class Var*> Vars::getVarList()
{
	return this->vars_list;
}
bool Var::isArray()
{
	if(this->declType == "Normal") return false;
	else return true;
}
int Var::getLength()
{
	return this->length;
}
void Statements::push_back(class Statement* stmnt)
{
	this->stmnts.push_back(stmnt);
	dum_statements=(this->stmnts).size() - 1;
	if(stmnt->labeling != NULL)
	{
		goto_symbols[stmnt->labeling->label] = dum_statements;
	}
}
void Var1::push_back(class Ident* ident)
{
	this->idents.push_back(ident);
}
void Varr::push_back(class Identr* identr)
{
	this->identsr.push_back(identr);
}

/*----------------Codegen Functions-----------------*/
Value* Prog::Codegen()
{
	Value* v = this->decls->Codegen();
	BasicBlock* blk = BasicBlock::Create(Context, "entry", mainfunc);
	Builder.SetInsertPoint(blk);
	for(map<string,int>::iterator it = goto_symbols.begin();it!=goto_symbols.end();++it)
	{
		goto_blocks[it->first] = BasicBlock::Create(Context,it->first,mainfunc);
	}
	v = this->code_block->Codegen();
	Builder.CreateRetVoid();
	return v;
}
Value* Decls_block::Codegen()
{
	return this->decl_list->Codegen();
}
Value* Decls::Codegen()
{
	for(int i=0;i<this->decl_list.size();i++)
	{
		this->decl_list[i]->Codegen();
	}
	Value* v = ConstantInt::get(getGlobalContext(), APInt(32,1));
	return v;
}
Value* Decl::Codegen()
{
	for(int i=0;i<this->var_list.size();i++)
	{
		this->var_list[i]->Codegen();
	}
	Value* v = ConstantInt::get(getGlobalContext(), APInt(32,1));
	return v;
}
Value* Var::Codegen()
{
	Type *ty;
	ty = Type::getInt32Ty(Context);
	if(this->isArray())
	{
		ArrayType* arrType = ArrayType::get(ty,this->getLength());
		PointerType* PointerTy_1 = PointerType::get(ArrayType::get(ty,this->getLength()),0);
		GlobalVariable* gv = new GlobalVariable(*ModuleOb,arrType,false,GlobalValue::ExternalLinkage,0,this->name);
		gv->setInitializer(ConstantAggregateZero::get(arrType));
	}
	else
	{
		ModuleOb->getOrInsertGlobal(this->name, Builder.getInt32Ty());
		GlobalVariable* gv = ModuleOb->getNamedGlobal(this->name);
		gv->setLinkage(GlobalValue::CommonLinkage);
		gv->setAlignment(4);
		ConstantInt* t = ConstantInt::get(getGlobalContext(),APInt(32,0));
		gv->setInitializer(t);
	}
	Value* v = ConstantInt::get(getGlobalContext(),APInt(32,1));
	return v;
}
Value* Code_block::Codegen()
{
	return this->stmnts->Codegen();
}
Value* Statements::Codegen()
{
	for(int i=0;i<this->stmnts.size();i++)
	{
		this->stmnts[i]->Codegen();
	}
	Value* v = ConstantInt::get(getGlobalContext(),APInt(32,1));
	return v;
}
Value* Statement::Codegen()
{
	if(this->type == 0) return this->assign->Codegen();
	if(this->type == 1) return this->printing->Codegen();
	if(this->type == 2) return this->reading->Codegen();
	if(this->type == 3) return this->forloop->Codegen();
	if(this->type == 4) return this->whileloop->Codegen();
	if(this->type == 5) return this->ifelse->Codegen();
	if(this->type == 6) return this->got1->Codegen();
	if(this->type == 7) return this->got2->Codegen();
	if(this->type == 8) return this->labeling->Codegen();
}
Value* Labeling::Codegen()
{
	Builder.CreateBr(goto_blocks[this->label]);
	Builder.SetInsertPoint(goto_blocks[this->label]);
	Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return v;
}
Value* Got1::Codegen()
{
	Builder.CreateBr(goto_blocks[this->label]);
	Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return v;
}
Value* Got2::Codegen()
{
	Value *cond = this->bl->Codegen();
	BasicBlock* GotBlock = BasicBlock::Create(Context,"got",mainfunc);
	Builder.CreateCondBr(cond,goto_blocks[this->label],GotBlock);
	Builder.SetInsertPoint(GotBlock);
	Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return v;
}
Value* Forloop::Codegen()
{
	Value *ini = this->start->Codegen();
	Value *bound = this->end->Codegen();
	Value *incr = NULL;
	if(this->step != NULL) incr = this->step->Codegen();
	else incr = Builder.getInt32(1);
	ModuleOb->getOrInsertGlobal("_iterator"+to_string(cnt), Builder.getInt32Ty());
	GlobalVariable* gv = ModuleOb->getNamedGlobal("_iterator" + to_string(cnt)); cnt++;
	gv->setLinkage(GlobalValue::CommonLinkage);
	gv->setAlignment(4);
	ConstantInt* t = ConstantInt::get(getGlobalContext(),APInt(32,0));
	gv->setInitializer(t);
	Value* id = gv;
	Value* init = Builder.CreateStore(ini,id);
	Value* id_temp = Builder.CreateLoad(id);
	Value* Condtn = Builder.CreateICmpULE(id_temp,bound,"limit");
	BasicBlock* LoopBlock = BasicBlock::Create(Context,"loop",mainfunc);
	BasicBlock* AfterLoop = BasicBlock::Create(Context,"afterloop",mainfunc);
	Builder.CreateCondBr(Condtn,LoopBlock,AfterLoop);
	Builder.SetInsertPoint(LoopBlock);
	Value* V = this->stmnts->Codegen();
	Value* temp = Builder.CreateLoad(id);
	Value* add = Builder.CreateAdd(temp,incr,"nextval");
	init = Builder.CreateStore(add,id,"update");
	Condtn = Builder.CreateICmpULE(add,bound,"limit");
	Builder.CreateCondBr(Condtn,LoopBlock,AfterLoop);
	Builder.SetInsertPoint(AfterLoop);
	Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return v;
}
Value* Var2::Codegen()
{
	if((this->identifier).size() > 0)
	{
		if(this->expr == NULL)
		{
			Value* V = ModuleOb->getNamedGlobal(this->identifier);
			V = Builder.CreateLoad(V);
			return V;
		}
		else
		{
			Value* V = ModuleOb->getNamedGlobal(this->identifier);
			Value* index = this->expr->Codegen();
			vector<Value*> arr_ind;
			arr_ind.push_back(Builder.getInt32(0));
			arr_ind.push_back(index);
			V = Builder.CreateGEP(V,arr_ind,this->identifier);
			V = Builder.CreateLoad(V);
			return V;
		}
	}
	else return ConstantInt::get(getGlobalContext(), APInt(32, this->number));
}
Value* Whileloop::Codegen()
{
	Value* cond = this->bl->Codegen();
	BasicBlock *whileBlock = BasicBlock::Create(Context, "while", mainfunc);
	BasicBlock *nextBlock = BasicBlock::Create(Context, "whilecont",mainfunc);
	Builder.CreateCondBr(cond, whileBlock, nextBlock);
	Builder.SetInsertPoint(whileBlock);
	Value* whileval = this->stmnts->Codegen();
	cond = this->bl->Codegen();
	Builder.CreateCondBr(cond,whileBlock, nextBlock);
	Builder.SetInsertPoint(nextBlock);
	Value *V = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return V;
}
Value* Ifelse::Codegen()
{
	Value* cond = this->bl->Codegen();
	Function* TheFunction = Builder.GetInsertBlock()->getParent();
	BasicBlock *ifBlock = BasicBlock::Create(Context, "if", TheFunction);
	BasicBlock *elseBlock = BasicBlock::Create(Context, "else");
	BasicBlock *nextBlock = BasicBlock::Create(Context, "ifcont");
	Builder.CreateCondBr(cond, ifBlock, elseBlock);	
	Builder.SetInsertPoint(ifBlock);
	Value* ifval  = this->stmnts_if->Codegen();
	Builder.CreateBr(nextBlock);
	ifBlock = Builder.GetInsertBlock();
	TheFunction->getBasicBlockList().push_back(elseBlock);
	Builder.SetInsertPoint(elseBlock);
	Value* elseval;
	if(this->stmnts_else != NULL) elseval = this->stmnts_else->Codegen();
	Builder.CreateBr(nextBlock);
	elseBlock = Builder.GetInsertBlock();
	TheFunction->getBasicBlockList().push_back(nextBlock);
	Builder.SetInsertPoint(nextBlock);
	Value *V = ConstantInt::get(getGlobalContext(), APInt(32,0));
	return V;
}
Value* Bool::Codegen()
{
	if(this->bl != NULL) return this->bl->Codegen();
	Value *lhs = this->lhs->Codegen();
	Value *rhs = this->rhs->Codegen();
	if(this->op == "<") return Builder.CreateICmpULT(lhs, rhs, "ltcomparetmp");
	if(this->op == ">") return Builder.CreateICmpUGT(lhs, rhs, "gtcomparetmp");
	if(this->op == "<=") return Builder.CreateICmpULE(lhs, rhs, "lecomparetmp");
	if(this->op == ">=") return Builder.CreateICmpUGE(lhs, rhs, "gecomparetmp");
	if(this->op == "==") return Builder.CreateICmpEQ(lhs, rhs, "equalcomparetmp");
	if(this->op == "!=") return Builder.CreateICmpNE(lhs, rhs, "notequalcomparetmp");
}
Value* Reading::Codegen()
{
	return this->varr->Codegen();
}
Value* Varr::Codegen()
{
	vector<Type*> argTypes;
	vector<Value*> Args;
	string dum="";
	for(int i=0;i<this->identsr.size();i++) dum += "%d";
	Value *dumstr = Builder.CreateGlobalStringPtr(dum);
	Args.push_back(dumstr);
	argTypes.push_back(dumstr->getType());
	for(int i=0;i<this->identsr.size();i++)
	{
		Value *tmp = this->identsr[i]->Codegen();
		Args.push_back(tmp);
		argTypes.push_back(tmp->getType());
	}
	ArrayRef<Type*> argsRef(argTypes);
	ArrayRef<Value*> funcargs(Args);
	FunctionType *Ftype = FunctionType::get(Type::getInt32Ty(Context), argsRef, false);
	Constant* func = ModuleOb->getOrInsertFunction("scanf", Ftype);
	return Builder.CreateCall(func, funcargs);
}
Value* Identr::Codegen()
{
	if(this->expr == NULL)
	{
		Value *v = ModuleOb->getNamedGlobal(this->identifier);
		return v;
	}
	else
	{
		Value* V = ModuleOb->getNamedGlobal(this->identifier);
		Value* index = this->expr->Codegen();
		vector<Value*> arr_ind;
		arr_ind.push_back(Builder.getInt32(0));
		arr_ind.push_back(index);
		V = Builder.CreateGEP(V,arr_ind,this->identifier);
		return V;
	}
}
Value* Printing::Codegen()
{
	return this->var1->Codegen();
}
Value* Var1::Codegen()
{
	vector<Type*> argTypes;
	vector<Value*> Args;
	string dum="";
	for(int i=0;i<this->idents.size();i++)
	{
		if(this->idents[i]->expr != NULL) dum += "%d ";
		else { dum += this->idents[i]->STR; dum += " ";}
	}
	dum += "\n";
	Value *dumstr = Builder.CreateGlobalStringPtr(dum);
	Args.push_back(dumstr);
	argTypes.push_back(dumstr->getType());
	for(int i=0;i<this->idents.size();i++)
	{
		if(this->idents[i]->expr != NULL)
		{
			Value *tmp = this->idents[i]->Codegen();
			Args.push_back(tmp);
			argTypes.push_back(tmp->getType());
		}
	}
	ArrayRef<Type*> argsRef(argTypes);
	ArrayRef<Value*> funcargs(Args);
	FunctionType *Ftype = FunctionType::get(Type::getInt32Ty(Context), argsRef, false);
	Constant* func = ModuleOb->getOrInsertFunction("printf", Ftype);
	return Builder.CreateCall(func, funcargs);
}
Value* Ident::Codegen()
{
	if(this->expr != NULL) return this->expr->Codegen();
	else return Builder.CreateGlobalStringPtr(this->STR);
}
Value* Assign::Codegen()
{
	if(this->expr2 == NULL)
	{
		Value *v1 = this->expr1->Codegen();
		Value *nm = ModuleOb->getNamedGlobal(this->name);
		return Builder.CreateStore(v1, nm);
	}
	else
	{
		Value *index = this->expr1->Codegen();
		Value *v1 = this->expr2->Codegen();
		Value *nm = ModuleOb->getNamedGlobal(this->name);
		vector<Value*> arr_ind;
		arr_ind.push_back(Builder.getInt32(0));
		arr_ind.push_back(index);
		nm = Builder.CreateGEP(nm, arr_ind, this->name);
		return Builder.CreateStore(v1, nm);
	}
}
Value* Expr::Codegen()
{
	if(this->type == 1)
	{
		Value *left = this->lhs->Codegen();
		Value *right = this->rhs->Codegen();
		Value *v;
		if(this->op == "+") v = Builder.CreateAdd(left,right,"addtmp");
		else if(this->op == "-") v = Builder.CreateSub(left,right,"subtmp");
		else if(this->op == "*") v = Builder.CreateMul(left,right,"multmp");	
		else if(this->op == "/") v = Builder.CreateUDiv(left,right,"divtmp");
		else if(this->op == "%") v = Builder.CreateURem(left,right,"modtmp");
		else if(this->op == "^") v = Builder.CreateXor(left, right, "xortmp");
		else if(this->op == "&") v = Builder.CreateAnd(left, right, "andtmp");
		else if(this->op == "|") v = Builder.CreateOr(left, right, "ortmp");
		return v;
	}
	else if(this->type == 2) return this->exp->Codegen();
	else if(this->type == 3) return ConstantInt::get(getGlobalContext(), APInt(32, this->num));
	else if(this->type == 4) 
	{ 
		Value* V = ModuleOb->getNamedGlobal(this->identifier);
		V = Builder.CreateLoad(V);
		return V;
	}
	else if(this->type == 5)
	{
		Value* V = ModuleOb->getNamedGlobal(this->identifier);
		Value* index = this->exp->Codegen();
		vector<Value*> arr_ind;
		arr_ind.push_back(Builder.getInt32(0));
		arr_ind.push_back(index);
		V = Builder.CreateGEP(V,arr_ind,this->identifier);
		V = Builder.CreateLoad(V);
		return V;
	}
}
void Prog::generateCode()
{
	ModuleOb->dump();
}

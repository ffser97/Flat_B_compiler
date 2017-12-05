#bcc:	parser.tab.c lex.yy.c
#	g++ parser.tab.c lex.yy.c classes.cpp -lfl -o bcc
#parser.tab.c: parser.y 
#	bison -d parser.y 
#parser.tab.h: parser.y
#	bison -d parser.y 
#lex.yy.c: scanner.l parser.tab.h
#	flex scanner.l

#.PHONY: clean 
#clean:
#	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true




bcc: lex.yy.c parser.tab.c classes.cpp classes.h
	g++ -w -Wno-deprecated -g -std=c++11 lex.yy.c parser.tab.c classes.cpp -lfl `llvm-config-3.8 --cxxflags --cppflags mcjit native --ldflags --libs core` -ltinfo -lpthread -ldl -o bcc

lex.yy.c: scanner.l
	flex scanner.l
parser.tab.c: parser.y
	bison -vd parser.y
clean:
	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true

/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DECL = 258,
    CODE = 259,
    NUMBER = 260,
    IDENTIFIER = 261,
    ETOK = 262,
    INT = 263,
    IF = 264,
    ELSE = 265,
    FOR = 266,
    WHILE = 267,
    GOT = 268,
    GOTO = 269,
    PRINT = 270,
    READ = 271,
    STRING = 272,
    BREAK = 273,
    CONTINUE = 274,
    OR_COND = 275,
    AND_COND = 276,
    ADD = 277,
    SUB = 278,
    MUL = 279,
    DIV = 280,
    MOD = 281,
    XOR = 282,
    EQ = 283,
    E = 284,
    NE = 285,
    GE = 286,
    GT = 287,
    LE = 288,
    LT = 289,
    OR = 290,
    AND = 291,
    COL = 292,
    OP = 293,
    CP = 294,
    SC = 295,
    OB = 296,
    CB = 297,
    COMMA = 298,
    OSB = 299,
    OCB = 300
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

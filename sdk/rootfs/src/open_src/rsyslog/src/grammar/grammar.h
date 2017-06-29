/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_GRAMMAR_H_INCLUDED
# define YY_YY_GRAMMAR_H_INCLUDED
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
    NAME = 258,
    FUNC = 259,
    BEGINOBJ = 260,
    ENDOBJ = 261,
    BEGIN_ACTION = 262,
    BEGIN_PROPERTY = 263,
    BEGIN_CONSTANT = 264,
    BEGIN_TPL = 265,
    BEGIN_RULESET = 266,
    STOP = 267,
    SET = 268,
    RESET = 269,
    UNSET = 270,
    CONTINUE = 271,
    CALL = 272,
    LEGACY_ACTION = 273,
    LEGACY_RULESET = 274,
    PRIFILT = 275,
    PROPFILT = 276,
    BSD_TAG_SELECTOR = 277,
    BSD_HOST_SELECTOR = 278,
    IF = 279,
    THEN = 280,
    ELSE = 281,
    FOREACH = 282,
    ITERATOR_ASSIGNMENT = 283,
    DO = 284,
    OR = 285,
    AND = 286,
    NOT = 287,
    VAR = 288,
    STRING = 289,
    NUMBER = 290,
    CMP_EQ = 291,
    CMP_NE = 292,
    CMP_LE = 293,
    CMP_GE = 294,
    CMP_LT = 295,
    CMP_GT = 296,
    CMP_CONTAINS = 297,
    CMP_CONTAINSI = 298,
    CMP_STARTSWITH = 299,
    CMP_STARTSWITHI = 300,
    UMINUS = 301
  };
#endif
/* Tokens.  */
#define NAME 258
#define FUNC 259
#define BEGINOBJ 260
#define ENDOBJ 261
#define BEGIN_ACTION 262
#define BEGIN_PROPERTY 263
#define BEGIN_CONSTANT 264
#define BEGIN_TPL 265
#define BEGIN_RULESET 266
#define STOP 267
#define SET 268
#define RESET 269
#define UNSET 270
#define CONTINUE 271
#define CALL 272
#define LEGACY_ACTION 273
#define LEGACY_RULESET 274
#define PRIFILT 275
#define PROPFILT 276
#define BSD_TAG_SELECTOR 277
#define BSD_HOST_SELECTOR 278
#define IF 279
#define THEN 280
#define ELSE 281
#define FOREACH 282
#define ITERATOR_ASSIGNMENT 283
#define DO 284
#define OR 285
#define AND 286
#define NOT 287
#define VAR 288
#define STRING 289
#define NUMBER 290
#define CMP_EQ 291
#define CMP_NE 292
#define CMP_LE 293
#define CMP_GE 294
#define CMP_LT 295
#define CMP_GT 296
#define CMP_CONTAINS 297
#define CMP_CONTAINSI 298
#define CMP_STARTSWITH 299
#define CMP_STARTSWITHI 300
#define UMINUS 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 43 "grammar.y" /* yacc.c:1909  */

	char *s;
	long long n;
	es_str_t *estr;
	enum cnfobjType objType;
	struct cnfobj *obj;
	struct cnfstmt *stmt;
	struct nvlst *nvlst;
	struct objlst *objlst;
	struct cnfexpr *expr;
	struct cnfarray *arr;
	struct cnffunc *func;
	struct cnffparamlst *fparams;
	struct cnfitr *itr;

#line 162 "grammar.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */

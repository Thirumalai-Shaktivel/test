/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_HH_INCLUDED
# define YY_YY_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 21 "parser.yy" /* glr.c:197  */

#include <lfortran/parser/parser.h>

#line 48 "parser.tab.hh" /* glr.c:197  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END_OF_FILE = 0,
    TK_NAME = 258,
    TK_DEF_OP = 259,
    TK_INTEGER = 260,
    TK_REAL = 261,
    TK_NEWLINE = 262,
    TK_STRING = 263,
    TK_DBL_DOT = 264,
    TK_DBL_COLON = 265,
    TK_POW = 266,
    TK_CONCAT = 267,
    TK_ARROW = 268,
    TK_EQ = 269,
    TK_NE = 270,
    TK_LT = 271,
    TK_LE = 272,
    TK_GT = 273,
    TK_GE = 274,
    TK_NOT = 275,
    TK_AND = 276,
    TK_OR = 277,
    TK_EQV = 278,
    TK_NEQV = 279,
    TK_TRUE = 280,
    TK_FALSE = 281,
    KW_ABSTRACT = 282,
    KW_ALL = 283,
    KW_ALLOCATABLE = 284,
    KW_ALLOCATE = 285,
    KW_ASSIGNMENT = 286,
    KW_ASSOCIATE = 287,
    KW_ASYNCHRONOUS = 288,
    KW_BACKSPACE = 289,
    KW_BIND = 290,
    KW_BLOCK = 291,
    KW_CALL = 292,
    KW_CASE = 293,
    KW_CHARACTER = 294,
    KW_CLASS = 295,
    KW_CLOSE = 296,
    KW_CODIMENSION = 297,
    KW_COMMON = 298,
    KW_COMPLEX = 299,
    KW_CONCURRENT = 300,
    KW_CONTAINS = 301,
    KW_CONTIGUOUS = 302,
    KW_CONTINUE = 303,
    KW_CRITICAL = 304,
    KW_CYCLE = 305,
    KW_DATA = 306,
    KW_DEALLOCATE = 307,
    KW_DEFAULT = 308,
    KW_DEFERRED = 309,
    KW_DIMENSION = 310,
    KW_DO = 311,
    KW_DOWHILE = 312,
    KW_DOUBLE = 313,
    KW_ELEMENTAL = 314,
    KW_ELSE = 315,
    KW_END = 316,
    KW_ENTRY = 317,
    KW_ENUM = 318,
    KW_ENUMERATOR = 319,
    KW_EQUIVALENCE = 320,
    KW_ERRMSG = 321,
    KW_ERROR = 322,
    KW_EXIT = 323,
    KW_EXTENDS = 324,
    KW_EXTERNAL = 325,
    KW_FILE = 326,
    KW_FINAL = 327,
    KW_FLUSH = 328,
    KW_FORALL = 329,
    KW_FORMAT = 330,
    KW_FORMATTED = 331,
    KW_FUNCTION = 332,
    KW_GENERIC = 333,
    KW_GO = 334,
    KW_IF = 335,
    KW_IMPLICIT = 336,
    KW_IMPORT = 337,
    KW_IMPURE = 338,
    KW_IN = 339,
    KW_INCLUDE = 340,
    KW_INOUT = 341,
    KW_INQUIRE = 342,
    KW_INTEGER = 343,
    KW_INTENT = 344,
    KW_INTERFACE = 345,
    KW_INTRINSIC = 346,
    KW_IS = 347,
    KW_KIND = 348,
    KW_LEN = 349,
    KW_LOCAL = 350,
    KW_LOCAL_INIT = 351,
    KW_LOGICAL = 352,
    KW_MODULE = 353,
    KW_MOLD = 354,
    KW_NAME = 355,
    KW_NAMELIST = 356,
    KW_NOPASS = 357,
    KW_NON_INTRINSIC = 358,
    KW_NON_OVERRIDABLE = 359,
    KW_NON_RECURSIVE = 360,
    KW_NONE = 361,
    KW_NULLIFY = 362,
    KW_ONLY = 363,
    KW_OPEN = 364,
    KW_OPERATOR = 365,
    KW_OPTIONAL = 366,
    KW_OUT = 367,
    KW_PARAMETER = 368,
    KW_PASS = 369,
    KW_POINTER = 370,
    KW_PRECISION = 371,
    KW_PRINT = 372,
    KW_PRIVATE = 373,
    KW_PROCEDURE = 374,
    KW_PROGRAM = 375,
    KW_PROTECTED = 376,
    KW_PUBLIC = 377,
    KW_PURE = 378,
    KW_QUIET = 379,
    KW_RANK = 380,
    KW_READ = 381,
    KW_REAL = 382,
    KW_RECURSIVE = 383,
    KW_RESULT = 384,
    KW_RETURN = 385,
    KW_REWIND = 386,
    KW_SAVE = 387,
    KW_SELECT = 388,
    KW_SEQUENCE = 389,
    KW_SHARED = 390,
    KW_SOURCE = 391,
    KW_STAT = 392,
    KW_STOP = 393,
    KW_SUBMODULE = 394,
    KW_SUBROUTINE = 395,
    KW_TARGET = 396,
    KW_TEAM = 397,
    KW_TEAM_NUMBER = 398,
    KW_THEN = 399,
    KW_TO = 400,
    KW_TYPE = 401,
    KW_UNFORMATTED = 402,
    KW_USE = 403,
    KW_VALUE = 404,
    KW_VOLATILE = 405,
    KW_WHERE = 406,
    KW_WHILE = 407,
    KW_WRITE = 408
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef LFortran::YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (LFortran::Parser &p);

#endif /* !YY_YY_PARSER_TAB_HH_INCLUDED  */

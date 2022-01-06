/* A Bison parser, made by GNU Bison 3.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

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
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 25 "parser.yy"

#include <lfortran/parser/parser.h>

#line 48 "parser.tab.hh"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END_OF_FILE = 0,
    TK_NEWLINE = 258,
    TK_NAME = 259,
    TK_DEF_OP = 260,
    TK_INTEGER = 261,
    TK_LABEL = 262,
    TK_REAL = 263,
    TK_BOZ_CONSTANT = 264,
    TK_PLUS = 265,
    TK_MINUS = 266,
    TK_STAR = 267,
    TK_SLASH = 268,
    TK_COLON = 269,
    TK_SEMICOLON = 270,
    TK_COMMA = 271,
    TK_EQUAL = 272,
    TK_LPAREN = 273,
    TK_RPAREN = 274,
    TK_LBRACKET = 275,
    TK_RBRACKET = 276,
    TK_RBRACKET_OLD = 277,
    TK_PERCENT = 278,
    TK_VBAR = 279,
    TK_STRING = 280,
    TK_COMMENT = 281,
    TK_EOLCOMMENT = 282,
    TK_DBL_DOT = 283,
    TK_DBL_COLON = 284,
    TK_POW = 285,
    TK_CONCAT = 286,
    TK_ARROW = 287,
    TK_EQ = 288,
    TK_NE = 289,
    TK_LT = 290,
    TK_LE = 291,
    TK_GT = 292,
    TK_GE = 293,
    TK_NOT = 294,
    TK_AND = 295,
    TK_OR = 296,
    TK_XOR = 297,
    TK_EQV = 298,
    TK_NEQV = 299,
    TK_TRUE = 300,
    TK_FALSE = 301,
    TK_FORMAT = 302,
    KW_ABSTRACT = 303,
    KW_ALL = 304,
    KW_ALLOCATABLE = 305,
    KW_ALLOCATE = 306,
    KW_ASSIGN = 307,
    KW_ASSIGNMENT = 308,
    KW_ASSOCIATE = 309,
    KW_ASYNCHRONOUS = 310,
    KW_BACKSPACE = 311,
    KW_BIND = 312,
    KW_BLOCK = 313,
    KW_CALL = 314,
    KW_CASE = 315,
    KW_CHANGE = 316,
    KW_CHANGE_TEAM = 317,
    KW_CHARACTER = 318,
    KW_CLASS = 319,
    KW_CLOSE = 320,
    KW_CODIMENSION = 321,
    KW_COMMON = 322,
    KW_COMPLEX = 323,
    KW_CONCURRENT = 324,
    KW_CONTAINS = 325,
    KW_CONTIGUOUS = 326,
    KW_CONTINUE = 327,
    KW_CRITICAL = 328,
    KW_CYCLE = 329,
    KW_DATA = 330,
    KW_DEALLOCATE = 331,
    KW_DEFAULT = 332,
    KW_DEFERRED = 333,
    KW_DIMENSION = 334,
    KW_DO = 335,
    KW_DOWHILE = 336,
    KW_DOUBLE = 337,
    KW_DOUBLE_PRECISION = 338,
    KW_ELEMENTAL = 339,
    KW_ELSE = 340,
    KW_ELSEIF = 341,
    KW_ELSEWHERE = 342,
    KW_END = 343,
    KW_END_PROGRAM = 344,
    KW_ENDPROGRAM = 345,
    KW_END_MODULE = 346,
    KW_ENDMODULE = 347,
    KW_END_SUBMODULE = 348,
    KW_ENDSUBMODULE = 349,
    KW_END_BLOCK = 350,
    KW_ENDBLOCK = 351,
    KW_END_BLOCK_DATA = 352,
    KW_ENDBLOCKDATA = 353,
    KW_END_SUBROUTINE = 354,
    KW_ENDSUBROUTINE = 355,
    KW_END_FUNCTION = 356,
    KW_ENDFUNCTION = 357,
    KW_END_PROCEDURE = 358,
    KW_ENDPROCEDURE = 359,
    KW_END_ENUM = 360,
    KW_ENDENUM = 361,
    KW_END_SELECT = 362,
    KW_ENDSELECT = 363,
    KW_END_IF = 364,
    KW_ENDIF = 365,
    KW_END_INTERFACE = 366,
    KW_ENDINTERFACE = 367,
    KW_END_TYPE = 368,
    KW_ENDTYPE = 369,
    KW_END_ASSOCIATE = 370,
    KW_ENDASSOCIATE = 371,
    KW_END_FORALL = 372,
    KW_ENDFORALL = 373,
    KW_END_DO = 374,
    KW_ENDDO = 375,
    KW_END_WHERE = 376,
    KW_ENDWHERE = 377,
    KW_END_CRITICAL = 378,
    KW_ENDCRITICAL = 379,
    KW_END_FILE = 380,
    KW_ENDFILE = 381,
    KW_END_TEAM = 382,
    KW_ENDTEAM = 383,
    KW_ENTRY = 384,
    KW_ENUM = 385,
    KW_ENUMERATOR = 386,
    KW_EQUIVALENCE = 387,
    KW_ERRMSG = 388,
    KW_ERROR = 389,
    KW_EVENT = 390,
    KW_EXIT = 391,
    KW_EXTENDS = 392,
    KW_EXTERNAL = 393,
    KW_FILE = 394,
    KW_FINAL = 395,
    KW_FLUSH = 396,
    KW_FORALL = 397,
    KW_FORMATTED = 398,
    KW_FORM = 399,
    KW_FORM_TEAM = 400,
    KW_FUNCTION = 401,
    KW_GENERIC = 402,
    KW_GO = 403,
    KW_GOTO = 404,
    KW_IF = 405,
    KW_IMAGES = 406,
    KW_IMPLICIT = 407,
    KW_IMPORT = 408,
    KW_IMPURE = 409,
    KW_IN = 410,
    KW_INCLUDE = 411,
    KW_INOUT = 412,
    KW_IN_OUT = 413,
    KW_INQUIRE = 414,
    KW_INTEGER = 415,
    KW_INTENT = 416,
    KW_INTERFACE = 417,
    KW_INTRINSIC = 418,
    KW_IS = 419,
    KW_KIND = 420,
    KW_LEN = 421,
    KW_LOCAL = 422,
    KW_LOCAL_INIT = 423,
    KW_LOGICAL = 424,
    KW_MEMORY = 425,
    KW_MODULE = 426,
    KW_MOLD = 427,
    KW_NAME = 428,
    KW_NAMELIST = 429,
    KW_NEW_INDEX = 430,
    KW_NOPASS = 431,
    KW_NON_INTRINSIC = 432,
    KW_NON_OVERRIDABLE = 433,
    KW_NON_RECURSIVE = 434,
    KW_NONE = 435,
    KW_NULLIFY = 436,
    KW_ONLY = 437,
    KW_OPEN = 438,
    KW_OPERATOR = 439,
    KW_OPTIONAL = 440,
    KW_OUT = 441,
    KW_PARAMETER = 442,
    KW_PASS = 443,
    KW_POINTER = 444,
    KW_POST = 445,
    KW_PRECISION = 446,
    KW_PRINT = 447,
    KW_PRIVATE = 448,
    KW_PROCEDURE = 449,
    KW_PROGRAM = 450,
    KW_PROTECTED = 451,
    KW_PUBLIC = 452,
    KW_PURE = 453,
    KW_QUIET = 454,
    KW_RANK = 455,
    KW_READ = 456,
    KW_REAL = 457,
    KW_RECURSIVE = 458,
    KW_REDUCE = 459,
    KW_RESULT = 460,
    KW_RETURN = 461,
    KW_REWIND = 462,
    KW_SAVE = 463,
    KW_SELECT = 464,
    KW_SELECT_CASE = 465,
    KW_SELECT_RANK = 466,
    KW_SELECT_TYPE = 467,
    KW_SEQUENCE = 468,
    KW_SHARED = 469,
    KW_SOURCE = 470,
    KW_STAT = 471,
    KW_STOP = 472,
    KW_SUBMODULE = 473,
    KW_SUBROUTINE = 474,
    KW_SYNC = 475,
    KW_SYNC_ALL = 476,
    KW_SYNC_IMAGES = 477,
    KW_SYNC_MEMORY = 478,
    KW_SYNC_TEAM = 479,
    KW_TARGET = 480,
    KW_TEAM = 481,
    KW_TEAM_NUMBER = 482,
    KW_THEN = 483,
    KW_TO = 484,
    KW_TYPE = 485,
    KW_UNFORMATTED = 486,
    KW_USE = 487,
    KW_VALUE = 488,
    KW_VOLATILE = 489,
    KW_WAIT = 490,
    KW_WHERE = 491,
    KW_WHILE = 492,
    KW_WRITE = 493,
    UMINUS = 494
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

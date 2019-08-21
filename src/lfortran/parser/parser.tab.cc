/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton implementation for Bison GLR parsers in C

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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 1






/* First part of user declarations.  */

#line 55 "parser.tab.cc" /* glr.c:240  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.tab.hh"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;

/* Copy the second part of user declarations.  */

#line 88 "parser.tab.cc" /* glr.c:263  */
/* Unqualified %code blocks.  */
#line 26 "parser.yy" /* glr.c:264  */


#define union struct

#define YYASSERT(Condition) LFORTRAN_ASSERT(Condition)

#include <lfortran/parser/parser.h>
#include <lfortran/parser/tokenizer.h>
#include <lfortran/parser/semantics.h>

int yylex(LFortran::YYSTYPE *yylval, YYLTYPE *yyloc, LFortran::Parser &p)
{
    return p.m_tokenizer.lex(*yylval, *yyloc);
} // ylex

void yyerror(YYLTYPE *yyloc, LFortran::Parser &p, const std::string &msg)
{
    LFortran::YYSTYPE yylval_;
    YYLTYPE yyloc_;
    p.m_tokenizer.cur = p.m_tokenizer.tok;
    int token = p.m_tokenizer.lex(yylval_, yyloc_);
    throw LFortran::ParserError(msg, *yyloc, token);
}


#line 116 "parser.tab.cc" /* glr.c:264  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT2 (0))
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#define YYASSERT2(Condition) ((void) ((Condition) || (abort (), 0)))
#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  151
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1695

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  162
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  160
/* YYNRULES -- Number of states.  */
#define YYNSTATES  203
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 12
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   408

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     160,   161,   156,   155,     2,   154,     2,   157,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   158,
       2,   159,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153
};

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   244,   244,   245,   249,   250,   251,   252,   253,   261,
     266,   271,   279,   280,   284,   285,   289,   290,   294,   296,
     300,   304,   308,   310,   312,   343,   344,   345,   346,   347,
     348,   349,   350,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "END_OF_FILE", "error", "$undefined", "TK_NAME", "TK_DEF_OP",
  "TK_INTEGER", "TK_REAL", "TK_NEWLINE", "TK_STRING", "TK_DBL_DOT",
  "TK_DBL_COLON", "TK_POW", "TK_CONCAT", "TK_ARROW", "TK_EQ", "TK_NE",
  "TK_LT", "TK_LE", "TK_GT", "TK_GE", "TK_NOT", "TK_AND", "TK_OR",
  "TK_EQV", "TK_NEQV", "TK_TRUE", "TK_FALSE", "KW_ABSTRACT", "KW_ALL",
  "KW_ALLOCATABLE", "KW_ALLOCATE", "KW_ASSIGNMENT", "KW_ASSOCIATE",
  "KW_ASYNCHRONOUS", "KW_BACKSPACE", "KW_BIND", "KW_BLOCK", "KW_CALL",
  "KW_CASE", "KW_CHARACTER", "KW_CLASS", "KW_CLOSE", "KW_CODIMENSION",
  "KW_COMMON", "KW_COMPLEX", "KW_CONCURRENT", "KW_CONTAINS",
  "KW_CONTIGUOUS", "KW_CONTINUE", "KW_CRITICAL", "KW_CYCLE", "KW_DATA",
  "KW_DEALLOCATE", "KW_DEFAULT", "KW_DEFERRED", "KW_DIMENSION", "KW_DO",
  "KW_DOWHILE", "KW_DOUBLE", "KW_ELEMENTAL", "KW_ELSE", "KW_END",
  "KW_ENTRY", "KW_ENUM", "KW_ENUMERATOR", "KW_EQUIVALENCE", "KW_ERRMSG",
  "KW_ERROR", "KW_EXIT", "KW_EXTENDS", "KW_EXTERNAL", "KW_FILE",
  "KW_FINAL", "KW_FLUSH", "KW_FORALL", "KW_FORMAT", "KW_FORMATTED",
  "KW_FUNCTION", "KW_GENERIC", "KW_GO", "KW_IF", "KW_IMPLICIT",
  "KW_IMPORT", "KW_IMPURE", "KW_IN", "KW_INCLUDE", "KW_INOUT",
  "KW_INQUIRE", "KW_INTEGER", "KW_INTENT", "KW_INTERFACE", "KW_INTRINSIC",
  "KW_IS", "KW_KIND", "KW_LEN", "KW_LOCAL", "KW_LOCAL_INIT", "KW_LOGICAL",
  "KW_MODULE", "KW_MOLD", "KW_NAME", "KW_NAMELIST", "KW_NOPASS",
  "KW_NON_INTRINSIC", "KW_NON_OVERRIDABLE", "KW_NON_RECURSIVE", "KW_NONE",
  "KW_NULLIFY", "KW_ONLY", "KW_OPEN", "KW_OPERATOR", "KW_OPTIONAL",
  "KW_OUT", "KW_PARAMETER", "KW_PASS", "KW_POINTER", "KW_PRECISION",
  "KW_PRINT", "KW_PRIVATE", "KW_PROCEDURE", "KW_PROGRAM", "KW_PROTECTED",
  "KW_PUBLIC", "KW_PURE", "KW_QUIET", "KW_RANK", "KW_READ", "KW_REAL",
  "KW_RECURSIVE", "KW_RESULT", "KW_RETURN", "KW_REWIND", "KW_SAVE",
  "KW_SELECT", "KW_SEQUENCE", "KW_SHARED", "KW_SOURCE", "KW_STAT",
  "KW_STOP", "KW_SUBMODULE", "KW_SUBROUTINE", "KW_TARGET", "KW_TEAM",
  "KW_TEAM_NUMBER", "KW_THEN", "KW_TO", "KW_TYPE", "KW_UNFORMATTED",
  "KW_USE", "KW_VALUE", "KW_VOLATILE", "KW_WHERE", "KW_WHILE", "KW_WRITE",
  "'-'", "'+'", "'*'", "'/'", "';'", "'='", "'('", "')'", "$accept",
  "units", "script_unit", "program", "subroutine", "function",
  "statements", "sep", "sep_one", "statement", "assignment_statement",
  "if_statement", "if_block", "expr", "id", YY_NULLPTR
};
#endif

#define YYPACT_NINF -173
#define YYTABLE_NINF -1

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const short int yypact[] =
{
    1240,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  1542,  -173,  -173,  -152,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  1542,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  1542,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    1391,     6,  -173,  -173,  -173,  -173,  -173,  -173,  -173,   -52,
      33,  -173,  -173,  -173,  -173,  -173,    -2,  1391,    -2,    -2,
      11,  -173,  -173,  -173,   177,  -173,   -70,  1391,  1391,  1391,
    1391,  1391,  1391,   329,    20,   329,   329,  -173,  -173,  -173,
    -173,     9,    -7,    -7,     9,     9,    43,    -2,  -173,    33,
    -133,    -2,    -2,   481,    -2,   633,   785,   -60,  -173,   329,
     -99,  -117,  -173,    -2,  -173,  -173,   937,    -5,  -152,   329,
    -173,    -2,  1089
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    33,    32,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
       0,     0,     3,     4,     5,     6,     7,    18,    19,     0,
       8,    31,    84,    87,   127,   147,     0,     0,     0,     0,
       0,     1,    16,    17,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     2,    14,
      21,    29,    26,    25,    27,    28,    20,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,    68,    12,     0,
      68,    68,    11,     0,     9,    10,     0,    67,     0,     0,
      24,     0,     0
};

  /* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -173,  -173,  -130,  -173,  -173,  -173,  -138,  -134,  -147,     0,
    -173,  -173,  -172,     1,   -50
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,   131,   132,   133,   134,   135,   177,   154,   155,   178,
     137,   138,   139,   179,   141
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const unsigned char yytable[] =
{
     136,   140,   152,   146,   157,   152,   151,   169,   147,   156,
     170,   184,   163,   152,   165,   166,   169,   192,   169,   169,
     157,   194,   157,   195,   168,   200,     0,   181,   182,     0,
       0,   157,     0,     0,     0,     0,   169,     0,   169,   169,
       0,     0,   169,   183,   157,     0,   148,   185,   186,   169,
     189,   193,   169,     0,   157,   169,     0,     0,     0,   196,
       0,   201,     0,   199,     0,     0,   149,   202,     0,     0,
       0,     0,     0,     0,     0,   198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   150,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   164,   160,
     161,     0,     0,   153,   136,   140,   153,     0,   171,   172,
     173,   174,   175,   176,   153,   158,   159,   160,   161,     0,
       0,     0,   167,     0,   158,   159,   160,   161,     0,     0,
       1,   180,     2,   188,   152,   188,   188,   158,   159,   160,
     161,     0,   162,     0,     0,     0,   188,   158,   159,   160,
     161,     0,   188,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,     0,     1,     0,     2,   153,   152,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,   142,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,   144,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   145,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,     0,     1,     0,     2,   153,   152,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,   187,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,   142,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,   144,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   145,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,     0,     1,     0,     2,   153,
     152,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,   190,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     142,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   144,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   145,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,     0,     1,     0,
       2,   153,   152,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,   191,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,   142,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,   144,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   145,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,     0,
       1,     0,     2,   153,   152,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,   197,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,   142,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,   144,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   145,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,     0,     1,     0,     2,   153,   152,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,   142,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,   144,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   145,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,     1,     0,     2,     0,   153,     0,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     1,     0,     2,     0,     0,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,   142,    54,
      55,   143,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,   144,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   145,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,     1,     0,     0,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,   142,
      54,    55,   143,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,   144,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   145,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129
};

static const short int yycheck[] =
{
       0,     0,     7,    53,    11,     7,     0,   154,   160,    61,
      80,   144,   146,     7,   148,   149,   163,    77,   165,   166,
      11,   120,    11,   140,   154,   197,    -1,   165,   166,    -1,
      -1,    11,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,
      -1,    -1,   189,   177,    11,    -1,    96,   181,   182,   196,
     184,   189,   199,    -1,    11,   202,    -1,    -1,    -1,   193,
      -1,   199,    -1,   197,    -1,    -1,   116,   201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,   156,
     157,    -1,    -1,   158,   154,   154,   158,    -1,   157,   158,
     159,   160,   161,   162,   158,   154,   155,   156,   157,    -1,
      -1,    -1,   161,    -1,   154,   155,   156,   157,    -1,    -1,
       3,   161,     5,   183,     7,   185,   186,   154,   155,   156,
     157,    -1,   159,    -1,    -1,    -1,   196,   154,   155,   156,
     157,    -1,   202,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,     3,    -1,     5,   158,     7,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,     3,    -1,     5,   158,     7,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,     3,    -1,     5,   158,
       7,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,     3,    -1,
       5,   158,     7,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
       3,    -1,     5,   158,     7,   160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,     3,    -1,     5,   158,     7,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,     3,    -1,     5,    -1,   158,    -1,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,     3,    -1,     5,    -1,    -1,    -1,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,     3,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     5,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     160,   163,   164,   165,   166,   167,   171,   172,   173,   174,
     175,   176,    77,    80,   120,   140,   176,   160,   176,   176,
     175,     0,     7,   158,   169,   170,    61,    11,   154,   155,
     156,   157,   159,   169,   175,   169,   169,   161,   164,   170,
      80,   175,   175,   175,   175,   175,   175,   168,   171,   175,
     161,   168,   168,   169,   144,   169,   169,    61,   171,   169,
      61,    61,    77,   168,   120,   140,   169,    60,    80,   169,
     174,   168,   169
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   162,   163,   163,   164,   164,   164,   164,   164,   165,
     166,   167,   168,   168,   169,   169,   170,   170,   171,   171,
     172,   173,   174,   174,   174,   175,   175,   175,   175,   175,
     175,   175,   175,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     1,     1,     1,     1,     1,     1,     7,
       7,     7,     3,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     8,    12,    10,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};


/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYIMMEDIATE[RULE-NUM] -- True iff rule #RULE-NUM is not to be deferred, as
   in the case of predicates.  */
static const yybool yyimmediate[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    22,     0,    23,     0
};

/* Error token number */
#define YYTERROR 1


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

# define YYRHSLOC(Rhs, K) ((Rhs)[K].yystate.yyloc)



#undef yynerrs
#define yynerrs (yystackp->yyerrcnt)
#undef yychar
#define yychar (yystackp->yyrawchar)
#undef yylval
#define yylval (yystackp->yyval)
#undef yylloc
#define yylloc (yystackp->yyloc)


static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                              \
  do {                                          \
    YYRESULTTAG yychk_flag = YYE;               \
    if (yychk_flag != yyok)                     \
      return yychk_flag;                        \
  } while (0)

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YYDPRINTF(Args)                        \
  do {                                          \
    if (yydebug)                                \
      YYFPRINTF Args;                           \
  } while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, LFortran::Parser &p)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (p);
  if (!yyvaluep)
    return;
  switch (yytype)
    {
          case 3: /* TK_NAME  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1235 "parser.tab.cc" /* glr.c:463  */
        break;

    case 4: /* TK_DEF_OP  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1241 "parser.tab.cc" /* glr.c:463  */
        break;

    case 27: /* KW_ABSTRACT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1247 "parser.tab.cc" /* glr.c:463  */
        break;

    case 28: /* KW_ALL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1253 "parser.tab.cc" /* glr.c:463  */
        break;

    case 29: /* KW_ALLOCATABLE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1259 "parser.tab.cc" /* glr.c:463  */
        break;

    case 30: /* KW_ALLOCATE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1265 "parser.tab.cc" /* glr.c:463  */
        break;

    case 31: /* KW_ASSIGNMENT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1271 "parser.tab.cc" /* glr.c:463  */
        break;

    case 32: /* KW_ASSOCIATE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1277 "parser.tab.cc" /* glr.c:463  */
        break;

    case 33: /* KW_ASYNCHRONOUS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1283 "parser.tab.cc" /* glr.c:463  */
        break;

    case 34: /* KW_BACKSPACE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1289 "parser.tab.cc" /* glr.c:463  */
        break;

    case 35: /* KW_BIND  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1295 "parser.tab.cc" /* glr.c:463  */
        break;

    case 36: /* KW_BLOCK  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1301 "parser.tab.cc" /* glr.c:463  */
        break;

    case 37: /* KW_CALL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1307 "parser.tab.cc" /* glr.c:463  */
        break;

    case 38: /* KW_CASE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1313 "parser.tab.cc" /* glr.c:463  */
        break;

    case 39: /* KW_CHARACTER  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1319 "parser.tab.cc" /* glr.c:463  */
        break;

    case 40: /* KW_CLASS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1325 "parser.tab.cc" /* glr.c:463  */
        break;

    case 41: /* KW_CLOSE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1331 "parser.tab.cc" /* glr.c:463  */
        break;

    case 42: /* KW_CODIMENSION  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1337 "parser.tab.cc" /* glr.c:463  */
        break;

    case 43: /* KW_COMMON  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1343 "parser.tab.cc" /* glr.c:463  */
        break;

    case 44: /* KW_COMPLEX  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1349 "parser.tab.cc" /* glr.c:463  */
        break;

    case 45: /* KW_CONCURRENT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1355 "parser.tab.cc" /* glr.c:463  */
        break;

    case 46: /* KW_CONTAINS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1361 "parser.tab.cc" /* glr.c:463  */
        break;

    case 47: /* KW_CONTIGUOUS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1367 "parser.tab.cc" /* glr.c:463  */
        break;

    case 48: /* KW_CONTINUE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1373 "parser.tab.cc" /* glr.c:463  */
        break;

    case 49: /* KW_CRITICAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1379 "parser.tab.cc" /* glr.c:463  */
        break;

    case 50: /* KW_CYCLE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1385 "parser.tab.cc" /* glr.c:463  */
        break;

    case 51: /* KW_DATA  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1391 "parser.tab.cc" /* glr.c:463  */
        break;

    case 52: /* KW_DEALLOCATE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1397 "parser.tab.cc" /* glr.c:463  */
        break;

    case 53: /* KW_DEFAULT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1403 "parser.tab.cc" /* glr.c:463  */
        break;

    case 54: /* KW_DEFERRED  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1409 "parser.tab.cc" /* glr.c:463  */
        break;

    case 55: /* KW_DIMENSION  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1415 "parser.tab.cc" /* glr.c:463  */
        break;

    case 56: /* KW_DO  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1421 "parser.tab.cc" /* glr.c:463  */
        break;

    case 57: /* KW_DOWHILE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1427 "parser.tab.cc" /* glr.c:463  */
        break;

    case 58: /* KW_DOUBLE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1433 "parser.tab.cc" /* glr.c:463  */
        break;

    case 59: /* KW_ELEMENTAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1439 "parser.tab.cc" /* glr.c:463  */
        break;

    case 60: /* KW_ELSE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1445 "parser.tab.cc" /* glr.c:463  */
        break;

    case 61: /* KW_END  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1451 "parser.tab.cc" /* glr.c:463  */
        break;

    case 62: /* KW_ENTRY  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1457 "parser.tab.cc" /* glr.c:463  */
        break;

    case 63: /* KW_ENUM  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1463 "parser.tab.cc" /* glr.c:463  */
        break;

    case 64: /* KW_ENUMERATOR  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1469 "parser.tab.cc" /* glr.c:463  */
        break;

    case 65: /* KW_EQUIVALENCE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1475 "parser.tab.cc" /* glr.c:463  */
        break;

    case 66: /* KW_ERRMSG  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1481 "parser.tab.cc" /* glr.c:463  */
        break;

    case 67: /* KW_ERROR  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1487 "parser.tab.cc" /* glr.c:463  */
        break;

    case 68: /* KW_EXIT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1493 "parser.tab.cc" /* glr.c:463  */
        break;

    case 69: /* KW_EXTENDS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1499 "parser.tab.cc" /* glr.c:463  */
        break;

    case 70: /* KW_EXTERNAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1505 "parser.tab.cc" /* glr.c:463  */
        break;

    case 71: /* KW_FILE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1511 "parser.tab.cc" /* glr.c:463  */
        break;

    case 72: /* KW_FINAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1517 "parser.tab.cc" /* glr.c:463  */
        break;

    case 73: /* KW_FLUSH  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1523 "parser.tab.cc" /* glr.c:463  */
        break;

    case 74: /* KW_FORALL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1529 "parser.tab.cc" /* glr.c:463  */
        break;

    case 75: /* KW_FORMAT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1535 "parser.tab.cc" /* glr.c:463  */
        break;

    case 76: /* KW_FORMATTED  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1541 "parser.tab.cc" /* glr.c:463  */
        break;

    case 77: /* KW_FUNCTION  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1547 "parser.tab.cc" /* glr.c:463  */
        break;

    case 78: /* KW_GENERIC  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1553 "parser.tab.cc" /* glr.c:463  */
        break;

    case 79: /* KW_GO  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1559 "parser.tab.cc" /* glr.c:463  */
        break;

    case 80: /* KW_IF  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1565 "parser.tab.cc" /* glr.c:463  */
        break;

    case 81: /* KW_IMPLICIT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1571 "parser.tab.cc" /* glr.c:463  */
        break;

    case 82: /* KW_IMPORT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1577 "parser.tab.cc" /* glr.c:463  */
        break;

    case 83: /* KW_IMPURE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1583 "parser.tab.cc" /* glr.c:463  */
        break;

    case 84: /* KW_IN  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1589 "parser.tab.cc" /* glr.c:463  */
        break;

    case 85: /* KW_INCLUDE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1595 "parser.tab.cc" /* glr.c:463  */
        break;

    case 86: /* KW_INOUT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1601 "parser.tab.cc" /* glr.c:463  */
        break;

    case 87: /* KW_INQUIRE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1607 "parser.tab.cc" /* glr.c:463  */
        break;

    case 88: /* KW_INTEGER  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1613 "parser.tab.cc" /* glr.c:463  */
        break;

    case 89: /* KW_INTENT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1619 "parser.tab.cc" /* glr.c:463  */
        break;

    case 90: /* KW_INTERFACE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1625 "parser.tab.cc" /* glr.c:463  */
        break;

    case 91: /* KW_INTRINSIC  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1631 "parser.tab.cc" /* glr.c:463  */
        break;

    case 92: /* KW_IS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1637 "parser.tab.cc" /* glr.c:463  */
        break;

    case 93: /* KW_KIND  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1643 "parser.tab.cc" /* glr.c:463  */
        break;

    case 94: /* KW_LEN  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1649 "parser.tab.cc" /* glr.c:463  */
        break;

    case 95: /* KW_LOCAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1655 "parser.tab.cc" /* glr.c:463  */
        break;

    case 96: /* KW_LOCAL_INIT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1661 "parser.tab.cc" /* glr.c:463  */
        break;

    case 97: /* KW_LOGICAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1667 "parser.tab.cc" /* glr.c:463  */
        break;

    case 98: /* KW_MODULE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1673 "parser.tab.cc" /* glr.c:463  */
        break;

    case 99: /* KW_MOLD  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1679 "parser.tab.cc" /* glr.c:463  */
        break;

    case 100: /* KW_NAME  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1685 "parser.tab.cc" /* glr.c:463  */
        break;

    case 101: /* KW_NAMELIST  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1691 "parser.tab.cc" /* glr.c:463  */
        break;

    case 102: /* KW_NOPASS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1697 "parser.tab.cc" /* glr.c:463  */
        break;

    case 103: /* KW_NON_INTRINSIC  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1703 "parser.tab.cc" /* glr.c:463  */
        break;

    case 104: /* KW_NON_OVERRIDABLE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1709 "parser.tab.cc" /* glr.c:463  */
        break;

    case 105: /* KW_NON_RECURSIVE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1715 "parser.tab.cc" /* glr.c:463  */
        break;

    case 106: /* KW_NONE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1721 "parser.tab.cc" /* glr.c:463  */
        break;

    case 107: /* KW_NULLIFY  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1727 "parser.tab.cc" /* glr.c:463  */
        break;

    case 108: /* KW_ONLY  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1733 "parser.tab.cc" /* glr.c:463  */
        break;

    case 109: /* KW_OPEN  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1739 "parser.tab.cc" /* glr.c:463  */
        break;

    case 110: /* KW_OPERATOR  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1745 "parser.tab.cc" /* glr.c:463  */
        break;

    case 111: /* KW_OPTIONAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1751 "parser.tab.cc" /* glr.c:463  */
        break;

    case 112: /* KW_OUT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1757 "parser.tab.cc" /* glr.c:463  */
        break;

    case 113: /* KW_PARAMETER  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1763 "parser.tab.cc" /* glr.c:463  */
        break;

    case 114: /* KW_PASS  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1769 "parser.tab.cc" /* glr.c:463  */
        break;

    case 115: /* KW_POINTER  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1775 "parser.tab.cc" /* glr.c:463  */
        break;

    case 116: /* KW_PRECISION  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1781 "parser.tab.cc" /* glr.c:463  */
        break;

    case 117: /* KW_PRINT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1787 "parser.tab.cc" /* glr.c:463  */
        break;

    case 118: /* KW_PRIVATE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1793 "parser.tab.cc" /* glr.c:463  */
        break;

    case 119: /* KW_PROCEDURE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1799 "parser.tab.cc" /* glr.c:463  */
        break;

    case 120: /* KW_PROGRAM  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1805 "parser.tab.cc" /* glr.c:463  */
        break;

    case 121: /* KW_PROTECTED  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1811 "parser.tab.cc" /* glr.c:463  */
        break;

    case 122: /* KW_PUBLIC  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1817 "parser.tab.cc" /* glr.c:463  */
        break;

    case 123: /* KW_PURE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1823 "parser.tab.cc" /* glr.c:463  */
        break;

    case 124: /* KW_QUIET  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1829 "parser.tab.cc" /* glr.c:463  */
        break;

    case 125: /* KW_RANK  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1835 "parser.tab.cc" /* glr.c:463  */
        break;

    case 126: /* KW_READ  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1841 "parser.tab.cc" /* glr.c:463  */
        break;

    case 127: /* KW_REAL  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1847 "parser.tab.cc" /* glr.c:463  */
        break;

    case 128: /* KW_RECURSIVE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1853 "parser.tab.cc" /* glr.c:463  */
        break;

    case 129: /* KW_RESULT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1859 "parser.tab.cc" /* glr.c:463  */
        break;

    case 130: /* KW_RETURN  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1865 "parser.tab.cc" /* glr.c:463  */
        break;

    case 131: /* KW_REWIND  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1871 "parser.tab.cc" /* glr.c:463  */
        break;

    case 132: /* KW_SAVE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1877 "parser.tab.cc" /* glr.c:463  */
        break;

    case 133: /* KW_SELECT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1883 "parser.tab.cc" /* glr.c:463  */
        break;

    case 134: /* KW_SEQUENCE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1889 "parser.tab.cc" /* glr.c:463  */
        break;

    case 135: /* KW_SHARED  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1895 "parser.tab.cc" /* glr.c:463  */
        break;

    case 136: /* KW_SOURCE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1901 "parser.tab.cc" /* glr.c:463  */
        break;

    case 137: /* KW_STAT  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1907 "parser.tab.cc" /* glr.c:463  */
        break;

    case 138: /* KW_STOP  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1913 "parser.tab.cc" /* glr.c:463  */
        break;

    case 139: /* KW_SUBMODULE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1919 "parser.tab.cc" /* glr.c:463  */
        break;

    case 140: /* KW_SUBROUTINE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1925 "parser.tab.cc" /* glr.c:463  */
        break;

    case 141: /* KW_TARGET  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1931 "parser.tab.cc" /* glr.c:463  */
        break;

    case 142: /* KW_TEAM  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1937 "parser.tab.cc" /* glr.c:463  */
        break;

    case 143: /* KW_TEAM_NUMBER  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1943 "parser.tab.cc" /* glr.c:463  */
        break;

    case 144: /* KW_THEN  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1949 "parser.tab.cc" /* glr.c:463  */
        break;

    case 145: /* KW_TO  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1955 "parser.tab.cc" /* glr.c:463  */
        break;

    case 146: /* KW_TYPE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1961 "parser.tab.cc" /* glr.c:463  */
        break;

    case 147: /* KW_UNFORMATTED  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1967 "parser.tab.cc" /* glr.c:463  */
        break;

    case 148: /* KW_USE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1973 "parser.tab.cc" /* glr.c:463  */
        break;

    case 149: /* KW_VALUE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1979 "parser.tab.cc" /* glr.c:463  */
        break;

    case 150: /* KW_VOLATILE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1985 "parser.tab.cc" /* glr.c:463  */
        break;

    case 151: /* KW_WHERE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1991 "parser.tab.cc" /* glr.c:463  */
        break;

    case 152: /* KW_WHILE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 1997 "parser.tab.cc" /* glr.c:463  */
        break;

    case 153: /* KW_WRITE  */
#line 16 "parser.yy" /* glr.c:463  */
      { fprintf(yyo, "%s", std::string(((*yyvaluep).string).p, ((*yyvaluep).string).n).c_str()); }
#line 2003 "parser.tab.cc" /* glr.c:463  */
        break;

    case 164: /* script_unit  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2009 "parser.tab.cc" /* glr.c:463  */
        break;

    case 165: /* program  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2015 "parser.tab.cc" /* glr.c:463  */
        break;

    case 166: /* subroutine  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2021 "parser.tab.cc" /* glr.c:463  */
        break;

    case 167: /* function  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2027 "parser.tab.cc" /* glr.c:463  */
        break;

    case 171: /* statement  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2033 "parser.tab.cc" /* glr.c:463  */
        break;

    case 172: /* assignment_statement  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2039 "parser.tab.cc" /* glr.c:463  */
        break;

    case 173: /* if_statement  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2045 "parser.tab.cc" /* glr.c:463  */
        break;

    case 174: /* if_block  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2051 "parser.tab.cc" /* glr.c:463  */
        break;

    case 175: /* expr  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2057 "parser.tab.cc" /* glr.c:463  */
        break;

    case 176: /* id  */
#line 17 "parser.yy" /* glr.c:463  */
      { std::cerr << "AST TYPE: " << ((*yyvaluep).ast)->type; }
#line 2063 "parser.tab.cc" /* glr.c:463  */
        break;


      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, LFortran::Parser &p)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, p);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                  \
  do {                                                                  \
    if (yydebug)                                                        \
      {                                                                 \
        YYFPRINTF (stderr, "%s ", Title);                               \
        yy_symbol_print (stderr, Type, Value, Location, p);        \
        YYFPRINTF (stderr, "\n");                                       \
      }                                                                 \
  } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

struct yyGLRStack;
static void yypstack (struct yyGLRStack* yystackp, size_t yyk)
  YY_ATTRIBUTE_UNUSED;
static void yypdumpstack (struct yyGLRStack* yystackp)
  YY_ATTRIBUTE_UNUSED;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
#  define YYSTACKEXPANDABLE 1
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (0)
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
  } while (0)
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the last token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state.  */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;
  /* To compute the location of the error token.  */
  yyGLRStackItem yyerror_range[3];

  int yyerrcnt;
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;

  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static _Noreturn void
yyFail (yyGLRStack* yystackp, YYLTYPE *yylocp, LFortran::Parser &p, const char* yymsg)
{
  if (yymsg != YY_NULLPTR)
    yyerror (yylocp, p, yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static _Noreturn void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) YY_ATTRIBUTE_UNUSED;
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
#if YYDEBUG
      yyvsp[i].yystate.yylrState = s->yylrState;
#endif
      yyvsp[i].yystate.yyresolved = s->yyresolved;
      if (s->yyresolved)
        yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      else
        /* The effect of using yysval or yyloc (in an immediate rule) is
         * undefined.  */
        yyvsp[i].yystate.yysemantics.yyfirstVal = YY_NULLPTR;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     YY_ATTRIBUTE_UNUSED;
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
static YYRESULTTAG
yyuserAction (yyRuleNum yyn, size_t yyrhslen, yyGLRStackItem* yyvsp,
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp, YYLTYPE *yylocp, LFortran::Parser &p)
{
  yybool yynormal YY_ATTRIBUTE_UNUSED = (yystackp->yysplitPoint == YY_NULLPTR);
  int yylow;
  YYUSE (yyvalp);
  YYUSE (yylocp);
  YYUSE (p);
  YYUSE (yyrhslen);
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)                                              \
  return yyerror (yylocp, p, YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT ((*yylocp), (yyvsp - yyrhslen), yyrhslen);
  yystackp->yyerror_range[1].yystate.yyloc = *yylocp;

  switch (yyn)
    {
        case 2:
#line 244 "parser.yy" /* glr.c:816  */
    { RESULT((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast)); }
#line 2444 "parser.tab.cc" /* glr.c:816  */
    break;

  case 3:
#line 245 "parser.yy" /* glr.c:816  */
    { RESULT((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast)); }
#line 2450 "parser.tab.cc" /* glr.c:816  */
    break;

  case 9:
#line 261 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = PROGRAM((((yyGLRStackItem const *)yyvsp)[YYFILL (-5)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.vec_ast), (*yylocp)); }
#line 2457 "parser.tab.cc" /* glr.c:816  */
    break;

  case 10:
#line 266 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = SUBROUTINE((((yyGLRStackItem const *)yyvsp)[YYFILL (-5)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.vec_ast), (*yylocp)); }
#line 2464 "parser.tab.cc" /* glr.c:816  */
    break;

  case 11:
#line 271 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = FUNCTION((((yyGLRStackItem const *)yyvsp)[YYFILL (-5)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.vec_ast), (*yylocp)); }
#line 2471 "parser.tab.cc" /* glr.c:816  */
    break;

  case 12:
#line 279 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).vec_ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.vec_ast); STMTS_ADD(((*yyvalp).vec_ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast)); }
#line 2477 "parser.tab.cc" /* glr.c:816  */
    break;

  case 13:
#line 280 "parser.yy" /* glr.c:816  */
    { STMTS_NEW(((*yyvalp).vec_ast)); STMTS_ADD(((*yyvalp).vec_ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast)); }
#line 2483 "parser.tab.cc" /* glr.c:816  */
    break;

  case 20:
#line 300 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = ASSIGNMENT((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2489 "parser.tab.cc" /* glr.c:816  */
    break;

  case 21:
#line 304 "parser.yy" /* glr.c:816  */
    {}
#line 2495 "parser.tab.cc" /* glr.c:816  */
    break;

  case 22:
#line 308 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = IF1((((yyGLRStackItem const *)yyvsp)[YYFILL (-5)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.vec_ast), (*yylocp)); }
#line 2502 "parser.tab.cc" /* glr.c:816  */
    break;

  case 23:
#line 310 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = IF2((((yyGLRStackItem const *)yyvsp)[YYFILL (-9)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-5)].yystate.yysemantics.yysval.vec_ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.vec_ast), (*yylocp)); }
#line 2509 "parser.tab.cc" /* glr.c:816  */
    break;

  case 24:
#line 312 "parser.yy" /* glr.c:816  */
    {
            ((*yyvalp).ast) = IF3((((yyGLRStackItem const *)yyvsp)[YYFILL (-7)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.vec_ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2516 "parser.tab.cc" /* glr.c:816  */
    break;

  case 25:
#line 343 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = ADD((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2522 "parser.tab.cc" /* glr.c:816  */
    break;

  case 26:
#line 344 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SUB((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2528 "parser.tab.cc" /* glr.c:816  */
    break;

  case 27:
#line 345 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = MUL((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2534 "parser.tab.cc" /* glr.c:816  */
    break;

  case 28:
#line 346 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = DIV((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2540 "parser.tab.cc" /* glr.c:816  */
    break;

  case 29:
#line 347 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = POW((((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast), (*yylocp)); }
#line 2546 "parser.tab.cc" /* glr.c:816  */
    break;

  case 30:
#line 348 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.ast); }
#line 2552 "parser.tab.cc" /* glr.c:816  */
    break;

  case 31:
#line 349 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.ast); }
#line 2558 "parser.tab.cc" /* glr.c:816  */
    break;

  case 32:
#line 350 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = INTEGER((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.n), (*yylocp)); }
#line 2564 "parser.tab.cc" /* glr.c:816  */
    break;

  case 33:
#line 354 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2570 "parser.tab.cc" /* glr.c:816  */
    break;

  case 34:
#line 355 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2576 "parser.tab.cc" /* glr.c:816  */
    break;

  case 35:
#line 356 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2582 "parser.tab.cc" /* glr.c:816  */
    break;

  case 36:
#line 357 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2588 "parser.tab.cc" /* glr.c:816  */
    break;

  case 37:
#line 358 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2594 "parser.tab.cc" /* glr.c:816  */
    break;

  case 38:
#line 359 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2600 "parser.tab.cc" /* glr.c:816  */
    break;

  case 39:
#line 360 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2606 "parser.tab.cc" /* glr.c:816  */
    break;

  case 40:
#line 361 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2612 "parser.tab.cc" /* glr.c:816  */
    break;

  case 41:
#line 362 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2618 "parser.tab.cc" /* glr.c:816  */
    break;

  case 42:
#line 363 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2624 "parser.tab.cc" /* glr.c:816  */
    break;

  case 43:
#line 364 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2630 "parser.tab.cc" /* glr.c:816  */
    break;

  case 44:
#line 365 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2636 "parser.tab.cc" /* glr.c:816  */
    break;

  case 45:
#line 366 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2642 "parser.tab.cc" /* glr.c:816  */
    break;

  case 46:
#line 367 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2648 "parser.tab.cc" /* glr.c:816  */
    break;

  case 47:
#line 368 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2654 "parser.tab.cc" /* glr.c:816  */
    break;

  case 48:
#line 369 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2660 "parser.tab.cc" /* glr.c:816  */
    break;

  case 49:
#line 370 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2666 "parser.tab.cc" /* glr.c:816  */
    break;

  case 50:
#line 371 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2672 "parser.tab.cc" /* glr.c:816  */
    break;

  case 51:
#line 372 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2678 "parser.tab.cc" /* glr.c:816  */
    break;

  case 52:
#line 373 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2684 "parser.tab.cc" /* glr.c:816  */
    break;

  case 53:
#line 374 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2690 "parser.tab.cc" /* glr.c:816  */
    break;

  case 54:
#line 375 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2696 "parser.tab.cc" /* glr.c:816  */
    break;

  case 55:
#line 376 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2702 "parser.tab.cc" /* glr.c:816  */
    break;

  case 56:
#line 377 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2708 "parser.tab.cc" /* glr.c:816  */
    break;

  case 57:
#line 378 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2714 "parser.tab.cc" /* glr.c:816  */
    break;

  case 58:
#line 379 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2720 "parser.tab.cc" /* glr.c:816  */
    break;

  case 59:
#line 380 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2726 "parser.tab.cc" /* glr.c:816  */
    break;

  case 60:
#line 381 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2732 "parser.tab.cc" /* glr.c:816  */
    break;

  case 61:
#line 382 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2738 "parser.tab.cc" /* glr.c:816  */
    break;

  case 62:
#line 383 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2744 "parser.tab.cc" /* glr.c:816  */
    break;

  case 63:
#line 384 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2750 "parser.tab.cc" /* glr.c:816  */
    break;

  case 64:
#line 385 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2756 "parser.tab.cc" /* glr.c:816  */
    break;

  case 65:
#line 386 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2762 "parser.tab.cc" /* glr.c:816  */
    break;

  case 66:
#line 387 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2768 "parser.tab.cc" /* glr.c:816  */
    break;

  case 67:
#line 388 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2774 "parser.tab.cc" /* glr.c:816  */
    break;

  case 68:
#line 389 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2780 "parser.tab.cc" /* glr.c:816  */
    break;

  case 69:
#line 390 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2786 "parser.tab.cc" /* glr.c:816  */
    break;

  case 70:
#line 391 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2792 "parser.tab.cc" /* glr.c:816  */
    break;

  case 71:
#line 392 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2798 "parser.tab.cc" /* glr.c:816  */
    break;

  case 72:
#line 393 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2804 "parser.tab.cc" /* glr.c:816  */
    break;

  case 73:
#line 394 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2810 "parser.tab.cc" /* glr.c:816  */
    break;

  case 74:
#line 395 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2816 "parser.tab.cc" /* glr.c:816  */
    break;

  case 75:
#line 396 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2822 "parser.tab.cc" /* glr.c:816  */
    break;

  case 76:
#line 397 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2828 "parser.tab.cc" /* glr.c:816  */
    break;

  case 77:
#line 398 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2834 "parser.tab.cc" /* glr.c:816  */
    break;

  case 78:
#line 399 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2840 "parser.tab.cc" /* glr.c:816  */
    break;

  case 79:
#line 400 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2846 "parser.tab.cc" /* glr.c:816  */
    break;

  case 80:
#line 401 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2852 "parser.tab.cc" /* glr.c:816  */
    break;

  case 81:
#line 402 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2858 "parser.tab.cc" /* glr.c:816  */
    break;

  case 82:
#line 403 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2864 "parser.tab.cc" /* glr.c:816  */
    break;

  case 83:
#line 404 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2870 "parser.tab.cc" /* glr.c:816  */
    break;

  case 84:
#line 405 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2876 "parser.tab.cc" /* glr.c:816  */
    break;

  case 85:
#line 406 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2882 "parser.tab.cc" /* glr.c:816  */
    break;

  case 86:
#line 407 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2888 "parser.tab.cc" /* glr.c:816  */
    break;

  case 87:
#line 408 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2894 "parser.tab.cc" /* glr.c:816  */
    break;

  case 88:
#line 409 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2900 "parser.tab.cc" /* glr.c:816  */
    break;

  case 89:
#line 410 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2906 "parser.tab.cc" /* glr.c:816  */
    break;

  case 90:
#line 411 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2912 "parser.tab.cc" /* glr.c:816  */
    break;

  case 91:
#line 412 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2918 "parser.tab.cc" /* glr.c:816  */
    break;

  case 92:
#line 413 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2924 "parser.tab.cc" /* glr.c:816  */
    break;

  case 93:
#line 414 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2930 "parser.tab.cc" /* glr.c:816  */
    break;

  case 94:
#line 415 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2936 "parser.tab.cc" /* glr.c:816  */
    break;

  case 95:
#line 416 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2942 "parser.tab.cc" /* glr.c:816  */
    break;

  case 96:
#line 417 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2948 "parser.tab.cc" /* glr.c:816  */
    break;

  case 97:
#line 418 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2954 "parser.tab.cc" /* glr.c:816  */
    break;

  case 98:
#line 419 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2960 "parser.tab.cc" /* glr.c:816  */
    break;

  case 99:
#line 420 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2966 "parser.tab.cc" /* glr.c:816  */
    break;

  case 100:
#line 421 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2972 "parser.tab.cc" /* glr.c:816  */
    break;

  case 101:
#line 422 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2978 "parser.tab.cc" /* glr.c:816  */
    break;

  case 102:
#line 423 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2984 "parser.tab.cc" /* glr.c:816  */
    break;

  case 103:
#line 424 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2990 "parser.tab.cc" /* glr.c:816  */
    break;

  case 104:
#line 425 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 2996 "parser.tab.cc" /* glr.c:816  */
    break;

  case 105:
#line 426 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3002 "parser.tab.cc" /* glr.c:816  */
    break;

  case 106:
#line 427 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3008 "parser.tab.cc" /* glr.c:816  */
    break;

  case 107:
#line 428 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3014 "parser.tab.cc" /* glr.c:816  */
    break;

  case 108:
#line 429 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3020 "parser.tab.cc" /* glr.c:816  */
    break;

  case 109:
#line 430 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3026 "parser.tab.cc" /* glr.c:816  */
    break;

  case 110:
#line 431 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3032 "parser.tab.cc" /* glr.c:816  */
    break;

  case 111:
#line 432 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3038 "parser.tab.cc" /* glr.c:816  */
    break;

  case 112:
#line 433 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3044 "parser.tab.cc" /* glr.c:816  */
    break;

  case 113:
#line 434 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3050 "parser.tab.cc" /* glr.c:816  */
    break;

  case 114:
#line 435 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3056 "parser.tab.cc" /* glr.c:816  */
    break;

  case 115:
#line 436 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3062 "parser.tab.cc" /* glr.c:816  */
    break;

  case 116:
#line 437 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3068 "parser.tab.cc" /* glr.c:816  */
    break;

  case 117:
#line 438 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3074 "parser.tab.cc" /* glr.c:816  */
    break;

  case 118:
#line 439 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3080 "parser.tab.cc" /* glr.c:816  */
    break;

  case 119:
#line 440 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3086 "parser.tab.cc" /* glr.c:816  */
    break;

  case 120:
#line 441 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3092 "parser.tab.cc" /* glr.c:816  */
    break;

  case 121:
#line 442 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3098 "parser.tab.cc" /* glr.c:816  */
    break;

  case 122:
#line 443 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3104 "parser.tab.cc" /* glr.c:816  */
    break;

  case 123:
#line 444 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3110 "parser.tab.cc" /* glr.c:816  */
    break;

  case 124:
#line 445 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3116 "parser.tab.cc" /* glr.c:816  */
    break;

  case 125:
#line 446 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3122 "parser.tab.cc" /* glr.c:816  */
    break;

  case 126:
#line 447 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3128 "parser.tab.cc" /* glr.c:816  */
    break;

  case 127:
#line 448 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3134 "parser.tab.cc" /* glr.c:816  */
    break;

  case 128:
#line 449 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3140 "parser.tab.cc" /* glr.c:816  */
    break;

  case 129:
#line 450 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3146 "parser.tab.cc" /* glr.c:816  */
    break;

  case 130:
#line 451 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3152 "parser.tab.cc" /* glr.c:816  */
    break;

  case 131:
#line 452 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3158 "parser.tab.cc" /* glr.c:816  */
    break;

  case 132:
#line 453 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3164 "parser.tab.cc" /* glr.c:816  */
    break;

  case 133:
#line 454 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3170 "parser.tab.cc" /* glr.c:816  */
    break;

  case 134:
#line 455 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3176 "parser.tab.cc" /* glr.c:816  */
    break;

  case 135:
#line 456 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3182 "parser.tab.cc" /* glr.c:816  */
    break;

  case 136:
#line 457 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3188 "parser.tab.cc" /* glr.c:816  */
    break;

  case 137:
#line 458 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3194 "parser.tab.cc" /* glr.c:816  */
    break;

  case 138:
#line 459 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3200 "parser.tab.cc" /* glr.c:816  */
    break;

  case 139:
#line 460 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3206 "parser.tab.cc" /* glr.c:816  */
    break;

  case 140:
#line 461 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3212 "parser.tab.cc" /* glr.c:816  */
    break;

  case 141:
#line 462 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3218 "parser.tab.cc" /* glr.c:816  */
    break;

  case 142:
#line 463 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3224 "parser.tab.cc" /* glr.c:816  */
    break;

  case 143:
#line 464 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3230 "parser.tab.cc" /* glr.c:816  */
    break;

  case 144:
#line 465 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3236 "parser.tab.cc" /* glr.c:816  */
    break;

  case 145:
#line 466 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3242 "parser.tab.cc" /* glr.c:816  */
    break;

  case 146:
#line 467 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3248 "parser.tab.cc" /* glr.c:816  */
    break;

  case 147:
#line 468 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3254 "parser.tab.cc" /* glr.c:816  */
    break;

  case 148:
#line 469 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3260 "parser.tab.cc" /* glr.c:816  */
    break;

  case 149:
#line 470 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3266 "parser.tab.cc" /* glr.c:816  */
    break;

  case 150:
#line 471 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3272 "parser.tab.cc" /* glr.c:816  */
    break;

  case 151:
#line 472 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3278 "parser.tab.cc" /* glr.c:816  */
    break;

  case 152:
#line 473 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3284 "parser.tab.cc" /* glr.c:816  */
    break;

  case 153:
#line 474 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3290 "parser.tab.cc" /* glr.c:816  */
    break;

  case 154:
#line 475 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3296 "parser.tab.cc" /* glr.c:816  */
    break;

  case 155:
#line 476 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3302 "parser.tab.cc" /* glr.c:816  */
    break;

  case 156:
#line 477 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3308 "parser.tab.cc" /* glr.c:816  */
    break;

  case 157:
#line 478 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3314 "parser.tab.cc" /* glr.c:816  */
    break;

  case 158:
#line 479 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3320 "parser.tab.cc" /* glr.c:816  */
    break;

  case 159:
#line 480 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3326 "parser.tab.cc" /* glr.c:816  */
    break;

  case 160:
#line 481 "parser.yy" /* glr.c:816  */
    { ((*yyvalp).ast) = SYMBOL((((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.string), (*yylocp)); }
#line 3332 "parser.tab.cc" /* glr.c:816  */
    break;


#line 3336 "parser.tab.cc" /* glr.c:816  */
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {

      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, LFortran::Parser &p)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (p);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys, LFortran::Parser &p)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval, &yys->yyloc, p);
  else
    {
#if YYDEBUG
      if (yydebug)
        {
          if (yys->yysemantics.yyfirstVal)
            YYFPRINTF (stderr, "%s unresolved", yymsg);
          else
            YYFPRINTF (stderr, "%s incomplete", yymsg);
          YY_SYMBOL_PRINT ("", yystos[yys->yylrState], YY_NULLPTR, &yys->yyloc);
        }
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh, p);
        }
    }
}

/** Left-hand-side symbol for rule #YYRULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-173)))

/** True iff LR state YYSTATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for YYSTATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  0

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *YYCONFLICTS to a pointer into yyconfl to a 0-terminated list
 *  of conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
                int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

/** Compute post-reduction state.
 * \param yystate   the current state
 * \param yysym     the nonterminal to push on the stack
 */
static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yysym)
{
  int yyr = yypgoto[yysym - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yysym - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

                                /* GLRStates */

/** Return a fresh GLRStackItem in YYSTACKP.  The item is an LR state
 *  if YYISSTATE, and otherwise a semantic option.  Callers should call
 *  YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 *  headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  YYRULE on the semantic values in YYRHS to the list of
 *  alternative actions for YYSTATE.  Assumes that YYRHS comes from
 *  stack #YYK of *YYSTACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
                     yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  YYASSERT (!yynewOption->yyisState);
  yynewOption->yystate = yyrhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
      yynewOption->yyloc = yylloc;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

                                /* GLRStacks */

/** Initialize YYSET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = YY_NULLPTR;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize *YYSTACKP to a single empty stack, with total maximum
 *  capacity for all stacks of YYSIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULLPTR;
  yystackp->yylastDeleted = YY_NULLPTR;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If *YYSTACKP is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
        {
          yyGLRState* yys0 = &yyp0->yystate;
          yyGLRState* yys1 = &yyp1->yystate;
          if (yys0->yypred != YY_NULLPTR)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULLPTR)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULLPTR)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULLPTR)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULLPTR)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                      yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULLPTR)
      yystackp->yytops.yystates[yyn] =
        YYRELOC (yystackp->yyitems, yynewItems,
                 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that YYS is a GLRState somewhere on *YYSTACKP, update the
 *  splitpoint of *YYSTACKP, if needed, so that it is at least as deep as
 *  YYS.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != YY_NULLPTR && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #YYK in *YYSTACKP.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULLPTR;
}

/** Undelete the last stack in *YYSTACKP that was marked as deleted.  Can
    only be done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULLPTR || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULLPTR;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULLPTR)
        {
          if (yyi == yyj)
            {
              YYDPRINTF ((stderr, "Removing dead stacks.\n"));
            }
          yystackp->yytops.yysize -= 1;
        }
      else
        {
          yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
          /* In the current implementation, it's unnecessary to copy
             yystackp->yytops.yylookaheadNeeds[yyi] since, after
             yyremoveDeletes returns, the parser immediately either enters
             deterministic operation or shifts a token.  However, it doesn't
             hurt, and the code might evolve to need it.  */
          yystackp->yytops.yylookaheadNeeds[yyj] =
            yystackp->yytops.yylookaheadNeeds[yyi];
          if (yyj != yyi)
            {
              YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
                          (unsigned long int) yyi, (unsigned long int) yyj));
            }
          yyj += 1;
        }
      yyi += 1;
    }
}

/** Shift to a new state on stack #YYK of *YYSTACKP, corresponding to LR
 * state YYLRSTATE, at input position YYPOSN, with (resolved) semantic
 * value *YYVALP and source location *YYLOCP.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
            size_t yyposn,
            YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yynewState->yyloc = *yylocp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #YYK of *YYSTACKP, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
                 size_t yyposn, yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;
  YYASSERT (yynewState->yyisState);

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULLPTR;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, yyrhs, yyrule);
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (0)

/*----------------------------------------------------------------------.
| Report that stack #YYK of *YYSTACKP is going to be reduced by YYRULE. |
`----------------------------------------------------------------------*/

static inline void
yy_reduce_print (int yynormal, yyGLRStackItem* yyvsp, size_t yyk,
                 yyRuleNum yyrule, LFortran::Parser &p)
{
  int yynrhs = yyrhsLength (yyrule);
  int yylow = 1;
  int yyi;
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  if (! yynormal)
    yyfillin (yyvsp, 1, -yynrhs);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyvsp[yyi - yynrhs + 1].yystate.yylrState],
                       &yyvsp[yyi - yynrhs + 1].yystate.yysemantics.yysval
                       , &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yyloc)                       , p);
      if (!yyvsp[yyi - yynrhs + 1].yystate.yyresolved)
        YYFPRINTF (stderr, " (unresolved)");
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop the symbols consumed by reduction #YYRULE from the top of stack
 *  #YYK of *YYSTACKP, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *YYVALP to the resulting value,
 *  and *YYLOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
            YYSTYPE* yyvalp, YYLTYPE *yylocp, LFortran::Parser &p)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULLPTR)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* yyrhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      YY_REDUCE_PRINT ((1, yyrhs, yyk, yyrule, p));
      return yyuserAction (yyrule, yynrhs, yyrhs, yystackp,
                           yyvalp, yylocp, p);
    }
  else
    {
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
        = yystackp->yytops.yystates[yyk];
      if (yynrhs == 0)
        /* Set default location.  */
        yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yys->yyloc;
      for (yyi = 0; yyi < yynrhs; yyi += 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      YY_REDUCE_PRINT ((0, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1, yyk, yyrule, p));
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp, yylocp, p);
    }
}

/** Pop items off stack #YYK of *YYSTACKP according to grammar rule YYRULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with YYRULE and store its value with the
 *  newly pushed state, if YYFORCEEVAL or if *YYSTACKP is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #YYK from
 *  *YYSTACKP.  In this case, the semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval, LFortran::Parser &p)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULLPTR)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YYRESULTTAG yyflag = yydoAction (yystackp, yyk, yyrule, &yysval, &yyloc, p);
      if (yyflag == yyerr && yystackp->yysplitPoint != YY_NULLPTR)
        {
          YYDPRINTF ((stderr, "Parse on stack %lu rejected by rule #%d.\n",
                     (unsigned long int) yyk, yyrule - 1));
        }
      if (yyflag != yyok)
        return yyflag;
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
           0 < yyn; yyn -= 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
                  "Reduced stack %lu by rule #%d; action deferred.  "
                  "Now in state %d.\n",
                  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULLPTR)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
            while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
              {
                if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
                  {
                    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
                    yymarkStackDeleted (yystackp, yyk);
                    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
                                (unsigned long int) yyk,
                                (unsigned long int) yyi));
                    return yyok;
                  }
                yyp = yyp->yypred;
              }
          }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == YY_NULLPTR)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULLPTR;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULLPTR)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULLPTR)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff YYY0 and YYY1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
           yyn = yyrhsLength (yyy0->yyrule);
           yyn > 0;
           yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
        if (yys0->yyposn != yys1->yyposn)
          return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (YYY0,YYY1), destructively merge the
 *  alternative semantic values for the RHS-symbols of YYY1 and YYY0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
        break;
      else if (yys0->yyresolved)
        {
          yys1->yyresolved = yytrue;
          yys1->yysemantics.yysval = yys0->yysemantics.yysval;
        }
      else if (yys1->yyresolved)
        {
          yys0->yyresolved = yytrue;
          yys0->yysemantics.yysval = yys1->yysemantics.yysval;
        }
      else
        {
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (yytrue)
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULLPTR)
                break;
              else if (*yyz0p == YY_NULLPTR)
                {
                  *yyz0p = yyz1;
                  break;
                }
              else if (*yyz0p < yyz1)
                {
                  yySemanticOption* yyz = *yyz0p;
                  *yyz0p = yyz1;
                  yyz1 = yyz1->yynext;
                  (*yyz0p)->yynext = yyz;
                }
              yyz0p = &(*yyz0p)->yynext;
            }
          yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
        }
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
                                   yyGLRStack* yystackp, LFortran::Parser &p);


/** Resolve the previous YYN states starting at and including state YYS
 *  on *YYSTACKP. If result != yyok, some states may have been left
 *  unresolved possibly with empty semantic option chains.  Regardless
 *  of whether result = yyok, each state has been left with consistent
 *  data so that yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp, LFortran::Parser &p)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp, p));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp, p));
    }
  return yyok;
}

/** Resolve the states for the RHS of YYOPT on *YYSTACKP, perform its
 *  user action, and return the semantic value and location in *YYVALP
 *  and *YYLOCP.  Regardless of whether result = yyok, all RHS states
 *  have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp, YYLTYPE *yylocp, LFortran::Parser &p)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp, p);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys, p);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  if (yynrhs == 0)
    /* Set default location.  */
    yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yyopt->yystate->yyloc;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    YYLTYPE yylloc_current = yylloc;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yylloc = yyopt->yyloc;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp, yylocp, p);
    yychar = yychar_current;
    yylval = yylval_current;
    yylloc = yylloc_current;
  }
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULLPTR)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
               (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
        {
          if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
            YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
                       yytokenName (yystos[yystates[yyi]->yylrState]));
          else
            YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
                       yytokenName (yystos[yystates[yyi]->yylrState]),
                       (unsigned long int) (yystates[yyi-1]->yyposn + 1),
                       (unsigned long int) yystates[yyi]->yyposn);
        }
      else
        yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
                   yySemanticOption* yyx1, YYLTYPE *yylocp, LFortran::Parser &p)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (yylocp, p, YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the locations for each of the YYN1 states in *YYSTACKP,
 *  ending at YYS1.  Has no effect on previously resolved states.
 *  The first semantic option of a state is always chosen.  */
static void
yyresolveLocations (yyGLRState* yys1, int yyn1,
                    yyGLRStack *yystackp, LFortran::Parser &p)
{
  if (0 < yyn1)
    {
      yyresolveLocations (yys1->yypred, yyn1 - 1, yystackp, p);
      if (!yys1->yyresolved)
        {
          yyGLRStackItem yyrhsloc[1 + YYMAXRHS];
          int yynrhs;
          yySemanticOption *yyoption = yys1->yysemantics.yyfirstVal;
          YYASSERT (yyoption != YY_NULLPTR);
          yynrhs = yyrhsLength (yyoption->yyrule);
          if (yynrhs > 0)
            {
              yyGLRState *yys;
              int yyn;
              yyresolveLocations (yyoption->yystate, yynrhs,
                                  yystackp, p);
              for (yys = yyoption->yystate, yyn = yynrhs;
                   yyn > 0;
                   yys = yys->yypred, yyn -= 1)
                yyrhsloc[yyn].yystate.yyloc = yys->yyloc;
            }
          else
            {
              /* Both yyresolveAction and yyresolveLocations traverse the GSS
                 in reverse rightmost order.  It is only necessary to invoke
                 yyresolveLocations on a subforest for which yyresolveAction
                 would have been invoked next had an ambiguity not been
                 detected.  Thus the location of the previous state (but not
                 necessarily the previous state itself) is guaranteed to be
                 resolved already.  */
              yyGLRState *yyprevious = yyoption->yystate;
              yyrhsloc[0].yystate.yyloc = yyprevious->yyloc;
            }
          {
            int yychar_current = yychar;
            YYSTYPE yylval_current = yylval;
            YYLTYPE yylloc_current = yylloc;
            yychar = yyoption->yyrawchar;
            yylval = yyoption->yyval;
            yylloc = yyoption->yyloc;
            YYLLOC_DEFAULT ((yys1->yyloc), yyrhsloc, yynrhs);
            yychar = yychar_current;
            yylval = yylval_current;
            yylloc = yylloc_current;
          }
        }
    }
}

/** Resolve the ambiguity represented in state YYS in *YYSTACKP,
 *  perform the indicated actions, and set the semantic value of YYS.
 *  If result != yyok, the chain of semantic options in YYS has been
 *  cleared instead or it has been left unmodified except that
 *  redundant options may have been removed.  Regardless of whether
 *  result = yyok, YYS has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp, LFortran::Parser &p)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;
  YYLTYPE *yylocp = &yys->yyloc;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULLPTR; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
        {
          yymergeOptionSets (yybest, yyp);
          *yypp = yyp->yynext;
        }
      else
        {
          switch (yypreference (yybest, yyp))
            {
            case 0:
              yyresolveLocations (yys, 1, yystackp, p);
              return yyreportAmbiguity (yybest, yyp, yylocp, p);
              break;
            case 1:
              yymerge = yytrue;
              break;
            case 2:
              break;
            case 3:
              yybest = yyp;
              yymerge = yyfalse;
              break;
            default:
              /* This cannot happen so it is not worth a YYASSERT (yyfalse),
                 but some compilers complain if the default case is
                 omitted.  */
              break;
            }
          yypp = &yyp->yynext;
        }
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp, p);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULLPTR; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                YYLTYPE yydummy;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other, &yydummy, p);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval, yylocp, p);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp, p);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULLPTR;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp, LFortran::Parser &p)
{
  if (yystackp->yysplitPoint != YY_NULLPTR)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             , p));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULLPTR)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULLPTR;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULLPTR;
  yystackp->yylastDeleted = YY_NULLPTR;

  while (yyr != YY_NULLPTR)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
                   size_t yyposn, YYLTYPE *yylocp, LFortran::Parser &p)
{
  while (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
                  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
        {
          YYRESULTTAG yyflag;
          yyRuleNum yyrule = yydefaultAction (yystate);
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          yyflag = yyglrReduce (yystackp, yyk, yyrule, yyimmediate[yyrule], p);
          if (yyflag == yyerr)
            {
              YYDPRINTF ((stderr,
                          "Stack %lu dies "
                          "(predicate failure or explicit user error).\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          if (yyflag != yyok)
            return yyflag;
        }
      else
        {
          yySymbol yytoken;
          int yyaction;
          const short int* yyconflicts;

          yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
          if (yychar == YYEMPTY)
            {
              YYDPRINTF ((stderr, "Reading a token: "));
              yychar = yylex (&yylval, &yylloc, p);
            }

          if (yychar <= YYEOF)
            {
              yychar = yytoken = YYEOF;
              YYDPRINTF ((stderr, "Now at end of input.\n"));
            }
          else
            {
              yytoken = YYTRANSLATE (yychar);
              YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
            }

          yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

          while (*yyconflicts != 0)
            {
              YYRESULTTAG yyflag;
              size_t yynewStack = yysplitStack (yystackp, yyk);
              YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
                          (unsigned long int) yynewStack,
                          (unsigned long int) yyk));
              yyflag = yyglrReduce (yystackp, yynewStack,
                                    *yyconflicts,
                                    yyimmediate[*yyconflicts], p);
              if (yyflag == yyok)
                YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                          yyposn, yylocp, p));
              else if (yyflag == yyerr)
                {
                  YYDPRINTF ((stderr, "Stack %lu dies.\n",
                              (unsigned long int) yynewStack));
                  yymarkStackDeleted (yystackp, yynewStack);
                }
              else
                return yyflag;
              yyconflicts += 1;
            }

          if (yyisShiftAction (yyaction))
            break;
          else if (yyisErrorAction (yyaction))
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              break;
            }
          else
            {
              YYRESULTTAG yyflag = yyglrReduce (yystackp, yyk, -yyaction,
                                                yyimmediate[-yyaction], p);
              if (yyflag == yyerr)
                {
                  YYDPRINTF ((stderr,
                              "Stack %lu dies "
                              "(predicate failure or explicit user error).\n",
                              (unsigned long int) yyk));
                  yymarkStackDeleted (yystackp, yyk);
                  break;
                }
              else if (yyflag != yyok)
                return yyflag;
            }
        }
    }
  return yyok;
}

static void
yyreportSyntaxError (yyGLRStack* yystackp, LFortran::Parser &p)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (&yylloc, p, YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULLPTR, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULLPTR;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;
          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULLPTR, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
          if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
              yyp += yytnamerr (yyp, yyarg[yyi++]);
              yyformat += 2;
            }
          else
            {
              yyp++;
              yyformat++;
            }
        }
      yyerror (&yylloc, p, yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (&yylloc, p, YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
static void
yyrecoverSyntaxError (yyGLRStack* yystackp, LFortran::Parser &p)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (yytrue)
      {
        yySymbol yytoken;
        if (yychar == YYEOF)
          yyFail (yystackp, &yylloc, p, YY_NULLPTR);
        if (yychar != YYEMPTY)
          {
            /* We throw away the lookahead, but the error range
               of the shifted error token must take it into account.  */
            yyGLRState *yys = yystackp->yytops.yystates[0];
            yyGLRStackItem yyerror_range[3];
            yyerror_range[1].yystate.yyloc = yys->yyloc;
            yyerror_range[2].yystate.yyloc = yylloc;
            YYLLOC_DEFAULT ((yys->yyloc), yyerror_range, 2);
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval, &yylloc, p);
          }
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = yylex (&yylval, &yylloc, p);
        if (yychar <= YYEOF)
          {
            yychar = yytoken = YYEOF;
            YYDPRINTF ((stderr, "Now at end of input.\n"));
          }
        else
          {
            yytoken = YYTRANSLATE (yychar);
            YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
          }
        yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, &yylloc, p, YY_NULLPTR);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULLPTR)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              /* First adjust its location.*/
              YYLTYPE yyerrloc;
              yystackp->yyerror_range[2].yystate.yyloc = yylloc;
              YYLLOC_DEFAULT (yyerrloc, (yystackp->yyerror_range), 2);
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval, &yyerrloc);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      yystackp->yyerror_range[1].yystate.yyloc = yys->yyloc;
      if (yys->yypred != YY_NULLPTR)
        yydestroyGLRState ("Error: popping", yys, p);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULLPTR)
    yyFail (yystackp, &yylloc, p, YY_NULLPTR);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (0)

/*----------.
| yyparse.  |
`----------*/

int
yyparse (LFortran::Parser &p)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;
  yylloc = yyloc_default;

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval, &yylloc);
  yyposn = 0;

  while (yytrue)
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (yytrue)
        {
          yyRuleNum yyrule;
          int yyaction;
          const short int* yyconflicts;

          yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {
               yystack.yyerror_range[1].yystate.yyloc = yylloc;
                  yyreportSyntaxError (&yystack, p);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue, p));
            }
          else
            {
              yySymbol yytoken;
              if (yychar == YYEMPTY)
                {
                  YYDPRINTF ((stderr, "Reading a token: "));
                  yychar = yylex (&yylval, &yylloc, p);
                }

              if (yychar <= YYEOF)
                {
                  yychar = yytoken = YYEOF;
                  YYDPRINTF ((stderr, "Now at end of input.\n"));
                }
              else
                {
                  yytoken = YYTRANSLATE (yychar);
                  YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
                }

              yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = YYEMPTY;
                  yyposn += 1;
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval, &yylloc);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {
               yystack.yyerror_range[1].yystate.yyloc = yylloc;
                  yyreportSyntaxError (&yystack, p);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue, p));
            }
        }

      while (yytrue)
        {
          yySymbol yytoken_to_shift;
          size_t yys;

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (&yystack, yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn, &yylloc, p));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, &yylloc, p, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack, p));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
           yystack.yyerror_range[1].yystate.yyloc = yylloc;
              yyreportSyntaxError (&yystack, p);
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to YYEMPTY
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = YYEMPTY;
          yyposn += 1;
          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            {
              int yyaction;
              const short int* yyconflicts;
              yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
              yygetLRActions (yystate, yytoken_to_shift, &yyaction,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yyglrShift (&yystack, yys, yyaction, yyposn,
                          &yylval, &yylloc);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack, p));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, p);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (&yylloc, p, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval, &yylloc, p);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
        {
          size_t yysize = yystack.yytops.yysize;
          size_t yyk;
          for (yyk = 0; yyk < yysize; yyk += 1)
            if (yystates[yyk])
              {
                while (yystates[yyk])
                  {
                    yyGLRState *yys = yystates[yyk];
                 yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
                  if (yys->yypred != YY_NULLPTR)
                      yydestroyGLRState ("Cleanup: popping", yys, p);
                    yystates[yyk] = yys->yypred;
                    yystack.yynextFree -= 1;
                    yystack.yyspaceLeft += 1;
                  }
                break;
              }
        }
      yyfreeGLRStack (&yystack);
    }

  return yyresult;
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == YY_NULLPTR)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULLPTR ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
        {
          YYASSERT (yyp->yystate.yyisState);
          YYASSERT (yyp->yyoption.yyisState);
          YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                     yyp->yystate.yyresolved, yyp->yystate.yylrState,
                     (unsigned long int) yyp->yystate.yyposn,
                     (long int) YYINDEX (yyp->yystate.yypred));
          if (! yyp->yystate.yyresolved)
            YYFPRINTF (stderr, ", firstVal: %ld",
                       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
        }
      else
        {
          YYASSERT (!yyp->yystate.yyisState);
          YYASSERT (!yyp->yyoption.yyisState);
          YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
                     yyp->yyoption.yyrule - 1,
                     (long int) YYINDEX (yyp->yyoption.yystate),
                     (long int) YYINDEX (yyp->yyoption.yynext));
        }
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
               (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif

#undef yylval
#undef yychar
#undef yynerrs
#undef yylloc




#ifndef LFORTRAN_CONFIG_H
#define LFORTRAN_CONFIG_H

/* Define if you want to enable ASSERT testing in LFortran */
#define WITH_LFORTRAN_ASSERT

/* LFortran version */
#define LFORTRAN_VERSION "0.12.0-1823-g8c5a03ea"

/* Define if LLVM is enabled */
#define HAVE_LFORTRAN_LLVM

/* Define if stacktrace is enabled */
#define HAVE_LFORTRAN_STACKTRACE
/* #undef HAVE_LFORTRAN_BFD */
#define HAVE_LFORTRAN_DWARFDUMP
/* #undef HAVE_LFORTRAN_LINK */
#define HAVE_LFORTRAN_MACHO
#define HAVE_LFORTRAN_UNWIND

/* Define if cxxabi.h is present */
#define HAVE_LFORTRAN_DEMANGLE

/* Define if XEUS is enabled */
/* #undef HAVE_LFORTRAN_XEUS */

/* Define if we should use binary modfiles */
#define WITH_LFORTRAN_BINARY_MODFILES

#endif // LFORTRAN_CONFIG_H

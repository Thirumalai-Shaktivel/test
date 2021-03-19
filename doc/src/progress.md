# LFortran Development Status


## Development

**LFortran** is currently under development and some of the features of Fortran might not be supported currently. The information regarding which functions are currently supported and which one's are still being worked on will be available in this section. You can submit any new issue you find while using LFortran to our [issue tracker](https://gitlab.com/lfortran/lfortran/issues). You can also check out any of the issues mentioned in the sections below and provide suggestions and ideas to help us get the feature implemented.

## Fortran Standards Supported

LFortran currently supports a subset of the latest Fortran standard [Fortran 2018 standard](https://j3-fortran.org/doc/year/18/18-007r1.pdf). This document describes details about which features are supported and which are not.

### Related Issues:

- [#120](https://gitlab.com/lfortran/lfortran/issues/120)

## Features Supported

LFortran is currently in development and you might find some unsupported features while using it. We are working on fixing the issues and improving and expanding LFortran. You can also help out by reporting any new bug you find or you can check out any of the issues mentioned in the section and help us implement the feature faster.

### Currently Supported:

- AST Generation
- ASR Generation for most nodes
- Fortran Modules
- Functions
- Subroutines
- Basic data types like int, float, character
- Binary Operations
- Conditional and iterative statements
- Printing

### Not Supported/Currently in Development

- Strings and Arrays
- Nested Function Definitions
- Complex Numbers
- Optional Arguments
- ASR not created for a few features supported by LFortran
    - Conditional and iterative statements
    - Function Calls
    - Arithmetic assignment(like a = 10)
    - Assignments with binary operations including any number
    - Logical Variables
    - Print Statements
    - Some intrinsic Fortran Functions (see below)

### Related Issues

- [#114 Strings](https://gitlab.com/lfortran/lfortran/issues/114)
- [#121 ASR](https://gitlab.com/lfortran/lfortran/issues/121)
- [#122 Nested Function Definitions](https://gitlab.com/lfortran/lfortran/issues/122)
- [#125 Complex Numbers](https://gitlab.com/lfortran/lfortran/issues/125)
- [#126 Optional Arguments](https://gitlab.com/lfortran/lfortran/issues/126)

## Intrinsic Functions

LFortran does not support some of the standard intrinsic functions for the Fortran standard. Work is being done on having support for these functions. The testing for the functions is still being done. The list will be updated with the rest of the functions soon.

### Currently Supported:

- log
- sin
- cos
- tan
- sinh
- cosh
- tanh

### Not Supported/Currently in Development:

- abs
- sqrt
- asin
- acos
- atan
- exp
- int
- real
- nint
- floor
- fraction
- real
- max
- min
- mod

### Implementation Details
This section is still subject to change. At the moment; the intrinsic functions are dispatched to `libm` and `glibc`. However, all functions are also being implemented so as to be usable from the Fortran runtime as well.

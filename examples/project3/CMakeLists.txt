cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(project3 Fortran)

add_executable(project3
    project3.f90
    types.f90
    constants.f90
    samplebug.f90
)

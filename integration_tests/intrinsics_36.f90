program intrinsics_36
    implicit none
    character(len=25) :: empty = adjustl('')
    character(len=25) :: all_spaces = adjustl('    ')
    character(len=25) :: simple = adjustl('gfortran')
    character(len=25) :: space_at_start = adjustl('   gfortran')
    character(len=25) :: space_at_end = adjustl('gfortran   ')
    character(len=25) :: space_in_between = adjustl('   g for tran   ')
    character(len=25) :: spaces_with_symbols = adjustl('  # gfor* t $ ran &    ')

    if (empty /= '') error stop
    if (all_spaces /= '    ') error stop
    if (simple /= 'gfortran') error stop
    if (space_at_start /= 'gfortran   ') error stop
    if (space_at_end /= 'gfortran   ') error stop
    if (space_in_between /= 'g for tran      ') error stop
    if (spaces_with_symbols /= '# gfor* t $ ran &      ') error stop

    print *, empty
    print *, all_spaces
    print *, simple
    print *, space_at_start
    print *, space_at_end
    print *, space_in_between
    print *, spaces_with_symbols

end program

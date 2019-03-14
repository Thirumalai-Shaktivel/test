import pytest

from lfortran.semantic.ast_to_asr import ast_to_asr
from lfortran.ast import parse
from lfortran.asr import asr as mod_asr

def test_function1():
    source = """\
module test
implicit none
contains

    integer function fn1(a, b) result(r)
    integer, intent(in) :: a, b
    r = a + b
    end function

end module
"""
    ast = parse(source)
    asr = ast_to_asr(ast)
    assert 'modx' in asr.global_scope.symbols
    m = asr.global_scope.symbols['modx']
    assert isinstance(m, mod_asr.Module)
    print(m.symtab.symbols)
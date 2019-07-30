from lfortran.codegen.evaluator import FortranEvaluator
from lfortran.tests.utils import linux_only

def test_intrinsics():
    e = FortranEvaluator()
    e.evaluate("""\
integer, parameter :: dp = kind(0.d0)
real(dp) :: a, b, c(4)
integer :: i, r
r = 0
a = 1.1_dp
b = 1.2_dp
print *, r
if (b-a > 0.2_dp) r = 1
""")
    print(e._source_ll)
    assert e.evaluate("r") == 0

def test_plot():
    e = FortranEvaluator()
    assert e.evaluate("""\
integer :: a, b
a = 1
b = 5
plot_test(a, b)
""") == 6

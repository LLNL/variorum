PROGRAM main
  USE variorum
  IMPLICIT NONE
  INTEGER :: val,r,ret

  val = 5
  r = variorum_print_power()
  ret = do_work(val)
  r = variorum_print_power()

CONTAINS

  FUNCTION do_work(val) result(res)
    IMPLICIT NONE
    INTEGER, INTENT(in) :: val ! input
    INTEGER :: i ! iterator
    INTEGER :: res ! output

    res = val

    do i = 1, 10000
        res = res + (val * i)
    end do
  END FUNCTION do_work

END PROGRAM main

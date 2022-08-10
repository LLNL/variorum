PROGRAM main
  IMPLICIT NONE
  INTEGER :: val,ret

  val = 5
  ret = variorum_print_power()
  do_work(val)
  ret = variorum_print_power()

CONTAINS

  FUNCTION do_work(val)
    IMPLICIT NONE
    INTEGER :: val,i,res

    res = val

    do i = 1, 10000
        res = res + (val * i)
    end do

  END FUNCTION do_work
END PROGRAM main

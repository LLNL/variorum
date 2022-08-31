! Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
! Variorum Project Developers. See the top-level LICENSE file for details.
!
! SPDX-License-Identifier: MIT

program main
  use variorum
  implicit none
  integer :: val,r,ret

  val = 5
  r = variorum_print_power()
  if (r .ne. 0) then
    print*, "Variorum print power failed."
  end if

  ret = do_work(val)

  r = variorum_print_power()
  if (r .ne. 0) then
    print*, "Variorum print power failed."
  end if

contains

  function do_work(val) result(res)
    implicit none
    integer, INTENT(in) :: val ! input
    integer :: i ! iterator
    integer :: res ! output

    res = val

    do i = 1, 10000
        res = res + (val * i)
    end do
  end function do_work

end program main

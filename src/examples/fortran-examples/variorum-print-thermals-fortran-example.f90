! Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
! Variorum Project Developers. See the top-level LICENSE file for details.
!
! SPDX-License-Identifier: MIT

program main
  use variorum
  implicit none
  integer :: r

  r = variorum_print_thermals()

  if (r .ne. 0) then
    print*, "Variorum print thermals failed."
  end if

end program main

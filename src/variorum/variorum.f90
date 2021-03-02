! Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
! Variorum Project Developers. See the top-level LICENSE file for details.
!
! SPDX-License-Identifier: MIT

!-----------------------------------------------------------------------------
module variorum
!-----------------------------------------------------------------------------

    use, intrinsic :: iso_c_binding
    implicit none

    !-------------------------------------------------------------------------
    interface
    !-------------------------------------------------------------------------

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_poll_power(output) &
            bind(C)
        import
        implicit none
        type(C_PTR), value, intent(in) ::output
    end function variorum_poll_power

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_monitoring(output) &
            bind(C)
        import
        implicit none
        type(C_PTR), value, intent(in) ::output
    end function variorum_monitoring

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_cap_each_socket_power_limit(socket_power_limit) &
            bind(C)
        import
        implicit none
        integer(kind=c_int), value, intent(in) ::socket_power_limit
    end function variorum_cap_each_socket_power_limit

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_cap_and_verify_node_power_limit(node_power_limit) &
            bind(C)
        import
        implicit none
        integer(kind=c_int), value, intent(in) ::node_power_limit
    end function variorum_cap_and_verify_node_power_limit

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_cap_node_power_limit(node_power_limit) &
            bind(C)
        import
        implicit none
        integer(kind=c_int), value, intent(in) ::node_power_limit
    end function variorum_cap_node_power_limit

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_cap_gpu_power_ratio(gpu_power_ratio) &
            bind(C)
        import
        implicit none
        integer(kind=c_int), value, intent(in) ::gpu_power_ratio
    end function variorum_cap_gpu_power_ratio

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_cap_each_core_frequency(cpu_freq_mhz) &
            bind(C)
        import
        implicit none
        integer(kind=c_int), value, intent(in) ::cpu_freq_mhz
    end function variorum_cap_each_core_frequency

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_power_limits() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_power_limits

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_power_limits() &
            bind(C)
        import
        implicit none
    end function variorum_print_power_limits

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_thermals() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_thermals

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_thermals() &
            bind(C)
        import
        implicit none
    end function variorum_print_thermals

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_counters() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_counters

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_counters() &
            bind(C)
        import
        implicit none
    end function variorum_print_counters

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_power() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_power

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_power() &
            bind(C)
        import
        implicit none
    end function variorum_print_power

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_clock_speed() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_clock_speed

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_clock_speed() &
            bind(C)
        import
        implicit none
    end function variorum_print_clock_speed

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_hyperthreading() &
            bind(C)
        import
        implicit none
    end function variorum_print_hyperthreading

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_topology() &
            bind(C)
        import
        implicit none
    end function variorum_print_topology

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_features() &
            bind(C)
        import
        implicit none
    end function variorum_print_features

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_turbo() &
            bind(C)
        import
        implicit none
    end function variorum_print_turbo

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_verbose_gpu_utilization() &
            bind(C)
        import
        implicit none
    end function variorum_print_verbose_gpu_utilization

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_print_gpu_utilization() &
            bind(C)
        import
        implicit none
    end function variorum_print_gpu_utilization

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_enable_turbo() &
            bind(C)
        import
        implicit none
    end function variorum_enable_turbo

    !-------------------------------------------------------------------------
    integer(kind=c_int) &
            function variorum_disable_turbo() &
            bind(C)
        import
        implicit none
    end function variorum_disable_turbo

    !-------------------------------------------------------------------------
    end interface
    !-------------------------------------------------------------------------

!-----------------------------------------------------------------------------
end module variorum
!-----------------------------------------------------------------------------

# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

from ctypes import c_int, c_char_p, CDLL, POINTER


class variorum:
    def __init__(self):
        try:
            self.variorum_c = CDLL("libvariorum.so")
        except Exception:
            print("\nVariorum shared library not found. Please update LD_LIBRARY_PATH.")

        """
        Variorum Print Functions
        """

        # Print Power
        self.variorum_print_power = self.variorum_c.variorum_print_power
        self.variorum_print_power.restype = c_int

        # Print Verbose Power
        self.variorum_print_verbose_power = self.variorum_c.variorum_print_verbose_power
        self.variorum_print_verbose_power.restype = c_int

        # Print Power Limit
        self.variorum_print_power_limit = self.variorum_c.variorum_print_power_limit
        self.variorum_print_power_limit.restype = c_int

        # Print Verbose Power Limit
        self.variorum_print_verbose_power_limit = (
            self.variorum_c.variorum_print_verbose_power_limit
        )
        self.variorum_print_verbose_power_limit.restype = c_int

        # Print Thermals
        self.variorum_print_thermals = self.variorum_c.variorum_print_thermals
        self.variorum_print_thermals.restype = c_int

        # Print Verbose Thermals
        self.variorum_print_verbose_thermals = (
            self.variorum_c.variorum_print_verbose_thermals
        )
        self.variorum_print_verbose_thermals.restype = c_int

        # Print Counters
        self.variorum_print_counters = self.variorum_c.variorum_print_counters
        self.variorum_print_counters.restype = c_int

        # Print Verbose Counters
        self.variorum_print_verbose_counters = (
            self.variorum_c.variorum_print_verbose_counters
        )
        self.variorum_print_verbose_counters.restype = c_int

        # Print Frequency
        self.variorum_print_frequency = self.variorum_c.variorum_print_frequency
        self.variorum_print_frequency.restype = c_int

        # Print Verbose Frequency
        self.variorum_print_verbose_frequency = (
            self.variorum_c.variorum_print_verbose_frequency
        )
        self.variorum_print_verbose_frequency.restype = c_int

        # Print GPU Utilization
        self.variorum_print_gpu_utilization = (
            self.variorum_c.variorum_print_gpu_utilization
        )
        self.variorum_print_gpu_utilization.restype = c_int

        # Print Verbose GPU Utilization
        self.variorum_print_verbose_gpu_utilization = (
            self.variorum_c.variorum_print_verbose_gpu_utilization
        )
        self.variorum_print_verbose_gpu_utilization.restype = c_int

        # Print Hyperthreading
        self.variorum_print_hyperthreading = (
            self.variorum_c.variorum_print_hyperthreading
        )
        self.variorum_print_hyperthreading.restype = c_int

        # Print Topology
        self.variorum_print_topology = self.variorum_c.variorum_print_topology
        self.variorum_print_topology.restype = c_int

        # Print Features
        self.variorum_print_features = self.variorum_c.variorum_print_features
        self.variorum_print_features.restype = c_int

        # Print Turbo
        self.variorum_print_turbo = self.variorum_c.variorum_print_turbo
        self.variorum_print_turbo.restype = c_int

        # Print Available Frequencies
        self.variorum_print_available_frequencies = (
            self.variorum_c.variorum_print_available_frequencies
        )
        self.variorum_print_available_frequencies.restype = c_int

        # Print Energy
        self.variorum_print_energy = self.variorum_c.variorum_print_energy
        self.variorum_print_energy.restype = c_int

        # Get Current version
        self.variorum_get_current_version = self.variorum_c.variorum_get_current_version
        self.variorum_get_current_version.restype = c_char_p

        """
        Variorum Cap Functions
        """

        # Cap Best Effort Node Power Limit
        self.variorum_cap_best_effort_node_power_limit = (
            self.variorum_c.variorum_cap_best_effort_node_power_limit
        )
        self.variorum_cap_best_effort_node_power_limit.argtypes = [c_int]
        self.variorum_cap_best_effort_node_power_limit.restype = c_int

        # Cap Each Socket Power Limit
        self.variorum_cap_each_socket_power_limit = (
            self.variorum_c.variorum_cap_each_socket_power_limit
        )
        self.variorum_cap_each_socket_power_limit.argtypes = [c_int]
        self.variorum_cap_each_socket_power_limit.restype = c_int

        # Cap Each Core Frequency Limit
        self.variorum_cap_each_core_frequency_limit = (
            self.variorum_c.variorum_cap_each_core_frequency_limit
        )
        self.variorum_cap_each_core_frequency_limit.argtypes = [c_int]
        self.variorum_cap_each_core_frequency_limit.restype = c_int

        # Cap Socket Frequency Limit
        self.variorum_cap_socket_frequency_limit = (
            self.variorum_c.variorum_cap_socket_frequency_limit
        )
        self.variorum_cap_socket_frequency_limit.argtypes = [c_int, c_int]
        self.variorum_cap_socket_frequency_limit.restype = c_int

        # Cap GPU Power Ratio
        self.variorum_cap_gpu_power_ratio = self.variorum_c.variorum_cap_gpu_power_ratio
        self.variorum_cap_gpu_power_ratio.argtypes = [c_int]
        self.variorum_cap_gpu_power_ratio.restype = c_int

        """
        Variorum JSON Functions
        """

        # Get Node Power in JSON Format
        self.variorum_get_node_power_json = self.variorum_c.variorum_get_node_power_json
        self.variorum_get_node_power_json.argtypes = [POINTER(c_char_p)]
        self.variorum_get_node_power_json.restype = c_int

        # Get Node Power Domain Information in JSON Format
        self.variorum_get_node_power_domain_info_json = (
            self.variorum_c.variorum_get_node_power_domain_info_json
        )
        self.variorum_get_node_power_domain_info_json.argtypes = [POINTER(c_char_p)]
        self.variorum_get_node_power_domain_info_json.restype = c_int

        """
        Variorum Miscellaneous Functions
        """

        # Enable TurboBoost
        self.variorum_enable_turbo = self.variorum_c.variorum_enable_turbo
        self.variorum_enable_turbo.restype = c_int

        # Disable TurboBoost
        self.variorum_disable_turbo = self.variorum_c.variorum_disable_turbo
        self.variorum_disable_turbo.restype = c_int

        """
        Variorum Topology Functions
        """

        # Get Number of Sockets
        self.variorum_get_num_sockets = self.variorum_c.variorum_get_num_sockets
        self.variorum_get_num_sockets.restype = c_int

        # Get Number of Cores
        self.variorum_get_num_cores = self.variorum_c.variorum_get_num_cores
        self.variorum_get_num_cores.restype = c_int

        # Get Number of Threads
        self.variorum_get_num_threads = self.variorum_c.variorum_get_num_threads
        self.variorum_get_num_threads.restype = c_int

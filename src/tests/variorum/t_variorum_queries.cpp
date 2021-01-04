// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include <variorum.h>
}

TEST(variorum_queries, test_verbose_print_power_limits)
{
    EXPECT_EQ(0, variorum_print_verbose_power_limits());
}

TEST(variorum_queries, test_verbose_print_thermals)
{
    EXPECT_EQ(0, variorum_print_verbose_thermals());
}

TEST(variorum_queries, test_verbose_print_counters)
{
    EXPECT_EQ(0, variorum_print_verbose_counters());
}

TEST(variorum_queries, test_verbose_print_power)
{
    EXPECT_EQ(0, variorum_print_verbose_power());
}

TEST(variorum_queries, test_verbose_print_clock_speed)
{
    EXPECT_EQ(0, variorum_print_verbose_clock_speed());
}

TEST(variorum_queries, test_print_power_limit)
{
    EXPECT_EQ(0, variorum_print_power_limits());
}

TEST(variorum_queries, test_print_thermals)
{
    EXPECT_EQ(0, variorum_print_thermals());
}

TEST(variorum_queries, test_print_counters)
{
    EXPECT_EQ(0, variorum_print_counters());
}

TEST(variorum_queries, test_print_power)
{
    EXPECT_EQ(0, variorum_print_power());
}

TEST(variorum_queries, test_print_clock_speed)
{
    EXPECT_EQ(0, variorum_print_clock_speed());
}

TEST(variorum_queries, test_print_hyperthreading)
{
    EXPECT_EQ(0, variorum_print_hyperthreading());
}

TEST(variorum_queries, test_print_turbo)
{
    EXPECT_EQ(0, variorum_print_turbo());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

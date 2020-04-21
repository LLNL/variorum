// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
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

TEST(variorum_queries, test_print_power_limit)
{
    EXPECT_EQ(0, variorum_print_power_limits());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

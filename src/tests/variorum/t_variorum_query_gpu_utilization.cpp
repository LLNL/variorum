// Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include <variorum.h>
}

TEST(variorum_queries, test_verbose_gpu_util)
{
    EXPECT_EQ(0, variorum_print_verbose_gpu_utilization());
}

TEST(variorum_queries, test_print_gpu_util)
{
    EXPECT_EQ(0, variorum_print_gpu_utilization());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

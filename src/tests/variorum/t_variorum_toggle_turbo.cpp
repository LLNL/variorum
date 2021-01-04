// Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include <variorum.h>
}

TEST(variorum_toggle_turbo, test_disable_turbo)
{
    EXPECT_EQ(0, variorum_disable_turbo());
}

TEST(variorum_toggle_turbo, test_enable_turbo)
{
    EXPECT_EQ(0, variorum_enable_turbo());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

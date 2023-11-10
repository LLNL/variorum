// Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"
#include <cstdlib>

extern "C" {
#include <variorum.h>
}

TEST(variorum_power_limit, test_cap_best_effort_node_power_limit)
{
    const char *ret = std::getenv("TEST_NODE_POWER_LIMIT");
    int node_pow_limit;

    if (!ret)
    {
        node_pow_limit = 200;
    }
    else
    {
        node_pow_limit = std::stoi(ret);
    }
    EXPECT_EQ(0, variorum_cap_best_effort_node_power_limit(node_pow_limit));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

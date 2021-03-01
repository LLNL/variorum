// Copyright 2021 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

extern "C" {
#include <variorum.h>
}

TEST(variorum_socket_frequency, test_set_socket_frequency)
{
    int socket0_frequency = 950;
    int socket1_frequency = 1200;
    EXPECT_EQ(0, variorum_set_socket_frequency(0, socket0_frequency));
    EXPECT_EQ(0, variorum_set_socket_frequency(1, socket1_frequency));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

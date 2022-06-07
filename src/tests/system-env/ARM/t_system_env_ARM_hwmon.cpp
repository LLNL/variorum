// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>

#include "gtest/gtest.h"

std::vector<int> BuildTestData()
{
    std::vector<int> values;
    values.push_back(1); /* SoC */
    values.push_back(2); /* big */
    values.push_back(3); /* LITTLE */
    values.push_back(4); /* GPU */

    return values;
}

int is_file_exist(const char *file)
{
    struct stat statbuf;
    return stat(file, &statbuf);
}

int check_user_read_permissions(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    if (statbuf.st_mode & S_IRUSR)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int check_user_id(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    return statbuf.st_uid;
}

class HwmonTest : public ::testing::TestWithParam<int>
{
};

TEST_P(HwmonTest, Exists)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/sys/class/hwmon/hwmon0/power%d_input", testparam);
    EXPECT_EQ(0, is_file_exist(filename));
    free(filename);

    asprintf(&filename, "/sys/class/hwmon/hwmon0/temp%d_input", testparam);
    EXPECT_EQ(0, is_file_exist(filename));
    free(filename);
}

TEST_P(HwmonTest, UserID)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/sys/class/hwmon/hwmon0/power%d_input", testparam);
    /* Check if user owner is root */
    EXPECT_EQ(0, check_user_id(filename));
    free(filename);

    asprintf(&filename, "/sys/class/hwmon/hwmon0/temp%d_input", testparam);
    /* Check if user owner is root */
    EXPECT_EQ(0, check_user_id(filename));
    free(filename);
}

TEST_P(HwmonTest, UserPerms)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/sys/class/hwmon/hwmon0/power%d_input", testparam);
    EXPECT_EQ(1, check_user_read_permissions(filename));
    free(filename);

    asprintf(&filename, "/sys/class/hwmon/hwmon0/temp%d_input", testparam);
    EXPECT_EQ(1, check_user_read_permissions(filename));
    free(filename);
}

INSTANTIATE_TEST_CASE_P(HwmonTest_Instance,
                        HwmonTest,
                        ::testing::ValuesIn(BuildTestData()));

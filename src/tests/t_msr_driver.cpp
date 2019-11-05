// Copyright 2019 Lawrence Livermore National Security, LLC and other
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
    int nlogicalcores = sysconf(_SC_NPROCESSORS_ONLN);
    std::vector<int> values;
    for (int i = 0; i < nlogicalcores; i++)
    {
        values.push_back(i);
    }

    return values;
}

int is_file_exist(const char *file)
{
    struct stat statbuf;
    return stat(file, &statbuf);
}

int check_user_permissions(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    if ((statbuf.st_mode & S_IRUSR) || !(statbuf.st_mode & S_IWUSR))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int check_group_permissions(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    if ((statbuf.st_mode & S_IRGRP) || !(statbuf.st_mode & S_IWGRP))
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

int check_group_id(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    return statbuf.st_gid;
}

class MsrDriverTest : public ::testing::TestWithParam<int>
{
};

TEST_P (MsrDriverTest, Exists)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/dev/cpu/%d/msr", testparam);
    EXPECT_EQ (0, is_file_exist(filename));
    free(filename);
}

TEST_P (MsrDriverTest, UserID)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/dev/cpu/%d/msr", testparam);
    /* Check if user owner is root */
    EXPECT_EQ (0, check_user_id(filename));
    free(filename);
}

TEST_P (MsrDriverTest, UserPerms)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/dev/cpu/%d/msr", testparam);
    EXPECT_EQ (1, check_user_permissions(filename));
    free(filename);
}

TEST_P (MsrDriverTest, GroupID)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/dev/cpu/%d/msr", testparam);
    /* Check if group owner is root */
    EXPECT_EQ (0, check_group_id(filename));
    free(filename);
}

TEST_P (MsrDriverTest, GroupPerms)
{
    int testparam = GetParam();
    char *filename;

    asprintf(&filename, "/dev/cpu/%d/msr", testparam);
    EXPECT_EQ (1, check_group_permissions(filename));
    free(filename);
}

INSTANTIATE_TEST_CASE_P(MsrDriverTest_Instance,
                        MsrDriverTest,
                        ::testing::ValuesIn(BuildTestData()));

// Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "gtest/gtest.h"

int is_file_exist(const char *file)
{
    struct stat statbuf;
    return stat(file, &statbuf);
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

int check_other_permissions(const char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf))
    {
        return EXIT_FAILURE;
    }
    if ((statbuf.st_mode & S_IROTH) || !(statbuf.st_mode & S_IWOTH))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int allowlist_size(const char *file)
{
    std::ifstream infile(file);
    std::string line;
    std::string tmp = "tmp_allowlist";
    std::ofstream outfile;

    outfile.open(tmp.c_str());
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        outfile << line << std::endl;
    }

    struct stat statbuf;
    if (stat(tmp.c_str(), &statbuf))
    {
        return EXIT_FAILURE;
    }
    remove(tmp.c_str());
    return statbuf.st_size;
}

TEST(MsrAllowlist, Exists)
{
#ifdef USE_MSR_SAFE_BEFORE_1_5_0
    const char *const filename = "/dev/cpu/msr_whitelist";
#else
    const char *const filename = "/dev/cpu/msr_allowlist";
#endif
    EXPECT_EQ(0, is_file_exist(filename));
}

TEST(MsrAllowlist, Perms)
{
#ifdef USE_MSR_SAFE_BEFORE_1_5_0
    const char *const filename = "/dev/cpu/msr_whitelist";
#else
    const char *const filename = "/dev/cpu/msr_allowlist";
#endif
    EXPECT_EQ(1, check_other_permissions(filename));
}

TEST(MsrAllowlist, Size)
{
#ifdef USE_MSR_SAFE_BEFORE_1_5_0
    const char *const filename = "/dev/cpu/msr_whitelist";
#else
    const char *const filename = "/dev/cpu/msr_allowlist";
#endif
    /* Valid list should not be empty */
    EXPECT_NE(0, allowlist_size(filename));
}


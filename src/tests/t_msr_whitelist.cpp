// Copyright 2019 Lawrence Livermore National Security, LLC and other
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

int whitelist_size(const char *file)
{
    std::ifstream infile(file);
    std::string line;
    std::string tmp = "tmp_whitelist";
    std::ofstream outfile;

    outfile.open(tmp);
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

TEST (MsrWhitelist, Exists)
{
    char *filename;

    asprintf(&filename, "/dev/cpu/msr_whitelist");
    EXPECT_EQ(0, is_file_exist(filename));
    free(filename);
}

TEST (MsrWhitelist, Perms)
{
    char *filename;

    asprintf(&filename, "/dev/cpu/msr_whitelist");
    EXPECT_EQ(1, check_other_permissions(filename));
    free(filename);
}

TEST (MsrWhitelist, Size)
{
    char *filename;

    asprintf(&filename, "/dev/cpu/msr_whitelist");
    /* Valid whitelist should not be empty */
    EXPECT_NE(0, whitelist_size(filename));
    free(filename);
}


/* highlander.c
 *
 * Copyright (c) 2011-2016, Lawrence Livermore National Security, LLC.
 * LLNL-CODE-645430
 *
 * Produced at Lawrence Livermore National Laboratory
 * Written by  Barry Rountree,   rountree@llnl.gov
 *             Daniel Ellsworth, ellsworth8@llnl.gov
 *             Scott Walker,     walker91@llnl.gov
 *             Kathleen Shoga,   shoga1@llnl.gov
 *
 * All rights reserved.
 *
 * This file is part of Variorum.
 *
 * Variorum is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Variorum is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with Variorum.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This material is based upon work supported by the U.S. Department
 * of Energy's Lawrence Livermore National Laboratory. Office of
 * Science, under Award number DE-AC52-07NA27344.
 *
 */

/// Highlander... because there can be only one.

#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

static int amHighlander = -1;

static sem_t *semk = NULL;
static sem_t *seml = NULL;

/// @brief Determines/initializes the process highlander status.
int highlander(void)
{
    if (amHighlander != -1)
    {
        return amHighlander;
    }

    semk = sem_open("/power_wrapperK", O_CREAT | O_EXCL, 0600, 0);
    if(semk == NULL || semk == SEM_FAILED)
    {
        semk = sem_open("/power_wrapperK", O_CREAT, 0600, 0);
        seml = sem_open("/power_wrapperL", O_CREAT, 0600, 0);
        sem_post(semk);
        int v;
        sem_getvalue(semk, &v);
        amHighlander = 0;
        return 0;
    }
    seml = sem_open("/power_wrapperL", O_CREAT, 0600, 0);
    amHighlander = 1;
    return 1;
}

int highlander_clean(void)
{
    // remove the named semaphores
    fprintf(stdout, "Removing named semaphore /power_wrapperK\n");
    sem_unlink("/power_wrapperK");
    fprintf(stdout, "Removing named semaphore /power_wrapperL\n");
    sem_unlink("/power_wrapperL");
    return 1;
}

/// @brief Causes the highlander to wait until all foes have called wait.
int highlander_wait(void)
{
    if (amHighlander < 0)
    {
        return 1;
    }
    if (amHighlander)
    {
        int v;
        sem_getvalue(semk, &v);
        while (v > 1)
        {
            sem_wait(seml);
        }

        sem_close(semk);
        sem_close(seml);
        sem_unlink("/power_wrapperK");
        sem_unlink("/power_wrapperL");

        return 0;
    }
    else
    {
        sem_wait(semk);
        sem_post(seml);
        sem_close(semk);
        sem_close(seml);
        return 0;
    }
    return 1;
}

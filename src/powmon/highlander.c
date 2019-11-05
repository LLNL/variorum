// Copyright 2019 Lawrence Livermore National Security, LLC and other
// Variorum Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

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

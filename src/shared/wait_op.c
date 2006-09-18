/* @(#) $Id$ */

/* Copyright (C) 2006 Daniel B. Cid <dcid@ossec.net>
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */


#include "shared.h"
#define LOCK_LOOP   5


/* Creates global lock */
void os_setwait()
{
    FILE *fp = NULL;
    if(isChroot())
    {
        fp = fopen(WAIT_FILE, "w");
    }
    else
    {
        fp = fopen(WAIT_FILE_PATH, "w");
    }

    if(fp)
    {
        fprintf(fp, "l");
        fclose(fp);
    }

    return;
}


/* Removes global lock */
void os_delwait()
{
    if(isChroot())
    {
        unlink(WAIT_FILE);
    }
    else
    {
        unlink(WAIT_FILE_PATH);
    }
    return;
}



/* Check for the wait file. If present, wait.
 * Works as a simple inter process lock (only the main
 * process is allowed to lock).
 */
void os_wait()
{
    struct stat file_status;
    
    /* If the wait file is not present, keep going.
     */
    if(isChroot())
    {
        if(stat(WAIT_FILE, &file_status) == -1)
            return;
    }
    else
    {
        if(stat(WAIT_FILE_PATH, &file_status) == -1)
            return;
    }
    

    /* Wait until the lock is gone. */
    verbose(WAITING_MSG, __local_name);
    while(1)
    {
        if(isChroot())
        {
            if(stat(WAIT_FILE, &file_status) == -1)
                break;
        }
        else
        {
            if(stat(WAIT_FILE_PATH, &file_status) == -1)
                break;
        }

        /* Sleep LOCK_LOOP seconds and check it lock is gone. */
        sleep(LOCK_LOOP);
    }

    verbose(WAITING_FREE, __local_name);
    return;
}



/* EOF */
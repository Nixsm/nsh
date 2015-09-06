#ifndef NSH_JOB_LIST_H
#define NSH_JOB_LIST_H

#include <unistd.h>
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    FOREGROUND = 0,
    BACKGROUND,
    STOPPED,
    DONE,
    TERMINATED
} job_status_t;

typedef struct job {
    char runString[256]; /* String called by the user */
    pid_t pgid; /* Group id of current proccess */
    int job; /* job index inside list */
    job_status_t jStatus; /* Status of this job */
    struct job* next;
} job_node_t;

typedef struct {
    unsigned int size;
    job_node_t* begin;
} Jobs;

Jobs createList();

void destroyList(INOUT Jobs* joblist);

int add(INOUT Jobs* jobList, IN pid_t pgid, IN char* cmd, IN job_status_t status);

int delete(INOUT Jobs* jobList, IN int job);

void showJobs(INOUT Jobs* jobList);

int setStatus(INOUT Jobs* jobList, IN int job, IN job_status_t status);

int getStatus(INOUT Jobs* jobList, IN int job, IN job_status_t status);

pid_t getProcess(INOUT Jobs* jobList, IN int job);

int getLargest(INOUT Jobs* jobList);

char* getStatusString(IN job_status_t status);

#endif//NSH_JOB_LIST_H

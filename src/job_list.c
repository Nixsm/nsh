#include "job_list.h"


Jobs createList() {
    Jobs jobs;
    jobs.size = 0;
    jobs.begin = NULL;
    return jobs;
}

void destroyList(INOUT Jobs* joblist) {
    job_node_t* job;
    while (joblist->begin) {
        job = joblist->begin;
        joblist->begin = joblist->begin->next;
        free(job);
    }

    joblist->size = 0;
    joblist->begin = NULL;
}

int add(INOUT Jobs* jobList, IN pid_t pgid, IN char* cmd, IN job_status_t status) {
    job_node_t* begin = jobList->begin;
    job_node_t* before = NULL;
    
    job_node_t* newNode = (job_node_t*)calloc(1, sizeof(job_node_t));
    
    newNode->pgid = pgid;
    strcpy(newNode->runString, cmd);
    newNode->job = ++(jobList->size);
    newNode->jStatus = status;
    newNode->next = NULL;
    
    while ((begin)) {
        before = begin;
        begin = begin->next;
    }
    
    if (before)
        before->next = newNode;
    else {
        jobList->begin = newNode;
    }
    
    return newNode->job;
}

int delete(INOUT Jobs* jobList, IN int job) {
    job_node_t* begin = jobList->begin;

    job_node_t* before = NULL;
    

    job_node_t* toBeDeleted = NULL;
    
    while (begin) {
        if (begin->job == job){
            break;
        }
        before = begin;
        begin = begin->next;
    }

    if (!before) {
        free(jobList->begin);
        jobList->begin = begin->next;
    } else {
        toBeDeleted = before->next;
        
        before->next = toBeDeleted->next;
        
        free(toBeDeleted);
    }

    jobList->size--;
}

char* getStatusString(IN job_status_t status) {
    switch (status) {
    case FOREGROUND:
        return "FOREGROUND";
    case BACKGROUND:
        return "BACKGROUND";
    case STOPPED:
        return "STOPPED";
    case DONE:
        return "DONE";
    case TERMINATED:
        return "TERMINATED";
    }
}

void showJobs(INOUT Jobs* jobList) {
    job_node_t* begin = jobList->begin;

    printf("[JobID]\t\tstatus\t\tgID\t\tCMD\n");
    while (begin) {
        printf("[%d]\t\t%s\t%ld\t\t%s\n", begin->job, getStatusString(begin->jStatus), begin->pgid, begin->runString);
        begin = begin->next;
    }
}

int setStatus(INOUT Jobs* jobList, IN int job, IN job_status_t status) {
    job_node_t* begin = jobList->begin;

    while (begin) {
        if (begin->job == job){
            break;
        }

        begin = begin->next;
    }

    begin->jStatus = status;
}

int getStatus(INOUT Jobs* jobList, IN int job, IN job_status_t status) { 
    job_node_t* begin = jobList->begin;

    while (begin) {
        if (begin->job == job){
            return begin->jStatus;
        }

        begin = begin->next;
    }   
}

pid_t getProcess(INOUT Jobs* jobList, IN int job) {
        job_node_t* begin = jobList->begin;

    while (begin) {
        if (begin->job == job){
            return begin->pgid;
        }

        begin = begin->next;
    }

}

int getLargest(INOUT Jobs* jobList) {
    
}

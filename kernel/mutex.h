#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "sleeplock.h"
#include "file.h"

int mutexalloc(struct file **f);

void mutexclose(struct sleeplock *lk);

#endif

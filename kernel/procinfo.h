#ifndef PROCINFO_H
#define PROCINFO_H

#define PNAMELEN 16

struct procinfo {
  int pid;
  int ppid;
  int state;
  char name[PNAMELEN];
  char pname[PNAMELEN];
};


#endif

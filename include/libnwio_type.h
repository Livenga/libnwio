#ifndef _NWIO_TYPE_H
#define _NWIO_TYPE_H

#include <stdint.h>

typedef struct _nwio_t nwio_t;

struct _nwio_t {
  int fd;

  char     *node;
  uint32_t service;
};


#endif

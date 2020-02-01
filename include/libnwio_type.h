#ifndef _NWIO_TYPE_H
#define _NWIO_TYPE_H

#include <stdint.h>
#include <openssl/ssl.h>

typedef struct _nwio_t nwio_t;

struct _nwio_t {
  int socket;

  uint8_t use_tls;

  char *node;
  char *service;

  size_t addrlen;
  struct sockaddr *addr;

  SSL     *ssl;
  SSL_CTX *ssl_ctx;
};


#endif

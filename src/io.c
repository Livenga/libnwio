#define __DEBUG__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include "../include/libnwio_type.h"


int nwio_connect(const nwio_t *self) {
  int status;

  status = connect(self->socket, self->addr, self->addrlen);
  if(status != 0) {
    return status;
  }

  if(self->use_tls) {
    status = SSL_connect(self->ssl);
    printf("%d\n", status);
  }

  return 0;
}


/**
 */
ssize_t nwio_read(
    nwio_t *self,
    void *buf,
    size_t count) {
  return (self->use_tls == 1)
    ? SSL_read(self->ssl, buf, count)
    : read(self->socket, buf, count);
}


/**
 */
ssize_t nwio_write(
    nwio_t *self,
    const void *buf,
    size_t count) {
  return (self->use_tls == 1)
    ? SSL_write(self->ssl, buf, count)
    : write(self->socket, buf, count);
}

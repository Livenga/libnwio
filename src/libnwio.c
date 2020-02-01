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


/**
 */
nwio_t *nwio_new(
    const char     *node,
    const uint32_t service,
    uint8_t        is_udp,
    uint8_t        use_tls) {
  nwio_t *p_nwio = NULL;

  if(node == NULL) {
    return NULL;
  }

  p_nwio = (nwio_t *)calloc(1, sizeof(nwio_t));
  if(p_nwio == NULL) {
    goto pt_end;
  }
  p_nwio->use_tls = use_tls;

  /* node 複製 */
  const uint32_t _node_len = strlen(node);
  p_nwio->node = (char *)calloc(
      _node_len + 1,
      sizeof(char));

  strncpy(p_nwio->node, node, _node_len);

  /* service 文字列化 */
  p_nwio->service = (char *)calloc(
      32,
      sizeof(char));
  sprintf(p_nwio->service, "%u", service);


  struct addrinfo ai_hints, *ai_res = NULL;
  memset((void *)&ai_hints, '\0', sizeof(struct addrinfo));

  ai_hints.ai_family    = AF_INET;
  ai_hints.ai_socktype  = (is_udp == 0)
    ? SOCK_STREAM
    : SOCK_DGRAM;
  ai_hints.ai_protocol  = 0;
  ai_hints.ai_flags     = 0;
  ai_hints.ai_addrlen   = 0;

  ai_hints.ai_addr      = NULL;
  ai_hints.ai_next      = NULL;
  ai_hints.ai_canonname = NULL;


  int status = getaddrinfo(
      p_nwio->node,
      p_nwio->service,
      (const struct addrinfo *)&ai_hints,
      &ai_res);

  if(status != 0) {
    // TODO:
  }


  struct addrinfo *p_ai = ai_res;

  for(;
      p_ai != NULL;
      p_ai = p_ai->ai_next) {
    if(p_ai->ai_addrlen == sizeof(struct sockaddr_in)) {
      p_nwio->addrlen = p_ai->ai_addrlen;
      p_nwio->addr    = (struct sockaddr *)calloc(1, p_ai->ai_addrlen);

      memcpy(
          (void *)p_nwio->addr,
          (const void *)p_ai->ai_addr,
          p_ai->ai_addrlen);

      break;
    }
  }
  freeaddrinfo(ai_res);


#ifdef __DEBUG__
  fprintf(stderr, "[D] %s(%s):%s\n",
      p_nwio->node,
      inet_ntoa(((struct sockaddr_in *)p_nwio->addr)->sin_addr),
      p_nwio->service);
#endif

  /**/
  p_nwio->socket = socket(
      AF_INET,
      (is_udp == 0)
      ? SOCK_STREAM
      : SOCK_DGRAM,
      0);


  // SSL
  if(use_tls == 1) {
    SSL_load_error_strings();
    SSL_library_init();

    p_nwio->ssl_ctx = SSL_CTX_new(SSLv23_method());
    p_nwio->ssl     = SSL_new(p_nwio->ssl_ctx);

    SSL_set_options(p_nwio->ssl, SSL_OP_NO_SSLv2);
    SSL_set_fd(p_nwio->ssl, p_nwio->socket);
  }


  return p_nwio;


pt_free_nwio:
  free((void *)p_nwio);
  p_nwio = NULL;

pt_end:
  return NULL;
}


/**
 */
void *nwio_free(nwio_t *self) {
  memset((void *)self->addr, '\0', self->addrlen);
  free((void *)self->addr);
  self->addr = NULL;

  memset((void *)self->service, '\0', strlen(self->service));
  free((void *)self->service);
  self->service = NULL;

  memset((void *)self->node, '\0', strlen(self->node));
  free((void *)self->node);
  self->node = NULL;

  if(self->use_tls == 1) {
    SSL_shutdown(self->ssl);
    SSL_free(self->ssl);
    self->ssl = NULL;

    close(self->socket);

    SSL_CTX_free(self->ssl_ctx);
    self->ssl_ctx = NULL;
  } else {
    close(self->socket);
  }


  memset((void *)self, '\0', sizeof(nwio_t));
  free((void *)self);
  self = NULL;

  return NULL;
}

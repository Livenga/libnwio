#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../include/libnwio_type.h"


/**
 */
nwio_t *nwio_new(
    const char     *node,
    const uint32_t service,
    uint8_t        use_tls) {
  nwio_t *p_nwio = NULL;

  if(node == NULL) {
    return NULL;
  }

  p_nwio = (nwio_t *)calloc(1, sizeof(nwio_t));
  if(p_nwio == NULL) {
    goto pt_end;
  }

  const uint32_t _node_len = strlen(node);
  p_nwio->node = (char *)calloc(
      _node_len + 1,
      sizeof(char));

  strncpy(p_nwio->node, node, _node_len);

  return p_nwio;


pt_free_nwio:
  free((void *)p_nwio);
  p_nwio = NULL;

pt_end:
  return NULL;
}

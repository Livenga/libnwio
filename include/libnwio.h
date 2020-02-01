#ifndef _NWIO_H
#define _NWIO_H

#include "libnwio_type.h"


/**
 */
extern nwio_t *nwio_new(
    const char     *node,
    const uint32_t service,
    uint8_t        is_udp,
    uint8_t        use_tls);

/**
 */
extern void *nwio_free(nwio_t *p);


/**
 */
extern int nwio_connect(const nwio_t *self);


/**
 */
extern ssize_t nwio_read(
    nwio_t *self,
    void *buf,
    size_t count);

/**
 */
extern ssize_t nwio_write(
    nwio_t *self,
    const void *buf,
    size_t count);
#endif

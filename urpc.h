#include "urpc_schema.h"
#include <stdbool.h>
#include <stdlib.h>


enum __urpcStatusCodes {
    URPC_ERROR = -1,
    URPC_SUCCESS = 0,
    URPC_AGAIN,
    URPC_BUSY,
    URPC_EXPECT_RESPONSE,
};

/* Transport read function interface
 * 
 * Reads bytes off the transport medium into a read buffer.
 * 
 * buf      - Buffer to hold bytes as they are read.
 * size     - Size (in bytes) that the buffer can hold.
 * 
 * return   - On success: number of bytes read
 *          - On error: negative error code
 */
typedef int (*TransportReadFn_t)(uint8_t *buf, size_t size);

/* Transport write function interface
 * 
 * Writes bytes to the transport medium.
 * 
 * src      - Source data to be written.
 * size     - Size (in bytes) of data to be written.
 * 
 * return   - On success: number of bytes written
 *          - On error: negative error code
 */
typedef int (*TransportWriteFn_t)(const void *src, size_t size);

typedef struct __urpcTransport {
    TransportReadFn_t read;
    TransportWriteFn_t write;
} URPCTransport_t;

typedef struct __urpcServerCtx {
    URPCTransport_t transport;
    volatile bool busy;
    URPCSchema_t *schema;
} URPCServerCtx_t;


/*******************************************************************************
 * uRPC API
 ******************************************************************************/
int urpc_server_init(URPCServerCtx_t *ctx, TransportReadFn_t readFn, TransportWriteFn_t writeFn);
int urpc_server_run(URPCServerCtx_t *ctx);
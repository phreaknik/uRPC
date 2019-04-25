#include "urpc.h"
#include "urpc_config.h"
#include <stdlib.h>

/*******************************************************************************
 * uRPC API
 ******************************************************************************/
int urpc_server_init(URPCServerCtx_t *ctx, TransportReadFn_t readFn, TransportWriteFn_t writeFn)
{
    // Initialize the transport
    ctx->transport.read = readFn;
    ctx->transport.write = writeFn;
    ctx->busy = false;
}

int urpc_server_run(URPCServerCtx_t *ctx)
{
    uint8_t readBuf[sizeof(URPCSchema_t)];
    size_t count = 0;

    if(ctx == NULL) return URPC_ERROR;
    if(ctx->busy) return URPC_BUSY;

    // Read data from transport
    ctx->busy = true;
    memset(readBuf, 0, sizeof(readBuf));
    int retries = URPC_SERVER_MAX_READ_RETRIES;
    while(retries--)
    {
        count += ctx->transport.read(&readBuf[count], sizeof(readBuf) - count);
    }

    // Parse data if anything was read
    if(count > 0) 
    {
        int rc = ctx->schema.parse(readBuf, count);
        if(rc != URPC_EXPECT_RESPONSE) ctx->busy = false;
        return rc;
    }

    return URPC_SUCCESS;
}
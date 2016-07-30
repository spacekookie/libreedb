#include <reedb/errors.h>

const char *rdb_error_getmsg(rdb_err_t *e)
{
    switch(*e) {
        case SUCCESS:                   return "Success";
        case FAILURE:                   return "Generic Failure";
        case MALLOC_FAILED:             return "Memory Allocation failed";
        case INVALID_BUFFER_SIZE:       return "Invalid sized buffer provided";
        case INVALID_PAYLOAD:           return "Invalid data payload in struct";
        case INVALID_PARAMS:            return "Invalid parameters provided";
        default:                        return "Unknown Issue";
    }
}
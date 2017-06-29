#include "connector_app.h"
#include <stdlib.h>


bool os_malloc(size_t const size, void const ** ptr){
    *ptr = malloc(size);
    return ptr != NULL;
}

bool os_realloc(size_t const size, void * ptr){
    ptr = realloc(ptr, size);
    return ptr != NULL;
}

void os_free(void ** const ptr){
    if(ptr != NULL){
        free(ptr);
    }
}

void os_time(uint32_t * const uptime){
    uint32_t cur_time = TickGet() / TICKS_PER_SECOND;
    *uptime = (cur_time - connector_config.start_time);
}

connector_callback_status_t connector_os_callback(connector_os_request_t const request_id,
                                            const void * const request_data,
                                            void * response_data){

    switch(request_id){
        case connector_os_malloc:
            return os_malloc(*((size_t *)request_data),
                    (void const ** )response_data)
                    ? connector_callback_continue : connector_callback_busy;
        case connector_os_free:
            os_free((void**)response_data);
            return connector_callback_continue;
        case connector_os_system_up_time:
            os_time((uint32_t *)response_data);
            return connector_callback_continue;
        default:
            DEBUG_PRINTF("connector_os_callback: unrecognized request [%d].\n", 
                    request_id);
            return connector_callback_unrecognized;
    }
}
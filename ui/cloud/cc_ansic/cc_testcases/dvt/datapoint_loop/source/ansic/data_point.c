/*
 * Copyright (c) 2013 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
#include <time.h>
#include <unistd.h>
#include "connector_api.h"
#include "platform.h"



static connector_bool_t app_dp_waiting_for_response = connector_false;

connector_request_data_point_t * app_allocate_datastreams_memory(size_t const stream_count, size_t const point_count)
{
    connector_request_data_point_t * dp_ptr = malloc(sizeof *dp_ptr);

    if (dp_ptr != NULL)
    {
        dp_ptr->stream = malloc(stream_count * sizeof *dp_ptr->stream);

        if (dp_ptr->stream != NULL)
        {
            connector_data_stream_t * stream = dp_ptr->stream;
            size_t stream_index;

            for (stream_index = 0; stream_index < stream_count; stream_index++)
            {
                stream->point = malloc(point_count * sizeof *dp_ptr->stream->point);

                if (stream->point != NULL)
                {
                    connector_data_point_t * point = stream->point;
                    size_t point_index;

                    for (point_index = 0; point_index < point_count; point_index++)
                    {
                        /* Chain stream to the next one. Last one points to NULL. */
                        if (point_index < point_count - 1)
                            point->next = point + 1;
                        else
                            point->next = NULL;

                        point++;
                    }
                }

                /* Chain stream to the next one. Last one points to NULL. */
                if (stream_index < stream_count - 1)
                    stream->next = stream + 1;
                else
                    stream->next = NULL;

                stream++;
            }

            dp_ptr->user_context = dp_ptr;
            dp_ptr->transport = connector_transport_tcp;
        }
        else
        {
            free(dp_ptr);
            dp_ptr = NULL;
        }
    }

    return dp_ptr;
}


/* Free all reserved memory in the DataStreams/DataPoints structure */
void app_free_datastreams_memory(connector_request_data_point_t * dp_ptr)
{
    APP_DEBUG("Freering the memory from the DataStreams structure.....\n");
    if (dp_ptr != NULL)
    {
        if (dp_ptr->stream != NULL)
        {
            size_t stream_index;

            /* Detect how many DataStreams we have in this structure */
            unsigned int numberOfStreams = 1;
            connector_data_stream_t * pointer_to_stream = dp_ptr->stream;
            while ( pointer_to_stream->next != NULL )
            {
                numberOfStreams++;
                pointer_to_stream = pointer_to_stream->next;
            }


            /* For each DataStream we free the DataPoints */
            for (stream_index = 0; stream_index < numberOfStreams; stream_index++)
            {
                connector_data_stream_t * const stream = &dp_ptr->stream[stream_index];

                /* Detect how many DataPoints we have in this DataStream */
                unsigned int numberOfPoints = 1;
                connector_data_point_t * pointer_to_point = stream->point;
                while ( pointer_to_point->next != NULL )
                {
                    numberOfPoints++;
                    pointer_to_point = pointer_to_point->next;
                }


                size_t point_index;

                /* For each DataPoint we show the info */
                for (point_index = 0; point_index < numberOfPoints; point_index++)
                {

                    /* Get the structure for each DataPoint*/
                    connector_data_point_t point = stream->point[point_index];

                    /* Free description */
                    if ( point.description != NULL )
                    {
                        free(point.description);
                    }

                    /* Free location */
                    if( point.location.value.text.latitude != NULL )
                        { free(point.location.value.text.latitude); }

                    if( point.location.value.text.longitude != NULL )
                        { free(point.location.value.text.longitude); }

                    if( point.location.value.text.elevation != NULL )
                        { free(point.location.value.text.elevation); }


                }


                /* All points were allocated in one malloc() call, so only one free() is necessary. */
                free(stream->point);
            }
            /* All streams were allocated in one malloc() call, so only one free() is necessary. */
            free(dp_ptr->stream);
        }
        free(dp_ptr);
    }

}



void app_show_datastreams(connector_request_data_point_t * dp_ptr)
{

    if (dp_ptr != NULL)
    {
        if (dp_ptr->stream != NULL)
        {
            size_t stream_index;

            /* Detect how many DataStreams we have in this structure */
            unsigned int numberOfStreams = 1;
            connector_data_stream_t * pointer_to_stream = dp_ptr->stream;
            while ( pointer_to_stream->next != NULL )
            {
                numberOfStreams++;
                pointer_to_stream = pointer_to_stream->next;
            }


            /* For each DataStream we show the info */
            for (stream_index = 0; stream_index < numberOfStreams; stream_index++)
            {
                connector_data_stream_t * const stream = &dp_ptr->stream[stream_index];

                APP_DEBUG("[Datastream] ID: '%s' -- UNITS: '%s'\n", stream->stream_id, stream->unit);


                if (stream->point != NULL)
                {
                    /* Detect how many DataPoints we have in this DataStream */
                    unsigned int numberOfPoints = 1;
                    connector_data_point_t * pointer_to_point = stream->point;
                    while ( pointer_to_point->next != NULL )
                    {
                        numberOfPoints++;
                        pointer_to_point = pointer_to_point->next;
                    }


                    size_t point_index;

                    /* For each DataPoint we show the info */
                    for (point_index = 0; point_index < numberOfPoints; point_index++)
                    {
                        /* Get the structure for each DataPoint*/
                        connector_data_point_t point = stream->point[point_index];

                        /* Print the DataPoint info */
                        APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%d'\n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.native.int_value);
                    }
                }

            }

        }

    }

}
















void app_setup_stream(connector_data_stream_t * stream, char * stream_id, char * units, connector_data_point_type_t const type, char * forward_to)
{
    stream->stream_id = stream_id;
    stream->unit = units;
    stream->type = type;
    stream->forward_to = forward_to;
}


static float app_get_incremental(void)
{
    static int incremental = 0;

    return incremental++;
}



float app_get_random_float(float const max_float)
{
    float randomFloat = ( rand() / (float)(RAND_MAX) ) * max_float;

    return randomFloat;
}


char * app_get_random_coordinate(connector_bool_t negative)
{
    /* Obtain a random coordinate */
    float number = app_get_random_float(99.99);

    if (negative)
    {/* Set coordinate in the opposite side */
        number = number * -1.0;
    }

    /* Allocate memory for each number */
    int max_size = 7;
    char *coordinate = malloc (sizeof (char) * max_size);

    /* Convert the float on a char[] */
    sprintf(coordinate, "%4.1f", number);

    return coordinate;
}


int app_get_random_integer(int const max_integer)
{
    int randomInteger = rand() % max_integer;
    return randomInteger;
}



/* Function to send to Device Cloud the Data Stream with the DataPoints */
connector_status_t app_send_datastreams(connector_handle_t const handle, connector_request_data_point_t * const dp_ptr)
{
    APP_DEBUG("Data point app: starting to send data...\n");
    connector_status_t status = connector_success;

    if (app_dp_waiting_for_response)
    {
        APP_DEBUG("Data point app: waiting for a response...\n");
        status = connector_service_busy;
        goto done;
    }

    app_dp_waiting_for_response = connector_true;
    status = connector_initiate_action(handle, connector_initiate_data_point, dp_ptr);
    APP_DEBUG("Data point message sent, status[%d]\n", status);
    if (status != connector_success)
        app_dp_waiting_for_response = connector_false;

done:
    return status;
}












/*********************************************/
/*********** DATASTREAM STRUCTURE ************/
/*********************************************/

connector_request_data_point_t * generateDataStreamStructure(size_t const numberStreams, size_t const numberPointsPerStream){

    /************ COMMON */
    /* Create structure to save the DataStreams with its DataPoints */
    connector_request_data_point_t * data_point_structure = app_allocate_datastreams_memory(numberStreams, numberPointsPerStream);
    if (data_point_structure == NULL)
    {/* Initialization was failed */
        return data_point_structure;
    }

    /* Save inside the user context var the pointer to the structure */
    data_point_structure->user_context = (void *)data_point_structure;





    /************* CREATE DATASTREAM */
    /* Configure the DataStream with the settings */
    app_setup_stream(&data_point_structure->stream[0],
                     "incremental", /* char * stream_id: string to identify the DataStream */
                     "Counts", /* char * units: string to identify the units of the samples */
                     connector_data_point_type_integer, /* connector_data_point_type_t data_type: internal type for the samples */
                     NULL); /* char * forward_to: string to save the datastream to forward the samples */


    /************* GENERATE DATAPOINTS */
    /* Setup the values for each DataPoint */
    size_t point_index = 0;
    size_t stream_index = 0;


    while( point_index < numberPointsPerStream )
    {
        /* Get the pointer to the DataPoint */
        connector_data_point_t * const point = &data_point_structure->stream[stream_index].point[point_index];

        /* Set the timestamp in the DataPoint */
        {
            time_t const current_time = time(NULL);

            point->time.source = connector_time_local_epoch_fractional;
            point->time.value.since_epoch_fractional.seconds = current_time;
            point->time.value.since_epoch_fractional.milliseconds = 0;
        }



        /* Set the location coordinates */
        {
            /* Generate random coordinates */
            char *latitude_str = app_get_random_coordinate(connector_false);
            char *longitude_str = app_get_random_coordinate(connector_true);
            char *elevation_str = app_get_random_coordinate(connector_true);

            point->location.type = connector_location_type_text;
            point->location.value.text.latitude = latitude_str;
            point->location.value.text.longitude = longitude_str;
            point->location.value.text.elevation = elevation_str;
        }

        /* Set the quality of the sample */
        point->quality.type = connector_quality_type_native; /* connector_quality_type_native (integer) or connector_quality_type_ignore*/
        if ( point->quality.type == connector_quality_type_native )
        {
            int quality_value = app_get_random_integer(2147483647); /* 2147483647 Maximun Integer */
            point->quality.value = quality_value;
        }

        /* Set the point description */
        /*point->description = NULL;*/
        char *description = malloc (sizeof (char) * 40);
        sprintf(description, "this is the description with index %d", (int)point_index);
        point->description = description;


        /* Set the data type */
        point->data.type = connector_data_type_native;

        /* Fill the value of the point */
        switch ( data_point_structure->stream[stream_index].type)
        {
            case connector_data_point_type_integer:
                point->data.element.native.int_value = app_get_incremental();
                break;

            case connector_data_point_type_float:
                point->data.element.native.float_value = app_get_random_float(1000.99);
                break;

            case connector_data_point_type_double:
                point->data.element.native.float_value = app_get_random_float(1000.99);
                break;

            case connector_data_point_type_long:
                #if (defined CONNECTOR_SUPPORTS_64_BIT_INTEGERS)
                {
                    /* function return signed long integers, uniformly distributed over the interval [-2**31,2**31] */
                    point->data.element.native.long_value = mrand48();

                }
                #else
                {
                    #define LONG_SIZE (sizeof(long) * 3)

                    char *value = malloc (LONG_SIZE);
                    /* Reset the memory allocated to 0 */
                    memset(value, 0, LONG_SIZE);


                    point->data.type = connector_data_type_text;
                    snprintf(value, LONG_SIZE, "%ld", mrand48());
                    point->data.element.text = value;
                }
                #endif
                break;

            default:
                ASSERT(0);
                break;
        }


        point_index++;

    }




    /* return DataPoints structure */
    return data_point_structure;
}











/*************************************************/
/**************** STARTED FUNCTION ***************/
/*************************************************/

connector_status_t app_start_test_case_datapoints_loop(connector_handle_t handlerForConnector, size_t const numberOfLoops, size_t const numberStreams, size_t const numberPointsPerStream){
    /* Initialize settings */
    connector_status_t status = connector_init_error;

    for(int i=0;i<(int)numberOfLoops;i++)
    {
        /* Initialize settings */
        status = connector_init_error;
        size_t busy_count = 0;

        APP_DEBUG("BEGIN LOOP %d ----------------------------------------\n",i);

        /* Generate and fill a DataStream/DataPoint structure */
        connector_request_data_point_t * const data_point_structure = generateDataStreamStructure(numberStreams, numberPointsPerStream);


        while( status != connector_success)
        {
            int const sample_interval_in_seconds = 2;

            /* Now it is time to send all collected samples to Device Cloud */
            status = app_send_datastreams(handlerForConnector, data_point_structure);

            switch (status)
            {
                case connector_init_error:
                case connector_service_busy:
                case connector_unavailable:
                {
                    APP_DEBUG("Busy to send data point. status: %d\n", status);
                    int const retry_delay_in_seconds = 4;

                    if (++busy_count > 5) goto done;
                    APP_DEBUG(".");
                    sleep(retry_delay_in_seconds);
                    break;
                }

                case connector_success:
                    APP_DEBUG("Success to send data point. status: %d\n", status);
                    busy_count = 0;
                    break;

                default:
                    APP_DEBUG("Failed to send data point. status: %d\n", status);
                    status = connector_init_error;
                    goto done;
            }

            sleep(sample_interval_in_seconds);

        }


        sleep(15);
    }


done:
    return status;


}
















/***************************************************/
/* COMMON FUNCTION TO MANAGE THE DATAPOINT SERVICE */
/***************************************************/
connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        case connector_request_id_data_point_response:
        {
            connector_data_point_response_t * const resp_ptr = data;

            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point response [%d]\n", resp_ptr->response);
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Hint: %s\n", resp_ptr->hint);
            }
            break;
        }

        case connector_request_id_data_point_status:
        {
            connector_data_point_status_t * const status_ptr = data;

            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point status [%d]\n", status_ptr->status);

            /* Print the DataStreams/DataPoints sent to Device Cloud */
            app_show_datastreams(status_ptr->user_context); /* Pointer to DataStreams Structure */

            /* Free memory from the data structure */
            app_free_datastreams_memory(status_ptr->user_context); /* Pointer to DataStreams Structure */


            APP_DEBUG("END LOOP ----------------------------------------\n\n");


            break;
        }

        default:
            APP_DEBUG("Data point callback: Request not supported: %d\n", request_id);
            status = connector_callback_unrecognized;
            break;
    }


    return status;
}

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
/* Import zlib to use the crc32() function */
#include <zlib.h>

#include <signal.h>

#include "device_request.h"


/* Import the pointer to the connector handler instance */
extern connector_handle_t CLOUD_HANDLER;


static connector_bool_t app_dp_waiting_for_response = connector_false;


/* Handler function to manage the system signal */
connector_bool_t app_stop_execution = connector_false;
void handler_stop_signal(int signal)
{
    APP_DEBUG("Catched the stop execution signal %d!!!!!!!\n", signal);
    app_stop_execution = connector_true;
}



/**************************************************************************/
/******** FUNCTIONS TO ALLOCATE MEMORY FOR THE DATAPOINT STRUCTURE ********/
/**************************************************************************/

/* Function to allocate all memory needed by N DataStreams wth N DataPoints */
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









/***********************************************************/
/******** FUNCTIONS TO FREE THE DATAPOINT STRUCTURE ********/
/***********************************************************/

/* Free all reserved memory in the DataStreams/DataPoints structure */
void app_free_datastreams_memory(connector_request_data_point_t * dp_ptr)
{
    APP_DEBUG("Freeing the memory from the DataStreams structure.....\n");
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

/* Free all reserved memory in the DataStreams/DataPoints structure */
void app_free_binary_datastream_memory(connector_request_data_point_binary_t * dp_ptr)
{
    APP_DEBUG("Freeing the memory from the DataStreams structure.....\n");
    if (dp_ptr == NULL) goto done;

    if (dp_ptr->path != NULL)
    {
        free(dp_ptr->path);
        dp_ptr->path = NULL;
    }

    if (dp_ptr->point != NULL)
    {
        free(dp_ptr->point);
        dp_ptr->point = NULL;
    }

    free(dp_ptr);

done:
    return;
}







/************************************/
/******** AUXILIAR FUNCTIONS ********/
/************************************/

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










/*********************************************/
/*********** DATASTREAM STRUCTURE ************/
/*********************************************/

/* Function to generate a fully DataStream/DataPoint structure */
connector_request_data_point_t * generateDataStreamStructure(size_t const numberStreams, size_t const numberPointsPerStream, char * streamIdentifier, connector_data_point_type_t const datapoints_type)
{

    /************ COMMON */
    /* Create structure to save the DataStreams with its DataPoints */
    connector_request_data_point_t * data_point_structure = app_allocate_datastreams_memory(numberStreams, numberPointsPerStream);
    if (data_point_structure == NULL)
    {/* Initialization was failed */
        return data_point_structure;
    }

    /* Save inside the user context var the pointer to the structure */
    data_point_structure->user_context = (void *)data_point_structure;



    /************* CREATE DATASTREAMS */
    for( size_t stream_index = 0; stream_index<numberStreams; stream_index++ )
    {
        /* Configure the DataStream with the settings */
        app_setup_stream(&data_point_structure->stream[stream_index],
                        streamIdentifier, /* char * stream_id: string to identify the DataStream */
                        "Counts", /* char * units: string to identify the units of the samples */
                        datapoints_type, /* connector_data_point_type_t data_type: internal type for the samples */
                        NULL); /* char * forward_to: string to save the datastream to forward the samples */
    }


    /************* GENERATE DATAPOINTS */
    /* Setup the values for each DataPoint */
    size_t point_index = 0;

    for( size_t stream_index = 0;stream_index<numberStreams;stream_index++ )
    {
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
                    #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                    {
                        point->data.element.native.float_value = (float)drand48();
                    }
                    #else
                    {
                        static char value[APP_MAX_POINTS][sizeof(float) * 2];

                        point->data.type = connector_data_type_text;
                        snprintf(value[test_case], sizeof value[test_case], "%f", (float)drand48()); /* this also requires floating point, works for DVT */
                        point->data.element.text = value[test_case];
                    }
                    #endif
                    break;

                case connector_data_point_type_double:
                    #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                    {
                        point->data.element.native.double_value = drand48();
                    }
                    #else
                    {
                        static char value[APP_MAX_POINTS][sizeof(double) * 2];

                        point->data.type = connector_data_type_text;
                        snprintf(value[test_case], sizeof value[test_case], "%lf", drand48()); /* this also requires floating point, works for DVT */
                        point->data.element.text = value[test_case];
                    }
                    #endif
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

                case connector_data_point_type_string:
                    #define TEST_STRING_PATTERNS 8
                    {
                    static int index = 0;
                    static char * value[TEST_STRING_PATTERNS] = {
                        "long string 300 chars...............................................................................1...................................................................................................2...................................................................................................3...................................................................................................",
                        "Hello World!",
                        "c,o,m,m,a",
                        /* Removed due to it is not posible verify it in test harness, move to other test case */
                        /* "line\nfeed",*/
                        "t\ta\tb",
                        "\"quote\"",
                        "s p a c e",
                        "long string 574 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7",
                        "long string 1611 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7..............................................................................8..............................................................................9..............................................................................10..............................................................................11..............................................................................12..............................................................................13..............................................................................14..............................................................................15..............................................................................16..............................................................................17..............................................................................18..............................................................................19.............................................................................."};


                    point->data.element.native.string_value = value[index % TEST_STRING_PATTERNS];
                    index++;
                    }

                    break;
                case connector_data_point_type_json:
                    #define TEST_JSON_PATTERNS 5
                    {
                    static int index = 0;
                    static char * value[TEST_JSON_PATTERNS] = {
                        "{\"address\":{\"streetAddress\": \"21 2nd Street\",\"city\":\"New York\"},\"phoneNumber\":[{\"type\":\"home\",\"number\":\"212 555-1234\"}]}",
                        "{\"array\":[1,2,3],\"boolean\":true,\"null\":null,\"number\":123,\"object\":{\"a\":\"b\",\"c\":\"d\",\"e\":\"f\"},\"string\":\"Hello World\"}",
                        "{\"index\":\"test\"}",
                        "{ long string 574 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7}",
                        "long string 1611 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7..............................................................................8..............................................................................9..............................................................................10..............................................................................11..............................................................................12..............................................................................13..............................................................................14..............................................................................15..............................................................................16..............................................................................17..............................................................................18..............................................................................19.............................................................................."};


                    point->data.element.native.string_value = value[index % TEST_JSON_PATTERNS];
                    index++;
                    }

                    break;

                case connector_data_point_type_geojson:
                    #define TEST_GEOJSON_PATTERNS 6
                    {
                    static int index = 1;
                    static char * value[TEST_GEOJSON_PATTERNS] = {
                        "{\"type\":\"FeatureCollection\",\"features\":[{\"type\":\"Feature\",\"geometry\":{\"type\":\"Point\",\"coordinates\":[102.0, 0.6]},\"properties\": {\"prop0\":\"value0\"}},{\"type\":\"Feature\",\"geometry\": {\"type\": \"LineString\",\"coordinates\": [[102.0, 0.0], [103.0, 1.0], [104.0, 0.0], [105.0, 1.0]]},\"properties\": {\"prop1\": 0.0,\"prop0\": \"value0\"}},{\"type\": \"Feature\",\"geometry\": {\"type\": \"Polygon\",\"coordinates\": [[[100.0, 0.0], [101.0, 0.0], [101.0, 1.0], [100.0, 1.0],[100.0, 0.0]]]},\"properties\": {\"prop1\": {\"this\": \"that\"},\"prop0\": \"value0\"}}]}",
                        "{\"coordinates\": [102.0, 0.6]}",
                        "{ \"type\": \"Point\", \"coordinates\": [30, 10]}",
                        "{\"type\": \"MultiPolygon\",\"coordinates\": [[ [[40, 40], [20, 45], [45, 30], [40, 40]]],[[[20, 35], [10, 30], [10, 10], [30, 5], [45, 20], [20, 35]],[[30, 20], [20, 15], [20, 25], [30, 20]]]]}",
                        "{ long string 574 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7 }",
                        "long string 1611 chars..............................................................................1..............................................................................2..............................................................................3..............................................................................4..............................................................................5..............................................................................6..............................................................................7..............................................................................8..............................................................................9..............................................................................10..............................................................................11..............................................................................12..............................................................................13..............................................................................14..............................................................................15..............................................................................16..............................................................................17..............................................................................18..............................................................................19.............................................................................."};

                    point->data.element.native.string_value = value[index % TEST_GEOJSON_PATTERNS];
                    index++;
                    }

                    break;

                default:
                    APP_DEBUG("ERROR: generateDataStreamStructure(), Unknown DataPoint type...\n");
                    ASSERT(0);
                    break;
            }


            point_index++;

        }
    }




    /* return DataPoints structure */
    return data_point_structure;
}





/* Function to generate a fully Binary DataStream/DataPoint structure */
connector_request_data_point_binary_t * generateBinaryDataStreamStructure(char * streamIdentifier)
{

    /************ COMMON */
    /* Create structure to save the DataStreams with its DataPoints */
    connector_request_data_point_binary_t * data_point_structure = malloc(sizeof(connector_request_data_point_binary_t));

    if (data_point_structure == NULL)
    {/* Initialization was failed */
        APP_DEBUG("app_send_data_point: Failed to malloc dp_ptr\n");
        goto error;
    }

    /* Save inside the user context var the pointer to the structure */
    data_point_structure->user_context = (void *)data_point_structure;
    /* Set the transport */
    data_point_structure->transport = connector_transport_tcp;


    /************* SETUP DATASTREAM */
    /* Clone the Stream Identifier */
    data_point_structure->path = malloc( (sizeof(char) * strlen(streamIdentifier)) + 1);
    sprintf(data_point_structure->path, "%s", streamIdentifier);


    /************* GENERATE DATAPOINT */
    /* Fill the value of the point */
    size_t const rand_bytes = (rand() % 65535) + 1; /* Binary Datapoints have a limit of 64 KB */
    unsigned char * data_ptr = malloc(rand_bytes);

    if (data_ptr != NULL)
    {
        size_t i;

        for (i = 0; i < rand_bytes; i++)
            data_ptr[i] = rand() % UCHAR_MAX;

        /* Save the bytes used in the data */
        data_point_structure->bytes_used = rand_bytes;
        APP_DEBUG("get_binary_point: rand_bytes %" PRIsize " bytes\n", rand_bytes);
    }
    else
    {
        APP_DEBUG("get_binary_point: Failed to malloc binary point %" PRIsize " bytes\n", rand_bytes);
        goto error;
    }

    /* Save the data */
    data_point_structure->point = data_ptr;

    goto done;

error:
    app_free_binary_datastream_memory(data_point_structure);
    data_point_structure = NULL;

done:
    APP_DEBUG("Binary DataPoint structure generated...\n");
    /* return DataPoints structure */
    return data_point_structure;
}










/******************************************************************/
/*********** FUNCTIONS TO SEND THE DATAPOINT STRUCTURE ************/
/******************************************************************/

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

    /* Start the action for a DataPoint */
    status = connector_initiate_action(handle, connector_initiate_data_point, dp_ptr);


    APP_DEBUG("Data point message sent, status[%d]\n", status);
    if (status != connector_success)
        app_dp_waiting_for_response = connector_false;

done:
    return status;
}



/* Function to send to Device Cloud the Data Stream with the DataPoints */
connector_status_t app_send_binary_datastreams(connector_handle_t const handle, connector_request_data_point_binary_t * const dp_ptr)
{
    APP_DEBUG("Data point app: starting to send binary data...\n");
    connector_status_t status = connector_success;

    if (app_dp_waiting_for_response)
    {
        APP_DEBUG("Data point app: waiting for a response...\n");
        status = connector_service_busy;
        goto done;
    }

    app_dp_waiting_for_response = connector_true;

    /* Start the action for a Binary DataPoint */
    status = connector_initiate_action(handle, connector_initiate_data_point_binary, dp_ptr);


    APP_DEBUG("Data point message sent, status[%d]\n", status);
    if (status != connector_success)
        app_dp_waiting_for_response = connector_false;

done:
    return status;
}















/************************************************************/
/******** FUNCTIONS TO PRINT THE DATAPOINT STRUCTURE ********/
/************************************************************/

/* Function to print by console the DataStream/DataPoint structure */
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
                        switch ( stream->type )
                        {
                            case connector_data_point_type_integer:
                                APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%d'\n",
                                    point.location.value.text.latitude,
                                    point.location.value.text.longitude,
                                    point.location.value.text.elevation,
                                    (int)point.quality.value,
                                    point.description,
                                    point.data.element.native.int_value);
                                break;

                            case connector_data_point_type_float:
                                #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%f'\n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.native.float_value);
                                #else
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: 's'\n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.text);
                                #endif
                                break;
                            case connector_data_point_type_double:
                                #if (defined CONNECTOR_SUPPORTS_FLOATING_POINT)
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%f'\n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.native.double_value);
                                #else
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%s' \n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.text);
                                #endif
                                break;
                            case connector_data_point_type_long:
                                #if (defined CONNECTOR_SUPPORTS_64_BIT_INTEGERS)
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%" PRId64 "' \n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.native.long_value);
                                #else
                                    APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%s' \n",
                                        point.location.value.text.latitude,
                                        point.location.value.text.longitude,
                                        point.location.value.text.elevation,
                                        (int)point.quality.value,
                                        point.description,
                                        point.data.element.text);
                                #endif
                                break;
                            case connector_data_point_type_string:
                            case connector_data_point_type_json:
                            case connector_data_point_type_geojson:
                                APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: '%s' \n",
                                    point.location.value.text.latitude,
                                    point.location.value.text.longitude,
                                    point.location.value.text.elevation,
                                    (int)point.quality.value,
                                    point.description,
                                    point.data.element.native.string_value);
                                break;
                            default:
                                APP_DEBUG("[DataPoint] Location: '%s,%s,%s' -- Quality: '%d' -- Description: '%s' -- Data: 'XX'\n",
                                    point.location.value.text.latitude,
                                    point.location.value.text.longitude,
                                    point.location.value.text.elevation,
                                    (int)point.quality.value,
                                    point.description);
                                break;

                        }



                    }
                }

            }

        }

    }

}


/* Function to print by console the Binary DataStream/DataPoint structure */
void app_show_binary_datastream(connector_request_data_point_binary_t * dp_ptr)
{

    if (dp_ptr != NULL)
    {

        APP_DEBUG("[Datastream] ID: '%s' -- UNITS: 'BINARY'\n", dp_ptr->path);

        if (dp_ptr->point != NULL)
        {
            /* We can not show the binary data, then we calculate the crc32 of the data */
            uLong crc = crc32(0L, Z_NULL, 0);

            crc = crc32(crc, (unsigned char *)dp_ptr->point, (unsigned int)dp_ptr->bytes_used);

            APP_DEBUG("[DataPoint] Data crc32_DEC: '%lu' -- Data crc32_HEX: '%08lX'\n", crc, crc);
        }

    }

}





/*****************************************************/
/**************** TEST_CASES FUNCTIONS ***************/
/*****************************************************/
void * app_start_test_case_datapoints_loop(void * args){

    /* Reset the stop value */
    app_stop_execution = connector_false;
    /* Add listener function for a defined user signal */
    signal(SIGUSR1,handler_stop_signal);




    /* Initialize settings */
    connector_status_t status = connector_init_error;
    /* Parse the passed arguments structure */
    test_thread_arguments_t * test_arguments = (test_thread_arguments_t *) args;

    /* Obtain the value type of the datapoints */
    connector_data_point_type_t datapoints_type;

    if(strcmp(test_arguments->valueType, "Integer") == 0)
    {
        datapoints_type = connector_data_point_type_integer;
    }
    else if(strcmp(test_arguments->valueType, "Float") == 0)
    {
        datapoints_type = connector_data_point_type_float;
    }
    else if(strcmp(test_arguments->valueType, "Double") == 0)
    {
        datapoints_type = connector_data_point_type_double;
    }
    else if(strcmp(test_arguments->valueType, "Long") == 0)
    {
        datapoints_type = connector_data_point_type_long;
    }
    else if(strcmp(test_arguments->valueType, "String") == 0)
    {
        datapoints_type = connector_data_point_type_string;
    }
    else if(strcmp(test_arguments->valueType, "Binary") == 0)
    {
        datapoints_type = connector_data_point_type_binary;
    }
    else if(strcmp(test_arguments->valueType, "JSON") == 0)
    {
        datapoints_type = connector_data_point_type_json;
    }
    else if(strcmp(test_arguments->valueType, "GeoJSON") == 0)
    {
        datapoints_type = connector_data_point_type_geojson;
    }
    else
    {
        APP_DEBUG("Error: unknown datapoint type '%s'\n",test_arguments->valueType);
        ASSERT(0);
    }


    for(unsigned int i=1;i<=test_arguments->numberOfLoops;i++)
    {
        /* Check if we must stop the execution */
        if ( app_stop_execution )
        {
            APP_DEBUG("Stop execution flag is set, we stop the execution!!\n");
            goto free;
        }


        /* Initialize settings */
        status = connector_init_error;
        size_t busy_count = 0;

        APP_DEBUG("BEGIN LOOP %d of %d ----------------------------------------\n",i,test_arguments->numberOfLoops);


        /* Generate and fill a DataStream/DataPoint structure */
        connector_request_data_point_t * const data_point_structure = generateDataStreamStructure(test_arguments->numberStreams, test_arguments->numberPointsPerStream, test_arguments->streamIdentifier, datapoints_type);


        while( status != connector_success)
        {
            int const sample_interval_in_seconds = 2;

            /* Now it is time to send all collected samples to Device Cloud */
            status = app_send_datastreams(CLOUD_HANDLER, data_point_structure);

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


free:
    /* Free the memory for the arguments passed */
    if ( test_arguments != NULL )
    {
        if ( test_arguments->valueType != NULL )
        {
            APP_DEBUG("Free test_arguments->valueType...\n");
            free(test_arguments->valueType);
        }
        if ( test_arguments->streamIdentifier != NULL )
        {
            APP_DEBUG("Free test_arguments->streamIdentifier...\n");
            free(test_arguments->streamIdentifier);
        }
        APP_DEBUG("Free test_arguments...\n");
        free(test_arguments);
    }


done:

    APP_DEBUG("Finished the Thread for app_start_test_case_datapoints_loop()\n");
    return NULL;

}



















/*************************************************/
/**************** STARTED FUNCTION ***************/
/*************************************************/
void * app_start_test_case_binary_datapoints_loop(void * args){

    /* Reset the stop value */
    app_stop_execution = connector_false;
    /* Add listener function for a defined user signal */
    signal(SIGUSR1,handler_stop_signal);



    /* Initialize settings */
    connector_status_t status = connector_init_error;
    /* Parse the passed arguments structure */
    test_thread_arguments_t * test_arguments = (test_thread_arguments_t *) args;



    if(strcmp(test_arguments->valueType, "Binary") != 0)
    {
        APP_DEBUG("ERROR: app_start_test_case_binary_datapoints_loop(), Unknown DataPoint type...\n");
        ASSERT(0);
    }


    for(unsigned int i=1;i<=test_arguments->numberOfLoops;i++)
    {

        /* Check if we must stop the execution */
        if ( app_stop_execution )
        {
            APP_DEBUG("Stop execution flag is set, we stop the execution!!\n");
            goto free;
        }


        /* Initialize settings */
        status = connector_init_error;
        size_t busy_count = 0;

        APP_DEBUG("BEGIN LOOP %d of %d ----------------------------------------\n",i,test_arguments->numberOfLoops);


        /* Generate and fill a DataStream/DataPoint structure */
        connector_request_data_point_binary_t * const data_point_structure = generateBinaryDataStreamStructure( test_arguments->streamIdentifier );


        while( status != connector_success)
        {
            int const sample_interval_in_seconds = 2;

            /* Now it is time to send all collected samples to Device Cloud */
            status = app_send_binary_datastreams(CLOUD_HANDLER, data_point_structure);

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


        sleep(5);
    }


free:
    /* Free the memory for the arguments passed */
    if ( test_arguments != NULL )
    {
        if ( test_arguments->valueType != NULL )
        {
            APP_DEBUG("Free test_arguments->valueType...\n");
            free(test_arguments->valueType);
        }
        if ( test_arguments->streamIdentifier != NULL )
        {
            APP_DEBUG("Free test_arguments->streamIdentifier...\n");
            free(test_arguments->streamIdentifier);
        }
        APP_DEBUG("Free test_arguments...\n");
        free(test_arguments);
    }


done:

    APP_DEBUG("Finished the Thread for app_start_test_case_datapoints_loop()\n");
    return NULL;

}




























/***************************************************/
/* COMMON FUNCTION TO MANAGE THE DATAPOINT SERVICE */
/***************************************************/
connector_callback_status_t app_data_point_handler(connector_request_id_data_point_t const request_id, void * const data)
{
    connector_callback_status_t status = connector_callback_continue;

    switch (request_id)
    {
        /* Callback for standard DataPoint */
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
        /* Callback for standard DataPoint */
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



        /* Callback for Binary DataPoint */
        case connector_request_id_data_point_binary_response:
        {
            connector_data_point_response_t * const resp_ptr = data;
            connector_request_data_point_binary_t * const data_point_structure = resp_ptr->user_context;

            if (data_point_structure == NULL)
            {
                APP_DEBUG("Error: Received null Binary DataPoint response context\n");
                status = connector_callback_error;
                goto done;
            }


            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point response [%d]\n", resp_ptr->response);
            if (resp_ptr->hint != NULL)
            {
                APP_DEBUG("Hint: %s\n", resp_ptr->hint);
            }
            break;



        }
        /* Callback for Binary DataPoint */
        case connector_request_id_data_point_binary_status:
        {
            connector_data_point_status_t * const status_ptr = data;
            connector_request_data_point_binary_t * const data_point_structure = status_ptr->user_context;

            if (data_point_structure == NULL)
            {
                APP_DEBUG("Error: Received null binary dp status context\n");
                status = connector_callback_error;
                goto done;
            }

            app_dp_waiting_for_response = connector_false;
            APP_DEBUG("Received data point status [%d]\n", status_ptr->status);

            /* Print the DataStreams/DataPoints sent to Device Cloud */
            app_show_binary_datastream(status_ptr->user_context); /* Pointer to DataStreams Structure */

            /* Free memory from the data structure */
            app_free_binary_datastream_memory(status_ptr->user_context); /* Pointer to Binary DataStream Structure */


            APP_DEBUG("END LOOP ----------------------------------------\n\n");
            break;
        }


        default:
            APP_DEBUG("Data point callback: Request not supported: %d\n", request_id);
            status = connector_callback_unrecognized;
            break;
    }

done:
    return status;
}

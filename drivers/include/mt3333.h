/*
 * Copyright (C) 2016 Unwired Devices [info@unwds.com]
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    
 * @ingroup     
 * @brief       
 * @{
 * @file		mt3333.h
 * @brief       Mediatek MT3333-based GNSS module driver
 * @author      Eugene Ponomarev
 */
#ifndef mt3333_H_
#define mt3333_H_

#include "thread.h"
#include "periph/uart.h"

#include <time.h>

/**
 * MT3333 NMEA message field indicies
 */
#define MT3333_TIME_FIELD_IDX 1
#define MT3333_VALID_FIELD_IDX 2
#define MT3333_LAT_FIELD_IDX 3
#define MT3333_NS_FIELD_IDX 4
#define MT3333_LON_FIELD_IDX 5
#define MT3333_EW_FIELD_IDX 6
#define MT3333_VELOCITY_FIELD_IDX 7
#define MT3333_DIRECTION_FIELD_IDX 8
#define MT3333_DATE_FIELD_IDX 9

#define MT3333_UART_BAUDRATE_DEFAULT 9600
#define MT3333_EOL '\n'

/**
 * @brief Input ring buffer size in bytes
 */
#define MT3333_RXBUF_SIZE_BYTES (256)

/**
 * @brief Reader&Parser thread stack size in bytes
 */
#define MT3333_READER_THREAD_STACK_SIZE_BYTES (2560)

typedef struct {
	int lat;            /**< Latitude, degrees * 1E6 */
	int lon;            /**< Logitude, degrees * 1E6 */
    int velocity;       /**< Velocity, mm/s */
    int direction;      /**< Direction, degrees * 1E3, relative to north */
	time_t time;        /**< Epoch */
    bool valid;         /**< Data validity */
} mt3333_gps_data_t;

/**
 * @biref Structure that holds the MT3333 driver parameters
 */
typedef struct {
	uart_t uart;	                        /**< The device descriptor on which the MT3333 module is attached */
    int baudrate;                           /**< UART baudrate, 9600 bps is the default value */
	void (*gps_cb)(mt3333_gps_data_t data);	/**< Callback which called when module give us a valid GPS NMEA GMRC message */
} mt3333_param_t;

/**
 * @biref Structure that holds the MT3333 driver internal state and parameters
 */
typedef struct {
	mt3333_param_t params;					/**< Holds driver parameters */
	char *reader_stack;	                    /**< Reader thread stack, has to be allocated by the application */
} mt3333_t;

typedef enum {
    MT3333_POWERSAVE_STANDBY,
    MT3333_POWERSAVE_BACKUP,
    MT3333_POWERSAVE_FULLON,
} mt3333_powersave_mode_t;

/**
 * @brief MT3333 driver initialization routine
 * @note Initializes the UART device specified in parameters
 * @note Initialies input ring buffer with size mt3333_RXBUF_SIZE_BYTES
 *
 * @param[out] dev device structure pointer
 * @param[in] param MT3333 driver parameters, data will be copied into device parameters
 *
 * @return 0 if initialization succeeded
 * @return <0 in case of error
 */
int mt3333_init(mt3333_t *dev, mt3333_param_t *param);

/**
 * @brief Change MT3333 baudrate
 */
void mt3333_set_baudrate(mt3333_t *dev, int baudrate);

/**
 * @brief Set MT3333 to low-power GLP mode (support depends on core firmware version)
 */
void mt3333_set_glp(mt3333_t *dev, bool enabled);

/**
 * @brief Set MT3333 to the specified power mode
 */
void mt3333_set_powersave(mt3333_t *dev, mt3333_powersave_mode_t mode);

/**
 * @brief Set MT3333 to periodic mode with specified parameters
 */
void mt3333_set_periodic(mt3333_t *dev, mt3333_powersave_mode_t mode, int run, int sleep, int run_ext, int sleep_ext);

#endif /* mt3333_H_ */

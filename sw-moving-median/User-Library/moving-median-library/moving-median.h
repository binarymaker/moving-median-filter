
/**
  ******************************************************************************
  * ______  _                             ___  ___        _               
  * | ___ \(_)                            |  \/  |       | |              
  * | |_/ / _  _ __    __ _  _ __  _   _  | .  . |  __ _ | | __ ___  _ __ 
  * | ___ \| || '_ \  / _` || '__|| | | | | |\/| | / _` || |/ // _ \| '__|
  * | |_/ /| || | | || (_| || |   | |_| | | |  | || (_| ||   <|  __/| |   
  * \____/ |_||_| |_| \__,_||_|    \__, | \_|  |_/ \__,_||_|\_\\___||_|   
  *                                 __/ |                                 
  *                                |___/                                  
  *                                                                       
  * Copyright (C) 2019 Binary Maker - All Rights Reserved
  *
  * This program and the accompanying materials are made available
  * under the terms described in the LICENSE file which accompanies
  * this distribution.
  * Written by Binary Maker <https://github.com/binarymaker>
  ******************************************************************************
  */

#ifndef BM_ba0db310_806f_11e9_a3ac_f914ac843fcc
#define BM_ba0db310_806f_11e9_a3ac_f914ac843fcc

/**
 * @file moving-median.h
 * @author binary maker <https://github.com/binarymaker>
 * @brief median filter based on FIFO buffer
 * @date 2019-05-27
 *
 */

/* 
                           input
                             +
                             |
                          +--+--+
                          | S/H | <-----------------+ sampling time
                          +--+--+
                             |
                             v
          +------------------+--------------------+
index +-->+                 DEMUX                 |
          +----+------+------+----------------+---+
               |      |      |                |
            +--+-+ +--+-+ +--+-+           +--+-+
            | B0 | | B1 | | B3 | . . . . . | Bn |   <+ buffer
            +-+--+ +-+--+ +-+--+           +-+--+      n is size+1
              |      |      |                |
          +---+------+------+----------------+----+
          |                 SORT                  |
          +----+------+------+----------------+---+
               |      |      |                |
            +--+-+ +--+-+ +--+-+           +--+-+
            | S0 | | S1 | | S2 | . . . . . | Sn |    <+ sorted list
            +-+--+ +-+--+ +-+--+           +-+--+
              |      |      |                |
              +------+------++---------------+
                             |
                    +--------+--------+
                    |  Fill index / 2 |               <+ list
                    +--------+--------+                  med value
                             |
                             v
                          filtered
 */

#include "stdint.h"

typedef struct movingMedian_s
{
  int16_t *buffer;      /**< Data buffer pointer */
  uint16_t size;        /**< Size of filter buffer */
  uint16_t index;       /**< Current location in buffer */
  uint16_t fill;        /**< Buffer filled level */
  int16_t filtered;     /**< Filtered output */
  uint16_t sample_time; /**< data sampling time interval */
  uint32_t last_time;   /**< last sampled time */
} movingMedian_t;

/**
 * @brief filter object initialization
 * 
 * @param context [in] instance of filter object
 * @param filter_size [in] size of filter buffer 
 *                         Odd size acceptable, Even size internally convert to odd
 * @param sample_time [in] filter sampling time in ms 
 */
void moving_median_create(movingMedian_t *context, uint16_t filter_size, uint16_t sample_time);

/**
 * @brief filter process function
 * 
 * @param context [in] instance of filter object
 * @param input [in] data sample to filter
 */
void moving_median_filter(movingMedian_t *context, int16_t input);

#endif // BM_ba0db310_806f_11e9_a3ac_f914ac843fcc

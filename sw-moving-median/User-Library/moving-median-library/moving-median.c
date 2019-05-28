
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

#include "moving-median.h"
#include "moving-median-cfg.h"
#include <stdlib.h>

#include "stm32f0xx_hal.h"

void
swap(int16_t * a, int16_t* b)
{
  int16_t temp;

  temp = *a;
  *a = *b;
  *b = temp;
}

void
moving_median_create(movingMedian_t *context,
                     uint16_t filter_size,
                     uint16_t sample_time)
{
  free(context->buffer);

  if (filter_size % 2 == 0)
  {
    context->size = filter_size - 1;
  }
  else
  {
    context->size = filter_size;
  }

  context->buffer = (int16_t*)malloc(filter_size * sizeof(int16_t));
  context->index = 0;
  context->fill = 0;
  context->filtered = 0;
  context->sample_time = sample_time;
  context->last_time = 0;
}

void
moving_median_filter(movingMedian_t *context,
                     int16_t filter_input)
{
  if ((TICK_TIMER - context->last_time) > context->sample_time)
  {
    context->last_time = TICK_TIMER;

    /* buffer filling */
    if (context->fill < context->size)
    {
      context->fill++;
    }

    int16_t *sort_buffer = (int16_t*)malloc(context->fill * sizeof(uint16_t));

    /* Replace old data by new */
    context->buffer[context->index] = filter_input;

    /* Copy buffer to temp sort buffer */
    for (uint16_t i = 0; i < context->fill; i++)
    {
      sort_buffer[i] = context->buffer[i];
    }

    /* Sort temporary buffer */
    for (uint16_t i = 0; i < context->fill; i++)
    {
      for (uint16_t j = i + 1; j < context->fill; j++)
      {
        if (sort_buffer[j] < sort_buffer[i])
        {
          swap(&sort_buffer[j], &sort_buffer[i]);
        }
      }
    }

    /* Increment buffer index */
    context->index++;
    if (context->index >= context->size)
    {
      context->index = 0;
    }

    /* sorted buffer med value */
    context->filtered = sort_buffer[(uint16_t)(context->fill / 2)];

    /* Delete temporary sort buffer */
    free(sort_buffer);
  }
}




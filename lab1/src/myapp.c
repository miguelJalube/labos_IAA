/**
 * ,---------,       ____  _ __
 * |  ,-^-,  |      / __ )(_) /_______________ _____  ___
 * | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * | / ,--´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *    +------`   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2019 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * hello_world.c - App layer application of a simple hello world debug print every
 *   2 seconds.
 */


#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"
#include "commander.h"

#define DEBUG_MODULE "HELLOWORLD"

static void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate) {
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawrate;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = vx;
  setpoint->velocity.y = vy;
  setpoint->velocity_body = true;
}

// States
typedef enum
{
  idle,
  hover,
  forward,
  backward,
  right,
  left,
  clockwise,
  unclockwise
} State;

#define ALTITUDE 0.5
#define VELOCITY 0.1

/*void appMain() {
  setpoint_t s;
  float current_altitude = 0.0;
  float current_xvelocity = 0.0;
  float current_yvelocity = 0.0;
  int run = 1;

  State current_state = idle;

  // Wait for the initialisation to complete
  vTaskDelay(M2T(5000));
  while(run){
    if(current_state == idle){
      current_altitude = ALTITUDE;
      setVelocitySetpoint(&s, current_xvelocity, current_yvelocity, current_altitude, 0);
      current_state = hover;
    commanderSetSetpoint(&s, 3);
    vTaskDelay(M2T(5000));
    }else if(current_state == hover){
      current_xvelocity = VELOCITY;
      setVelocitySetpoint(&s, current_xvelocity, current_yvelocity, current_altitude, 0);
      current_state = forward;
    commanderSetSetpoint(&s, 3);
    vTaskDelay(M2T(5000));
    }else if(current_state == forward){
      current_xvelocity = -VELOCITY;
      setVelocitySetpoint(&s, current_xvelocity, current_yvelocity, current_altitude, 0);
      current_state = backward;
    commanderSetSetpoint(&s, 3);
    vTaskDelay(M2T(5000));
    }else if (current_state == backward){
      current_xvelocity = 0.0;
      current_altitude = 0.0;
      setVelocitySetpoint(&s, current_xvelocity, current_yvelocity, current_altitude, 0);
      current_state = idle;
      run = 0;
    commanderSetSetpoint(&s, 3);
    vTaskDelay(M2T(5000));
    }
  }
}*/


void appMain() {
  setpoint_t s;
  setVelocitySetpoint(&s, 0, 0, 0.5, 0);
  // wait for the initialization to complete
  vTaskDelay(M2T(5000));
  while (1) {
    commanderSetSetpoint(&s, 3);
    vTaskDelay(M2T(10));
  }
}
/**
 * \par 
 * \class AurigaMotor
 * \brief   Driver for Auriga SLOT 1|2
 * @file    AurigaMotor.h
 * @author  Nicolas Bourré
 * @version V1.0.0
 * @date    2022/10/03
 * @brief   Header for AurigaMotor.cpp
 *
 * \par Description
 * This file is a drive for Auriga Motor only without the encoder
 *
 * \par Method List:
 *
 *    1. void MeDCMotor::setpin(uint8_t ena, uint8_t enb, uint8_t pwm_pin)
 *    2. void MeDCMotor::run(int16_t speed)
 *    3. void MeDCMotor::stop(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Nicolas Bourré   2022/10/03     1.0.0            Created a new lib
 * </pre>
 *
 */
 
#ifndef _AURIGA_MOTOR_H_
#define _AURIGA_MOTOR_H_

#include "Arduino.h"

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

/**
 * Class: MeDCMotor
 * \par Description
 * Declaration of Class MeDCMotor
 */

class AurigaMotor
{
public:
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  AurigaMotor(void);

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  AurigaMotor(uint8_t port);

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   dir_pin - arduino port for direction pin(should analog pin)
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
  AurigaMotor(uint8_t ena_pin, uint8_t enb_pin, uint8_t pwm_pin);
  
/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the DC motor available PIN by its arduino port.
 * \param[in]
 *   dir_pin - arduino port for direction pin(should analog pin)
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setpin(uint8_t ena_pin, uint8_t enb_pin, uint8_t pwm_pin);


/**
 * \par Function
 *   run
 * \par Description
 *   Control the motor forward or reverse
 * \param[in]
 *   speed - Speed value from -255 to 255
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void run(int16_t speed);

/**
 * \par Function
 *   stop
 * \par Description
 *   Stop the rotation of the motor
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void stop(void);
private:
  volatile uint8_t ena;
  volatile uint8_t enb;
  volatile uint8_t pwm;
  int16_t  last_speed;
};

#endif
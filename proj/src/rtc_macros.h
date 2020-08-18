#pragma once

/** @defgroup rtc_macros rtc_macros
 * @{
 *
 * Constants used to manipulate the RTC.
 */

#include "i8042.h"

#define RTC_ADDR_REG    0x70  /**< @brief RTC address register */
#define RTC_DATA_REG    0x71  /**< @brief RTC data register */

#define RTC_IRQ     8         /**< @brief RTC IRQ line */

//Time of the day, alarm and date registers

//>address location
#define SECONDS_ADDRESS         0x00  /**< @brief seconds address */
#define SECONDS_ALARM_ADDRESS   0x01  /**< @brief seconds alarm address */
#define MINUTES_ADDRESS         0x02  /**< @brief minutes address */
#define MINUTES_ALARM_ADDRESS   0x03  /**< @brief minutes alarm address */
#define HOURS_ADDRESS           0x04  /**< @brief hours address */
#define HOURS_ALARM_ADDRESS     0x05  /**< @brief hours alarm address */
#define WEEKDAY_ADDRESS         0x06  /**< @brief week-day address */
#define DAY_ADDRESS             0x07  /**< @brief day address */
#define MONTH_ADDRESS           0x08  /**< @brief month address */
#define YEAR_ADDRESS            0x09  /**< @brief year address */

//>registers
#define REGISTER_A_ADDRESS   0x0A     /**< @brief register A address */
#define REGISTER_B_ADDRESS   0x0B     /**< @brief register B address  */
#define REGISTER_C_ADDRESS   0x0C     /**< @brief register C address  */
#define REGISTER_D_ADDRESS   0x0D     /**< @brief register D address  */

//flags and masks
//Reg A
#define UIP     BIT(7)        /**< UIP bit */

#define VALID   0x00          /**<  Indicates validity */

//Reg B
#define SET           BIT(7)  /**< Set bit */
#define PIE           BIT(6)  /**< PIE bit */
#define AIE           BIT(5)  /**< AIE bit */
#define UIE           BIT(4)  /**< UIE bit */
#define SQWE          BIT(3)  /**< SQWE bit */
#define DM            BIT(2)  /**< DM bit */
#define HOUR_MODE     BIT(1)  /**< Hour Mode bit */
#define DSE           BIT(0)  /**< DSE bit */

//Reg C
#define IRQF          BIT(7)  /**< IRQF bit */
#define PF            BIT(6)  /**< PF bit */
#define AF            BIT(5)  /**< AF bit */
#define UF            BIT(4)  /**< UF bit */

//Reg D
#define VRT           BIT(7)  /**< VRT bit */

//OTHER
#define UPDATE     3    /**< Indicates an update */

#define ALARM_RING      2   /**< Indicates an alarm */

#define UNIMPORTANT     1   /**< Indicates unimportant event */

#define MS_4BITS    0xF0      /**< Selects the 4 most significant bits of a 8 bit value */
#define LS_4BITS    0x0F      /**< Selects the 4 most significant bits of a 8 bit value */

#define SECONDS_IN_HOUR       3600  /**< Amount of seconds in an hour */
#define SECONDS_IN_MINUTE     60    /**< Amount of seconds in a minute */

/** @} */

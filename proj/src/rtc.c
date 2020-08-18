#include "rtc.h"

int global_rtc_id;

void convert_BCD_decimal(uint8_t *value)
{
  uint8_t value_msb = (*value) & MS_4BITS;

  value_msb = (value_msb >> 4);

  uint8_t value_lsb = (*value) & LS_4BITS;

  *value = 10*value_msb + value_lsb;
}

void convert_decimal_BCD(uint8_t *value)
{
  uint8_t value_dozens = (*value / 10);
  uint8_t value_units = (*value);

  value_dozens = value_dozens % 10;
  value_units = value_units % 10;

  value_dozens = (value_dozens << 4);

  *value = value_dozens + value_units;
}

int rtc_subscribe_interrupt(uint8_t rtc_id)
{
  global_rtc_id = rtc_id;
  uint8_t policy = IRQ_REENABLE;

  if(sys_irqsetpolicy(RTC_IRQ, policy, &global_rtc_id) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int rtc_unsubscribe_interrupt()
{
	if(sys_irqrmpolicy(&global_rtc_id) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int rtc_writeInfo(uint8_t address, uint8_t value)
{
  if(sys_outb(RTC_ADDR_REG, address) != SUCCESS)
  {
    return FAIL;
  }

  if(sys_outb(RTC_DATA_REG, value) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int rtc_readInfo(uint8_t address, uint8_t * value)
{
  if(sys_outb(RTC_ADDR_REG, address) != SUCCESS)
  {
    return FAIL;
  }

  uint32_t value_32;

  if(sys_inb(RTC_DATA_REG, &value_32) != SUCCESS)
  {
    return FAIL;
  }

  *value = (uint8_t) value_32;

  return SUCCESS;
}

int enable_alarm()
{
  uint8_t status;

  if(rtc_readInfo(REGISTER_B_ADDRESS, &status) != SUCCESS)
  {
    return FAIL;
  }
  
  status = (status | AIE);

  if(rtc_writeInfo(REGISTER_B_ADDRESS, status) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int disable_alarm()
{
  uint8_t status;

  if(rtc_readInfo(REGISTER_B_ADDRESS, &status) != SUCCESS)
  {
    return FAIL;
  }
  
  status = (status & (~AIE));

  if(rtc_writeInfo(REGISTER_B_ADDRESS, status) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int disable_periodic()
{
  uint8_t status;

  if(rtc_readInfo(REGISTER_B_ADDRESS, &status) != SUCCESS)
  {
    return FAIL;
  }
  
  status = (status & (~PIE));

  if(rtc_writeInfo(REGISTER_B_ADDRESS, status) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int enable_update()
{
  uint8_t status;

  if(rtc_readInfo(REGISTER_B_ADDRESS, &status) != SUCCESS)
  {
    return FAIL;
  }
  
  status = (status | UIE);

  if(rtc_writeInfo(REGISTER_B_ADDRESS, status) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int disable_update()
{
  uint8_t status;

  if(rtc_readInfo(REGISTER_B_ADDRESS, &status) != SUCCESS)
  {
    return FAIL;
  }
  
  status = (status & (~UIE));

  if(rtc_writeInfo(REGISTER_B_ADDRESS, status) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

int rtc_ih()
{
  uint8_t cause;

  if(rtc_readInfo(REGISTER_C_ADDRESS, &cause) != SUCCESS)
  {
    return FAIL;
  }

  if((cause & AF) == AF)
  {
    return ALARM_RING;
  }
  else if((cause & UF) == UF)
  {
    return UPDATE;
  }

  return UNIMPORTANT;
}

int get_UIPflag()
{
  uint32_t status;

  if(sys_outb(RTC_ADDR_REG, REGISTER_A_ADDRESS) != SUCCESS)
  {
    return FAIL;
  }
  
  if(sys_inb(RTC_DATA_REG, &status) != SUCCESS)
  {
    return FAIL;
  }

  return ((uint8_t) status) & UIP;
}

int get_date(uint8_t *day, uint8_t *month, uint8_t *year)
{
  while(get_UIPflag() != VALID)
  {
    continue;
  }

  if(rtc_readInfo(DAY_ADDRESS, day)!= SUCCESS)
  {
    return FAIL;
  }

  if(rtc_readInfo(MONTH_ADDRESS, month)!= SUCCESS)
  {
    return FAIL;
  }

  if(rtc_readInfo(YEAR_ADDRESS, year)!= SUCCESS)
  {
    return FAIL;
  }

  convert_BCD_decimal(day);
  convert_BCD_decimal(month);
  convert_BCD_decimal(year);

  return SUCCESS;
}

unsigned get_seconds()
{
  uint8_t seconds;
  rtc_readInfo(SECONDS_ADDRESS, &seconds);
  convert_BCD_decimal(&seconds);
  return seconds;
}

int get_time(uint8_t *second, uint8_t *minute, uint8_t *hour)
{
  while(get_UIPflag() != VALID)
  {
    continue;
  }

  if(rtc_readInfo(SECONDS_ADDRESS, second)!= SUCCESS)
  {
    return FAIL;
  }

  if(rtc_readInfo(MINUTES_ADDRESS, minute)!= SUCCESS)
  {
    return FAIL;
  }

  if(rtc_readInfo(HOURS_ADDRESS, hour)!= SUCCESS)
  {
    return FAIL;
  }

  convert_BCD_decimal(second);
  convert_BCD_decimal(minute);
  convert_BCD_decimal(hour);

  return SUCCESS;
}

int set_alarm(uint8_t time)
{
  uint8_t second, minute, hour;

  get_time(&second, &minute, &hour);

  uint32_t time_in_seconds = ((hour*SECONDS_IN_HOUR) + (minute*SECONDS_IN_MINUTE) + second ) + time;

  hour = (time_in_seconds / SECONDS_IN_HOUR) % 24;
  
  time_in_seconds = time_in_seconds - hour*SECONDS_IN_HOUR;

  minute = (time_in_seconds / SECONDS_IN_MINUTE) % 60;

  time_in_seconds = time_in_seconds - minute*SECONDS_IN_MINUTE;

  second = time_in_seconds % 60;

  convert_decimal_BCD(&second);
  convert_decimal_BCD(&minute);
  convert_decimal_BCD(&hour);

  if(rtc_writeInfo(SECONDS_ALARM_ADDRESS, second) != SUCCESS)
  {
    return FAIL;
  }

  if(rtc_writeInfo(MINUTES_ALARM_ADDRESS, minute) != SUCCESS)
  {
    return FAIL;
  }

  if(rtc_writeInfo(HOURS_ALARM_ADDRESS, hour) != SUCCESS)
  {
    return FAIL;
  }

  return SUCCESS;
}

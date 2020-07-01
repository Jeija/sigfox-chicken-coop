#ifndef _INA219_H
#define _INA219_H

typedef enum {
	INA219_FSR_16V = 0,
	INA219_FSR_32V = 1
} ina219_fsr_t;

typedef enum {
	INA219_PGA_40MV = 0,
	INA219_PGA_80MV = 1,
	INA219_PGA_160MV = 2,
	INA219_PGA_320MV = 3
} ina219_pga_t;

typedef enum {
	INA219_ADC_9BIT    = 0,  // conversion time 84us.
	INA219_ADC_10BIT   = 1,  // conversion time 148us.
	INA219_ADC_11BIT   = 2,  // conversion time 276us.
	INA219_ADC_12BIT   = 3,  // conversion time 532us.
	INA219_ADC_2SAMP   = 9,  // conversion time 1.06ms
	INA219_ADC_4SAMP   = 10, // conversion time 2.13ms
	INA219_ADC_8SAMP   = 11, // conversion time 4.26ms
	INA219_ADC_16SAMP  = 12, // conversion time 8.51ms
	INA219_ADC_32SAMP  = 13, // conversion time 17.02ms
	INA219_ADC_64SAMP  = 14, // conversion time 34.05ms
	INA219_ADC_128SAMP = 15, // conversion time 68.10ms
} ina219_adc_t;

typedef enum {
	INA219_MODE_PWR_DOWN    = 0,
	INA219_MODE_TRIG_SH     = 1,
	INA219_MODE_TRIG_BUS    = 2,
	INA219_MODE_TRIG_SH_BUS = 3,
	INA219_MODE_ADC_OFF     = 4,
	INA219_MODE_CONT_SH     = 5,
	INA219_MODE_CONT_BUS    = 6,
	INA219_MODE_CONT_SH_BUS = 7
} ina219_mode_t;

esp_err_t ina219_init(ina219_mode_t mode, ina219_fsr_t fsr, ina219_pga_t pga, ina219_adc_t busadc, ina219_adc_t shuntadc);
esp_err_t ina219_bus_voltage(float *voltage);
esp_err_t ina219_shunt_voltage(float *voltage);
esp_err_t ina219_powerdown(void);

void ina219_stall_reset(void);
esp_err_t ina219_stall_detect(bool *stall);

#endif

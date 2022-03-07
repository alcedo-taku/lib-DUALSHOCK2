/**
 * @file DUALSHOCK2.hpp
 * @brief
 * @date Mar 6, 2022
 */
#ifndef DUALSHOCK2_HPP_
#define DUALSHOCK2_HPP_

#include "spi.h"
#include <array>

namespace CMD {
	constexpr std::array<uint8_t,  5> READ_DATA 			= {0x01, 0x42, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t,  9> READ_DATA_EX	    	= {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t, 21> READ_DATA_EXEX		= {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t,  5> CONFIG_MODE_ENTER		= {0x01, 0x43, 0x00, 0x01, 0x00};
	constexpr std::array<uint8_t,  9> CONFIG_MODE_EXIT		= {0x01, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t,  9> SET_MODE_AND_LOCK		= {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00}; // アナログ、ロック
	constexpr std::array<uint8_t,  9> PRESSURE_ENABLE		= {0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t,  9> PRES_TRANS_START		= {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
	constexpr std::array<uint8_t,  9> QUERY_MODEL_AND_MODE	= {0x01, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	constexpr std::array<uint8_t,  9> QUERY_MODEL_AND_MODE2	= {0x01, 0x45, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
	constexpr std::array<uint8_t,  9> VIBRATION_ENABLE		= {0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
};

class DUALSHOCK2 {
private:
	SPI_HandleTypeDef* hspi;
	std::array<uint8_t, 21> receive_data;
public:
	DUALSHOCK2(SPI_HandleTypeDef& hspi);
	void init(uint32_t timeout);
	void update(uint32_t timeout);
	std::array<uint8_t,  9> get_data_ex();
	std::array<uint8_t, 21> get_data_exex();

	/**
	 *
	 * @tparam T
	 * @param command
	 * @param timeout
	 */
	template <class T> void send_command(T command, uint32_t timeout){
		HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_RESET);
		for (uint8_t i = 0; i < command.size(); i++) {
			HAL_SPI_TransmitReceive(hspi,(uint8_t*)&command[i], (uint8_t*)&receive_data[i], sizeof(command[i]), timeout);
		}
		HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
	}
};

#define IS_ANALOG(x)       	(((x)[1] == 0x73)
#define IS_DIGITAL(x)      	(((x)[1] == 0x41)
#define IS_MODE_CHANGED(x)	(((x)[2] == 0x5A)

#define ANALOG_RIGHT_X_STATE(x) ((x)[ 5])
#define ANALOG_RIGHT_Y_STATE(x) ((x)[ 6])
#define ANALOG_LEFT_X_STATE(x)  ((x)[ 7])
#define ANALOG_LEFT_Y_STATE(x)  ((x)[ 8])
#define ANALOG_RIGHT(x)			((x)[ 9])
#define ANALOG_LEFT(x)			((x)[10])
#define ANALOG_UP(x)			((x)[11])
#define ANALOG_DOWN(x)			((x)[12])
#define ANALOG_TRIANGLE(x)		((x)[13])
#define ANALOG_CIRCLE(x)		((x)[14])
#define ANALOG_CROSS(x)			((x)[15])
#define ANALOG_SQUARE(x)		((x)[16])
#define ANALOG_L1(x)			((x)[17])
#define ANALOG_R1(x)			((x)[18])
#define ANALOG_L2(x)			((x)[19])
#define ANALOG_R2(x)			((x)[20])

#define BTN_LEFT(x)        	(!((x)[3] & (1<<7)))
#define BTN_DOWN(x)        	(!((x)[3] & (1<<6)))
#define BTN_RIGHT(x)       	(!((x)[3] & (1<<5)))
#define BTN_UP(x)          	(!((x)[3] & (1<<4)))
#define BTN_START(x)       	(!((x)[3] & (1<<3)))
#define BTN_STICK_RIGHT(x) 	(!((x)[3] & (1<<2)))
#define BTN_STICK_LEFT(x)  	(!((x)[3] & (1<<1)))
#define BTN_SELECT(x)      	(!((x)[3] & 1))

#define BTN_SQUARE(x)      	(!((x)[4] & (1<<7)))
#define BTN_CROSS(x)       	(!((x)[4] & (1<<6)))
#define BTN_CIRCLE(x)		(!((x)[4] & (1<<5)))
#define BTN_TRIANGLE(x)   	(!((x)[4] & (1<<4)))
#define BTN_R1(x)          	(!((x)[4] & (1<<3)))
#define BTN_L1(x)          	(!((x)[4] & (1<<2)))
#define BTN_R2(x)          	(!((x)[4] & (1<<1)))
#define BTN_L2(x)          	(!((x)[4] & 1))

#endif /* DUALSHOCK2_HPP_ */

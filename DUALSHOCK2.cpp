/**
 * @file DUALSHOCK2.cpp
 * @brief
 * @date Mar 6, 2022
 */
#include <DUALSHOCK2.hpp>

/**
 * コンストラクタ
 * @param hspi 使うSPIのハンドル
 */
DUALSHOCK2::DUALSHOCK2(
	SPI_HandleTypeDef& hspi
):
	hspi(&hspi)
{
}

void DUALSHOCK2::init(uint32_t timeout){
	HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

	send_command(CMD::READ_DATA_EX, timeout);
	send_command(CMD::READ_DATA_EX, timeout);
	send_command(CMD::CONFIG_MODE_ENTER, timeout);

	send_command(CMD::QUERY_MODEL_AND_MODE2, timeout);
	send_command(CMD::SET_MODE_AND_LOCK, timeout);
	send_command(CMD::SET_MODE_AND_LOCK, timeout);

	send_command(CMD::CONFIG_MODE_EXIT, timeout);

	send_command(CMD::CONFIG_MODE_ENTER, timeout);
	send_command(CMD::PRESSURE_ENABLE, timeout);
	send_command(CMD::PRESSURE_ENABLE, timeout);
	send_command(CMD::PRES_TRANS_START, timeout);

	send_command(CMD::READ_DATA_EX, timeout);
}

/**
 *
 * @param timeout SPI通信のtimeout
 */
void DUALSHOCK2::update(uint32_t timeout){
	HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_RESET);
//	for (uint8_t i = 0; i < CMD::READ_DATA_EX.size(); i++) {
//		HAL_SPI_TransmitReceive(hspi,(uint8_t*)&CMD::READ_DATA_EX[i], (uint8_t*)&receive_data_ex[i], sizeof(CMD::READ_DATA_EX[i]), timeout);
//	}
	for (uint8_t i = 0; i < CMD::READ_DATA_EXEX.size(); i++) {
		HAL_SPI_TransmitReceive(hspi,(uint8_t*)&CMD::READ_DATA_EXEX[i], (uint8_t*)&receive_data[i], sizeof(CMD::READ_DATA_EXEX[i]), timeout);
	}
	HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_SET);
}


/**
 *
 * @return
 */
std::array<uint8_t, 9> DUALSHOCK2::get_data_ex(){
	std::array<uint8_t, 9> tmp;
	std::copy(receive_data.begin(), receive_data.begin()+sizeof(tmp), tmp.begin());
	return tmp;
}

/**
 *
 * @return
 */
std::array<uint8_t,21> DUALSHOCK2::get_data_exex(){
	return receive_data;
}

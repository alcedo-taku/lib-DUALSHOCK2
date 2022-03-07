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

/**
 * 初期化関数
 * @param timeout SPI通信のtimeout
 */
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
 * コントローラのデータを更新する関数
 * @param timeout SPI通信のtimeout
 */
void DUALSHOCK2::update(uint32_t timeout){
	send_command(CMD::READ_DATA_EXEX, timeout);
}


/**
 * コントローラのデータを取得する関数
 * @return コントローラのデータ
 */
std::array<uint8_t, 9> DUALSHOCK2::get_data_ex(){
	std::array<uint8_t, 9> tmp;
	std::copy(receive_data.begin(), receive_data.begin()+sizeof(tmp), tmp.begin());
	return tmp;
}

/**
 * コントローラのデータを取得する関数
 * @return コントローラのデータ
 */
std::array<uint8_t,21> DUALSHOCK2::get_data_exex(){
	return receive_data;
}

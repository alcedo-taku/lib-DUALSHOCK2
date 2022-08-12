/**
 * @file DUALSHOCK2.cpp
 * @brief
 * @date Mar 6, 2022
 */
#include <DUALSHOCK2.hpp>

/**
 * コンストラクタ
 * @param hspi 使用するSPIのハンドル
 * @param ss_port SSピンとして設定したGPIOのPort
 * @param ss_pin SSピンとして設定したGPIOのPin
 */
DUALSHOCK2::DUALSHOCK2(
	SPI_HandleTypeDef& hspi,
	GPIO_TypeDef *ss_port,
	uint16_t ss_pin,
	uint32_t timeout
):
	hspi(&hspi),
	ss_port(ss_port),
	ss_pin(ss_pin),
	timeout(timeout)
{
}

/**
 * 初期化関数
 * @param timeout SPI通信のtimeout
 */
void DUALSHOCK2::init(){
	HAL_GPIO_WritePin(ss_port, ss_pin, GPIO_PIN_SET);
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
}

void DUALSHOCK2::reset_stick(){
	send_command(CMD::READ_DATA_EX, timeout);
	std::copy(receive_data.begin()+5, receive_data.begin()+9, stick_offset.begin());
}

/**
 * コントローラのデータを更新する関数
 * @param timeout SPI通信のtimeout
 * @param stk_0_range スティックの0の範囲を定義
 */
void DUALSHOCK2::update(uint8_t stk_0_range){
	// receive_data を更新
	send_command(CMD::READ_DATA_EXEX, timeout);
	if(receive_data[1] == 65){ // 通信が失敗していたら
		init();
		send_command(CMD::READ_DATA_EXEX, timeout);
	}

	// スティックの値を更新
	stick_data[0] =   (receive_data[5] - stick_offset[0] );
	stick_data[1] = - (receive_data[6] - stick_offset[1] );
	stick_data[2] =   (receive_data[7] - stick_offset[2] );
	stick_data[3] = - (receive_data[8] - stick_offset[3] );
	// 0の範囲内ならスティックの値を0にする
	for(uint8_t i=0; i < 4; i++){
		if ( stk_0_range < stick_data[i] ){
			stick_data[i] -= stk_0_range;
		}else if ( stick_data[i] < -stk_0_range ){
			stick_data[i] += stk_0_range;
		}else {
			stick_data[i] = 0;
		}
	}
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

/**
 * コントローラのスティックの値を取得する関数
 * @return コントローラのスティックの値
 */
std::array<int16_t, 4> DUALSHOCK2::get_stick_data(){
	return stick_data;
}

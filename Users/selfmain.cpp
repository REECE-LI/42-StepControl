#include "selfmain.h"
#include "mt6816_stm32.h"
#include "tb67h450_stm32.h"
#include "button_stm32.h"
#include "configurations.h"
#include "can_base.h"
#include "encoder_calibrator_stm32.h"
#include "stockpile_f303cc.h"

#include "oled_task.h"

BoardConfig_t boardConfig = {.canNodeId = 35};

Motor motor;
TB67H450 tb67H450;
MT6816 mt6816;
EncoderCalibrator encoderCalibrator(&motor);
Button button1(1, 500), button2(2, 500);

void OnButton1Event(Button::Event _event);

void OnButton2Event(Button::Event _event);

void SelfMain() {

    Stockpile_Flash_Data_Read_Data(&stockpile_data, &boardConfig, sizeof(BoardConfig_t));

    if (boardConfig.configStatus != CONFIG_OK) // use default settings
    {
        boardConfig = BoardConfig_t{
                .configStatus = CONFIG_OK,
                .canNodeId = 1,
                .encoderHomeOffset = 0,
                .defaultMode = Motor::MODE_COMMAND_VELOCITY,
                .currentLimit = 3 * 1000,    // A
                .velocityLimit = 1600 * motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS, // r/s
                .velocityAcc = 100 * motor.MOTOR_ONE_CIRCLE_SUBDIVIDE_STEPS,   // r/s^2
                .calibrationCurrent=1500,
                .dce_kp = 200,
                .dce_kv = 80,
                .dce_ki = 300,
                .dce_kd = 250,
                .enableMotorOnBoot=false,
                .enableStallProtect=false
        };

        Stockpile_Flash_Data_QWrite_Data16(&stockpile_data, (uint16_t *) &boardConfig, sizeof(BoardConfig_t));
    }
    // 加载板载数据
    motor.config.motionParams.encoderHomeOffset = boardConfig.encoderHomeOffset;
    motor.config.motionParams.ratedCurrent = boardConfig.currentLimit;
    motor.config.motionParams.ratedVelocity = boardConfig.velocityLimit;
    motor.config.motionParams.ratedVelocityAcc = boardConfig.velocityAcc;
    motor.motionPlanner.velocityTracker.SetVelocityAcc(boardConfig.velocityAcc);
    motor.motionPlanner.positionTracker.SetVelocityAcc(boardConfig.velocityAcc);
    motor.config.motionParams.caliCurrent = boardConfig.calibrationCurrent;
    motor.config.ctrlParams.dce.kp = boardConfig.dce_kp;
    motor.config.ctrlParams.dce.kv = boardConfig.dce_kv;
    motor.config.ctrlParams.dce.ki = boardConfig.dce_ki;
    motor.config.ctrlParams.dce.kd = boardConfig.dce_kd;
    motor.config.ctrlParams.stallProtectSwitch = boardConfig.enableStallProtect;

    /*---------------- Init Motor ----------------*/
    motor.AttachDriver(&tb67H450);
    motor.AttachEncoder(&mt6816);
    motor.controller->Init();
    motor.driver->Init();
    motor.encoder->Init();

    /*------------- Init peripherals -------------*/
    button1.SetOnEventListener(OnButton1Event);
    button2.SetOnEventListener(OnButton2Event);


    /*------- Start Close-Loop Control Tick ------*/
    CANInit(&hcan);

    HAL_TIM_Base_Start_IT(&htim4);  // 100Hz
    HAL_TIM_Base_Start_IT(&htim3);  // 20kHz


    if (button1.IsPressed() && button2.IsPressed())
        encoderCalibrator.isTriggered = true;

    OledTaskInit();
    for (;;) {

        encoderCalibrator.TickMainLoop();
        if (boardConfig.configStatus == CONFIG_COMMIT) {
            boardConfig.configStatus = CONFIG_OK;
            Stockpile_Flash_Data_QWrite_Data16(&stockpile_data, (uint16_t *) &boardConfig, sizeof(BoardConfig_t));
        } else if (boardConfig.configStatus == CONFIG_RESTORE) {
            Stockpile_Flash_Data_QWrite_Data16(&stockpile_data, (uint16_t *) &boardConfig, sizeof(BoardConfig_t));
            HAL_NVIC_SystemReset();
        }

    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {

        if (encoderCalibrator.isTriggered)
            encoderCalibrator.Tick20kHz();
        else
            motor.Tick20kHz();
    } else if (htim->Instance == TIM4) {
        button1.Tick(20);
        button2.Tick(20);
    }
}


// button event
void OnButton1Event(Button::Event _event) {
    switch (_event) {
        case ButtonBase::UP:
            break;
        case ButtonBase::DOWN:
            break;
        case ButtonBase::LONG_PRESS:
            break;
        case ButtonBase::CLICK:
            break;
    }

}

void OnButton2Event(Button::Event _event) {
    switch (_event) {
        case ButtonBase::UP:
            break;
        case ButtonBase::DOWN:
            break;
        case ButtonBase::LONG_PRESS:
            break;
        case ButtonBase::CLICK:
            break;
    }
}
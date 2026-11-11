/* Bench-port candidate only: GPIOs are NOT validated against PAZI/UA4C wiring.
 * Do not connect ignition/injector/boost/VVT outputs to a real engine.
 */
/**
 * @file board_configuration.cpp
 * PAZI Mega Legacy 512KB bench port (STM32F407VET6)
 * Arduino Mega socket form factor
 * Reference: https://github.com/rusefi/fw-custom-mega100-f4
 *
 * UA4C reference pinout only (INJ: B15/D8/D9/D10, IGN: E15/E4/D13/E5).
 * Runtime pinout switching disabled to reduce flash usage.
 * No SD card, no Lua (EFI_LUA=FALSE in board.mk). USB enabled for TunerStudio.
 */

#include "pch.h"
#include "board_overrides.h"

Gpio getCommsLedPin() { return config->communityCommsLedPin; }
Gpio getRunningLedPin() { return Gpio::Unassigned; }
Gpio getWarningLedPin() { return Gpio::Unassigned; }

static void customBoardDefaultConfiguration() {
    config->communityCommsLedPin = Gpio::Unassigned; // BENCH: LED GPIO unverified

    // Trigger GPIO PD3 remains a reference from Mazduino; unverified on PAZI.
    // Disable it until continuity testing. CAM also not assigned.
    engineConfiguration->triggerInputPins[0] = Gpio::Unassigned; // BENCH
    engineConfiguration->camInputs[0] = Gpio::Unassigned;

    // Analog sensors - ADC1
    // PA3 (EFI_ADC_3) reserved for knock via ADC3
    engineConfiguration->map.sensor.hwChannel  = EFI_ADC_0;  // PA0
    engineConfiguration->afr.hwChannel         = EFI_ADC_1;  // PA1
    engineConfiguration->vbattAdcChannel        = EFI_ADC_2;  // PA2
    engineConfiguration->tps1_1AdcChannel       = EFI_ADC_6;  // PA6
    engineConfiguration->clt.adcChannel         = EFI_ADC_4;  // PA4
    engineConfiguration->iat.adcChannel         = EFI_ADC_5;  // PA5

    engineConfiguration->analogInputDividerCoefficient = (470.0f + 910.0f) / 910.0f; // 470R high side/910R low side
    engineConfiguration->vbattDividerCoeff = (82.0f + 10.0f) / 10.0f; // 82k high side/10k low side
    engineConfiguration->adcVcc = 3.3f;
    engineConfiguration->clt.config.bias_resistor = 2700;
    engineConfiguration->iat.config.bias_resistor = 2700;

    // Injectors - UA4C reference pinout only; not continuity-tested on PAZI
    engineConfiguration->injectionPins[0] = Gpio::B15;
    engineConfiguration->injectionPins[1] = Gpio::D8;
    engineConfiguration->injectionPins[2] = Gpio::D9;
    engineConfiguration->injectionPins[3] = Gpio::D10;

    // Ignition - UA4C reference pinout only; verify output stages and polarity
    engineConfiguration->ignitionPins[0] = Gpio::E15;
    engineConfiguration->ignitionPins[1] = Gpio::E4;
    engineConfiguration->ignitionPins[2] = Gpio::D13;
    engineConfiguration->ignitionPins[3] = Gpio::E5;

    // Aux outputs
    engineConfiguration->fuelPumpPin               = Gpio::Unassigned; // BENCH: no unverified pump output
    engineConfiguration->mainRelayPin               = Gpio::Unassigned; // BENCH
    engineConfiguration->idle.solenoidPin           = Gpio::Unassigned; // CRITICAL: D9 is now injector 3
    engineConfiguration->fanPin                     = Gpio::Unassigned; // BENCH
    engineConfiguration->tachOutputPin              = Gpio::Unassigned; // BENCH
    engineConfiguration->acSwitch                   = Gpio::Unassigned; // BENCH
    engineConfiguration->acRelayPin                 = Gpio::Unassigned; // BENCH
    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned; // BENCH
    engineConfiguration->clutchDownPin              = Gpio::Unassigned; // BENCH
    engineConfiguration->clutchDownPinMode          = PI_INVERTED_PULLUP;

    // CAN
    engineConfiguration->canTxPin = Gpio::Unassigned; // BENCH
    engineConfiguration->canRxPin = Gpio::Unassigned; // BENCH

    // Secondary TS channel (Bluetooth) on USART3. rusEFI defaults these to PC10/PC11, which this board
    // uses for the SD card on SPI3. Wiring: PB10 -> HC-05 RXD, PB11 <- HC-05 TXD.
    engineConfiguration->binarySerialTxPin = Gpio::Unassigned; // BENCH
    engineConfiguration->binarySerialRxPin = Gpio::Unassigned; // BENCH

    // No SD card (EFI_FILE_LOGGING=FALSE, USE_FATFS=no in board.mk)
}

void setup_custom_board_overrides() {
    custom_board_DefaultConfiguration = customBoardDefaultConfiguration;
}

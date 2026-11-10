# ArpTuner PAZI STM32F407VET6 512KB — UA4C GPIO reference build (BENCH ONLY)

This source profile is based on the documented **pazi88/speeduino** `STM32_MEGA` branch,
`speeduino/init.cpp`, `case 41` / `STM32F407xx`. It is NOT electrically verified
against the user's exact PAZI PCB revision, installed UA4C driver board, or tune.

## Source-derived GPIO map

| Function | STM32 pin | Action in source |
|---|---|---|
| CKP primary | PD3 | Assigned |
| CMP primary | PD4 | Assigned |
| CAM 2 | PB6 | Reference only, not assigned |
| Injector 1 / 2 / 3 / 4 | PB15 / PD8 / PD9 / PD10 | Assigned |
| Ignition 1 / 2 / 3 / 4 | PE15 / PE4 / PD13 / PE5 | Assigned |
| Fuel pump relay drive | PC8 | Assigned |
| Fan relay drive | PA15 | Assigned |
| Boost PWM | PA8 | Assigned |
| VVT1 PWM | PB13 | Assigned |
| VVT2 PWM | PE6 | Assigned, verify hardware |
| Idle1 | PB14 | Assigned |
| Idle2 | PB12 | Reference, not assigned |
| Tach output | PC9 | Assigned |
| Launch input | PE14 | Reference, not assigned; input polarity/pullup unknown |
| FLEX / VSS | PD7 | Shared reference: do not allocate both! |
| AUX1 | PC7 | Reference only, not assigned |
| AUX3 | PC6 | Reference only, not assigned |
| MAP analog | PA0 | Assigned |
| TPS analog | PA3 | Assigned (correct Pazi mapping, not PA6) |
| WBO2 1 / WBO2 2 | PA1 / PB1 | First channel only assigned |
| VBATT analog | PA2 | Assigned; divider not verified |
| CLT / IAT | PA4 / PA5 | Assigned; bias not verified |
| Baro / extra analog | PA7 / PB0 | Reference only |

## CRITICAL restrictions

- This is **not an engine-ready firmware** even if compile passes.
- DO NOT connect a running engine, pump, injectors, coil, boost or VVT loads to this bench firmware.
- Output polarity, ignition driver interface, VR conditioners, harness wiring and trigger phasing have not been verified.
- Hardware variant native USB/FTDI DIP SWITCHES: firmware enables native USB, whereas
  user's known-working Speeduino binary is named `InternalFlash_FTDI_UA4C.bin`.
  Pazi hardware README: for native USB, **USB ON / FTDI OFF**; for FTDI adapter **USB OFF / FTDI ON**.
  Verify on a separate board, and do not confuse DFU BOOT0 with USB CDC runtime.
- Some source assignments are **default config only**. Existing saved rusEFI tune can override them.
- `analogInputDividerCoefficient`, `vbattDividerCoeff`, thermistor pullups and sensor calibrations
  are reference placeholders inherited from Mazduino, not measured PAZI values.
- 512KB flash is divided in firmware build into 384KB code and 128KB tune.
  Check compiled .bin < 393216 bytes and confirm runtime tune sector erase/write behavior independently.
- Built with no DBW/Lua/SD. BOOST/VVT feature code is present but closed-loop calibration and solenoid
  safety are untested. AUX VTEC/MIVEC and launch not yet mapped in runtime output configuration.

## Test sequence before future engine use

1. Save original engine-running Speeduino .bin and .msq and keep the functional ECU untouched.
2. Read STM32 marking and PAZI PCB revision; inspect UA4C driver output stages.
3. Confirm continuity from GPIO to harness output for every listed channel.
4. Current-limited 12V bench supply with NO injectors, ignition coils, fuel pump or solenoids connected.
5. Check USB enumeration and TunerStudio with matching build .ini.
6. Feed CKP/CAM through appropriate VR/Hall conditioning, check RPM/sync with simulator.
7. Check ignition, injector, pump, fan, boost and VVT outputs with scope and dummy loads,
   verify polarity and fail-safe behavior (especially on reset/lost sync).
8. Calibrate MAP/TPS/CLT/IAT/VBATT/wideband against known inputs; check tune save/restore.
9. After these checks, independently verify trigger angle and ignition timing with timing light
   before considering a controlled engine test.

Sources:
- https://github.com/pazi88/speeduino/blob/STM32_MEGA/speeduino/init.cpp
- https://github.com/pazi88/STM32_mega
- https://github.com/mazduino/mazduino-fw/blob/master/boards/mazduino-mega100-512/board.mk

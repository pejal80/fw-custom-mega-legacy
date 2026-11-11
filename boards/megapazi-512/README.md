# PAZI Mega Legacy 512KB — experimental bench build

This is a **separate build target** for the STM32F407VET6 (512KB), ported
from the `pejal80/mazduino-fw` board `mazduino-mega100-512` after its
successful GitHub Actions compile. The original legacy board files are untouched.

## NOT READY FOR A VEHICLE

- Output pin assignments in `board_configuration.cpp` are the **reference
  board_04 mappings**, **not confirmed PAZI/UA4C harness mapping**.
- Four injector and four ignition outputs are configured, but must not be
  connected to loads before electrical verification and polarity checks.
- Only CKP input is assigned. The CAM input is **not configured**.
- Boost Control and VVT PID feature flags are TRUE, but actual
  boost solenoid, VVT PWM and VTEC/MIVEC switching outputs are **not assigned**.
- No DBW, SD, Lua. This test port does not use external SST26 NOR storage.
- Firmware code is linked for a nominal 384KB region, leaving a nominal
  128KB internal flash region for tune data. Independently verify the
  flash layout/linker map, write/erase behavior and firmware size.

## Verification needed before flash

1. Back up the existing working Speeduino firmware/tune (if read protection allows).
2. Verify MCU identity STM32F407VET6 and bootloader/USB wiring.
3. Map every PAZI GPIO to the UA4C carrier and verify all hardware driver stages.
4. Check output polarity, crank/cam conditioning (max 3.3V at MCU) and
   all power supplies using a current-limited bench setup.
5. Confirm tune storage does not erase executable code.
6. Build, check .bin and .map, bench test with a simulator; only then
   consider a controlled engine test.

This build demonstrates source compatibility, **not ECU compatibility**.

# nodemcu-rc
Remote Control using NodeMCU WiFi

# Hardware hints

### digital input

When using switches from `GND` to the input, use also 10k..100k resistors from 3.3V to the input. (see https://en.wikipedia.org/wiki/Pull-up_resistor)

### digital output

When connecting LEDs, use at least 470 Ohm resistors in series to not damage the chip.
Charlieplexing is possible with low-current LEDs (~12mA).

### analog input

0V will result in a `0`, 3.3V will result in a `4096`. Higher voltage will kill the chip.

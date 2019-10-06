# nodemcu-rc
Remote Control using NodeMCU WiFi

# Hardware hints

### digital input

Safe to use are
- GPIO16 (D0)
- GPIO5 (D1)
- GPIO4 (D2)
- GPIO14 (D5)
- GPIO12 (D6)
- GPIO13 (D7)
- GPIO0 (D3) and GPIO2 (D4) are safe **if HIGH during boot**, use pull-up resistors.
- GPIO15 (D8) is safe **if LOW during boot**, use a pull-down resistor.
- Also available are GPIO1 (U0TXD) and GPIO3 (U0RXD) **if not using serial**.

In general, [pull resistors](https://en.wikipedia.org/wiki/Pull-up_resistor) shoult be between 10kΩ..100kΩ.

### digital output

When connecting LEDs, use at least 470Ω resistors in series to not damage the chip.
Charlieplexing is possible with low-current LEDs (~12mA).

### analog input

There is only one, A0.
0V will result in a `0`, 3.3V will result in a `4096`. Higher voltage will kill the chip.

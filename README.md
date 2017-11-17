# esp-ulp-adc-bug

This is a minimal working example for a bug which occurs when taking continuous ADC measurements from a ULP program. The ADC measurements give the maximum possible readings during wake up of the main chip, i.e. with 12-bit measurements the results will be 2^12-1=4095.

Make sure to connect PIN 32 to GND, so the good measurements will be around zero.

## Typical output

```
I (99) main: initial boot
I (99) main: resetting counters
I (99) main: starting ULP program
I (99) main: entering deep sleep

I (99) main: samples taken 204
I (99) main: first bad sample at 201
I (99) main: number of bad samples 2
I (99) main: resetting counters
I (99) main: entering deep sleep
```

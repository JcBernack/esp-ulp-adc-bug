// ### .data section - constants ###
.data
    // set ADC channel 4
    .set adc_channel, 4+1

// ### .bss section - zero-initialized variables ###
.bss
    .global sample_count
sample_count:
    .long 0

    .global first_bad_sample
first_bad_sample:
    .long 0

    .global bad_sample_count
bad_sample_count:
    .long 0

// ### .text secton - program code ###
.text
    .global entry
entry:

    // increment sample counter
    move r1, sample_count
    ld r0, r1, 0
    add r0, r0, 1
    st r0, r1, 0

    // take measurement
    adc r0, 0, adc_channel
    jumpr count_good_samples, 4095, LT

    // increment bad sample counter
    move r1, bad_sample_count
    ld r0, r1, 0
    add r0, r0, 1
    st r0, r1, 0

count_good_samples:
    // increment first_bad_sample counter if bad_sample_counter == 0
    move r1, bad_sample_count
    ld r0, r1, 0
    jumpr exit, 1, GE
    move r1, first_bad_sample
    ld r0, r1, 0
    add r0, r0, 1
    st r0, r1, 0

exit:
    halt

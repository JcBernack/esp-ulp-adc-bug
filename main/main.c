#include <esp_log.h>
#include <esp_sleep.h>
#include <esp32/ulp.h>
#include <driver/adc.h>
#include <ulp_main.h>

#define MEASUREMENT_INTERVAL (10 * 1000)
#define WAKEUP_INTERVAL (5000 * 1000)

static const char* const TAG = "main";

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");

void app_main() {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    if (cause == ESP_SLEEP_WAKEUP_UNDEFINED) {
        ESP_LOGI(TAG, "initial boot");
        // load the ULP program
        ESP_ERROR_CHECK(ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t)));
        ESP_ERROR_CHECK(ulp_set_wakeup_period(0, MEASUREMENT_INTERVAL));
        // configure the ADC channel
        ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
        ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));
        // move control of the ADC to the ULP
        adc1_ulp_enable();
        // start the ULP program
        ESP_LOGI(TAG, "starting ULP program");
        ESP_ERROR_CHECK(ulp_run((uint32_t)(&ulp_entry - RTC_SLOW_MEM) / sizeof(uint32_t)));
    } else {
        ESP_LOGI(TAG, "samples taken %d", ulp_sample_count & UINT16_MAX);
        ESP_LOGI(TAG, "first bad sample at %d", ulp_first_bad_sample & UINT16_MAX);
        ESP_LOGI(TAG, "number of bad samples %d", ulp_bad_sample_count & UINT16_MAX);
    }
    ESP_LOGI(TAG, "resetting counters");
    ulp_sample_count = 0;
    ulp_first_bad_sample = 0;
    ulp_bad_sample_count = 0;
    ESP_LOGI(TAG, "entering deep sleep\n");
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(WAKEUP_INTERVAL));
    esp_deep_sleep_start();
}

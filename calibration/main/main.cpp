/* The example of ESP-IDF
 *
 * I used this as a reference.
 * https://zenn.dev/aeronext_dev/articles/ba4a4108c9f2ba
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"

// bno055 stuff
#include "bno055.h"
#include "bno055_driver.h"

static const char *TAG = "MAIN";

extern "C" {
	void app_main(void);
}

void app_main()
{
	// Initialize NVS
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK( err );

	// Initialize i2c
	struct bno055_t bno055;
	interface_init(&bno055);

	// Initialize imu
	BNO055_RETURN_FUNCTION_TYPE ret;
	ret = bno055_init(&bno055);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_init failed. ret=%d", ret);
		return;
	}

	// set the power mode as NORMAL
	u8 power_mode = BNO055_POWER_MODE_NORMAL;
	ret = bno055_set_power_mode(power_mode);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_set_power_mode failed. ret=%d", ret);
		return;
	}

	// set the operation mode as NDOF
	//ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG);
	ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_set_operation_mode failed. ret=%d", ret);
		return;
	}

	// Wait until the calibration status is complete
	while(1) {
		u8 accel_calib;
		ret = bno055_get_accel_calib_stat(&accel_calib);
		if (ret != BNO055_SUCCESS) {
			ESP_LOGE(TAG, "bno055_get_accel_calib_stat failed. ret=%d", ret);
			return;
		}
		//printf("bno055_get_accel_calib_stat=%d %d\n", ret, accel_calib);

		u8 gyro_calib;
		ret = bno055_get_gyro_calib_stat(&gyro_calib);
		if (ret != BNO055_SUCCESS) {
			ESP_LOGE(TAG, "bno055_get_gyro_calib_stat failed. ret=%d", ret);
			return;
		}
		//printf("bno055_get_gyro_calib_stat=%d %d\n", ret, gyro_calib);

		u8 mag_calib;
		ret = bno055_get_mag_calib_stat(&mag_calib);
		if (ret != BNO055_SUCCESS) {
			ESP_LOGE(TAG, "bno055_get_mag_calib_stat failed. ret=%d", ret);
			return;
		}
		//printf("bno055_get_mag_calib_stat=%d %d\n", ret, mag_calib);

		u8 sys_calib;
		ret = bno055_get_sys_calib_stat(&sys_calib);
		if (ret != BNO055_SUCCESS) {
			ESP_LOGE(TAG, "bno055_get_sys_calib_stat failed. ret=%d", ret);
			return;
		}
		//printf("bno055_get_sys_calib_stat=%d %d\n", ret, sys_calib);

		printf("bno055_get_calib_stat=%d %d %d %d\n", accel_calib, gyro_calib, mag_calib, sys_calib);

		if (accel_calib == 3 && gyro_calib == 3 && mag_calib == 3 && sys_calib == 3) break;
		//if (accel_calib == 3 && gyro_calib == 3 && mag_calib == 3) break;
		vTaskDelay(10);
	}

#if 0
	// Save current operation mode
	u8 prev_opmode_u8;
	ret = bno055_get_operation_mode(&prev_opmode_u8);
#endif

	// Switch to CONFIG mode
	ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_set_operation_mode failed. ret=%d", ret);
		return;
	}
	vTaskDelay(10);

	// Read the current offset register
	struct bno055_accel_offset_t accel_offset;
	ret = bno055_read_accel_offset(&accel_offset);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_read_accel_offset failed. ret=%d", ret);
		return;
	}
	printf("accel_offset x=%d y=%d z=%d r=%d\n", accel_offset.x, accel_offset.y, accel_offset.z, accel_offset.r);

	struct bno055_gyro_offset_t gyro_offset;
	ret = bno055_read_gyro_offset(&gyro_offset);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_read_gyro_offset failed. ret=%d", ret);
		return;
	}
	printf("gyro_offset x=%d y=%d z=%d\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);

	struct bno055_mag_offset_t mag_offset;
	ret = bno055_read_mag_offset(&mag_offset);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_read_mag_offset failed. ret=%d", ret);
		return;
	}
	printf("mag_offset x=%d y=%d z=%d r=%d\n", mag_offset.x, mag_offset.y, mag_offset.z, mag_offset.r);

#if 0
	// Return to the current operating mode
	bno055_set_operation_mode(prev_opmode_u8);
#endif

#if 0
accel_offset x=-7 y=-11 z=-47 r=1000
gyro_offset x=-1 y=0 z=0
mag_offset x=74 y=40 z=-166 r=1049
#endif

	// Open NVS
	nvs_handle_t my_handle;
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_open error (%s)", esp_err_to_name(err));
		return;
	}

	// Write NVS
	err = nvs_set_i16(my_handle, "accel_offset_x", accel_offset.x);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 accel_offset_x set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "accel_offset_y", accel_offset.y);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 accel_offset_y set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "accel_offset_z", accel_offset.z);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 accel_offset_z set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "accel_offset_r", accel_offset.r);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 accel_offset_r set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "gyro_offset_x", gyro_offset.x);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 gyro_offset_x set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "gyro_offset_y", gyro_offset.y);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 gyro_offset_y set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "gyro_offset_z", gyro_offset.z);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 gyro_offset_z set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "mag_offset_x", mag_offset.x);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 mag_offset_x set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "mag_offset_y", mag_offset.y);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 mag_offset_y set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "mag_offset_z", mag_offset.z);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 mag_offset_z set failed");
		return;
	}

	err = nvs_set_i16(my_handle, "mag_offset_r", mag_offset.r);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_set_i16 mag_offset_r set failed");
		return;
	}

	// Commit written value.
	// After setting any values, nvs_commit() must be called to ensure changes are written
	// to flash storage. Implementations may write to storage at other times,
	// but this is not guaranteed.
	err = nvs_commit(my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_commit failed");
		return;
	}

	// Close NVS
	nvs_close(my_handle);
	ESP_LOGI(TAG, "The offset value has been written to NVS");
}

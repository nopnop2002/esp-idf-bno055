/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/message_buffer.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "cJSON.h"

#include "parameter.h"

extern QueueHandle_t xQueueTrans;
extern MessageBufferHandle_t xMessageBufferToClient;

static const char *TAG = "IMU";

// bno055 stuff
#include "bno055.h"
#include "bno055_driver.h"

esp_err_t bno055_set_offset()
{
	struct bno055_accel_offset_t accel_offset;
	struct bno055_gyro_offset_t gyro_offset;
	struct bno055_mag_offset_t mag_offset;
	BNO055_RETURN_FUNCTION_TYPE ret;

	// Open NVS
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "nvs_open error (%s)", esp_err_to_name(err));
		return err;
	}

	// Read NVS
	int16_t offset;
	err = nvs_get_i16(my_handle, "accel_offset_x", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "accel_offset_x get failed");
		nvs_close(my_handle);
		return err;
	}
	accel_offset.x = offset;

	err = nvs_get_i16(my_handle, "accel_offset_y", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "accel_offset_y get failed");
		nvs_close(my_handle);
		return err;
	}
	accel_offset.y = offset;

	err = nvs_get_i16(my_handle, "accel_offset_z", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "accel_offset_z get failed");
		nvs_close(my_handle);
		return err;
	}
	accel_offset.z = offset;

	err = nvs_get_i16(my_handle, "accel_offset_r", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "accel_offset_r get failed");
		nvs_close(my_handle);
		return err;
	}
	accel_offset.r = offset;
	printf("accel_offset x=%d y=%d z=%d r=%d\n", accel_offset.x, accel_offset.y, accel_offset.z, accel_offset.r);

	err = nvs_get_i16(my_handle, "gyro_offset_x", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "gyro_offset_x get failed");
		nvs_close(my_handle);
		return err;
	}
	gyro_offset.x = offset;

	err = nvs_get_i16(my_handle, "gyro_offset_y", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "gyro_offset_y get failed");
		nvs_close(my_handle);
		return err;
	}
	gyro_offset.y = offset;

	err = nvs_get_i16(my_handle, "gyro_offset_z", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "gyro_offset_z get failed");
		nvs_close(my_handle);
		return err;
	}
	gyro_offset.z = offset;
	printf("gyro_offset x=%d y=%d z=%d\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);

	err = nvs_get_i16(my_handle, "mag_offset_x", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "mag_offset_x get failed");
		nvs_close(my_handle);
		return err;
	}
	mag_offset.x = offset;

	err = nvs_get_i16(my_handle, "mag_offset_y", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "mag_offset_y get failed");
		nvs_close(my_handle);
		return err;
	}
	mag_offset.y = offset;

	err = nvs_get_i16(my_handle, "mag_offset_z", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "mag_offset_z get failed");
		nvs_close(my_handle);
		return err;
	}
	mag_offset.z = offset;

	err = nvs_get_i16(my_handle, "mag_offset_r", &offset);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "mag_offset_r get failed");
		nvs_close(my_handle);
		return err;
	}
	mag_offset.r = offset;
	printf("mag_offset x=%d y=%d z=%d r=%d\n", mag_offset.x, mag_offset.y, mag_offset.z, mag_offset.r);

	// Close NVS
	nvs_close(my_handle);

	ret = bno055_write_accel_offset(&accel_offset);
	ESP_LOGI(TAG, "bno055_write_accel_offset ret=%d", ret);
	if (ret != BNO055_SUCCESS) return ESP_FAIL;

	ret = bno055_write_gyro_offset(&gyro_offset);
	ESP_LOGI(TAG, "bno055_write_gyro_offset ret=%d", ret);
	if (ret != BNO055_SUCCESS) return ESP_FAIL;

	ret = bno055_write_mag_offset(&mag_offset);
	ESP_LOGI(TAG, "bno055_write_mag_offset ret=%d", ret);
	if (ret != BNO055_SUCCESS) return ESP_FAIL;
	return ESP_OK;
}

void bno055(void *pvParameters){
	// Initialize i2c
	struct bno055_t bno055;
	interface_init(&bno055);

	// Initialize imu
	BNO055_RETURN_FUNCTION_TYPE ret;
	ret = bno055_init(&bno055);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_init failed. ret=%d", ret);
		vTaskDelete(NULL);
	}

	// set the power mode as NORMAL
	u8 power_mode = BNO055_POWER_MODE_NORMAL;
	ret = bno055_set_power_mode(power_mode);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_set_power_mode failed. ret=%d", ret);
		vTaskDelete(NULL);
	}

	// set the operation mode as NDOF
	ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
	if (ret != BNO055_SUCCESS) {
		ESP_LOGE(TAG, "bno055_set_operation_mode failed. ret=%d", ret);
		vTaskDelete(NULL);
	}

	// set euler unit
	u8 euler_unit;
	bno055_set_euler_unit(BNO055_EULER_UNIT_DEG);
	ret = bno055_get_euler_unit(&euler_unit);
	ESP_LOGI(TAG, "euler_unit=%d", euler_unit);

	// set offset
	esp_err_t err = bno055_set_offset();
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "bno055_set_offset failed.");
		vTaskDelete(NULL);
	}

	struct bno055_euler_t reg_euler;
	struct bno055_euler_float_t euler_hpr;
	while(1){
		// reads Euler data hrp values from register
		ret = bno055_read_euler_hrp(&reg_euler);
		ESP_LOGD(TAG, "euler heading=%d roll=%d pitch=%d", reg_euler.h, reg_euler.r, reg_euler.p);
		// convert the Euler hrp raw data to degree output as float
		ret = bno055_convert_float_euler_hpr_deg(&euler_hpr);
		ESP_LOGI(TAG, "heading=%f roll=%f pitch=%f", euler_hpr.h, euler_hpr.r, euler_hpr.p);

		float _roll = euler_hpr.r;
		float _pitch = euler_hpr.p;
		float _yaw = euler_hpr.h;

		// Send UDP packet
		POSE_t pose;
		pose.roll = _roll;
		pose.pitch = _pitch;
		pose.yaw = _yaw;
		if (xQueueSend(xQueueTrans, &pose, 100) != pdPASS ) {
			ESP_LOGE(TAG, "xQueueSend fail");
		}

		// Send WEB request
		cJSON *request;
		request = cJSON_CreateObject();
		cJSON_AddStringToObject(request, "id", "data-request");
		cJSON_AddNumberToObject(request, "roll", _roll);
		cJSON_AddNumberToObject(request, "pitch", _pitch);
		cJSON_AddNumberToObject(request, "yaw", _yaw);
		char *my_json_string = cJSON_Print(request);
		ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
		size_t xBytesSent = xMessageBufferSend(xMessageBufferToClient, my_json_string, strlen(my_json_string), 100);
		if (xBytesSent != strlen(my_json_string)) {
			ESP_LOGE(TAG, "xMessageBufferSend fail");
		}
		cJSON_Delete(request);
		cJSON_free(my_json_string);
		vTaskDelay(100/portTICK_PERIOD_MS);
	}

	// Never reach here
	vTaskDelete(NULL);
}

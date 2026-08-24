idf_component_register(
	SRCS "bno055.c" "bno055_driver.c"
	PRIV_REQUIRES driver esp_driver_i2c esp_timer
	INCLUDE_DIRS "."
)

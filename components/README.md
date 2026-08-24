# bno055   
I created it based on [this](https://github.com/boschsensortec/BNO055_SensorAPI).   

# websocket   
I used [this](https://github.com/Molorius/esp32-websocket) ESP-IDF WebSocket Component.   
The original uses mbedtls_sha1(), but I changed to mbedtls_md().   
This is because mbedtls_sha1() was deprecated in ESP-IDF V6.   

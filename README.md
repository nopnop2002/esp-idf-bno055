# esp-idf-bno055
A demo showing the pose of the BNO055 9DoF IMU sensor in 3D using esp-idf.

The BNO055 provides a variety of output signals, which can be chosen by selecting the appropriate operation mode.   
<table>
	<tbody>
		<tr>
			<td colspan="2" rowspan="2" align="center">Operating Mode</td>
			<td colspan="3">Available sensor signals</td>
			<td colspan="2" align="center">Fusion Data</td>
		</tr>
		<tr>
			<td align="center">Accel</td>
			<td align="center">Mag</td>
			<td align="center">Gyro</td>
			<td>Relative orientation</td>
			<td>Absolute orientation</td>
		</tr>
		<tr>
			<td rowspan=8">Non Fusion Mode</td>
			<td>CONFIGMODE</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>ACCONLY</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>MAGONLY</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>GYROONLY</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>ACCMAG</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>ACCGYRO</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>MAGGYRO</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>AMG</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td rowspan=5">Fusion Mode</td>
			<td>IMU</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>COMPASS</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">-</td>
			<td align="center">X</td>
		</tr>
		<tr>
			<td>M4G</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">X</td>
			<td align="center">-</td>
		</tr>
		<tr>
			<td>NDOF_FMC_OFF</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">X</td>
		</tr>
		<tr>
			<td>NDOF</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">X</td>
			<td align="center">-</td>
			<td align="center">X</td>
		</tr>
	</tbody>
</table>

In Fusion NDOF mode, in addition to accelerometer, magnetometer, and gyroscope data, Euler angles can be output.   
Euler angles are roll, pitch and yaw.   
It's very intuitive and easy to understand.   
![a-Pitch-yaw-and-roll-angles-of-an-aircraft-with-body-orientation-O-u-v-original](https://user-images.githubusercontent.com/6020549/224452743-d4cf419d-f936-4e46-9ece-a12f21bf2e32.jpg)   
You can view like this.   
![Image](https://github.com/user-attachments/assets/cb9f1408-b372-4e55-84f6-1191f068cd1a)

# Installation overview

- Perform a calibration of your sensor.   
	In Fusion NDOF mode, calibration data for the accelerometer, gyroscope, and magnetic sensor can be automatically acquired.   
	The sensor calibration values are written to NVS.   

- Get Euler angles from IMU.   
	The sensor calibration values are read from NVS.   

- Display Euler angles in browser.

# Software requirements
ESP-IDF V5.2 or later.   
Because this project uses the new I2C driver.   

# Hardware requirements
BNO055 Intelligent 9-axis absolute orientation sensor.   
This sensor features both UART and I2C interfaces.   
To use the I2C interface, it is necessary to short-circuit certain parts of the circuitry.   

<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/7be2a602-1666-4534-ae6e-e04daf99fc38" />
<img width="864" height="576" alt="Image" src="https://github.com/user-attachments/assets/aa7f1358-8a04-4806-aa0c-8ab877d60d86" />

# Wireing
|BNO055||ESP32|ESP32-S2/S3|ESP32-Cx||
|:-:|:-:|:-:|:-:|:-:|:-:|
|VCC|--|3.3V|3.3V|3.3V||
|GND|--|GND|GND|GND||
|SCL/Rx|--|GPIO22|GPIO12|GPIO5|(*1)|
|SDA/Tx|--|GPIO21|GPIO11|GPIO4|(*1)|
|ADO(*3)|--|GND/3.3V|GND/3.3V|GND/3.3V|(*2)|

(*1)You can change it to any pin using menuconfig.   

(*2)Choosing an i2c address.   
GND:i2c address is 0x28.   
3.3V:i2c address is 0x29.   

(*3)It may be marked as I2C or COM3.   

# Find the sensor
We can find the sensor using [i2c-tools](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/i2c/i2c_tools).   

- Select SCL and SDA using menuconfig.   
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/48f0c1c3-616d-4399-95bc-3940935ee7ea" />

- Detect senser.   
	The i2c address for this sensor is 0x28 or 0x29.   
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/5e4f9fc1-80e7-44a3-8d98-a141bb6f3e7d" />

- Read register.   
	Register #0 for this sensor is 0xA0.   
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/00742055-e23c-4013-b349-38c60044dc19" />


# Perform a calibration of your sensor
```
git clone https://github.com/nopnop2002/esp-idf-bno055
cd calibration
idf.py menuconfig
idf.py flash
```

### Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/15780618-4af4-45a5-834d-ead13937bf12" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/f51e0e5a-2e4a-4174-9893-f76efac4499d" />

### Accelerometer Calibration
- Place the device in 6 different stable positions for a period of few seconds to allow the accelerometer to calibrate.   
- Make sure that there is slow movement between 2 stable positions.   
- The 6 stable positions could be in any direction, but make sure that the device is lying at least once perpendicular to the x, y and z axis.   
- The register CALIB_STAT can be read to see the calibration status of the accelerometer.   

### Gyroscope Calibration   
- Place the device in a single stable position for a period of few seconds to allow the gyroscope to calibrate.   
- The register CALIB_STAT can be read to see the calibration status of the gyroscope.   

### Magnetometer Calibration   
- Make some random movements (for example: writing the number '8' on air) until the CALIB_STAT register indicates fully calibrated.   
- The register CALIB_STAT can be read to see the calibration status of the magnetometer.   

### About Calib Status   
From the left: Accelerometer Calib status, Gyroscope Calib status, Magnetometer Calib status, System Calib status.   
For Calib Status, 3 indicates completion.   
"0 3 0 0" indicates that only the gyroscope calibration is complete.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/d55f0544-4f48-4bbe-8294-3693c33b3317" />

"3 3 3 3" indicates that all calibrations have been completed.   
Once all calibration is complete, the offset value is displayed.   
The offset value is written to NVS.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/3216a025-af47-48d1-b2da-6617c52a8e63" />


# Get Euler angles from IMU
```
cd euler
idf.py menuconfig
idf.py flash
```

### Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/5d55054a-a927-4df6-a23f-4d9da72c9208" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/a4c2b4dd-d7de-4411-be5d-92c14f5c86ff" />


# View Euler angles with built-in web server   
ESP32 acts as a web server.   
I used [this](https://github.com/Molorius/esp32-websocket) component.   
This component can communicate directly with the browser.   
Enter the following in the address bar of your web browser.   
```
http:://{IP of ESP32}/
or
http://esp32.local/
```

![browser-roll-pitch-yaw](https://user-images.githubusercontent.com/6020549/232365926-ccc6198b-42ec-44f7-891d-6caa93c3411c.JPG)

WEB pages are stored in the html folder.   
I used [this](https://threejs.org/) for 3D display.   
I used [this](https://canvas-gauges.com/) for gauge display.   
Configuration Options for the gauge display is [here](https://canvas-gauges.com/documentation/user-guide/configuration).   
You can change the design and color according to your preference like this.   
![Image](https://github.com/user-attachments/assets/d0e1ca46-0d46-41ed-bbbc-9f26af28900d)


# View Euler angles using PyTeapot   
You can view Euler angles using [this](https://github.com/thecountoftuscany/PyTeapot-Quaternion-Euler-cube-rotation) tool.   
It works as a UDP display server.   
This is a great application.   

```
+-------------+          +-------------+          +-------------+
|             |          |             |          |             |
|     IMU     |--(i2c)-->|    ESP32    |--(UDP)-->| pyteapot.py |
|             |          |             |          |             |
+-------------+          +-------------+          +-------------+
```

### Installation for Linux
```
$ python3 --version
Python 3.11.2
$ sudo apt install python3-pip python3-setuptools
$ python3 -m pip install -U pip
$ python3 -m pip install pygame
$ python3 -m pip install PyOpenGL PyOpenGL_accelerate
$ git clone https://github.com/thecountoftuscany/PyTeapot-Quaternion-Euler-cube-rotation
$ cd PyTeapot-Quaternion-Euler-cube-rotation
$ python3 pyteapot.py
```

The posture of your sensor is displayed.   
![pyteapot_2023-03-11_09-11-46](https://user-images.githubusercontent.com/6020549/224452173-2350704d-1fc4-4a12-8324-434c11f62c52.png)

### Installation for Windows   
Install Git for Windows from [here](https://gitforwindows.org/).   
Install Python Releases for Windows from [here](https://www.python.org/downloads/windows/).   
Open Git Bash and run:   
```
$ python --version
Python 3.11.9
$ python -m pip install -U pip
$ python -m pip install pygame
$ python -m pip install PyOpenGL PyOpenGL_accelerate
$ git clone https://github.com/thecountoftuscany/PyTeapot-Quaternion-Euler-cube-rotation
$ cd PyTeapot-Quaternion-Euler-cube-rotation
$ python pyteapot.py
```

The posture of your sensor is displayed.   
![PyTeapot-Windows](https://github.com/user-attachments/assets/2b0a1a70-40cb-47e5-8f51-eb4fe3adb1ab)


### String passed over Wifi to pyteapot.py
Yaw angle should be betweem two ```y```.   
Pitch angle should be between two ```p```.   
Roll angles should be between two ```r```.   
```
# yaw = 168.8099
# pitch = 12.7914
# roll = -11.8401
# Euler angles only
y168.8099yp12.7914pr-11.8401r

```

# View Euler angles using panda3d library   
You can view Euler angles using [this](https://www.panda3d.org/) library.   
It works as a UDP display server.   

```
+-------------+          +-------------+          +-------------+
|             |          |             |          |             |
|     IMU     |--(ic2)-->|    ESP32    |--(UDP)-->|  panda.py   |
|             |          |             |          |             |
+-------------+          +-------------+          +-------------+
```

### Installation for Linux
```
$ python3 --version
Python 3.11.2
$ sudo apt install python3-pip python3-setuptools
$ python3 -m pip install -U pip
$ python3 -m pip install panda3d
$ cd esp-idf-mpu6050-dmp/panda3d
$ python3 panda.py --help
usage: panda.py [-h] [--model {jet,biplain,707,fa18}]

options:
  -h, --help            show this help message and exit
  --model {jet,biplain,707,fa18}
```
![Image](https://github.com/user-attachments/assets/83804b5e-3ffe-4e18-966e-0ce180c1ab21)

### Installation for Windows
Install Git for Windows from [here](https://gitforwindows.org/).   
Install Python Releases for Windows from [here](https://www.python.org/downloads/windows/).   
Open Git Bash and run:   
```
$ python --version
Python 3.11.9
$ python -m pip install -U pip
$ python -m pip install panda3d
$ cd esp-idf-mpu6050-dmp/panda3d
$ python panda.py --help
usage: panda.py [-h] [--model {jet,biplain,707,fa18}]

options:
  -h, --help            show this help message and exit
  --model {jet,biplain,707,fa18}
```
![Image](https://github.com/user-attachments/assets/7f2fbdf4-97d9-40c3-87db-9f8386741220)

### How to use   
See [here](https://github.com/nopnop2002/esp-idf-mpu6050-dmp/tree/main/panda3d)   

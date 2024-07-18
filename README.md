# environment-data-analysis

Using only Arduino Uno, it is possible to keep track of enviromental changes around us. Using shields and sensors, it's possible to track temperature, humidity, pressure, CO levels, CO2 levels, PM 1/2.5/10, UV index, UVA, UVB and wind speed. I have included both the code for the Arduino board and the flow for publishing the data through MQTT protocol. It is possible to also use data mining software such as Orange to visualise data and classify it. 

The two Arduino code files contain different sensors. In the sketchSensors1.ino file, the sensors read UVA, UVB, UV index and pressure. In sketchSensors2.ino, the sensors read temperature, humidity, CO and CO2.

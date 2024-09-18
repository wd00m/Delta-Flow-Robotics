This code was made for a simple soil monitoring station.
It uses a esp-wroom-32 board, a DHT11, a rain sensor and a soil humidity sensor.
It basically reads the soil and air humidity, temp (in celsius) and if the soil has recieved water.
It saves the data in a 3D array, 3 days, 24 hours, 4 different types of data.
The values read in a second are summed up and divided by 60, generating the hourly medium that will be saved.
After 3 days, it erases all data in the array for the sake of memory.
The data can only be collected by the built-in bluetooth module in the ESP32.

That's it, i think 

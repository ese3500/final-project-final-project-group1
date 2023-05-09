# Compiling and running:

The project was developed in Mirochip studio and an Arduino mega(ATmega 2560p), and so to run the project:

1. Import the project into Microchip studio
2. Build the solution
3. Run the avrdude cmd `{$PATH_TO_AVRDUDE}\avrdude\avrdude.exe` with arguments `-C "${PATH_TO_AVRDUDE}\avrdude\avrdude.conf" -p atmega2560 -c wiring -P {COM_PORT} -b 115200 -U flash:w:"$(ProjectDir)Debug\$(TargetName).hex":i -D"` to flash the code onto the Arduino Mega.
4. Change the Wifi SSID and password in the `esp32_blynk_module.ino` file to the target network.
5. Flash the esp32 code onto the ESP32 Feather V2 and power both microcontrollers. A specialized Blynk joystick app is required to connect to the Feather V2 for player inputs.

Devpost Link: https://devpost.com/software/trawn

Video Link: https://www.youtube.com/watch?v=0pfpQ1ARy7Y

# Compiling and running:

The project was developed in Mirochip studio and an Arduino mega(ATmega 2560p), and so to run the project:

1. Import the project into Microchip studio
2. Build the solution
3. Run the avrdude cmd `{$PATH_TO_AVRDUDE}\avrdude\avrdude.exe` with arguments `-C "${PATH_TO_AVRDUDE}\avrdude\avrdude.conf" -p atmega2560 -c wiring -P {COM_PORT} -b 115200 -U flash:w:"$(ProjectDir)Debug\$(TargetName).hex":i -D"`

Devpost Link: 

Video Link: 
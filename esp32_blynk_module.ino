
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on NodeMCU.
  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino
  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!
  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino
  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/**
 * FILL OUT THE FOLLOWING BASED ON YOUR TEMPLATE INFORMATION
 * 1. BLYNK_TEMPLATE_ID (line 48)
 * 2. BLYNK_DEVICE_NAME (line 49)
 * 3. BLYNK_AUTH_TOKEN (line 50)
 */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL2fd4ocZpp"
#define BLYNK_TEMPLATE_NAME "ese350fp"
#define BLYNK_AUTH_TOKEN "HOwzWaq2Ss7TF2XJu8bNFqOcWTGhmrOA"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
// Changed from my own to avoid pass leaks
char ssid[] = "WIFI_SSID_HERE";
char pass[] = "WIFI_PASSWORD_HERE";


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

BLYNK_WRITE(V0) {
  double jx = param.asDouble();
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  if (jx > 200) {
    digitalWrite(A0, HIGH);
  } else if (jx < 55) {
    digitalWrite(A1, HIGH);
  }
}

BLYNK_WRITE(V1) {
  double jy = param.asDouble();
  //deadzone managed by feather
  //outputs on 2 different pins(up/down)
  digitalWrite(SCK, LOW);
  digitalWrite(A5, LOW);
  if (jy > 200) {
    digitalWrite(SCK, HIGH);
  } else if (jy < 55) {
    digitalWrite(A5, HIGH);
  }
}

BLYNK_WRITE(V2) {
  int b1 = param.asInt();
  digitalWrite(12, b1);
}

BLYNK_WRITE(V3) {
  int b2 = param.asInt();
  digitalWrite(27, b2);
}

BLYNK_WRITE(V4) {
  double jx = param.asDouble();
  digitalWrite(MOSI, LOW);
  digitalWrite(MISO, LOW);
  if (jx > 200) {
    digitalWrite(MOSI, HIGH);
  } else if (jx < 55) {
    digitalWrite(MISO, HIGH);
  }
}

BLYNK_WRITE(V5) {
  double jy = param.asDouble();
  //deadzone managed by feather
  //outputs on 2 different pins(up/down)
  digitalWrite(RX, LOW);
  digitalWrite(13, LOW);
  if (jy > 200) {
    digitalWrite(RX, HIGH);
  } else if (jy < 55) {
    digitalWrite(13, HIGH);
  }
}

BLYNK_WRITE(V6) {
  int b1 = param.asInt();
  digitalWrite(15, b1);
}

BLYNK_WRITE(V7) {
  int b2 = param.asInt();
  digitalWrite(33, b2);
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  //joystick 1 outputs
  pinMode(SCK, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(27, OUTPUT);
  //joystick 2 outputs
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(RX, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(33, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}

#include <LWiFi.h>
#include "web_server.h"
#include "led_ctrl_async.h"
const int lamp_pin = 10;
const int ctrl_pin =  6;
char ssid[] = "Secure";      // your network SSID (name)
char pass[] = "zonetwelvepwd";   // your network password
int status = WL_IDLE_STATUS;
int pattern[] = { 200, 500, 1000, '\0' };



char HOST[] = "local.zonetwelve.io";
int  PORT   = 54188;

led_ctrl signel = led_ctrl( LED_BUILTIN );
led_ctrl light  = led_ctrl( lamp_pin );

WiFiClient client;
void setup() {
  Serial.println("Debug: setup\n\n");
  Serial.begin( 115200 );
  pinMode( lamp_pin, OUTPUT );
  pinMode( ctrl_pin, INPUT_PULLUP );
  //signel._debug = true;
  signel.set_pattern( pattern );

  //light._debug = true;
  light.set_delay( 200 );
  light.high();
  
  while(status != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin( ssid, pass );
    
  }
  for(int i = 0 ; i < 0xff ; i++ )
    signel.blink();

  printWifiStatus();
  if( client.connect(HOST, PORT) ){
    client.println("CONN Lamp-V1-Linkit\n");
  }
}

void btnPress( ){
  if( digitalRead( 6 ) == HIGH ){
    light.opposite();
  }
}

void loop() {
  String cmd = ""; // for storage command character
  while( client.available() ){
    char c = client.read();

    if( c == 0x0A ){
      int res = execute( cmd );
      client.write( ( res ) );
      cmd = "";
    }
    cmd += c;
    btnPress( );
  }
  btnPress( );


  if (WiFi.status() == WL_CONNECTED){
      int connected = client.connected();
      if (!connected){
        Serial.println("Reconnect to server");
        client.connect(HOST, PORT);
        client.println("CONN Lamp-V1-Linkit\n");
        delay( 50 );
      }

  }
}

int execute(String cmd){
  Serial.println( cmd );
  if( cmd == "SET 0" ){
    light.low();
    return 0;
  }else if( cmd == "SET 1"){
    light.high();
    return 0;
  }
  return 1;
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "AndroidAP"
#define wifi_password "87654321"

#define mqtt_server "m23.cloudmqtt.com"
#define mqtt_user "uiiuvlvw"
#define mqtt_password "UHckXpGNCpyT"

WiFiClient espClient;
PubSubClient client(espClient);

char cmd_arr1[100];
int cmd_count1;


//*****************************************************************
void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
    
  digitalWrite(5, HIGH);
  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}
//*****************************************************************
void serial_get_command()
{
  char inchar=0;
  if(Serial.available() > 0)
  {
    inchar = Serial.read();    
    if(inchar == '<')
    {
      cmd_count1=0;
      while(inchar != '>' && cmd_count1<50)
      {
        if(Serial.available() > 0)
        {
          inchar = Serial.read();
          cmd_arr1[cmd_count1++] = inchar;
          cmd_arr1[cmd_count1] = '\0';
        }        
      }
      if(inchar == '>')
      {
        cmd_count1--;
        cmd_arr1[cmd_count1] = '\0';
        if(cmd_arr1[0]=='I')
        {
          cmd_arr1[0]=' ';
          client.publish("/dc/push1", cmd_arr1, false);
        }
        else if(cmd_arr1[0]=='J')
        {
          cmd_arr1[0]=' ';
          client.publish("/dc/push2", cmd_arr1, false);
        }
      }
    }
  }
}
//*****************************************************************
void callback(char* topic, byte* payload, unsigned int length) 
{
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
//*****************************************************************
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) 
    {
      Serial.println("connected");
      client.subscribe("/dc/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//*****************************************************************
void setup() 
{
  Serial.begin(9600);
  //Serial.println("Program Started");
  pinMode(5, OUTPUT);digitalWrite(5, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 19616);
  client.setCallback(callback);
}
//*****************************************************************
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  serial_get_command();
}


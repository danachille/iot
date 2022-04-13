#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>

String payload = "";

LiquidCrystal_I2C Afficheur_G(0x27, 20, 4);

void setup()
{
  
Afficheur_G.init(5,4); // Initialisation de l'afficheur, SDA=5, SCL=4


  //CONNEXION AU WIFI
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin("SSID", "Password");     //Modifier ici le SSID ( Nom du réseau) et le mot de passe !!

    Afficheur_G.clear(); // Effacement de l'écran
    Afficheur_G.backlight(); // Allumage
      // Affichage du message
    Afficheur_G.setCursor(0, 1); // Curseur 1ère colonne / 2ème ligne
    Afficheur_G.print("   Connexion...");
    
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }

    Afficheur_G.setCursor(0, 2); // Curseur 1ère colonne / 3ère ligne
    Afficheur_G.print("   Connected !");
    
  Serial.println("Connected !");
  delay(1000);
  Serial.print("Local IP:");
  Serial.println((WiFi.localIP().toString()));
  Serial.print("Mac adress :");                 //AFFICHE DES INFOS POUR LE DEBOGAGE  
  Serial.println((WiFi.macAddress().c_str()));
  Serial.print("Hostname :");
  Serial.println((WiFi.hostname()));

  //LED ALWAYS ON
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http; //Declare an object of class HTTPClient

    http.begin("http://"); //Url de la data a recup
    http.addHeader("Content-Type", "text/html");
    int httpCode = http.GET(); //Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload

      // Stream& input;

      StaticJsonDocument<200> doc;

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* line1 = doc["line1"];   
      const char* line2 = doc["line2"];       //DEFINIR LES LIGNES 1 A 4 DANS DES CONSTANTES
      const char* line3 = doc["line3"];   
      const char* line4 = doc["line4"]; 



    Afficheur_G.clear(); // Effacement de l'écran
    Afficheur_G.backlight(); // Allumage
      // Affichage du message
    Afficheur_G.setCursor(1, 0); // Curseur 1ère colonne / 1ère ligne
    Afficheur_G.print(line1);    //Afficher
    Afficheur_G.setCursor(1, 1); // Curseur 1ème colonne / 2ème ligne
    Afficheur_G.print(line2);    //Afficher
    Afficheur_G.setCursor(1, 2); // Curseur 1ème colonne / 3ème ligne
    Afficheur_G.print(line3);    //Afficher
    Afficheur_G.setCursor(1, 3); // Curseur 1ème colonne / 4ème ligne
    Afficheur_G.print(line4);    //Afficher
    


    } else {
      Serial.println("Marche pas");
    }

    http.end(); //Close connection
  }

  Serial.println(payload);
  delay(30000); //Send a request every 30 seconds
}

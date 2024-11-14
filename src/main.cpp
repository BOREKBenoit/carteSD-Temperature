// Code crée par Benoit Borek CIEL 2B


#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define sdCardPinChipSelect   53                          
#define nomDuFichier "temp.xls"  
#define nomDuFichierH "hum.xls"                 
File monFichier;
File monFichierH;

unsigned long lastTime = 0;  // Variable pour gérer le timing des mesures
unsigned long interval = 10000;  // Intervalle de 10 secondes (10000 ms)
float lastTemperature = 0.0;  // Variable pour stocker la dernière température mesurée

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.begin(9600);
  Serial.println(F("Program de test de lecture/écriture de fichier sur Carte SD, reliée à un Arduino Mega"));
  Serial.println(F("====================================================================================="));
  Serial.println();

  Serial.println(F("Étape 1 : Initialisation de la carte SD :"));
  if (!SD.begin(sdCardPinChipSelect)) {
    Serial.println(F("Échec de l'initialisation !"));
    while (1);   
  }
  Serial.println(F("Initialisation terminée."));
  Serial.println();

  // Initialisation du fichier
  monFichier = SD.open(nomDuFichier, FILE_WRITE);
  if (monFichier) {
    monFichier.close();  // Fermer immédiatement le fichier
    Serial.println(F("Fichier de températures prêt à enregistrer."));
  } else {
    Serial.println(F("Échec de l'ouverture du fichier pour initialisation !"));
    while (1);
  }

    monFichierH = SD.open(nomDuFichierH, FILE_WRITE);
  if (monFichierH) {
    monFichierH.close();  // Fermer immédiatement le fichier
    Serial.println(F("Fichier de l'humidités prêt à enregistrer."));
  } else {
    Serial.println(F("Échec de l'ouverture du fichier pour initialisation !"));
    while (1);
  }


  lcd.setCursor(0, 0);
  lcd.print("Temp: --.- C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: --.- %");
  Serial.println(F("Prêt pour enregistrer et afficher la température et l'humidité."));
}

void loop() {
  unsigned long currentMillis = millis();

  // Si 10 secondes se sont écoulées, prendre une nouvelle lecture
  if (currentMillis - lastTime >= interval) {
    lastTime = currentMillis;  // Réinitialiser le timer

    // Lire la température
    float temp = dht.readTemperature();  // Lire la température en Celsius
    float hum = dht.readHumidity();  // Lire l'humidité en %

    if (isnan(temp)) {
      Serial.println(F("Échec de la lecture de la température!"));
      return;
    }
    if (isnan(hum)) {
      Serial.println(F("Échec de la lecture de l'humidité!"));
      return;
    }
    // Afficher la température sur le LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(hum);
    lcd.print(" %");

    // Enregistrer la température dans le fichier
    monFichier = SD.open(nomDuFichier, FILE_WRITE);
    if (monFichier) {
      monFichier.print(temp);  // Enregistrer la température
      monFichier.print(" C");
      monFichier.println();  // Ajouter un retour à la ligne
      monFichier.close();    // Fermer le fichier
      Serial.print(F("Température enregistrée: "));
      Serial.println(temp);
    } else {
      Serial.println(F("Erreur d'ouverture du fichier pour l'écriture !"));
    }

    
        monFichierH = SD.open(nomDuFichierH, FILE_WRITE);
    if (monFichierH) {
      monFichierH.print(hum);  // Enregistrer la température
      monFichierH.print(" %");
      monFichierH.println();  // Ajouter un retour à la ligne
      monFichierH.close();    // Fermer le fichier
      Serial.print(F("Humidité enregistrée: "));
      Serial.println(hum);
    } else {
      Serial.println(F("Erreur d'ouverture du fichier pour l'écriture !"));
    }
  }
}

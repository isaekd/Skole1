// Definer pinner
const int trigPin = A0;
const int echoPin = A1;
const int reciever = 13;

// Variabler for ultrasonisk sensor
long duration;
int distance;

// Kalibrerings- og objektdeteksjonsflagg
int standard;
bool detected = false;
bool cali = false;

// Variabler for å registrere tidspunktet for deteksjon
unsigned long read1 = 0;
unsigned long read2 = 0;

// Flagg for å sikre at signalbehandling skjer bare én gang
bool signalProcessed = false;

// Variabel for å stabilisere sensormålinger før kalibrering
float scans = 0;

void setup() {
  // Setter pinnefunksjoner
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(reciever, INPUT_PULLUP); // Antar at mottakeren er en inngangspinne
  
  // Start seriell kommunikasjon
  Serial.begin(9600); 
}

void loop() {
  // Utfør avstandsmåling
  if(!detected) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    
    // Skriv ut avstand for feilsøking
    Serial.print("Avstand: ");
    Serial.println(distance);
    
    // Øk skannetelleren
    scans++;
    
    // Sjekk for objektdeteksjon
    if(distance < standard && cali == true) {
      // Registrerer tidspunktet for deteksjon
      if(read1 == 0) {
        read1 = millis(); // Setter read1 hvis det ikke er satt ennå
        Serial.print("Les 1: ");
        Serial.println(read1);
        detected = true;
      } else if (read2 == 0) {
        read2 = millis(); // Setter read2 hvis read1 allerede er satt
        Serial.print("Les 2: ");
        Serial.println(read2);
        detected = true;
      }
    }
  }
  
  // Utfør kalibrering
  if(!cali && scans > 10) {
    delay(1500); // Forsinkelse for stabilisering
    if(distance > 149) {
      standard = 150;
    } else {
      standard = distance - 5;
    }
    cali = true;
    // Skriv ut kalibreringsresultat
    Serial.println("Kalibrering ferdig. Standardavstand: " + String(standard));
  }
  
  // Behandler signalet mottatt fra den andre Arduinoen
  if(digitalRead(reciever) == HIGH && !signalProcessed) {
    if(read1 == 0) {
      read1 = millis(); // Setter read1 hvis det ikke er satt ennå
      Serial.print("Les 1: ");
      Serial.println(read1);
      signalProcessed = true;
    } else if (read2 == 0) {
      read2 = millis(); // Setter read2 hvis read1 allerede er satt
      Serial.print("Les 2: ");
      Serial.println(read2);
      signalProcessed = true;
    }
  }
}

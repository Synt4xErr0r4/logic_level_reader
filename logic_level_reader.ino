#define NCFG 8

struct PinConfig {
  uint8_t pull, sig;
};

enum PinLevel {
  LEVEL_LOW,
  LEVEL_HIGH,
  LEVEL_FLOATING
};

const PinConfig pincfg[NCFG] = {
  { PIN_PD7, PIN_PD5 },
  { PIN_PB0, PIN_PD6 },
  { PIN_PB1, PIN_PB7 },
  { PIN_PB2, PIN_PB6 },
  { PIN_PD2, PIN_PC3 },
  { PIN_PD3, PIN_PC5 },
  { PIN_PD4, PIN_PC4 },
  { PIN_PE0, PIN_PC2 },
};

void setup() {
  Serial.begin(115200);

  pinMode(PIN_PC0, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);

  for (int i = 0; i < NCFG; ++i) {
    pinMode(pincfg[i].sig, INPUT);
    pinMode(pincfg[i].pull, OUTPUT);
    digitalWrite(pincfg[i].pull, LOW);
  }
}

void measure(int level, bool states[NCFG]) {
  for (int i = 0; i < NCFG; ++i) {
    digitalWrite(pincfg[i].pull, level);
  }
  
  delay(1);

  for (int i = 0; i < NCFG; ++i) {
    states[i] = digitalRead(pincfg[i].sig) != LOW;
  }
}

void loop() {
  bool states_lo[NCFG];
  bool states_hi[NCFG];
  PinLevel pinlevel[NCFG];

  measure(LOW, states_lo);
  measure(HIGH, states_hi);

  for (int i = 0; i < NCFG; ++i) {
    if (states_lo[i] != states_hi[i]) {
      pinlevel[i] = LEVEL_FLOATING;
    }
    else if (states_lo[i]) {
      pinlevel[i] = LEVEL_HIGH;
    }
    else {
      pinlevel[i] = LEVEL_LOW;
    }

    Serial.printf("%d %d\n", i, pinlevel[i]);
  }

  switch (pinlevel[0]) {
    case LEVEL_LOW:
      digitalWrite(PIN_PC0, HIGH);
      digitalWrite(PIN_PC1, LOW);
      break;
    
    case LEVEL_HIGH:
      digitalWrite(PIN_PC0, LOW);
      digitalWrite(PIN_PC1, HIGH);
      break;
    
    case LEVEL_FLOATING:
      digitalWrite(PIN_PC0, LOW);
      digitalWrite(PIN_PC1, LOW);
      break;
  }

  delay(100);
}

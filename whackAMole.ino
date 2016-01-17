#include <avr/interrupt.h>
int redLight = 10;
int greenLight = 11;
int blueLight = 12;
int yellowLight = 13;
int pick_a_light;
volatile int lightTime = 0;
volatile int clockTime = 0;
volatile int actualLightTime = 0;
float duration;
int previous = 0;
int mode = 3;
#define redPIN 2
#define greenPIN 7
#define bluePIN 8
#define playPIN 9
#define yellowPIN A0
#define speaker 4
int timer = 6;
int timer2 = 5;
int countdown = 74;
int start_clock = 0;
float game_duration = 0;
bool game_mode = true;
volatile int clockTrack = 0;
float turnScore = 0;
float totalScore = 0;
int clock_tracker = 0;
volatile int scoreTimer = 0;
volatile float reaction_time = 0;
volatile double counter = 0;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(redLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(blueLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(timer, OUTPUT);
  pinMode(timer2, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  attachInterrupt(1, speedChange, FALLING);
  attachInterrupt(0, redLightChange, FALLING);
  cli();
  PCICR |= 0b00000111;
  PCMSK0 |= 0b00000011;
  PCMSK2 |= 0b10000000;
  PCMSK1 |= 0b00000001;
  TCCR1A = 0;             // normal counting mode
  TIMSK1 |= (1 << TOIE1);
  TCCR1B = _BV(CS10);      // set prescale to 1
  TCNT1 = 0;              // clear the timer count
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (!game_mode && countdown == 74) {
    count_down();
    countdown++;
  }

  game();
}

void modeChange() {
  game_mode = false;
  countdown = 74;
  clock_tracker = 0;

}
void count_down () {
  if (!game_mode) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(timer, HIGH);
      tone(speaker, 750, 200);
      delay(500);
      digitalWrite(timer, LOW);
      delay(500);
    }

  }
}

void gameClock() {
  if (!game_mode) {
    if (clock_tracker == 0) {
      clockTime = clocker();
      tone(speaker, 900, 200);
      clock_tracker++;

    }
    clockTrack = clocker();
    game_duration = (clockTrack - clockTime) / 1000;
    Serial.println(game_duration);
    if (game_duration <= 15) {

      if (game_duration >= 3 && game_duration < 7) {
        digitalWrite(timer, HIGH);
      }
      else if (game_duration >= 7 && game_duration < 12) {
        digitalWrite(timer2, HIGH);
        digitalWrite(timer, LOW);

      }
      else if (game_duration >= 13 && game_duration < 15) {
        digitalWrite(timer2, HIGH);
        digitalWrite(timer, HIGH);

      }

    }
  }
  else {
    digitalWrite(timer2, LOW);
    digitalWrite(timer, LOW);

  }
}
void game () {
  digitalWrite(previous, LOW);
  gameClock();
  if (game_duration > 15) {
    game_mode = true;
    Serial.println("Game over");
    Serial.println("Total Score");
    Serial.println(totalScore);
    mode = 3;
    game_duration = 0;
    totalScore = 0;
    tone(speaker, 950, 500);
    tone(speaker, 900, 500);
    tone(speaker, 850, 500);
  }
  pick_a_light = random(redLight, yellowLight + 1);
  while (pick_a_light == previous) {
    pick_a_light = random(redLight, yellowLight + 1);
  }
  lightTime = clocker();
  if (pick_a_light == redLight) {
    digitalWrite(redLight, HIGH);

    duration = (actualLightTime - lightTime) / 1000;

    while (duration < mode)
    {

      if (digitalRead(redLight) == LOW) {
        scoreTimer = clocker();
        reaction_time = (scoreTimer - lightTime);
        Serial.println("Reaction Time:");
        Serial.println(reaction_time);
        turnScore = (mode * 1000 - reaction_time);
        if (mode == 1) {
          turnScore = turnScore * 10;
        }
        Serial.println("Score: ");
        Serial.println(turnScore);
        totalScore += turnScore;
        Serial.println("Total Score: ");
        Serial.println(totalScore);
        game();
      }
      actualLightTime = clocker();
      duration = (actualLightTime - lightTime) / 1000;

    }
    digitalWrite(redLight, LOW);
  }

  else if (pick_a_light == greenLight) {
    digitalWrite(greenLight, HIGH);

    duration = (actualLightTime - lightTime) / 1000;

    while (duration < mode)
    {
      if (digitalRead(greenLight) == LOW) {
        scoreTimer = clocker();
        reaction_time = (scoreTimer - lightTime);
        turnScore = (mode * 1000 - reaction_time);
        if (mode == 1) {
          turnScore = turnScore * 10;
        }
        Serial.println("Reaction Time:");
        Serial.println(reaction_time);
        Serial.println("Score: ");
        Serial.println(turnScore);
        totalScore += turnScore;
        Serial.println("Total Score: ");
        Serial.println(totalScore);
        game();
      }
      actualLightTime = clocker();
      duration = (actualLightTime - lightTime) / 1000;

    }
    digitalWrite(greenLight, LOW);
  }

  else if (pick_a_light == blueLight) {
    digitalWrite(blueLight, HIGH);

    duration = (actualLightTime - lightTime) / 1000;
    while (duration < mode)
    {
      if (digitalRead(blueLight) == LOW) {
        scoreTimer = clocker();
        reaction_time = (scoreTimer - lightTime);
        turnScore = (mode * 1000 - reaction_time);
        if (mode == 1) {
          turnScore = turnScore * 10;
        }
        Serial.println("Reaction Time:");
        Serial.println(reaction_time);
        Serial.println("Score: ");
        Serial.println(turnScore);
        totalScore += turnScore;
        Serial.println("Total Score: ");
        Serial.println(totalScore);
        game();
      }
      actualLightTime = clocker();
      duration = (actualLightTime - lightTime) / 1000;

    }
    digitalWrite(blueLight, LOW);
  }
  else {

    digitalWrite(yellowLight, HIGH);
    duration = (actualLightTime - lightTime) / 1000;
    while (duration < mode)
    {

      if (digitalRead(yellowLight) == LOW) {
        scoreTimer = clocker();
        reaction_time = (scoreTimer - lightTime);
        turnScore = (mode * 1000 - reaction_time);
        if (mode == 1) {
          turnScore = turnScore * 10;
        }
        Serial.println("Reaction Time:");
        Serial.println(reaction_time);
        Serial.println("Score: ");
        Serial.println(turnScore);
        totalScore += turnScore;
        Serial.println("Total Score: ");
        Serial.println(totalScore);
        game();
      }
      actualLightTime = clocker();

      duration = (actualLightTime - lightTime) / 1000;

    }
    digitalWrite(yellowLight, LOW);
  }
  previous = pick_a_light;
  //reset score


}

void speedChange () {
  if (game_mode) {
    if (mode == 3) {
      mode = 1;
    }

    else {
      mode = 3;
    }

  }


}


void redLightChange() {
  if (!game_mode) {

    digitalWrite(redLight, LOW);


  }

}

ISR(PCINT0_vect) {

  if (!game_mode) {

    if (digitalRead(bluePIN) == HIGH) {

      digitalWrite(blueLight, LOW);

    }
  }

  else if (digitalRead(playPIN) == HIGH) {
    modeChange();
  }



}
ISR(PCINT1_vect) {
  if (!game_mode) {

    digitalWrite(yellowLight, LOW);


  }

}
ISR(PCINT2_vect) {
  if (!game_mode) {

    digitalWrite(greenLight, LOW);

  }

}

unsigned long clocker () {
  return (TCNT1 + counter * 65536) / ( F_CPU / 1000L);
}

unsigned long clockerSeconds () {
  return (TCNT1 + counter * 65536) / ( F_CPU);
}

ISR(TIMER1_OVF_vect)
{
  counter++;
}




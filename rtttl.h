#define OCTAVE_OFFSET 0

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

int notes[] = { 0,
                NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
                NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
                NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
                NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7 };


#define isdigit(n) (n >= '0' && n <= '9')

void play_led(int f, LedArray &led){
  switch (f) {
    case NOTE_C4:
      led.on(1);
      break;
    case NOTE_D4:
      led.on(2);
      break;
    case NOTE_E4:
      led.on(3);
      break;
    case NOTE_F4:
      led.on(4);
      break;
    case NOTE_G4:
      led.on(5);
      break;
    case NOTE_A4:
      led.on(6);
      break;
    case NOTE_B4:
      led.on(7);
      break;
    case NOTE_C5:
      led.on(8);
      break;
    default:
      led.off();
  }
}

void play_rtttl(char *p, uint8_t pin, LedArray &led) {
  // Absolutely no error checking in here
  Serial.print("playyng: ");
  Serial.println(p);
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while (*p != ':') p++;  // ignore name
  p++;                    // skip ':'

  // get default duration
  if (*p == 'd') {
    p++;
    p++;  // skip "d="
    num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    if (num > 0) default_dur = num;
    p++;  // skip comma
  }


  // get default octave
  if (*p == 'o') {
    p++;
    p++;  // skip "o="
    num = *p++ - '0';
    if (num >= 3 && num <= 7) default_oct = num;
    p++;  // skip comma
  }


  // get BPM
  if (*p == 'b') {
    p++;
    p++;  // skip "b="
    num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    Serial.print("bpm: ");
    Serial.println(bpm);
    p++;  // skip colon
  }


  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)



  // now begin note loop
  while (*p) {
    // first, get note duration, if available
    num = 0;
    while (isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }

    if (num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch (*p) {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if (*p == '#') {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if (*p == '.') {
      duration += duration / 2;
      p++;
    }

    // now, get scale
    if (isdigit(*p)) {
      scale = *p - '0';
      p++;
    } else {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if (*p == ',')
      p++;  // skip comma for next note (or we may be at the end)

    // now play the note

    if (note) {
      tone(pin, notes[(scale - 4) * 12 + note]);
      play_led(notes[(scale - 4) * 12 + note], led);
      delay(duration);
      noTone(pin);
      led.off();
    } else {
      led.off();
      delay(duration);
    }
  }
}



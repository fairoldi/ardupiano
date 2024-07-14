class LedArray {
  uint8_t pins[4];
  
  public:
  LedArray(uint8_t inh_pin, uint8_t a_pin, uint8_t b_pin, uint8_t c_pin) {
   
    pins[0] = inh_pin;
    pins[1] = a_pin;
    pins[2] = b_pin;
    pins[3] = c_pin;

    for (int i=0; i<4; i++) {
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], HIGH);
    }

  }

  void on(uint8_t n) {
    if (n==0) {
      digitalWrite(pins[0], HIGH);
      return;
    }
    n = n-1;
    digitalWrite(pins[1], (n >> 0) & 0x1);
    digitalWrite(pins[2], (n >> 1) & 0x1);
    digitalWrite(pins[3], (n >> 2) & 0x1);
    digitalWrite(pins[0], LOW);


  }

  void off() {
    digitalWrite(pins[0], HIGH);
  }
};
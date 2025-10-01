const int RED_PIN = 9;
const int BLUE_PIN = 10;
const int GREEN_PIN = 11;

void setColor(unsigned int r, unsigned int g, unsigned int b)
{
  analogWrite(RED_PIN, 255 - r);
  analogWrite(GREEN_PIN, 255 - g);
  analogWrite(BLUE_PIN, 255 - b);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

int count = 255;
bool increasing = false;

void loop() {
  setColor(255 - count, 0, 0);
  delay(1);


  if (increasing) {
    count+=1;
    if (count >= 255) {
      increasing = false;
    }
  } else {
    count-=1;
    if (count <= 0) {
      increasing = true;
    }
  }
}

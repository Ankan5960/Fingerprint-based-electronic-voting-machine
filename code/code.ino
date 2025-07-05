#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>

#define enroll A0
#define del A1
#define up A2
#define down A3
#define match A4
#define indVote 2
#define sw1 10
#define sw2 11
#define sw3 12
#define resultsw A5
#define indFinger 0
#define buzzer 13
#define records 25

const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t id;
SoftwareSerial mySerial (2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int vote1, vote2, vote3;
int flag;

void setup() {
  delay(1000);
  pinMode(enroll, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(del, INPUT_PULLUP);
  pinMode(match, INPUT_PULLUP);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(resultsw, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indVote, OUTPUT);
  pinMode(indFinger, OUTPUT);
  Serial.begin(57600);
  lcd.begin(16, 2);

  if (digitalRead(resultsw) ==0)
  {
    for (int i = 0; i < records; i++) EEPROM.write(i + 10, 0xff);
    EEPROM.write(0, 0);
    EEPROM.write(1, 0);
    EEPROM.write(2, 0);
    lcd.clear();
    lcd.print("System Reset");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Voting Machine");
  lcd.setCursor(0, 1);
  lcd.print("by Finger Print");
  delay(2000);
  if (EEPROM.read(0) == 0xff)
    EEPROM.write(0, 0);
  if (EEPROM.read(1) == 0xff) 
    EEPROM.write(1, 0);
  if (EEPROM.read(1) == 0xff) 
    EEPROM.write(1, 0);

  finger.begin(57600);
  Serial.begin(57600);
  delay(5);
  lcd.clear();
  lcd.print("Finding Module");
  lcd.setCursor(0, 1);
  delay(1000);
  if (finger.verifyPassword())
  {
    lcd.clear();
    lcd.print("Found Module ");
    delay(1000);
  }
  else
  {
    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
    while (1);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cn1");
  lcd.setCursor(6, 0);
  lcd.print("Cn2");
  lcd.setCursor(12, 0);
  lcd.print("Cn3");
  lcd.setCursor(0, 1);
  vote1 = EEPROM.read(0);
  lcd.print(votel);
  lcd.setCursor(6, 1);
  vote2 = EEPROM.read(1);
  lcd.print(vote2);
  lcd.setCursor(12, 1);
  vote3 - EEPROM.read(2);
  lcd.print(vote3);
  delay(2000);
}

void checkkeys()
{
  if (digitalRead(resultsw) == 0)
  {
    result();
  }
  if (digitalRead(enroll) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(enroll) == 0)
      Enroll();
  }
  else if (digitalRead(del) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
  }
}

void Enroll()
{
  int count = 0;
  lcd.clear();
  lcd.print("Enter Finger ID:");
  while (1)
  {
    lcd.setCursor(0, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
    count++;
    if (count > 25)
      count = 0;
    delay(500);
    }
    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      id = count;
      getFingerprintEnroll();
      for (int i=0; i < records; i++)
      {
        if (EEPROM.read(1 + 10) 0xff)
        {
          EEPROM.write(i + 10, id);
          break;
        }
      }
      return;
    } 
    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

void delet()
{
  int count = 0;
  lcd.clear();
  lcd.print("Enter Finger ID");
  while (1)
  {
    lcd.setCursor(0, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
      count++;
      if (count > 25)
        count = 0;
      delay(500);
    }
    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      id = count;
      deleteFingerprint(id);
      for (int i = 0; i < records; i++)
      {
        if (EEPROM.read(i + 10) == id)
        {
          EEPROM.write(i + 10, 0xff);
          break;
        }
      }
      return;
    }
    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) 
    return -1;

  p = finger.image2Tz(); 
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch(); 
  if (p != FINGERPRINT_OK)
  {
    lcd.clear(); 
    lcd.print("Finger Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Try Later");
    delay(2000);
    return -1;
  }
  // found a match! 
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id)
{
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
    delay(1000);
  }
  else
  {
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0, 1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }
}

void Vote()
{
  lcd.clear();
  lcd.print("Please Place");
  lcd.setCursor(0, 1);
  lcd.print("Your Vote ");
  digitalWrite(indVote, HIGH);
  digitalWrite(indFinger, LOW);
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(1000);
  while (1)
  {
    if (digitalRead(sw1) == 0)
    {
      vote1++;
      EEPROM.write(0, vote1);
      voteSubmit(1); ;
      while (digitalRead(sw1) == 0)
      return;
    }
    if (digitalRead(sw2) == 0)
    {
      vote2++;
      voteSubmit(2);
      EEPROM.write(1, vote2);
      while (digitalRead(sw2) == 0);
      return;
    } 
    if (digitalRead(sw3) - 0)
    {
      vote3++;
      voteSubmit(3);
      EEPROM.write(2, vote3);
      while (digitalRead(sw3) == 0);
      return;
    }
    if (digitalRead(resultsw) == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Can1");
      lcd.setCursor(6, 0);
      lcd.print("Can2");
      lcd.setCursor(12, 0);
      lcd.print("Can3");
      for (int i=0; i < 3; 1++)
      {
        lcd.setCursor(16, 1);
        lcd.print(EEPROM.read(i));
      }
      delay(2000);
      int vote votel + vote2 + vote3;
      if (vote)
      {
        if ((votel > vote2 && vote1 > vote3))
        {
          lcd.clear();
          lcd.print("Cani Wins");
          delay(3000);
          lcd.clear();
        }
        else if (vote2 > votel && vote2 > vote3)
        {
          lcd.clear();
          lcd.print("Can2 Wins");
          delay(3000);
          lcd.clear();
        }
        else if ((vote3 > votel && vote3 > vote2))
        {
          lcd.clear();
          lcd.print("Can3 Wins");
          delay(3000);
          lcd.clear();
        } 
        else if (votel <= vote2 && vote1 > vote3)
        {
          lcd.clear();
          lcd.print(" Tie Up Or ");
          lcd.setCursor(0, 1);
          lcd.print(" No Result ");
          delay(1000);
          lcd.clear();
        }
      }
      else
      {
        lcd.clear();
        lcd.print("No Voting....");
        delay(1000);
        lcd.clear();
      }
      vote1 = 0;
      vote2 = 0;
      vote3 = 0;
      vote = 0;
      lcd.clear();
      return;
    }
  } 
  digitalWrite(indVote, LOW);
}

void result()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Can1");
  lcd.setCursor(6, 0);
  lcd.print("Can2");
  lcd.setCursor(12, 0);
  lcd.print("Can3");
  for (int i = 0; i < 3; i++)
  {
    lcd.setCursor(i * 6, 1);
    lcd.print(EEPROM.read(i));
  }
  delay(2000);
  int vote = votel + vote2 + vote3;
  if (vote)
  {
    if ((vote1 > vote2 && vote1 > vote3))
    {
      lcd.clear();
      lcd.print("Can1 Wins");
      delay(2000);
      lcd.clear();
    }
    else if (vote2 > vote1 && vote2 > vote3)
    {
      lcd.clear();
      lcd.print("Can2 Wins");
      delay(2000);
      lcd.clear();
    }
    else if ((vote3 > vote1 && vote3 > vote2))
    {
      lcd.clear();
      lcd.print("Can3 Wins");
      delay(2000);
      lcd.clear();
    }
    else
    {
      lcd.clear();
      lcd.print(" Tie Up Or ");
      lcd.setCursor(0, 1);
      lcd.print(" No Result ");
      delay(1000);
      lcd.clear();
    }
  }
  else
  {
    lcd.clear();
    lcd.print("No Voting....");
    delay(1000);
    lcd.clear();
    else 
    {
      lcd.clear();
      lcd.print("No Voting....");
      delay(1000);
      lcd.clear();
    }
  votel = 0;
  vote2 = 0;
  vote3 = 0;
  vote = 0;
  lcd.clear();
  return;
}

void voteSubmit(int cn)
{
  lcd.clear();
  if (cn == 1)
    lcd.print("Can1");
  else if (cn == 2)
    lcd.print("Can2");
  else if (cn == 3)
    lcd.print("Can3");
  lcd.setCursor(0, 1);
  lcd.print("Vote Submitted");
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
  digitalWrite(indVote, LOW);
  return;
}

void loop() {
  lcd.setCursor(0, 0); lcd.print("Press Match Key "); 
  lcd.setCursor(0, 1); 
  lcd.print("to start system"); 
  digitalWrite(indFinger, LOW);
  digitalWrite(indVote, LOW);
  if (digitalRead(match) == 0)
  {
    digitalWrite(buzzer, HIGH); 
    delay(200); 
    digitalWrite(buzzer, LOW); 
    digitalWrite(indFinger, HIGH); 
    for (int i = 0; i < 3; i++)
    {
      lcd.clear(); 
      lcd.print("Place Finger"); 
      delay(2000); 
      int result = getFingerprintIDez(); 
      if (result >= 0)
      {
        flag = 0;
        for (int i = 0; i < records; i++)
        { 
          if (result EEPROM.read(i + 10))
          { 
            lcd.clear(); 
            lcd.print("Authorised Voter"); 
            lcd.setCursor(0, 1); 
            lcd.print("Please Wait...."); 
            delay(1000); 
            EEPROM.write(i + 10, 0xff);
            Vote();
            flag = 1;
            return;
          }
        }
        if(flag == 0)
        {
          lcd.clear();
          lcd.print("Already Voted");
          digitalWrite(buzzer, HIGH);
          delay(2000);
          digitalWrite(buzzer, LOW);
          return;
        }
      }
    }
    lcd.clear();
  }
  checkkeys();
  delay(1000);
}

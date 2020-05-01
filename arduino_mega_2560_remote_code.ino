//new shot at remote
//accurate lcd and potentiometer functionality
#include <LiquidCrystal.h>
#include <VirtualWire.h>
LiquidCrystal lcd(48, 46, 42, 40, 38, 36);
int potentiometerpin1 = 7;
int potentiometerpin2 = 8;
int potentiometerpin3 = 9;
int potentiometerpin4 = 10;

int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int analog_button = 8;
int x_axis = A0;
int y_axis = A1;
int buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, analog_button};

void setup() {
  initializelcdscreen();
  pinMode(53, INPUT_PULLUP);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_rx_pin(44);
  for (int i; i < 7; i++) {
   pinMode(buttons[i], INPUT);
   digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
  while (digitalRead(select_button) == 1) {
    delay(100);
  }
  if (digitalRead(select_button) == 0) {
    Serial.println("1000");
    vw_rx_start();
    delay(1000);
  }
}
void loop() {
  int failsafestatus = digitalRead(53);
  if (failsafestatus == 0) { //avoidance on
    uint8_t buflen = VW_MAX_MESSAGE_LEN; //reads the data
    uint8_t buf[buflen];
    if (vw_get_message(buf, &buflen)) {
      for(int i = 0;i < buflen;i++) {
        if (buf[i] == 51) {
          //safe character recieved
          avoidanceremote();
         // Serial.println("safe");
        }
        else if (buf[i] == 49) {
          //alarm character recieved
          Serial.println("22");
          int randomval = 0;
          while (randomval == 0) {
            int updatedavoidance = systemupdate();
            if (updatedavoidance == 0) { return; }
            if (digitalRead(53) == 1) { return; }
            updatepotentiometers();
          }
        }
      }
    }
  }
  if (failsafestatus == 1) { //avoidance off
    remote();
  }
}

int readpotentiometervalue(int potentiometerpin) {
  int potentiometerrawvalue = analogRead(potentiometerpin);
  int potentiometermapvalue = map(potentiometerrawvalue, 0, 1023, 0, 10); //could be problematic with 1-11 value instead of 1-10
  switch (potentiometerpin) {
    case 7:
      lcd.setCursor(0,1);
      lcd.print(potentiometermapvalue);
      break;
    case 8:
      lcd.setCursor(4,1);
      lcd.print(potentiometermapvalue);
      break;
    case 9:
      lcd.setCursor(8,1);
      lcd.print(potentiometermapvalue);
      break;
    case 10:
      lcd.setCursor(13,1);
      lcd.print(potentiometermapvalue);
      break;
  }
  return potentiometermapvalue;
}
int updatepotentiomete(int pin, int compare_number) {
  updatepotentiometers();
  int newpot = readpotentiometervalue(pin) + 1;
  if (newpot != compare_number) {
    return 1;
  }
  if (digitalRead(53) == 1) {
    return 1;
  }
  else {
    return 0;
  }
}
int updatepotentiometer(int pin, int compare_number) {
  updatepotentiometers();
  int newpot = readpotentiometervalue(pin) + 1;
  if (digitalRead(53) == 0) {
    return 1;
  }
  if (newpot != compare_number) {
    return 1;
  }
  else {
    return 0;
  }
}
void initializelcdscreen() { //initializes the lcd screen with settings information
  lcd.begin(16,2); //starts the lcd as a 16x2 display
  lcd.setCursor(0,0);
  lcd.print("F/B");
  lcd.setCursor(4,0);
  lcd.print("L/R");
  lcd.setCursor(8,0);
  lcd.print("U/D");
  lcd.setCursor(13,0);
  lcd.print("R");
  lcd.setCursor(0, 1); //prints the settings page on the LCD
}
void updatepotentiometers() {
  int pot1 = readpotentiometervalue(potentiometerpin1) + 1;
  int pot2 = readpotentiometervalue(potentiometerpin2) + 1;
  int pot3 = readpotentiometervalue(potentiometerpin3) + 1;
  int pot4 = readpotentiometervalue(potentiometerpin4) + 1;
}
void remote() {
  int pot1 = readpotentiometervalue(potentiometerpin1) + 1;
  int pot2 = readpotentiometervalue(potentiometerpin2) + 1;
  int pot3 = readpotentiometervalue(potentiometerpin3) + 1;
  int pot4 = readpotentiometervalue(potentiometerpin4) + 1;
  if (digitalRead(select_button) == 0) {
    Serial.println("1"); //takeoff
    while (digitalRead(select_button) == 0) {
      updatepotentiometers();
    }//while loop to prevent repeated digits
  }
  if (digitalRead(up_button) == 0) {
    int upspeed = 50 + pot3;
    Serial.println(upspeed);
    while (digitalRead(up_button) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin3, pot3);
      if (updatereturn == 1) { return; }
    } //same type of while loop
    //going up with correct potenitometer speed that is set
    //numbers are designated for certain actions... speed is decoded from the numbers later on
  }
  if (digitalRead(down_button) == 0) {
    int downspeed = 60 + pot3;
    Serial.println(downspeed);
    while (digitalRead(down_button) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin3, pot3);
      if (updatereturn == 1) { return; }
    }
    //going down with same pattern
  } 
  if (digitalRead(left_button) == 0) {
    int ccspeed = 80 + pot4;
    Serial.println(ccspeed);
    while (digitalRead(left_button) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin4, pot4);
      if (updatereturn == 1) { return; }
    }
    //going left with same pattern
  } 
  if (digitalRead(right_button) == 0) {
    int cspeed = 70 + pot4;
    Serial.println(cspeed);
    while (digitalRead(right_button) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin4, pot4);
      if (updatereturn == 1) { return; }
    }
    //going right with same pattern
  }
  if (digitalRead(start_button) == 0) {
    Serial.println("0");
    while (digitalRead(start_button) == 0) {
      updatepotentiometers();
    }
    //landing with same pattern
  }
  int x = map(analogRead(x_axis), 0, 1000, -1, 1); //reads the x data from the joystick
  int y = map(analogRead(y_axis), 0, 1000, -1, 1); //reads the y data from the joystick
  if (x == 0 && y == 1) {
    //forward
    int fspeed = pot1 + 10;
    Serial.println(fspeed); //same type of pattern
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == 1) {
      int updatereturn = updatepotentiometer(potentiometerpin1, pot1);
      if (updatereturn == 1) { return; }
    }//prevents repeated data
  }
  if (x == 0 && y == -1) {
    int bspeed = pot1 + 20;
    Serial.println(bspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == -1) {
      int updatereturn = updatepotentiometer(potentiometerpin1, pot1);
      if (updatereturn == 1) { return; }
    }
    //backward with same type of pattern
  }
  if (x == -1 && y == 0) {
    //left with same type of pattern
    int lspeed = 40 + pot2;
    Serial.println(lspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == -1 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin2, pot2);
      if (updatereturn == 1) { return; }
    }
  }
  if (x == 1 && y == 0) {
    //right with same type of pattern
    int rspeed = 30 + pot2;
    Serial.println(rspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 1 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0) {
      int updatereturn = updatepotentiometer(potentiometerpin2, pot2);
      if (updatereturn == 1) { return; }
    }
  }
  if (x == 0 && y == 0 && digitalRead(start_button) == 1 && digitalRead(select_button) == 1 && digitalRead(up_button) == 1 && digitalRead(down_button) == 1 && digitalRead(left_button) == 1 && digitalRead(right_button) == 1) {
    Serial.println("2");
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0 && digitalRead(start_button) == 1 && digitalRead(select_button) == 1 && digitalRead(up_button) == 1 && digitalRead(down_button) == 1 && digitalRead(left_button) == 1 && digitalRead(right_button) == 1) {
      if (digitalRead(53) == 0) {
        return;
      }
      updatepotentiometers();
    }
  }  
}
void avoidanceremote() {
  int pot1 = readpotentiometervalue(potentiometerpin1) + 1;
  int pot2 = readpotentiometervalue(potentiometerpin2) + 1;
  int pot3 = readpotentiometervalue(potentiometerpin3) + 1;
  int pot4 = readpotentiometervalue(potentiometerpin4) + 1;
  if (digitalRead(select_button) == 0) {
    Serial.println("1"); //takeoff
    while (digitalRead(select_button) == 0) {
      updatepotentiometers();
    }//while loop to prevent repeated digits
  }
  if (digitalRead(up_button) == 0) {
    int upspeed = 50 + pot3;
    Serial.println(upspeed);
    while (digitalRead(up_button) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin3, pot3);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    } //same type of while loop
    //going up with correct potenitometer speed that is set
    //numbers are designated for certain actions... speed is decoded from the numbers later on
  }
  if (digitalRead(down_button) == 0) {
    int downspeed = 60 + pot3;
    Serial.println(downspeed);
    while (digitalRead(down_button) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin3, pot3);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
    //going down with same pattern
  } 
  if (digitalRead(left_button) == 0) {
    int ccspeed = 80 + pot4;
    Serial.println(ccspeed);
    while (digitalRead(left_button) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin4, pot4);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
    //going left with same pattern
  } 
  if (digitalRead(right_button) == 0) {
    int cspeed = 70 + pot4;
    Serial.println(cspeed);
    while (digitalRead(right_button) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin4, pot4);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
    //going right with same pattern
  }
  if (digitalRead(start_button) == 0) {
    Serial.println("0");
    while (digitalRead(start_button) == 0) {
      updatepotentiometers();
    }
    //landing with same pattern
  }
  int x = map(analogRead(x_axis), 0, 1000, -1, 1); //reads the x data from the joystick
  int y = map(analogRead(y_axis), 0, 1000, -1, 1); //reads the y data from the joystick
  if (x == 0 && y == 1) {
    //forward
    int fspeed = pot1 + 10;
    Serial.println(fspeed); //same type of pattern
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == 1) {
      int updatereturn = updatepotentiomete(potentiometerpin1, pot1);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }//prevents repeated data
  }
  if (x == 0 && y == -1) {
    int bspeed = pot1 + 20;
    Serial.println(bspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == -1) {
      int updatereturn = updatepotentiomete(potentiometerpin1, pot1);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
    //backward with same type of pattern
  }
  if (x == -1 && y == 0) {
    //left with same type of pattern
    int lspeed = 40 + pot2;
    Serial.println(lspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == -1 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin2, pot2);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
  }
  if (x == 1 && y == 0) {
    //right with same type of pattern
    int rspeed = 30 + pot2;
    Serial.println(rspeed);
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 1 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0) {
      int updatereturn = updatepotentiomete(potentiometerpin2, pot2);
      if (updatereturn == 1) { return; }
      updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
    }
  }
  if (x == 0 && y == 0 && digitalRead(start_button) == 1 && digitalRead(select_button) == 1 && digitalRead(up_button) == 1 && digitalRead(down_button) == 1 && digitalRead(left_button) == 1 && digitalRead(right_button) == 1) {
    Serial.println("2");
    while (map(analogRead(x_axis), 0, 1000, -1, 1) == 0 && map(analogRead(y_axis), 0, 1000, -1, 1) == 0 && digitalRead(start_button) == 1 && digitalRead(select_button) == 1 && digitalRead(up_button) == 1 && digitalRead(down_button) == 1 && digitalRead(left_button) == 1 && digitalRead(right_button) == 1) {
      if (digitalRead(53) == 1) {
        return;
      }
      int updatereturn = systemupdate();
      if (updatereturn == 1) { return; }
      updatepotentiometers();
    }
  }   
}
int systemupdate() {
  uint8_t buflen = VW_MAX_MESSAGE_LEN; //reads the data
  uint8_t buf[buflen];
  if (vw_get_message(buf, &buflen)) {
    for(int i = 0;i < buflen;i++) {
      if (buf[i] == 51) {
        return 0;
      }
      else if (buf[i] == 49) {
        return 1;
      }
    }
  }
}

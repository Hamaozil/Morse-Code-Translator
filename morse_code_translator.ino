#include<LiquidCrystal.h>
//##############################
#define rs 11
#define en 12
#define d4 5
#define d5 4
#define d6 3
#define d7 2
#define MEG_SEN A0 // hall effect sensor pin
int column = 0;
int row = 0;
//##############################
#define DOT_DURATION 400 //MS
#define DASH_DURATION 800 //MS
#define LETTER_DURATION 6000 //MS
#define SPACE_DURATION DOT_DURATION*6 //MS
//################################
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//##############################
String MORSE_CODE = ""; // store morse code as input from user 
String message = ""; //store encoded msg
char alph_num_collection[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G',
                              'H', 'I', 'J', 'K', 'L', 'M', 'N',
                              'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                              'V', 'W', 'X', 'Y', 'Z',
                              '1', '2', '3', '4', '5',
                              '6', '7', '8', '9', '0',
                              '.', ',', '?'
                             };
char* morse_code_array[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
                            "....", "..", ".---", "-.-", ".-..", "--", "-.",
                            "---", ".--.", "--.-", ".-.", "...", "-", "..-",
                            "...-", ".--", "-..-", "-.--",
                            "--..", ".----", "..---", "...--", "....-",
                            ".....", "-....", "--...", "---..", "----.",
                            "-----", ".-,-,-", "--..--", "..--.."
                           } ;
//##########################################
void setup()
{
  pinMode(MEG_SEN, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(column, row);
}
int time_pressed = 0; //to know how many times btn have been pressed
unsigned long current_time = 0;
unsigned long pervious_time = 0;
// both current_time & pervious_time will be used to measure time from writting one letter  
int meg_sen_read;//store value of btn
void loop()
{
  meg_sen_read = analogRead(MEG_SEN); //take read pf push button
  //ifmeg_sen_read=1 >>clicked check
  current_time = millis();
  pervious_time = millis();
  if (meg_sen_read < 600)
    meg_sen_read = 1;
  else  if (meg_sen_read >= 600)
    meg_sen_read = 0;
  if (meg_sen_read == 1) //btn have been pressed
  {
    MORSE_CODE = get_morse_code();
  }
  if (meg_sen_read == 0) // not pressed
  {
    print_alph_num_msg();
  }
  delay(1);

}
//########################################
int get_index(String morse_input)
{
  int length = sizeof(morse_code_array) / sizeof(morse_code_array[0]);//get length of morse array
  for (int array_search_counter = 0; array_search_counter < length; array_search_counter++) // loop in array
  {
    if (morse_input == morse_code_array[array_search_counter])//looking for match
    {
      return array_search_counter;//return index where it matched
    }
  }
}
//######################################
String get_morse_code()
{
  for (; current_time - pervious_time != 6000;)//wait 6000ms until full letter been entered
  {
    int meg_sen_read = analogRead(MEG_SEN);//keep getting value of btn
    if (meg_sen_read < 600)
      meg_sen_read = 1;
    else  if (meg_sen_read >= 600)
      meg_sen_read = 0;
    if (meg_sen_read == 1)
    {
      time_pressed = 0;
      while (meg_sen_read == 1) //still btn pressed
      {
        time_pressed += 1;
        delay(1);
        meg_sen_read = analogRead(MEG_SEN);
        if (meg_sen_read < 600)
          meg_sen_read = 1;
        else  if (meg_sen_read >= 600)
          meg_sen_read = 0;
      }
      if (time_pressed <= DOT_DURATION)//means dot
      {
        //Serial.print('.');
        MORSE_CODE += '.';
      }
      else if (time_pressed > DOT_DURATION)//means dash
      {
        //Serial.print('-');
        MORSE_CODE += '-';
      }
    }
    current_time = millis();//update current time
  }
  //Serial.println(current_time - pervious_time);
  return MORSE_CODE ; //full code
}
//#######################################
void print_alph_num_msg()
{
  if (MORSE_CODE != "")//not empty
  {
    int char_index = get_index(MORSE_CODE);
    char alph_num_char = alph_num_collection[char_index];//encoded char
    message += alph_num_char;// append it tp msg

    Serial.print(alph_num_char);//send letter to py
    Serial.println();
    lcd.print(alph_num_char);
    column += 1; // increase number of column

    if (column == 16 && row == 0) // end of first row
    {
      column = 0;
      row = 1;
      lcd.setCursor(column, row);
    }
    if (column == 16 && row == 1) // end of second row
    {
      column = 0;
      row = 0;
      lcd.setCursor(column, row);
    }
    MORSE_CODE = "";//clear morse
  }
  if (message != "")//if we have at least 1 char
  {
    int waitting_time = 0;
    while (meg_sen_read == 0) //still btn pressed
    {
      waitting_time += 1;
      delay(1);
      meg_sen_read = analogRead(MEG_SEN);
      if (meg_sen_read < 600)
        meg_sen_read = 1;
      else  if (meg_sen_read >= 600)
        meg_sen_read = 0;
      if (waitting_time >= SPACE_DURATION)//means full word have been written
        break;
    }
    if (waitting_time >= SPACE_DURATION)//means full word have been written
    {
      message = "";
      Serial.print(' ');
      Serial.println();
      lcd.print(' ');
      column += 1;
      if (column == 16 && row == 0) //end of second row
      {
        column = 0;
        row = 1;
        lcd.setCursor(column, row);
      }
      if (column == 16 && row == 1) // end of second row
      {
        column = 0;
        row = 0;
        lcd.setCursor(column, row);
      }
    }
  }
}

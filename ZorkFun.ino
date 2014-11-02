#include <avr/pgmspace.h>

/*********************************
These are the scenes for the game
they're stored and retrieved from
program memory to avoid SRAM bugs
*********************************/
prog_char a[] PROGMEM = "0010zzzThis room contains IKEA   desks and Apple computers.This must be Marketing andcommunications. Sweet!";
prog_char b[] PROGMEM = "0010EzzYou find yourself standingin Nate's office. His pen is laying on the desk but He's nowhere to be found.";
prog_char c[] PROGMEM = "0010AzzThe dark room is the IT   Den. No one is around at  the moment. You step on a Keycard that someone has dropped";
prog_char d[] PROGMEM = "1111zzzThis room doesn't exist.  Are you the one? Neo?"; //NonRoom
prog_char e[] PROGMEM = "0010zzzThis appears to be the    Customer Service office   Gordon is in the middle ofa joke. FOB is on the     phone.";
prog_char f[] PROGMEM = "1110zzzThis is the end of the    hallway. There's a door tothe North and the hall    continues around the      corner.";
prog_char g[] PROGMEM = "1101zzzYou're standing in a hall-way. There's a door to theNorth.";
prog_char h[] PROGMEM = "1101zzzYou're standing in front  of a set of restrooms. Theroom to your North is dark";
prog_char i[] PROGMEM = "0111zzzThere's nothing much goingon in this hallway. The   door in front of you has  a sign indicating it's    broken.";
prog_char j[] PROGMEM = "1001zzzThis is the Tech Support  office. Single T is on thephone and Toni_K is tryingto fix a code bug.";
prog_char k[] PROGMEM = "1110HAAYou're in an entryway witha shaded atrium to your   West. To your East is a   door with a card reader.";
prog_char l[] PROGMEM = "0001BzzYou scan your card and    enter the server room.You notice a Pintglass on the ground. The room hums with rack fans.";
prog_char m[] PROGMEM = "0100zDDYou find yourself in the  Catalog Control office.   NPoole tells you he could stop the robots if he had a Redboard.";
prog_char n[] PROGMEM = "1111JzzYou're standing in an     entryway with an atrium toyou're East. It looks likethe door has a lock on the outside.";
prog_char o[] PROGMEM = "0001zzzYou walk through the door into a sun-lit atrium.    There are bikes on the    wall below.";
prog_char p[] PROGMEM = "1110zzzYou walk into an empty    hallway. There's a door toyour East";
prog_char q[] PROGMEM = "0001zzzYou open the door and findyourself faced with a rockclimbing wall. This is the'gun factory' workout room";
prog_char r[] PROGMEM = "0110ICCYou're in the classroom.  There's an unfinished SFE RedBoard microcontroller  on one of the tables.";
prog_char s[] PROGMEM = "1111zzzYou're standing in a hall-way. There's a door to theWest";
prog_char t[] PROGMEM = "0001FGGThis is the Engineering   bullpen. You meet Pete    Dokter who is wearing a   kilt and Soldering at a   workbench.";
prog_char u[] PROGMEM = "1100zBBYou're standing in the    breakroom, there is a pingpong table and a kegeratorThere's a hallway to your North and East.";
prog_char v[] PROGMEM = "0101zzzYou're looking at a wall  of employee pictures.Thereare 130 or so (wow.)";
prog_char w[] PROGMEM = "1101zzzThe restroom doors aren't very interesting,but thereis a door to your North.";
prog_char x[] PROGMEM = "1101zzzThe Entryway here is quietThere's a GPS Clock on thewall. The conference room to the East is labeled    'Rick James'";
prog_char y[] PROGMEM = "0001zzzYou're standing in the    Rick James conference rooma large window overlooks alight flooded atrium.";

PROGMEM const char *scene_table[5][5] =// Scene lookup table
{   
  {a, b, c, d, e},
  {f, g, h, i, j},
  {k, l, m, n, o},
  {p, q, r, s, t},
  {u, v, w, x, y}
};

/*Some other in-game text with also be stored in PROGMEM... apparently.*/

prog_char one[] PROGMEM = "You walk out into a sunlitAtrium. The door shuts andlocks behind you. You're  attacked by a robot and   die. Game Over.";
prog_char two[] PROGMEM = "You swipe the key card and the door unlocks. You walk in.";
prog_char three[] PROGMEM = "You pull a cool,refreshing draft from the tap.";
prog_char four[] PROGMEM = "You repair the RedBoard   and take it with you."; //NonRoom
prog_char five[] PROGMEM = "You and NPoole leave      torward the exit to stop  the robot apocolypse but  you're attacked.";
prog_char six[] PROGMEM = "As the robot strikes down NPoole you remember the   pen on Nate's desk. The   robot wins. You die.";
prog_char seven[] PROGMEM = "Luckily, you have a pen   which you throw at the    robot, thus disabling it. The human race lives on. You Win.";
prog_char eight[] PROGMEM = "You hand Pete the cool and refreshing beer. He gives you the soldering iron.";
prog_char nine[] PROGMEM = "";

PROGMEM const char *text_table[3][3] =// Scene lookup table
{   
  {one,two,three},
  {four,five,six},
  {seven,eight,nine}
};

char buffer[150]; //PROGMEM BUFFER

boolean wait = 1; //Input Flag
String action; //First word from parser
String object; //Second word from parser
String commandPass; //Global dropbox for input

String scene; //Scene string
String text; //In-Game Text String
int xco=4; //Coordinate space
int yco=0;

char ndir; //Move permissions
char edir;
char sdir;
char wdir;

char item; //Item present
char u1; //Item use permission
char u2; //Item effect code

String inventory = "xxxxx"; //5 Item Inventory String

void setup() {

Serial.begin(300); //Pour a bowl of Serial
  
//Don't start program until user hits a key
while(!Serial.available()){}
Serial.read();//flush

//Title Screen
Serial.print("Welcome to ZorkFun 2012   "); delay(500);
Serial.print("The RTFMTastic Text-Based "); delay(500);
Serial.print("Adventure Game            "); delay(500);
Serial.print("++++++++++++++++++++++++++"); delay(500);
Serial.print("Press Any Key");delay(500);
emptyLine();

//Wait on Key
while(!Serial.available()){}
Serial.read();//flush

delay(1000);
  clearLCD();  
  Serial.print("                         "); delay(500);
    refreshScreen();
    
delay(1000);
}

void loop() {
  delay(1000);
  /*******************************
          INPUT MECHANISM
  *******************************/
  
  commandPass = commandPass.substring(0,0); //setup input 
  
  while(wait == 1){
  char in; 
  while(!Serial.available()){}; //wait on key
  int i = commandPass.length(); //find line length
  backLCD(i); //back dat ass up
  in = Serial.read(); //get key
  if(in == 96){interpret(); wait = 0; break;}//interpret command if RTN
  if(in != 8){commandPass += in;}
  Serial.print(commandPass);}
  wait = 1;
  
  if(action == "quit"){escape();} //check for escape code

    refreshScreen();
    
}

/***************************
LCD and game function stuff
***************************/


void escape(){
  Serial.print("~");}
  
void lineFeed(){
 Serial.write(byte(0)); 
}

void clearLCD(){
 Serial.write(byte(96)); 
}

void emptyLine(){
 Serial.print("                          "); delay(500);}
 
void backLCD(int i){

  for(int b = 0; b<i; b++){
   Serial.write(0x08); //backspace
   delay(10);
  }
}
 
void refreshScreen(){
   clearLCD();
   Serial.print("                         "); delay(500);
   getScene(xco, yco); //grab the scene 

  
  /******************************
         MOVE PERMISSIONS
    the first 4 characters are
    always switches which tell
    the motion engine whether
    or not you can move in a 
    certain direction.
  ******************************/
  
  ndir = scene.charAt(0); 
  edir = scene.charAt(1);
  sdir = scene.charAt(2);
  wdir = scene.charAt(3);
  
  /*****************************
            ITEM FLAG  
  the fifth character is always
  a character which represents 
  an object present in the room.
  These are identified by the 
  item parser. Items may also be
  NPCs.
  ******************************/
  
  item = scene.charAt(4);
  
  /******************************
       ITEM USE AND EFFECT
  the sixth and seventh character
  represent the permission to use
  an item in a particular location
  and the effect lookup code that
  is associated with that use.
  ******************************/
  
  u1 = scene.charAt(5);
  u2 = scene.charAt(6);
  
  /******************************
            THE SCENE
     the rest of the string is
     the 'scene' or the text
     displayed to the user
     discribing the current
     room.
  ******************************/
   
  scene = scene.substring(7);
  
  Serial.print(scene); //show the scene 
  lineFeed();
 
} 


/*******************************
            Parser
*******************************/

void interpret()
{
  
  lineFeed();
  
  int cmdIndex = commandPass.indexOf(' '); //read up to frist space for command name
  action = commandPass.substring(0,cmdIndex); //keep command name
  object = commandPass.substring(cmdIndex); //the rest is the argument
  String argTest = commandPass.substring(cmdIndex, cmdIndex+1); //see if there is an argument




/*******************************
       ACTION GENERATORS
  WARNING: 
  MATRYOSHKA FUNCTIONS AHEAD
*******************************/


  //"MOVE" ACTION
  if(action=="move" /*|| action=="Move" || action=="MOVE"*/){
    
    if(/*object==" North" || */object==" north" ||/* object==" NORTH" || object==" N" ||*/ object==" n"){
      if(ndir=='0'){clearLCD(); Serial.print("I'm afraid you can't go North"); delay(3000); return;}
      if(ndir=='1'){xco=xco-1; return;}}
    if(/*object==" East" || */object==" east" || /*object==" EAST" || object==" E" ||*/ object==" e"){
      if(item=='H'){clearLCD(); Serial.print("You try the door but it's locked. There's a card reader."); delay(3000); return;}
      if(edir=='0'){clearLCD(); Serial.print("I'm afraid you can't go East"); delay(3000); return;}
      if(edir=='1'){yco=yco+1; return;}}
      if(/*object==" South" ||*/ object==" south" ||/* object==" SOUTH" || object==" S" ||*/ object==" s"){
      if(sdir=='0'){clearLCD(); Serial.print("I'm afraid you can't go South"); delay(3000);return;}
      if(sdir=='1'){xco=xco+1; return;}}
    if(/*object==" West" || */object==" west" || /*object==" WEST" || object==" W" */ object==" w"){
      if(item=='J'){ boolean match = 0;
      for(int q=0; q<=4; q++){if(inventory.charAt(q)=='A'){match=1;}}
   
   if(match==1){yco=yco-1; return;}
   
   if(match==0){clearLCD(); getText(0,0); Serial.print(text); delay(10000); escape();}}
      if(wdir=='0'){clearLCD(); Serial.print("I'm afraid you can't go West"); delay(3000);return;}
      if(wdir=='1'){yco=yco-1; return;}}
  clearLCD();
  delay(3000);
  Serial.print("That's not a direction I recognize.");
  return;}
  
  //"TAKE" ACTION
  if(/*action=="Get" || action=="GET" ||*/ action=="get" ||/* action=="Take" || action=="TAKE" ||*/ action=="take" ||/* action=="GRAB" || action=="Grab" ||*/ action=="grab"){
  if(item=='F'){clearLCD(); Serial.print("Pete says he'll trade you a beer for it."); delay(3000); return;}  
  if(item=='I'){clearLCD(); Serial.print("This is no good right now"); delay(3000); return;}  
  if(itemLookup(object)==item){inventory.setCharAt(inventory.indexOf('x'), item); clearLCD(); delay(3000); Serial.print("You grabbed the "); Serial.print(object); Serial.print(". Nice."); return;}
     else{clearLCD(); delay(3000); Serial.print("I don't see a "); Serial.print(object); Serial.print(" worth taking. Do you?"); return;}}
     
  //"USE" ACTION
  if(/*action=="Use" || action=="USE" ||*/ action=="use" || /*action=="Give" || */action=="give"/* || action=="GIVE"*/ ){
     boolean match = 0;
   
 for(int q=0; q<=4; q++){if(inventory.charAt(q)==itemLookup(object)){match=1;}}
   
   if(match==0){clearLCD(); delay(3000); Serial.print("You don't have a "); Serial.print(object); return;}
   
   if(match==1){
    
    if(itemLookup(object)!=u1){clearLCD(); delay(3000); Serial.print("You can't use that here."); return;} 
    if(itemLookup(object)==u1){effectEngine(u2); delay(3000); return;}
  
   }
    }
  /*******************************
            THE BIG ELSE
  *******************************/  
    
  clearLCD();
 delay(1000); 
 Serial.print("I don't understand "); //"I don't understand fuck, sorry."
 delay(500);
 Serial.print(action);
 delay(500);
 Serial.print(", sorry.");
 delay(3000);
  return; 
}

/*******************************
         Scene Lookup
*******************************/

void getScene(int x, int y){
  
  strcpy_P(buffer, (char*)pgm_read_word(&(scene_table[x][y])));
  scene = String(buffer);
  return;
}

/*******************************
          Item Lookup
     for 5 item inventory
*******************************/

char itemLookup(String obj){
 
 if(/*obj == " Card" || obj == " card" || obj == " Key Card" || obj == " key card" || obj == " KeyCard" || */obj == " Keycard"){delay(1000); return 'A';}
 if(obj == " Pintglass" /*|| obj == " Pint glass" || obj == " pint glass" || obj == " pintglass" || obj == " Pint Glass" || obj == " glass" || obj == " Glass"*/){delay(1000); return 'B';}
 if(obj == " Soldering Iron" || /*obj == " soldering iron" || obj == " Soldering iron" ||*/ obj == " Iron" || obj == " iron"){delay(1000); return 'C';}
 if(/*obj == " Redboard" || obj == " Red board" || */obj == " RedBoard"/* || obj == " Red Board" || obj == " board" || obj == " Board"*/){delay(1000); return 'D';}
 if(obj == " Pen" || obj == " pen"){delay(1000); return 'E';}
 if(obj == " Beer" || obj == " beer"){delay(1000); return 'G';}
 
 delay(1000); return 'z';
}

/********************************
         Effect Engine
********************************/

void effectEngine(char effect){
 
 switch(effect){
   
  case 'A':
  clearLCD(); getText(0,1); Serial.print(text); delay(3000); xco=2; yco=1;return;
  break; 
  case 'B':
  clearLCD(); getText(0,2); Serial.print(text); delay(2000); inventory.setCharAt(inventory.indexOf('B'), 'G');Serial.print("Beer added to inventory"); delay(2000); return;
  break; 
  case 'C':
  clearLCD(); getText(1,0); Serial.print(text); delay(2000); inventory.setCharAt(inventory.indexOf('x'), 'D');Serial.print("Redboard added to inventory"); delay(2000); return;
    break; 
  case 'D':
  {clearLCD();
  getText(1,1); Serial.print(text); delay(4000);
  boolean match = 0;
   
  for(int q=0; q<=4; q++){if(inventory.charAt(q)=='E'){match=1;}}
   
   if(match==0){clearLCD();delay(3000); getText(1,2); Serial.print(text); escape();}
   
   if(match==1){clearLCD();delay(3000); getText(2,0); Serial.print(text); delay(10000); escape();}
  }
  break; 
  case 'G':
  clearLCD(); getText(2,1); Serial.print(text); delay(4000); inventory.setCharAt(inventory.indexOf('x'), 'C');Serial.print("Iron added to inventory"); delay(2000); return;
  break;
  
 }
  return;
}

/********************************
      In-Game Text Lookup
********************************/
void getText(int x, int y){
  
  strcpy_P(buffer, (char*)pgm_read_word(&(text_table[x][y])));
  text = String(buffer);
  return;
}


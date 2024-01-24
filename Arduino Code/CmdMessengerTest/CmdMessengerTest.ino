#include <CmdMessenger.h>  // CmdMessenger

// Blinking led variables 
unsigned long previousToggleLed = 0;   // Last time the led was toggled
bool ledState                   = 0;   // Current state of Led
const int kBlinkLed             = 13;  // Pin of internal Led


// set up the field seperators and command seperators.  
char field_separator = ',';
char command_separator = '#';

char *nameTeamA = "Team A";
char *nameTeamB = "Team B";

char teamAScore[2] = "00";
char teamBScore[2] = "00";
char minutes[2] = "00";
char seconds[2] = "00";
char halftime[2] = "00";


// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);


// This is the list of recognized commands. These can be commands that can either be sent or received. 
// In order to receive, attach a callback function to these events
enum
{
  // Commands
  kAcknowledge         , // Command to acknowledge that cmd was received          COMMAND 0
  kError               , // Command to report errors                              COMMAND 1
  kSetTeamName         , // Command to set the Team Name for LED-Matrix           COMMAND 2 
  kUpdateScoreboard    , // Command to update goals, halftimem minute and seconds COMMAND 3
};

// Commands we send from the PC and want to receive on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below.

void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSetTeamName, setTeamName);
  cmdMessenger.attach(kUpdateScoreboard, updateScoreboard);
}

// ------------------  C A L L B A C K S -----------------------

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,"Command without attached callback");
}

// Callback function that responds that Arduino is ready (has booted up)
void OnArduinoReady()
{
  cmdMessenger.sendCmd(kAcknowledge,"Arduino ready");
}

void setTeamName()
{
  int team = cmdMessenger.readInt16Arg();
  if (team == 0) {
    char *nameTeamA = cmdMessenger.readStringArg();
    cmdMessenger.sendCmd(kAcknowledge,"Received Command to change Team A Name");
    cmdMessenger.sendCmd(kAcknowledge,nameTeamA);
  } else {
    char *nameTeamB = cmdMessenger.readStringArg();
    cmdMessenger.sendCmd(kAcknowledge,"Received Command to change Team B Name");
    cmdMessenger.sendCmd(kAcknowledge,nameTeamB);
  }
}


void updateScoreboard()
{
    //String buf;
    char *buf;
    uint8_t i;
    int halftime;
    char tempString[8] = { '\0' }; 
    buf = cmdMessenger.readStringArg(); //Read TeamA Score
    strncpy(tempString, buf, 2);
    buf = cmdMessenger.readStringArg(); //Read TeamB Score
    strncat(tempString, buf, 2);
    buf = cmdMessenger.readStringArg(); //Read Minutes
    strncat(tempString, buf, 2);
    buf = cmdMessenger.readStringArg(); //Read Seconds
    strncat(tempString, buf, 2);
    halftime = cmdMessenger.readInt16Arg(); //Read halftime
    cmdMessenger.sendCmd(kAcknowledge, "Scoreboard-String");
    cmdMessenger.sendCmd(kAcknowledge, tempString);
    cmdMessenger.sendCmd(kAcknowledge, "Halftime");
    cmdMessenger.sendCmd(kAcknowledge, halftime);

    //polpoShifter.display(tempString);
}

// ------------------ M A I N  ----------------------

// Setup function
void setup() 
{
  // Listen on serial connection for messages from the pc
  Serial.begin(115200); 

  // Adds newline to every command
  cmdMessenger.printLfCr();   

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();

  // Send the status to the PC that says the Arduino has booted
  cmdMessenger.sendCmd(kAcknowledge,"Arduino has started!");

  // set pin for blink LED
  pinMode(kBlinkLed, OUTPUT);
}

// Returns if it has been more than interval (in ms) ago. Used for periodic actions
bool hasExpired(unsigned long &prevTime, unsigned long interval) {
  if (  millis() - prevTime > interval ) {
    prevTime = millis();
    return true;
  } else     
    return false;
}

// Loop function
void loop() 
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();

  // Toggle LED periodically. If the LED does not toggle every 2000 ms, 
  // this means that cmdMessenger are taking a longer time than this  
  if (hasExpired(previousToggleLed,2000)) // Toggle every 2 secs
  {
    toggleLed();  
  } 
}

// Toggle led state 
void toggleLed()
{  
  ledState = !ledState;
  digitalWrite(kBlinkLed, ledState?HIGH:LOW);
}  

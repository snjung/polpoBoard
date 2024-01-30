// Commands, the Raspberry will send to the Arduino
// Command to Update the Scoreboard (CMD-code 004):     4,TeamAScore,TeamBScore,TimeMinutes,TimeSeconds#

// Shifter library available from http://www.proto-pic.com/Resources/shifter.zip
#include <polpoShifter.h>

#include <polpoDisplay.h>

// CmdMessenger library available from https://github.com/dreamcat4/cmdmessenger
#include <CmdMessenger.h>

// Base64 library available from https://github.com/adamvr/arduino-base64
#include <Base64.h>

// Streaming4 library available from http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

// set up the field seperators and command seperators.  
char field_separator = ',';
char command_separator = '#';

// set up the chharacter arrays we will use later
char teamAScore[2] = { '\0' };
char teamBScore[2] = { '\0' };
char minutes[2] = { '\0' };
char seconds[2] = { '\0' };
char quarter[2] = { '\0' };

// set up the headings 
char teamAHeading[] = "TeamA";
char teamBHeading[] = "TeamB";
char minutesHeading[] = "Minuten:";
char secondsHeading[] = "Sekunden:";
char quarterHeading[] = "Halbzeit:";

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// dreamcat4 default
// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// We can define up to a default of 50 cmds total, including both directions (send + recieve)
// and including also the first 4 default command codes for the generic error handling.
// If you run out of message slots, then just increase the value of MAXCALLBACKS in CmdMessenger.h

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  // kICE_CREAM_READY,
  // kICE_CREAM_PRICE,
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Must not delete this line
};

// set up the shifter object
int confirmationPin = 6;
Shifter polpoShifter(8, confirmationPin);  // 8 digits (4x2), confirmation PIN to be discussed

void update_scoreboard()
{
  while ( cmdMessenger.available() )
  {
    
    // lets assume we always get the correctly formatted string
    // its lazy, but should be ok
    
    char buf[2] = { '\0' };             // Initiate a 4 byte buffer to hold the data
    char tempString[8] = { '\0' }; 
    
    // Team A Score
    cmdMessenger.copyString(buf, 2);
    strcpy(teamAScore,buf);
    cmdMessenger.sendCmd(kACK, teamAHeading);
    cmdMessenger.sendCmd(kACK, teamAScore);
    
    // Team B Score
    memset(buf, '\0', 2);               // Erase the buffer content
    cmdMessenger.copyString(buf, 2);
    strcpy(teamBScore,buf);
    cmdMessenger.sendCmd(kACK, teamBHeading);
    cmdMessenger.sendCmd(kACK, teamBScore);
    
    // Minutes
    memset(buf, '\0', 2);
    cmdMessenger.copyString(buf, 2);
    strcpy(minutes, buf);
    cmdMessenger.sendCmd(kACK, minutesHeading);
    cmdMessenger.sendCmd(kACK, minutes);
    
    // Seconds
    memset(buf, '\0', 2);
    cmdMessenger.copyString(buf, 2);
    strcpy(seconds, buf);
    cmdMessenger.sendCmd(kACK, secondsHeading);
    cmdMessenger.sendCmd(kACK, seconds);

    // Quarter
    memset(buf, '\0', 1);
    cmdMessenger.copyString(buf, 1);
    strcpy(quarter, buf);
    cmdMessenger.sendCmd(kACK, quarterHeading);
    cmdMessenger.sendCmd(kACK, quarter);


    strcpy(tempString,teamAScore);
    strcat(tempString,teamBScore);
    strcat(tempString,minutes);
    strcat(tempString,seconds);

    cmdMessenger.sendCmd(kACK, tempString);
    
    polpoShifter.display(tempString);
    
    
  }
}

void test_mode()
{
    char buf[] = { "Test Mode" };
    cmdMessenger.sendCmd(kACK, buf);
    polpoShifter.display("11122233");
}

// default dreamcat4 stuff

// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  update_scoreboard,            // 004
  test_mode,                    // 005
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup()
{
  // Listen on serial connection for messages from the pc
  Serial.begin(57600); // Arduino Uno, Mega, with AT8u2 USB

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  
  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  attach_callbacks(messengerCallbacks);

  arduino_ready();
  
  // Set the displays to 000 on boot
  //shifterSet1.display("000000000");
  //shifterSet2.display("000000");
  polpoShifter.display("--0--0--0");
}

// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 2000; // 2 seconds
long previousMillis = 0;
int counter = 0;

void timeout()
{
  // blink
  if (counter % 2)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
  counter ++;
}  

void loop()
{
  
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();

  // handle timeout function, if any
  if (  millis() - previousMillis > timeoutInterval )
  {
    timeout();
    previousMillis = millis();
  }
  
}

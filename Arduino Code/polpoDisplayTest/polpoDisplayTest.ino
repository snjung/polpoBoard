#include <polpoDisplay.h>

#include <CmdMessenger.h>

char *msg = "";
char *msg2 = "";


char field_separator = ',';
char command_separator = '#';

polpoDisplay *polpoDisplay1;
polpoDisplay *polpoDisplay2;

CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);



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

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"Arduino ready");
}

void update_LED_matrix()
{
  while ( cmdMessenger.available() )
    {
        char value = cmdMessenger.readCharArg();
        cmdMessenger.sendCmd(kACK, value);
        msg = value;
    }
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  update_LED_matrix1,           // 004
  test_mode,                    // 005
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')

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

void setup(void) {
  polpoDisplay1 = new polpoDisplay(9);
  polpoDisplay2 = new polpo Display(10);
  Serial.begin(57600);
  // initialize digital pin LED_BUILTIN as an output.
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);
  attach_callbacks(messengerCallbacks);
  arduino_ready();
}

// the loop function runs over and over again forever
void loop(void) {
  cmdMessenger.feedinSerialData();
  polpoDisplay1 ->display(msg);        // wait for a second
  polpoDisplay2 ->display(msg2);        // wait for a second
}

#include <keyboardDriver.h>

#define BUFFER_LENGTH 32 

  static char keys[] = {
    0,  //Error
    27,  //Escape
    '1','2','3','4','5','6','7','8','9','0','-','=',
    8,  //Backspace
    '\t',  //Tab
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',  //Enter
    0,  //LControl
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  //LShift
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,  //RShift
    '*',
    0,  //LAlt
    ' ',
    0,  //CapsLock
    0,0,0,0,0,0,0,0,0,0,     //F1-F10
    0,  //NumLock
    0,  //ScrollLock
    '7','8','9','-','4','5','6','+','1','2','3','0','.'
};

static char capKeys[] = {
    0,  //Error
    27,  //Escape
    '1','2','3','4','5','6','7','8','9','0','-','=',
    8,  //Backspace
    '\t',  //Tab
    'Q','W','E','R','T','Y','U','I','O','P','[',']',
    '\n',  //Enter
    0,  //LControl
    'A','S','D','F','G','H','J','K','L',';','\'','`',
    0,  //LShift
    '\\','Z','X','C','V','B','N','M',',','.','/',
    0,  //RShift
    '*',
    0,  //LAlt
    ' ',
    0,  //CapsLock
    0,0,0,0,0,0,0,0,0,0,     //F1-F10
    0,  //NumLock
    0,  //ScrollLock
    '7','8','9','-','4','5','6','+','1','2','3','0','.'
};

static char shiftCapKeys[] = {
    0,  //Error
    27, //Escape
    '!','@','#','$','%','^','&','*','(',')','_','+',
    8,  //Backspace
    '\t',
    'q','w','e','r','t','y','u','i','o','p','{','}',
    '\n',   //Enter
    0,  //LControl
    'a','s','d','f','g','h','j','k','l',':','\"',
    '~',
    0,  //LShift
    '|',
    'z','x','c','v','b','n','m','<','>','?',
    0,  //RShift
    '*',
    0,  //LAlt
    ' ',
    0,  //CapsLock
    0,0,0,0,0,0,0,0,0,0,    //F1-F10
    0,  //NumLock
    0,  //ScrollLock
    '7','8','9','-','4','5','6','+','1','2','3','0','.'
};

static char shiftedKeys[] = {
    0,  //Error
    27, //Escape
    '!','@','#','$','%','^','&','*','(',')','_','+',
    8,  //Backspace
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}',
    '\n',   //Enter
    0,  //LControl
    'A','S','D','F','G','H','J','K','L',':','\"',
    '~',
    0,  //LShift
    '|',
    'Z','X','C','V','B','N','M','<','>','?',
    0,  //RShift
    '*',
    0,  //LAlt
    ' ',
    0,  //CapsLock
    0,0,0,0,0,0,0,0,0,0,    //F1-F10
    0,  //NumLock
    0,  //ScrollLock
    '7','8','9','-','4','5','6','+','1','2','3','0','.'
};

static uint8_t buffer[BUFFER_LENGTH];
static unsigned int realDim=0,last=0;
static int shift=0, capsLock=0, control=0, alt=0;

static void add(char key);

void keyboard_handler(){
    if (! read_port(0x64) & 0x01)
      return;
    uint16_t key=read_port(0x60);
    //Check if key was pressed
    char pressed;
    key = key & 0x7F;
    switch (key){
      case 0x3a:
        capsLock = !capsLock;
        return;
        break;
      case 0x2a:
      case 0x36:
        shift = 1;
        return;
        break;
      case 0x38:
        alt = 1;
        return;
        break;
      default:
        break;
    }
    if (capsLock && !shift)
      pressed = capKeys[key];
    else if (capsLock && shift)
      pressed = shiftCapKeys[key];
    else if (!capsLock && !shift)
      pressed = keys[key];
    else
      pressed = shiftedKeys[key];
    add(pressed);
}

static void add(char key){
    buffer[realDim] = key;
    if (realDim+1 == BUFFER_LENGTH){
        realDim = 0;
        last = -1;
    }
}

uint64_t readBuffer(char* output, uint64_t count){
    uint64_t i = 0;
    for (; i < count && last < realDim && last < BUFFER_LENGTH; i++)
    {
        output[i] = buffer[last++];
    }
    if (last == BUFFER_LENGTH){
        realDim = last = 0;
    }
    return i;    
}
#ifndef  __TBL_OUTMESSAGE__
#define  __TBL_OUTMESSAGE__

#include <string>



enum OUTMESSAGE_LOCAL {
	LOCAL_JP = 0,
	LOCAL_EN,
	LOCAL_TEAM
};

void MessageInit();

void ConsoleMessage(std::ostream& dest, const char* string);
void ConsoleOutMessage(std::string MessageNumber, ...);
void ConsoleErrMessage(std::string MessageNumber, ...);


#endif
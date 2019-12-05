#ifndef  __TBL_OUTMESSAGE__
#define  __TBL_OUTMESSAGE__

#include <string>



enum OUTMESSAGE_LOCAL {
	LOCAL_JP = 0,
	LOCAL_EN,
	LOCAL_TEAM
};


void MessageInit();

void ConsoleOutMessage(std::string MessageNumber, ...);


#endif
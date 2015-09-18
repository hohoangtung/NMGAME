
// HOW TO USE
/*
	use operator += to regist an event
	use operator -= to remove an event
	chose where active event by using call fireEvent

	you have to explicit cast to (EventFunction) when use operator += and -=
	example:
		Event myevent;
		myevent += (EventFunction) &FUNCTION;	// must have reference (&)

	FUNCTION have to have prototype:
	void <FunctionName> (EventArg* e);			or
	void <FunctionName> (XXXEventArg* e);		// must use pointer in argument
										// because using pointer, you should check if e == NULL in define of FucntionName 
	with: XXXEventArg is inheritanced from EventArg and have some argument if you wish
		you can define argument class when you use.
		class XXXEventArg : public EventArg
		{
			public:
				XXXEventArg([type] arg1, [type])
				{
					this->_arg1 = arg1;
					this->_arg2 = arg2;
				}
				[type]	_arg1;
				[type]	_arg2;
				...
		}
	
	when call fireEvent you can call
		myevent.fireEvent(NULL);
		myevent.fireEvent(new EventArg());
		myevent.fireEvent(new XXXEventArg(x1,x2));
*/
#ifndef __EVENT_H__
#define __EVENT_H__
#include <vector>
using namespace std;
#include "define.h"
#include <functional>
NS_FRAMEWORK
NS_FRAMEWORK_BEGIN
class EventArg;
typedef void(*EventFunction)(EventArg*);

class Event
{
public:
	Event();
	virtual ~Event();
	virtual void fireEvent(EventArg* e);			// active an event - kích hoạt sự kiện
	void operator+= (EventFunction pFunction);	// regist an action - đăng kí sự kiện
	void operator-= (EventFunction pFucntion);	// remove an action - huỷ sự kiện
private:
	vector<EventFunction> _listaction;
};
typedef Event* pEvent;
class EventArg
{
public:
	EventArg(){}
	virtual ~EventArg(){}
};
NS_FRAMEWORK_END
#endif // !__EVENT_H__

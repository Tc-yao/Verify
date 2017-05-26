#ifndef _MACRO_H_
#define _MACRO_H_
#include <functional>
#include <exception>
#include <process.h>
#include <windows.h>
//资源管理
namespace macro
{
	class CAutoSection
	{
	public:
		CAutoSection(CRITICAL_SECTION &cs):m_csLock(cs){EnterCriticalSection(&m_csLock);}
		virtual ~CAutoSection(void){LeaveCriticalSection(&m_csLock);}
	private:
		CRITICAL_SECTION &m_csLock;
	};
	class CAutoConsole
	{
	public:
		CAutoConsole(){
			AllocConsole();
			freopen("CONIN$", "r+t", stdin);
			freopen("CONOUT$", "w+t", stdout);
		}
		~CAutoConsole(){
			fclose(stdin);
			fclose(stdout);
			FreeConsole();
		}

	private:

	};

	class ScopeGuard
	{
	public:
		explicit ScopeGuard(std::function<void()> onExitScope,bool dismissed = false)
			: onExitScope_(onExitScope), dismissed_(dismissed)
		{ }
		ScopeGuard(){dismissed_ = true;}
		ScopeGuard(ScopeGuard &SG)
		{
			onExitScope_ = SG.onExitScope_;
			dismissed_ = SG.dismissed_;
		}

		~ScopeGuard()
		{
			if(!dismissed_)
			{
				onExitScope_();
			}
		}

		void Replace(std::function<void()> onExitScope)
		{
			onExitScope_ = onExitScope;
		}

		void Dismiss(bool dismissed = true)
		{
			dismissed_ = dismissed;
		}

	private:
		std::function<void()> onExitScope_;
		bool dismissed_;

	private: // noncopyable
		//ScopeGuard(ScopeGuard const&);
		//ScopeGuard& operator=(ScopeGuard const&);
	};

#define LINENAME_CAT(name, line) name##line
#define LINENAME(name, line) LINENAME_CAT(name, line)
#define NOP(nop_size) BYTE LINENAME(nop,__LINE__)[nop_size]
#define MUTEXLOCK(X) CAutoMutex LINENAME(amt,__LINE__)(X)
#define ON_SCOPE_EXIT(callback) ScopeGuard LINENAME(EXIT, __LINE__)(callback)
#define SCOPE_EXIT(callback) ScopeGuard LINENAME(EXIT, __LINE__)([&](){callback;})
#define SCOPE_EXIT_CHANGE(callback) g_sgCall.Replace([&](){callback})
#define COPY_SCOPE_EXIT(SG) ScopeGuard LINENAME(EXIT, __LINE__)(SG)
#define COPY_SCOPE_EXIT_AND_UNDIS(SG) ScopeGuard LINENAME(EXIT, __LINE__)(SG);\
	LINENAME(EXIT, __LINE__).Dismiss(false)
}

#define HOOKAPIPRE(fun) auto true##fun = fun
#define HOOKAPI(fun) Mhook_SetHook((LPVOID *)& true##fun, my##fun)
#define HOOKAPINEW(fun) decltype(fun) my##fun
//调试输出
namespace macro
{
#ifdef OutputDebugString
#undef OutputDebugString 
#endif
#ifdef _TESTVERTION//测试模式
	void PrintMsg(__in_opt LPCWSTR lpOutputString);
	void PrintMsg(__in_opt LPCSTR lpOutputString);
	void OutputDebugString(__in_opt CStringW lpOutputString);
	void OutputDebugString(__in_opt CStringA lpOutputString);
	void OutputDebugString(__in_opt LPCWSTR lpOutputString);
	void OutputDebugString(__in_opt LPCSTR lpOutputString);
#else
#define OutputDebugString(X)
#define PrintMsg(X)
#endif

#define ASSERT_AND_RETURN(expr) if(expr);\
								else\
								{ASSERT(0);return;}
};
//错误处理
namespace macro{

class Assert
{
public:
	Assert& SMART_ASSERT_A;
	Assert& SMART_ASSERT_B;
	Assert(const char *expression):SMART_ASSERT_A(*this),SMART_ASSERT_B(*this){
		_my_msg = new char[strlen(expression)+1];
		strcpy(_my_msg,expression);
	}
	//whatever member functions
	Assert& print_current_val(bool val,const char* value){

	}
	//打印文本 暂时默认为输出到_MyMsg
	void print_text(const char *text){
		if(0==text || strlen(text)==0) return;
		char *new_msg = new char[strlen(_my_msg)+strlen(text)+2];
		sprintf(new_msg,"%s\n%s\0",_my_msg,text);
		delete []_my_msg;
		_my_msg = new_msg;
	}
	~Assert(){if(_my_msg) delete []_my_msg;}
private:
	char * _my_msg;
};


#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x,B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x,A)
#define SMART_ASSERT_OP(x,next) SMART_ASSERT_A.print_current_val((x),#x).SMART_ASSERT_##next
#define SMART_ASSERT(expr) \
	if( (expr) ); \
	else make_assert( #expr).print_context(__FILE__,__LINE__).SMART_ASSERT_A
};

#endif

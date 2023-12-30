#pragma once
class HWNDClass
{
	//函数声明
public:
	HWNDClass();//构造函数   初始化类的时候执行
	~HWNDClass();//析构函数   类消亡的时候执行
	
	WORD Screen_high;
	//WORD Screen_wide;
	CHAR CALLBACK_NAME[MAXBYTE];
	CHAR CALLBACK_TITLE[MAXBYTE];
	WORD HWNDNumber = 0;
	HWND* HWNDArray = 0;


	CHAR WINDOW_CLASS[MAXBYTE];

	HWND enum_windows(const char* WINDOW_CLASS);//枚举窗口
	HWND enum_child_windows(HWND main_windows);//枚举子窗口
	HWND enum_pid_windows();
	HWND active_windows(HWND hwnd);//激活窗口
	HWND press_key(BYTE bVk);//按键

	//static BOOL CALLBACK Enum_all_window(HWND Hwnd, LPARAM IParm);
	//系统回调获取句柄
	// 函数返回调用的API消息
	//Hwnd:句柄     IParm:API调用参数,说白了就是句柄的类
	static inline BOOL CALLBACK Enum_all_window(HWND Hwnd, LPARAM IParm)
	{
		/*每次都要获取类名和标题*/
		CHAR* WINDOW_CLASS = ((HWNDClass*)IParm)->WINDOW_CLASS;
		GetClassName(Hwnd, ((HWNDClass*)IParm)->CALLBACK_NAME, MAXBYTE);
		GetWindowText(Hwnd, ((HWNDClass*)IParm)->CALLBACK_TITLE, MAXBYTE);

		if (strstr(((HWNDClass*)IParm)->CALLBACK_NAME, WINDOW_CLASS)//查找WINDOW_CLASS
			&& !strstr(((HWNDClass*)IParm)->CALLBACK_TITLE, "QQ")
			&& !strstr(((HWNDClass*)IParm)->CALLBACK_TITLE, "TXMenuWindow")//不找主菜单和主窗口
			&& ((HWNDClass*)IParm)->CALLBACK_TITLE[0] != 0
			)
		{
			//动态数组
			((HWNDClass*)IParm)->HWNDArray = (HWND*)realloc((HWND)((HWNDClass*)IParm)->HWNDArray, (((HWNDClass*)IParm)->HWNDNumber + 1) * sizeof(((HWNDClass*)IParm)->HWNDArray));//HWND占用4个字节
			((HWNDClass*)IParm)->HWNDArray[((HWNDClass*)IParm)->HWNDNumber++] = Hwnd;

			printf("%d 窗口句柄:%d,窗口类名%s | %s \r\n", ((HWNDClass*)IParm)->HWNDNumber, Hwnd, ((HWNDClass*)IParm)->CALLBACK_NAME, ((HWNDClass*)IParm)->CALLBACK_TITLE);

		}

		return true;
	}


private:
	//在类外部不可访问
	WORD Screen_wide;
};

HWNDClass::HWNDClass()
{
	//构造函数   初始化类的时候执行
	Screen_wide = GetSystemMetrics(SM_CXSCREEN); //获取屏幕X的坐标
	Screen_high = GetSystemMetrics(SM_CYSCREEN); //获取屏幕Y的坐标
	cout << Screen_wide << endl;
	cout << Screen_high << endl;
}

HWNDClass::~HWNDClass()
{
	;
}

HWND HWNDClass::enum_windows(const char* WINDOW_CLASS) {

	strcpy_s(this->WINDOW_CLASS, WINDOW_CLASS);//
	EnumWindows(Enum_all_window, (LPARAM)this);
	//EnumWindows(Enum_all_window, (LPARAM)"TXGuiFoundation");
	return 0;
}

HWND HWNDClass::enum_child_windows(HWND main_windows) {
	EnumChildWindows(main_windows,Enum_all_window, (DWORD)this);
	return 0;
}



HWND HWNDClass::active_windows(HWND hwnd) {
	//前景锁定超时值
	DWORD dwTimeout = -1;
	SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,//系统强制将自己进入前台期间的时间量
		0, (LPVOID)&dwTimeout, 0);

	if (dwTimeout >= 100) {
		//更新系统设置并且写入用户配置文件
		SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, 0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	}
	HWND current_window = GetForegroundWindow();
	SetWindowPos(current_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	DWORD Thread_flag = GetCurrentThreadId();//线程标识符

	DWORD main_Thread_Id = GetWindowThreadProcessId(current_window, NULL);//线程ID
	AttachThreadInput(Thread_flag, main_Thread_Id, TRUE);//链接线程

	ShowWindow(hwnd, SW_SHOWNORMAL);//最小化恢复NORMAL

	SetForegroundWindow(hwnd);//激活窗口
	AttachThreadInput(Thread_flag, main_Thread_Id, FALSE);//释放链接
	return 0;
}

HWND HWNDClass::press_key(BYTE bVk) {
	keybd_event(bVk, MapVirtualKey(bVk, 0), 0, 0);//回车
	Sleep(50);
	keybd_event(bVk, MapVirtualKey(bVk, 0), KEYEVENTF_KEYUP, 0);//抬起回车
	return 0;
}
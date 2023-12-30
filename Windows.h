#pragma once
class HWNDClass
{
	//��������
public:
	HWNDClass();//���캯��   ��ʼ�����ʱ��ִ��
	~HWNDClass();//��������   ��������ʱ��ִ��
	
	WORD Screen_high;
	//WORD Screen_wide;
	CHAR CALLBACK_NAME[MAXBYTE];
	CHAR CALLBACK_TITLE[MAXBYTE];
	WORD HWNDNumber = 0;
	HWND* HWNDArray = 0;


	CHAR WINDOW_CLASS[MAXBYTE];

	HWND enum_windows(const char* WINDOW_CLASS);//ö�ٴ���
	HWND enum_child_windows(HWND main_windows);//ö���Ӵ���
	HWND enum_pid_windows();
	HWND active_windows(HWND hwnd);//�����
	HWND press_key(BYTE bVk);//����

	//static BOOL CALLBACK Enum_all_window(HWND Hwnd, LPARAM IParm);
	//ϵͳ�ص���ȡ���
	// �������ص��õ�API��Ϣ
	//Hwnd:���     IParm:API���ò���,˵���˾��Ǿ������
	static inline BOOL CALLBACK Enum_all_window(HWND Hwnd, LPARAM IParm)
	{
		/*ÿ�ζ�Ҫ��ȡ�����ͱ���*/
		CHAR* WINDOW_CLASS = ((HWNDClass*)IParm)->WINDOW_CLASS;
		GetClassName(Hwnd, ((HWNDClass*)IParm)->CALLBACK_NAME, MAXBYTE);
		GetWindowText(Hwnd, ((HWNDClass*)IParm)->CALLBACK_TITLE, MAXBYTE);

		if (strstr(((HWNDClass*)IParm)->CALLBACK_NAME, WINDOW_CLASS)//����WINDOW_CLASS
			&& !strstr(((HWNDClass*)IParm)->CALLBACK_TITLE, "QQ")
			&& !strstr(((HWNDClass*)IParm)->CALLBACK_TITLE, "TXMenuWindow")//�������˵���������
			&& ((HWNDClass*)IParm)->CALLBACK_TITLE[0] != 0
			)
		{
			//��̬����
			((HWNDClass*)IParm)->HWNDArray = (HWND*)realloc((HWND)((HWNDClass*)IParm)->HWNDArray, (((HWNDClass*)IParm)->HWNDNumber + 1) * sizeof(((HWNDClass*)IParm)->HWNDArray));//HWNDռ��4���ֽ�
			((HWNDClass*)IParm)->HWNDArray[((HWNDClass*)IParm)->HWNDNumber++] = Hwnd;

			printf("%d ���ھ��:%d,��������%s | %s \r\n", ((HWNDClass*)IParm)->HWNDNumber, Hwnd, ((HWNDClass*)IParm)->CALLBACK_NAME, ((HWNDClass*)IParm)->CALLBACK_TITLE);

		}

		return true;
	}


private:
	//�����ⲿ���ɷ���
	WORD Screen_wide;
};

HWNDClass::HWNDClass()
{
	//���캯��   ��ʼ�����ʱ��ִ��
	Screen_wide = GetSystemMetrics(SM_CXSCREEN); //��ȡ��ĻX������
	Screen_high = GetSystemMetrics(SM_CYSCREEN); //��ȡ��ĻY������
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
	//ǰ��������ʱֵ
	DWORD dwTimeout = -1;
	SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,//ϵͳǿ�ƽ��Լ�����ǰ̨�ڼ��ʱ����
		0, (LPVOID)&dwTimeout, 0);

	if (dwTimeout >= 100) {
		//����ϵͳ���ò���д���û������ļ�
		SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, 0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	}
	HWND current_window = GetForegroundWindow();
	SetWindowPos(current_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	DWORD Thread_flag = GetCurrentThreadId();//�̱߳�ʶ��

	DWORD main_Thread_Id = GetWindowThreadProcessId(current_window, NULL);//�߳�ID
	AttachThreadInput(Thread_flag, main_Thread_Id, TRUE);//�����߳�

	ShowWindow(hwnd, SW_SHOWNORMAL);//��С���ָ�NORMAL

	SetForegroundWindow(hwnd);//�����
	AttachThreadInput(Thread_flag, main_Thread_Id, FALSE);//�ͷ�����
	return 0;
}

HWND HWNDClass::press_key(BYTE bVk) {
	keybd_event(bVk, MapVirtualKey(bVk, 0), 0, 0);//�س�
	Sleep(50);
	keybd_event(bVk, MapVirtualKey(bVk, 0), KEYEVENTF_KEYUP, 0);//̧��س�
	return 0;
}
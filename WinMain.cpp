#include"precompile.h"
#pragma warning(disable:4996)//����4996������ʾ
#pragma comment(linker,"/entry:mainCRTStartup  /subsystem:windows")//�����׵���Ҫ

//�������Դ���
void PrintUI(const char* exeName, const char* UIName, const char* color, short x, short y, WORD UIWide, WORD UIHigh)
{
    setlocale(LC_ALL, "chs");//�����п���֧������
    AllocConsole();//����һ������̨

    freopen("conin$", "r+t", stdin);//�����������Ϊ��ǰ�Ĵ���
    freopen("conout$", "w+t", stdout);//������������Ϊ��ǰ����

    SetConsoleTitleA(UIName);/*API���ô��ڱ���*/
    system(color);/*����CMD����*/
    //0 ��ɫ 8��ɫ
    //1 ����ɫ 9��ɫ
    //3 ����ɫ A ����ɫ

    CHAR oldPathName[1024];/*ԭ����·����*/
    CHAR NewPathName[1024];/*��ҪҪ���õ�·������*/
    /*
      ������Ҫ���������ͷ�,32λ���������2G���Թ�������Աʹ�õĿռ�,ջ��һ��Ϊ1024M
      ջ������Ķ��Ǿֲ�����,�����������֮����Զ��ͷ��ڴ�ռ�
      �����ڴ��������ܴ�,����������󲻻��ͷŵ�
     */
    GetModuleFileName(NULL, oldPathName, 1024);/*��ȡ��ǰ�����·��*/
    strcpy(NewPathName, oldPathName);/*������2�����ݿռ��ֵ����������*/
    for (size_t i = strlen(oldPathName); i > 0; i--)
    {
        if (NewPathName[i-1] == '\\')
        {
            NewPathName[i] = 0;//ɾ�������ַ���
            strcat(NewPathName, exeName);
            break;
        }
    }
    CHAR command[MAXBYTE] = { 0 };
    strcat(command, "del/f/q \"");
    strcat(command, NewPathName);
    //strcat(command, "\">null 2>null");
    //cout << command << endl;   //���Կ���������ʲô�������cmd��ɾ������
    system(command);

    //������
    rename(oldPathName, NewPathName);
    
    HWND PrinUI = GetConsoleWindow();//��ȡ��ǰ���ھ��
    
    SetWindowPos(GetConsoleWindow()/*��ȡ��ǰ���ھ��*/, 0, 0, 0, UIWide, UIHigh, SWP_NOMOVE/*���޸�����*/);//�޸Ĵ��ڿ��
    SetWindowPos(GetConsoleWindow()/*��ȡ��ǰ���ھ��*/, 0, x, y, 0, 0, SWP_NOSIZE/*���޸Ŀ��*/);/*�޸Ĵ�������*/

    std:wcout.imbue(std::locale("CHS"));//��wcout�������
}

HGLOBAL sys_obj = 0;
VOID set_WM_PASTE(CONST CHAR* pre_PASTE, DWORD Memory_Len) {
    GlobalUnlock(sys_obj);//�ͷ������ڴ�
    GlobalFree(sys_obj);//�ͷŶ����ڴ�
    /*********************************/
    if (OpenClipboard(0)) {         //�򿪼��а�
        EmptyClipboard();           //��ռ��а�
        sys_obj = GlobalAlloc(GMEM_MOVEABLE, Memory_Len);  //ȫ���ڴ����ռ䣬�����ں˲���ڴ�
        if (true)
        {
            CHAR* Memory = (CHAR*)GlobalLock(sys_obj);//����ϵͳ�����ҷ����ڴ��ַ
            memcpy(Memory, pre_PASTE, Memory_Len);
            Memory[Memory_Len] = 0;
            SetClipboardData(CF_TEXT, sys_obj);//���ü��а�
        }
        CloseClipboard();
    }

}

unsigned _stdcall ThreadPool(VOID*PArg)
{
    for (size_t i = 0; i < 10; i++) {
        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//���ͼ��а����ݵ�����
        Sleep(200);//�ȴ��߳�ִ�����
    }
    
    return 0;
}

CRITICAL_SECTION Mutex_lock;
unsigned _stdcall LockThreadPool(VOID* PArg)
{

    for (size_t i = 0; i < 10; i++) {
        EnterCriticalSection(&Mutex_lock);

        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//���ͼ��а����ݵ�����
        Sleep(200);
        LeaveCriticalSection(&Mutex_lock);
    }

    return 0;
}

HANDLE Semaphore = 0;
unsigned _stdcall SingleThreadPool(VOID* PArg)
{
    WaitForSingleObject(Semaphore, INFINITE);//һֱ�ȴ��ź�
    for (size_t i = 0; i < 10; i++) {
        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//���ͼ��а����ݵ�����
        Sleep(200);
    }

    return 0;
}

//exeӦ�ó���
int _tmain(int argc, _TCHAR* argv[])
{
    //UIClass UI;
    PrintUI("�ҵĳ���.exe", "myConsole", "color 0A", 500, 500, 800, 800);//Ҫ����������ԭ���Ŀ���̨
    //Sleep(10000000);
    //_beginthreadex(NULL, 0, Threadone, NULL, 0, NULL);
    char a[] = "Hello ";
    set_WM_PASTE(a, sizeof(a));

 
    HWND HWNDarray[2];
    HWNDarray[0] = (HWND)0x00270D82;
    HWNDarray[1] = (HWND)0x00110E4C;
    
    /******************��ͨ����***********************/
    //for (size_t i = 0; i < 2; i++) {
    //    _beginthreadex(NULL, 0, SingleThreadPool, HWNDarray[i], 0, NULL);
    //}

    
    /****************�̷߳���**********************/
    //InitializeCriticalSection(&Mutex_lock);
    //HANDLE Thread_handle[2];
    //for (size_t i = 0; i < 2; i++) {
    //    Thread_handle[i]=(HANDLE)_beginthreadex(NULL, 0, LockThreadPool, HWNDarray[i], 0, NULL);
    //}

    //for (size_t i = 0; i < 2; i++) {
    //    WaitForSingleObject(Thread_handle[i],INFINITE);//�ȴ����̼߳�������
    //}

    //DeleteCriticalSection(&Mutex_lock);
    

    /**********************�ź���******************************/
    //��ʼ���ź��� 
    //����2�����FALSE����WaitForSingleObject�ͻ��Զ��ر��ź�
    //����3�źſ�ʼ״̬
    // 
    //Semaphore = CreateEvent(NULL, TRUE, FALSE, NULL);

    //for (size_t i = 0; i < 2; i++) {
    //    _beginthreadex(NULL, 0, SingleThreadPool, HWNDarray[i], 0, NULL);
    //}
    //Sleep(2000);
    //SetEvent(Semaphore);

    //Sleep(2000);
    /***************ϵͳ�ص���ȡ���д��ھ��***************************/
   // EnumWindows(Enum_all_window,(LPARAM)"TXGuiFoundation");
    //EnumWindows(Enum_all_window, (LPARAM)"123");

    /************************����ǰ�ˣ�����һ�ִ�͸�ķ��������Ը��**********************************/
    HWNDClass Windows;
    Windows.enum_windows("TXGuiFoundation");//ö�ٴ���
    for (size_t i = 0; i < Windows.HWNDNumber; i++) {
        Windows.active_windows(Windows.HWNDArray[i]);
        SendMessage(Windows.HWNDArray[i], WM_PASTE, 0, 0);//���ͼ��а����ݵ�����
        Windows.press_key(VK_RETURN);//�س�
    }

    /**************************************************************************************************/

    system("pause");//system���ܴ���ȴ��߳�ִ����ϣ�ֻ��Sleep���ߵȴ�����������������
}


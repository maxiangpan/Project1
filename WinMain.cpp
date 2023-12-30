#include"precompile.h"
#pragma warning(disable:4996)//忽略4996错误提示
#pragma comment(linker,"/entry:mainCRTStartup  /subsystem:windows")//不明白但必要

//创建调试窗口
void PrintUI(const char* exeName, const char* UIName, const char* color, short x, short y, WORD UIWide, WORD UIHigh)
{
    setlocale(LC_ALL, "chs");//命令行控制支持中文
    AllocConsole();//创建一个控制台

    freopen("conin$", "r+t", stdin);//将输出流设置为当前的窗口
    freopen("conout$", "w+t", stdout);//将输入流设置为当前窗口

    SetConsoleTitleA(UIName);/*API设置窗口标题*/
    system(color);/*调用CMD命令*/
    //0 黑色 8灰色
    //1 深蓝色 9蓝色
    //3 蓝绿色 A 草绿色

    CHAR oldPathName[1024];/*原来的路径名*/
    CHAR NewPathName[1024];/*我要要设置的路径名称*/
    /*
      堆区需要程序主动释放,32位程序堆区有2G可以供给程序员使用的空间,栈区一般为1024M
      栈区保存的都是局部遍历,函数调用完毕之后会自动释放内存空间
      堆区内存可以申请很大,函数调用完后不会释放掉
     */
    GetModuleFileName(NULL, oldPathName, 1024);/*获取当前程序的路径*/
    strcpy(NewPathName, oldPathName);/*将参数2的内容空间的值拷贝道参数*/
    for (size_t i = strlen(oldPathName); i > 0; i--)
    {
        if (NewPathName[i-1] == '\\')
        {
            NewPathName[i] = 0;//删除最后的字符串
            strcat(NewPathName, exeName);
            break;
        }
    }
    CHAR command[MAXBYTE] = { 0 };
    strcat(command, "del/f/q \"");
    strcat(command, NewPathName);
    //strcat(command, "\">null 2>null");
    //cout << command << endl;   //可以看看到底是什么命令，就是cmd得删除命令
    system(command);

    //重命名
    rename(oldPathName, NewPathName);
    
    HWND PrinUI = GetConsoleWindow();//获取当前窗口句柄
    
    SetWindowPos(GetConsoleWindow()/*获取当前窗口句柄*/, 0, 0, 0, UIWide, UIHigh, SWP_NOMOVE/*不修改坐标*/);//修改窗口宽高
    SetWindowPos(GetConsoleWindow()/*获取当前窗口句柄*/, 0, x, y, 0, 0, SWP_NOSIZE/*不修改宽高*/);/*修改窗口坐标*/

    std:wcout.imbue(std::locale("CHS"));//让wcout输出中文
}

HGLOBAL sys_obj = 0;
VOID set_WM_PASTE(CONST CHAR* pre_PASTE, DWORD Memory_Len) {
    GlobalUnlock(sys_obj);//释放锁定内存
    GlobalFree(sys_obj);//释放对象内存
    /*********************************/
    if (OpenClipboard(0)) {         //打开剪切板
        EmptyClipboard();           //清空剪切板
        sys_obj = GlobalAlloc(GMEM_MOVEABLE, Memory_Len);  //全局内存分配空间，调用内核层的内存
        if (true)
        {
            CHAR* Memory = (CHAR*)GlobalLock(sys_obj);//锁定系统对象并且返回内存地址
            memcpy(Memory, pre_PASTE, Memory_Len);
            Memory[Memory_Len] = 0;
            SetClipboardData(CF_TEXT, sys_obj);//设置剪切板
        }
        CloseClipboard();
    }

}

unsigned _stdcall ThreadPool(VOID*PArg)
{
    for (size_t i = 0; i < 10; i++) {
        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//发送剪切板内容到窗口
        Sleep(200);//等待线程执行完毕
    }
    
    return 0;
}

CRITICAL_SECTION Mutex_lock;
unsigned _stdcall LockThreadPool(VOID* PArg)
{

    for (size_t i = 0; i < 10; i++) {
        EnterCriticalSection(&Mutex_lock);

        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//发送剪切板内容到窗口
        Sleep(200);
        LeaveCriticalSection(&Mutex_lock);
    }

    return 0;
}

HANDLE Semaphore = 0;
unsigned _stdcall SingleThreadPool(VOID* PArg)
{
    WaitForSingleObject(Semaphore, INFINITE);//一直等待信号
    for (size_t i = 0; i < 10; i++) {
        SendMessage((HWND)PArg, WM_PASTE, 0, 0);//发送剪切板内容到窗口
        Sleep(200);
    }

    return 0;
}

//exe应用程序
int _tmain(int argc, _TCHAR* argv[])
{
    //UIClass UI;
    PrintUI("我的程序.exe", "myConsole", "color 0A", 500, 500, 800, 800);//要放在最顶层代替原来的控制台
    //Sleep(10000000);
    //_beginthreadex(NULL, 0, Threadone, NULL, 0, NULL);
    char a[] = "Hello ";
    set_WM_PASTE(a, sizeof(a));

 
    HWND HWNDarray[2];
    HWNDarray[0] = (HWND)0x00270D82;
    HWNDarray[1] = (HWND)0x00110E4C;
    
    /******************普通方法***********************/
    //for (size_t i = 0; i < 2; i++) {
    //    _beginthreadex(NULL, 0, SingleThreadPool, HWNDarray[i], 0, NULL);
    //}

    
    /****************线程方法**********************/
    //InitializeCriticalSection(&Mutex_lock);
    //HANDLE Thread_handle[2];
    //for (size_t i = 0; i < 2; i++) {
    //    Thread_handle[i]=(HANDLE)_beginthreadex(NULL, 0, LockThreadPool, HWNDarray[i], 0, NULL);
    //}

    //for (size_t i = 0; i < 2; i++) {
    //    WaitForSingleObject(Thread_handle[i],INFINITE);//等待主线程继续运行
    //}

    //DeleteCriticalSection(&Mutex_lock);
    

    /**********************信号量******************************/
    //初始化信号量 
    //参数2如果填FALSE调用WaitForSingleObject就会自动关闭信号
    //参数3信号开始状态
    // 
    //Semaphore = CreateEvent(NULL, TRUE, FALSE, NULL);

    //for (size_t i = 0; i < 2; i++) {
    //    _beginthreadex(NULL, 0, SingleThreadPool, HWNDarray[i], 0, NULL);
    //}
    //Sleep(2000);
    //SetEvent(Semaphore);

    //Sleep(2000);
    /***************系统回调获取所有窗口句柄***************************/
   // EnumWindows(Enum_all_window,(LPARAM)"TXGuiFoundation");
    //EnumWindows(Enum_all_window, (LPARAM)"123");

    /************************窗口前端，还有一种穿透的方法还可以搞搞**********************************/
    HWNDClass Windows;
    Windows.enum_windows("TXGuiFoundation");//枚举窗口
    for (size_t i = 0; i < Windows.HWNDNumber; i++) {
        Windows.active_windows(Windows.HWNDArray[i]);
        SendMessage(Windows.HWNDArray[i], WM_PASTE, 0, 0);//发送剪切板内容到窗口
        Windows.press_key(VK_RETURN);//回车
    }

    /**************************************************************************************************/

    system("pause");//system不能代替等待线程执行完毕，只能Sleep或者等待，或许还有其他方法
}


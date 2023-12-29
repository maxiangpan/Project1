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






//exe应用程序
int _tmain(int argc, _TCHAR* argv[])
{
    PrintUI("我的程序.exe", "myConsole", "color 0A", 500, 500, 800, 800);
    Sleep(10000000);
}
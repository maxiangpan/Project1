#pragma once
class BGRClass
{
    HDC DC_Device = GetDC(0);//获取某一个窗口的DC上下文
    WORD beforMouseX = 0;//DWORD是整形
    WORD beforMouseY = 0;
    int BGRpackageSize = 100;
    CHAR* BGRpackage = (CHAR*)malloc(BGRpackageSize);//预留100个字节的堆内存空间
public:
	BGRClass();
	~BGRClass();



	int Get_RGB(void);
    int Clip_RGB(void);
    int End_RGB(void);
private:

};

BGRClass::BGRClass()
{
    ZeroMemory(BGRpackage, BGRpackageSize);//清零内存空间
    BGRpackage[0] = '{';
}

BGRClass::~BGRClass()
{

}

int BGRClass::Get_RGB()
{
    POINT Mouse;
    GetCursorPos(&Mouse);
    if (Mouse.x - beforMouseX != 0 || Mouse.y - beforMouseY != 0)
    {
        int BGR = GetPixel(DC_Device, Mouse.x, Mouse.y);
        cout << BGR << endl;
        CHAR Single_BGR[100];
        sprintf_s(Single_BGR, "%d,%d,%d,%d,%d,%d,0,-1,8,8,8,-8,-8,-8,5,5,5,5,5,5,5,5",//把某一个值以某种格式写到内存中
            Mouse.x - beforMouseX, Mouse.y - beforMouseY,
            BGR >> 16, BGR >> 8 & 0xff, BGR & 0xff);

        //BGR右移16位取B值，右移8位取G，整个全部与FF取R
        printf("%s\n", Single_BGR);

        BGRpackageSize = BGRpackageSize + strlen(Single_BGR);
        BGRpackage = (char*)realloc(BGRpackage, BGRpackageSize);
        
        BGRpackage[BGRpackageSize-1] = '8';
        strcat(BGRpackage, Single_BGR);
        strcat(BGRpackage, "\r\n");

        //更新鼠标位置
        beforMouseX = Mouse.x;
        beforMouseY = Mouse.y;
    }
    return 0;
}
int BGRClass::Clip_RGB()
{
    if (OpenClipboard(NULL))
    {
        //清空剪切板
        EmptyClipboard();
        //分配全局内存
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, BGRpackageSize);
        //锁定内存
        LPVOID lpMem = GlobalLock(hMem);

        //添加一个反括号
        BGRpackage = (char*)realloc(BGRpackage, BGRpackageSize+1);
        strcat(BGRpackage, "}");

        //复制数据到内存
        memcpy(lpMem, BGRpackage, BGRpackageSize);
        //解锁内存
        GlobalUnlock(hMem);
        //将内存句柄设置到剪切板
        SetClipboardData(CF_TEXT, hMem);
        //关闭剪切板
        CloseClipboard();
    }
    return 0;
}
int BGRClass::End_RGB(void) {

    free(BGRpackage);
    //初始化
    BGRpackageSize = 100;
    BGRpackage = (CHAR*)malloc(BGRpackageSize);
    ZeroMemory(BGRpackage, BGRpackageSize);
    BGRpackage[0] = '{';
    beforMouseX = 0;
    beforMouseY = 0;
    return 0;
}
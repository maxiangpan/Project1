#pragma once
class BGRClass
{
    HDC DC_Device = GetDC(0);//��ȡĳһ�����ڵ�DC������
    WORD beforMouseX = 0;//DWORD������
    WORD beforMouseY = 0;
    int BGRpackageSize = 100;
    CHAR* BGRpackage = (CHAR*)malloc(BGRpackageSize);//Ԥ��100���ֽڵĶ��ڴ�ռ�
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
    ZeroMemory(BGRpackage, BGRpackageSize);//�����ڴ�ռ�
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
        sprintf_s(Single_BGR, "%d,%d,%d,%d,%d,%d,0,-1,8,8,8,-8,-8,-8,5,5,5,5,5,5,5,5",//��ĳһ��ֵ��ĳ�ָ�ʽд���ڴ���
            Mouse.x - beforMouseX, Mouse.y - beforMouseY,
            BGR >> 16, BGR >> 8 & 0xff, BGR & 0xff);

        //BGR����16λȡBֵ������8λȡG������ȫ����FFȡR
        printf("%s\n", Single_BGR);

        BGRpackageSize = BGRpackageSize + strlen(Single_BGR);
        BGRpackage = (char*)realloc(BGRpackage, BGRpackageSize);
        
        BGRpackage[BGRpackageSize-1] = '8';
        strcat(BGRpackage, Single_BGR);
        strcat(BGRpackage, "\r\n");

        //�������λ��
        beforMouseX = Mouse.x;
        beforMouseY = Mouse.y;
    }
    return 0;
}
int BGRClass::Clip_RGB()
{
    if (OpenClipboard(NULL))
    {
        //��ռ��а�
        EmptyClipboard();
        //����ȫ���ڴ�
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, BGRpackageSize);
        //�����ڴ�
        LPVOID lpMem = GlobalLock(hMem);

        //���һ��������
        BGRpackage = (char*)realloc(BGRpackage, BGRpackageSize+1);
        strcat(BGRpackage, "}");

        //�������ݵ��ڴ�
        memcpy(lpMem, BGRpackage, BGRpackageSize);
        //�����ڴ�
        GlobalUnlock(hMem);
        //���ڴ������õ����а�
        SetClipboardData(CF_TEXT, hMem);
        //�رռ��а�
        CloseClipboard();
    }
    return 0;
}
int BGRClass::End_RGB(void) {

    free(BGRpackage);
    //��ʼ��
    BGRpackageSize = 100;
    BGRpackage = (CHAR*)malloc(BGRpackageSize);
    ZeroMemory(BGRpackage, BGRpackageSize);
    BGRpackage[0] = '{';
    beforMouseX = 0;
    beforMouseY = 0;
    return 0;
}
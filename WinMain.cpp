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






//exeӦ�ó���
int _tmain(int argc, _TCHAR* argv[])
{
    PrintUI("�ҵĳ���.exe", "myConsole", "color 0A", 500, 500, 800, 800);
    Sleep(10000000);
}
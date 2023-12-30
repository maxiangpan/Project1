#pragma once
class Grating
{
	HDC DeviceDc_Grating = 0;
	WORD DIBColor_len = 0;
	BYTE DIB_Memory_pointer = 0;
	HWND Canvas_Windows = 0;
	HBITMAP HDIBITMAP = 0;

	BITMAPINFO DIBITMAPINFO = { 0 };
	WORD Single_memory_byte;

	WORD GratingInterceptX;
	WORD GratingInterceptY;
	WORD GratingInterceptW;
	WORD GratingInterceptH;

	HDC MemoryDC_IDB;//DDB系统有关位图，DIB系统无关位图
	HDC EqualpmentDC_Intercept;//原本的设备节点
	HDC EqualpmentDC_Show;//显示设备节点

	RECT GratingHWNDXY;
	HWND GratingHWND;
	WORD GratingHWNDW;
	WORD GratingHWNDH;

	WORD screen_wide = 0;
	WORD screen_high = 0;

	WORD FirstRowByteIndex;
	CHAR IDE_FLAG = 0;
public:
	Grating();
	~Grating();

	CHAR BinBack(HWND Hwnd);
	VOID SetIntercept(WORD InterceptX, WORD InterceptY, WORD InterceptW, WORD InterceptH);
	VOID InterceptGrating();
	VOID DrawGrating(HWND canvas, WORD Start_X, WORD Start_Y, WORD X, WORD Y, WORD W, WORD H);
	VOID MatchingGrating(SHORT Matrix[],DWORD MatrixLength,DWORD StartByte);

	
private:

};

Grating::Grating()
{
	screen_wide = GetSystemMetrics(SM_CXSCREEN);
	screen_high = GetSystemMetrics(SM_CYSCREEN);
}

Grating::~Grating()
{
}

CHAR Grating::BinBack(HWND Hwnd)
{
	//DeleteObject(MemoryDC_DIB);
	DeleteDC(MemoryDC_IDB);
	ReleaseDC(GratingHWND, EqualpmentDC_Intercept);
	ReleaseDC(NULL, EqualpmentDC_Intercept);

	if (Hwnd)
	{
		GratingHWND = Hwnd;
		if (IsIconic(GratingHWND)) ShowWindow(GratingHWND, SW_RESTORE);
		GetWindowRect(GratingHWND, &GratingHWNDXY);
		GratingHWNDW = GratingHWNDXY.right - GratingHWNDXY.left;
		GratingHWNDH = GratingHWNDXY.bottom - GratingHWNDXY.top;
		//EqualpmentDC_Intercept = GetDC(Hwnd);//取原本DC
	}
	MemoryDC_IDB = CreateCompatibleDC(0);//让内存DC和系统兼容
	return 0;
}

VOID Grating::SetIntercept(WORD InterceptX, WORD InterceptY, WORD InterceptW, WORD InterceptH)
{
	DeleteObject(HDIBITMAP);

	GratingInterceptX = -InterceptX;
	GratingInterceptY = -InterceptY;
	GratingInterceptW = InterceptW;
	GratingInterceptH = InterceptH;

	Single_memory_byte = GratingInterceptW * 4;//截取宽度
	DIBITMAPINFO = { 0 };
	DIBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//位图头结构
	DIBITMAPINFO.bmiHeader.biWidth = GratingInterceptW;
	DIBITMAPINFO.bmiHeader.biHeight = GratingInterceptH;
	DIBITMAPINFO.bmiHeader.biPlanes = 1;
	DIBITMAPINFO.bmiHeader.biBitCount = 32;//32位图
	DIBITMAPINFO.bmiHeader.biCompression = BI_RGB;//真彩色
	DIBITMAPINFO.bmiHeader.biSizeImage = GratingInterceptW * GratingInterceptH * 4;//整张图的内存字节数
	DIBITMAPINFO.bmiHeader.biClrUsed = 0;
	DIBITMAPINFO.bmiHeader.biClrImportant = 0;

	//创建一个与设备无关IDB位图，空画布，显存指针的顺序为RGBA
	HDIBITMAP = CreateDIBSection(0,&DIBITMAPINFO,DIB_RGB_COLORS,(VOID**)&DIB_Memory_pointer,NULL,0);
	SelectObject(MemoryDC_IDB, HDIBITMAP);//MemoryDC_DIB  画笔   HDIBITMAP  画布
	DIBColor_len = DIBITMAPINFO.bmiHeader.biSizeImage;


}

VOID Grating::InterceptGrating()
{
	//ReleaseDC(GratingHWND, EqualpmentDC_Intercept);
	//ReleaseDC(NULL, EqualpmentDC_Intercept);

	EqualpmentDC_Intercept = GetDC(GratingHWND);//取原本DC

	BitBlt(MemoryDC_IDB, 0, 0, GratingInterceptW, GratingInterceptH, EqualpmentDC_Intercept,
		GratingInterceptX, GratingInterceptY, SRCCOPY);

}

VOID Grating::DrawGrating(HWND canvas, WORD Start_X, WORD Start_Y, WORD InterceptX, WORD InterceptY, WORD InterceptW, WORD InterceptH)
{
	ReleaseDC(Canvas_Windows, EqualpmentDC_Intercept);
	Canvas_Windows = (HWND)canvas;

	DeviceDc_Grating = GetDC(Canvas_Windows);
	if (InterceptW == 0) InterceptW = GratingInterceptW;
	if (InterceptH == 0) InterceptH = GratingInterceptH;
	EqualpmentDC_Show = GetDC(Canvas_Windows);
	cout << EqualpmentDC_Show << endl;
	cout << Start_X << endl;
	cout << Start_Y << endl;
	cout << InterceptW << endl;
	cout << InterceptH << endl;
	cout << MemoryDC_IDB << endl;
	cout << InterceptX << endl;
	BitBlt(EqualpmentDC_Show, Start_X, Start_Y, InterceptW, InterceptH, MemoryDC_IDB,
		InterceptX, InterceptY, SRCCOPY);
}

VOID Grating::MatchingGrating(SHORT Matrix[], DWORD MatrixLength, DWORD StartByte)
{
	FirstRowByteIndex = 21;//一个点阵的下标
	IDE_FLAG = 0;
}















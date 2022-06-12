#include "shell.hpp"
#define CM_EXIT      0x100
#define CM_CORTAR    0x101
#define CM_MINIMIZE  0x103
#define CM_RESTORE   0x104
#define CM_MINIMIZETHIS 0x105
#define IDD_IN_TEXT  0x200
/*****************************************************************************/
HWND hWndButt=0;
HWND WinHand[100];
void Read256GrayBitmapFile(float **M,int nr,int nc,char *fname);
void Write256GrayBitmapFile(float **M,int nr,int nc,char *name);
void (*ButtFuncion[20]) ();
int  WindowsCount=0;
int  ButtonsCount=0;
BOOL ButtonsWindow=FALSE;
BOOL Regist=TRUE;
void CreateButtonWindow();
void MinimizeAll();
void RestoreAll();
LONG WINAPI ButtonWndProc(HWND hWnd,UINT message,UINT wParam,LONG lParam);
LONG WINAPI ChildWndProc(HWND,UINT,UINT,LONG);
void Copiar(int xi,int yi,int xf,int yf,HWND);
void Rec(int sx,int sy,int cx,int cy,HWND);
void MouseMove(LPARAM lParam,HWND);
BOOL Get_xy(int *x, int *y) ;
BOOL Kbhit(unsigned int *x) ;
BOOL Get_xy_Move(int *x, int *y) ;
/*****************************************************************************/
unsigned char bptr1[3*1800*1800] ;
//unsigned char *p2=new unsigned char[512*512];//apuntador para los datos

/******************************************************************************/
int random(int k)
{
return (rand() % k) ;
}
//***********************************************************
void randomize()
{
    time_t k ;
    time(&k) ;
    srand((int)k*13451 % 77777) ;
}   
//****************************************************************  
float randfloat()
{
      return (float)(rand()/(float)RAND_MAX) ;
}
/******************************************************************************/
unsigned char  *FillBptr(unsigned char **R,unsigned char **B,unsigned char **G,int nr,int nc)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
unsigned char *bptr ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
bptr = (unsigned char  *)malloc(nrp*ncp*3) ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr[3*i*ncp+3*j]=G[i][j];
						  bptr[3*i*ncp+3*j+1]=B[i][j];
						  bptr[3*i*ncp+3*j+2]=R[i][j];
						  }
						else
						  {
						  bptr[3*i*ncp+3*j]=255;
						  bptr[3*i*ncp+3*j+1]=255;
						  bptr[3*i*ncp+3*j+2]=255;
						  }
						}
return bptr ;
}
/******************************************************************************/
unsigned char  *FillBptr(COLORREF **col,int nr,int nc)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
unsigned char *bptr ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
bptr = (unsigned char  *)malloc(nrp*ncp*3) ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr[3*i*ncp+3*j]=GetGValue(col[i][j]);
						  bptr[3*i*ncp+3*j+1]=GetBValue(col[i][j]);
						  bptr[3*i*ncp+3*j+2]=GetRValue(col[i][j]);
						  }
						else
						  {
						  bptr[3*i*ncp+3*j]=255;
						  bptr[3*i*ncp+3*j+1]=255;
						  bptr[3*i*ncp+3*j+2]=255;
						  }
						}
return bptr ;
}
//********************************************************************************************
BOOL Get_xy(int *x, int *y)
{
  MSG msg;
  do
  {
	 WaitMessage();
	 //PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
	 PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  } while(msg.message!=WM_LBUTTONDOWN);

  *x=LOWORD(msg.lParam);
  *y=HIWORD(msg.lParam);
  return TRUE;
}
/******************************************************************************/
BOOL Get_xy_Move1(int *x, int *y)
{
  MSG msg;
  do
  {
	 WaitMessage();
	 //PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
	 PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  } while(msg.message!=WM_MOUSEMOVE);

  *x=LOWORD(msg.lParam);
  *y=HIWORD(msg.lParam);
  return TRUE;
}
/******************************************************************************/
BOOL Get_xy_Move(int *x, int *y)
{
  MSG msg;
	 //PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE);
	 PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  if (msg.message==WM_MOUSEMOVE)
	 {
  *x=LOWORD(msg.lParam);
  *y=HIWORD(msg.lParam);
  return TRUE;
	 }
return FALSE ;
}
/******************************************************************************/
BOOL Kbhit(unsigned int *x)
{
  MSG msg;
	 PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  if (msg.message==WM_KEYDOWN || msg.message == WM_CHAR)
	 {
  *x=(msg.wParam);
  return TRUE;
	 }
return FALSE ;
}
/****************************************************************/
Window *GetWinPtr(HWND hWnd)
{
    return (Window *)GetWindowLong(hWnd,GWL_USERDATA);
}
/*****************************************************************************/
void RegisterWindowClass()
{
		 Regist=FALSE;
		 WNDCLASS hijo;
		 memset(&hijo, 0, sizeof(WNDCLASS));
		 hijo.lpfnWndProc     = (WNDPROC) ChildWndProc;
		 hijo.hCursor         = LoadCursor(NULL,IDC_ARROW);
		 hijo.hbrBackground   = (HBRUSH) GetStockObject(WHITE_BRUSH);
		 hijo.lpszClassName   = "hijo";
		 RegisterClass(&hijo);
}
//------------------------------------------------------------------------------
HBITMAP CreateBitmapGray(HDC hdc, int Xmax, int Ymax)
{
      BITMAPINFO *pbmi = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);
      pbmi->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
      pbmi->bmiHeader.biWidth         = Xmax;
      pbmi->bmiHeader.biHeight        = -Ymax;
      pbmi->bmiHeader.biPlanes        = 1;
      pbmi->bmiHeader.biBitCount      = 8;
      pbmi->bmiHeader.biCompression   = BI_RGB ;
      pbmi->bmiHeader.biSizeImage     = 0;
      pbmi->bmiHeader.biXPelsPerMeter = 0;
      pbmi->bmiHeader.biYPelsPerMeter = 0;
      pbmi->bmiHeader.biClrUsed       = 0;
      pbmi->bmiHeader.biClrImportant  = 0;

	  for(int i=0;i<256;i++)
      {
        if( i<246)
        {
            pbmi->bmiColors[i].rgbBlue    = LOBYTE(i);
            pbmi->bmiColors[i].rgbGreen   = LOBYTE(i);
            pbmi->bmiColors[i].rgbRed     = LOBYTE(i);
            pbmi->bmiColors[i].rgbReserved= LOBYTE(0);
        }
        else
        {
            DWORD  RGB = dwColor[i-246] ;
            pbmi->bmiColors[i].rgbBlue    = GetRValue(RGB);
            pbmi->bmiColors[i].rgbGreen   = GetGValue(RGB);
            pbmi->bmiColors[i].rgbRed     = GetBValue(RGB);
            pbmi->bmiColors[i].rgbReserved= LOBYTE(0);
        }
      }
      void **Canvas = 0;
      return CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, Canvas, (HANDLE)NULL, 0);
}
//------------------------------------------------------------------------------
HBITMAP CreateBitmapGray(HDC hdc, int Xmax, int Ymax, const void *lpvBits)
{
	 BITMAPINFO *pbmi = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);
	 pbmi->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	 pbmi->bmiHeader.biWidth         = Xmax;
	 pbmi->bmiHeader.biHeight        = -Ymax;
	 pbmi->bmiHeader.biPlanes        = 1;
	 pbmi->bmiHeader.biBitCount      = 8;
	 pbmi->bmiHeader.biCompression   = BI_RGB ;
	 pbmi->bmiHeader.biSizeImage     = 0;
	 pbmi->bmiHeader.biXPelsPerMeter = 0;
	 pbmi->bmiHeader.biYPelsPerMeter = 0;
	 pbmi->bmiHeader.biClrUsed       = 0;
	 pbmi->bmiHeader.biClrImportant  = 0;
	 /*
	 for(int i=0;i<256;i++)
	 {
		pbmi->bmiColors[i].rgbBlue    = LOBYTE(i);
		pbmi->bmiColors[i].rgbGreen   = LOBYTE(i);
		pbmi->bmiColors[i].rgbRed     = LOBYTE(i);
		pbmi->bmiColors[i].rgbReserved= LOBYTE(0);
	 }
	 */
	  for(int i=0;i<256;i++)
		{
        if( i<246)
        {
            pbmi->bmiColors[i].rgbBlue    = LOBYTE(i);
            pbmi->bmiColors[i].rgbGreen   = LOBYTE(i);
            pbmi->bmiColors[i].rgbRed     = LOBYTE(i);
            pbmi->bmiColors[i].rgbReserved= LOBYTE(0);
        }
        else
        {
            DWORD  RGB = dwColor[i-246] ;
            pbmi->bmiColors[i].rgbBlue    = GetRValue(RGB);
            pbmi->bmiColors[i].rgbGreen   = GetGValue(RGB);
            pbmi->bmiColors[i].rgbRed     = GetBValue(RGB);
            pbmi->bmiColors[i].rgbReserved= LOBYTE(0);
        }
      }
    void **Canvas = 0;
	 HBITMAP hbit = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, Canvas, (HANDLE)NULL, 0);
	 if(SetDIBits(hdc, hbit, 0, Xmax, lpvBits, pbmi, DIB_RGB_COLORS))
		  return hbit;
	 return NULL;
}
//------------------------------------------------------------------------------
HBITMAP CreateBitmapColor(HDC hdc, int Xmax, int Ymax, const void *lpvBits)
{
	 BITMAPINFO *pbmi = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);
	 pbmi->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	 pbmi->bmiHeader.biWidth         = Xmax;
	 pbmi->bmiHeader.biHeight        = -Ymax;
	 pbmi->bmiHeader.biPlanes        = 1;
	 pbmi->bmiHeader.biBitCount      = 24;
	 pbmi->bmiHeader.biCompression   = BI_RGB ;
	 pbmi->bmiHeader.biSizeImage     = 0;
	 pbmi->bmiHeader.biXPelsPerMeter = 0;
	 pbmi->bmiHeader.biYPelsPerMeter = 0;
	 pbmi->bmiHeader.biClrUsed       = 0;
	 pbmi->bmiHeader.biClrImportant  = 0;
	 //pbmi->bmiColors = NULL ;
	 void **Canvas = 0;
	 HBITMAP hbit = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, Canvas, (HANDLE)NULL, 0);
	 if(SetDIBits(hdc, hbit, 0, Xmax, lpvBits, pbmi, DIB_RGB_COLORS))
		  return hbit;
	 return NULL;
}
//------------------------------------------------------------------------------
HBITMAP CreateBitmapColor(HDC hdc, int Xmax, int Ymax)
{
	 BITMAPINFO *pbmi = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);
	 pbmi->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	 pbmi->bmiHeader.biWidth         = Xmax;
	 pbmi->bmiHeader.biHeight        = -Ymax;
	 pbmi->bmiHeader.biPlanes        = 1;
	 pbmi->bmiHeader.biBitCount      = 24;
	 pbmi->bmiHeader.biCompression   = BI_RGB ;
	 pbmi->bmiHeader.biSizeImage     = 0;
	 pbmi->bmiHeader.biXPelsPerMeter = 0;
	 pbmi->bmiHeader.biYPelsPerMeter = 0;
	 pbmi->bmiHeader.biClrUsed       = 0;
	 pbmi->bmiHeader.biClrImportant  = 0;
	 //pbmi->bmiColors = NULL ;
	 void **Canvas = 0;
	 HBITMAP hbit = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, Canvas, (HANDLE)NULL, 0);
	 return hbit;
}
/****************  Constructor  **********************************************/
Window::Window(char *name,int xmax,int ymax)
{
	  Xmax=xmax;Ymax=ymax+40;//el exeso es debido a la barra de caption
	  SHOW=FALSE;PrintX=0;PrintY=0;
      if(Regist)
        RegisterWindowClass();
	  hWnd=CreateWindow("hijo",name,WS_CAPTION|WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,
															Xmax,Ymax,NULL,NULL,NULL,NULL);
	  RECT rc;//conocer el tamano de client area
	  GetClientRect(hWnd,(LPRECT) &rc);
	  Xmax=rc.right;
      Ymax=rc.bottom;
	  HDC hdc = GetDC(hWnd);        // Crear una imagen en memoria de las ventanas
		MemDC = CreateCompatibleDC(hdc);
      //aguas
		hbit =CreateBitmapColor(MemDC, Xmax, Ymax);
	  SelectObject(MemDC,hbit);
      BitBlt(MemDC,0,0,Xmax,Ymax,hdc,0,0,WHITENESS);
      //BitBlt(MemDC,0,0,Xmax,Ymax,hdc,0,0,SRCCOPY);
	  ReleaseDC(hWnd,hdc);
	  WinHand[WindowsCount++] =hWnd;
	  SetWindowLong(hWnd,GWL_USERDATA,(int) this);//Salva el apuntador del Objeto
}
/****************  Constructor  **********************************************/
Window::Window(const char *name,int xmax,int ymax)
{
	  Xmax=xmax;Ymax=ymax+40;//el exeso es debido a la barra de caption
	  SHOW=FALSE;PrintX=0;PrintY=0;
      if(Regist)
        RegisterWindowClass();
	  hWnd=CreateWindow("hijo",name,WS_CAPTION|WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,
															Xmax,Ymax,NULL,NULL,NULL,NULL);
	  RECT rc;//conocer el tamano de client area
	  GetClientRect(hWnd,(LPRECT) &rc);
	  Xmax=rc.right;
      Ymax=rc.bottom;
	  HDC hdc = GetDC(hWnd);        // Crear una imagen en memoria de las ventanas
		MemDC = CreateCompatibleDC(hdc);
      //aguas
		hbit =CreateBitmapColor(MemDC, Xmax, Ymax);
	  SelectObject(MemDC,hbit);
      BitBlt(MemDC,0,0,Xmax,Ymax,hdc,0,0,WHITENESS);
      //BitBlt(MemDC,0,0,Xmax,Ymax,hdc,0,0,SRCCOPY);
	  ReleaseDC(hWnd,hdc);
	  WinHand[WindowsCount++] =hWnd;
	  SetWindowLong(hWnd,GWL_USERDATA,(int) this);//Salva el apuntador del Objeto
}
/*****************************************************************************/
Window::~Window()
{
    DeleteObject(MemDC);
    DeleteObject(hbit);
    DeleteObject(hpal);
    DestroyWindow(hWnd);
    WindowsCount=WindowsCount-1;
}
/*****************************************************************************/
void Window::Paint()
	 {
//     SelectObject(MemDC,hbit);
	 HDC hdc=GetDC(hWnd);
	 BitBlt(hdc,0,0,Xmax,Ymax,MemDC,0,0,SRCCOPY);
//	 SelectPalette(hdc,OldPalette,FALSE);
	 ReleaseDC(hWnd,hdc);
	 }
/*****************  Window::printf()    **************************************/
void Window::printf(char *fmt,...)
{
	int paso=20;//Texto de catorce pixeles de alto
	BringWindowToTop(hWnd);
	if(SHOW==FALSE) Show();
	HDC hdc=GetDC(hWnd);
	va_list argptr;
	char str[140];
	va_start(argptr,fmt);
	vsprintf(str,fmt,argptr);
	va_end(argptr);
	if(PrintY+20>Ymax)
    {
        PrintY=PrintY-paso;
        BitBlt(MemDC,0,0,Xmax,Ymax,MemDC,0,paso,SRCCOPY);
    }
	TextOut(MemDC,PrintX,PrintY,str,strlen(str));
	PrintY=PrintY+paso;
	BitBlt(hdc,0,0,Xmax,Ymax,MemDC,0,0,SRCCOPY);
	ReleaseDC(hWnd,hdc);
}
/*****************  Window::printf()    **************************************/
void Window::printf(const char *fmt,...)
{
	int paso=20;//Texto de catorce pixeles de alto
	BringWindowToTop(hWnd);
	if(SHOW==FALSE) Show();
	HDC hdc=GetDC(hWnd);
	va_list argptr;
	char str[140];
	va_start(argptr,fmt);
	vsprintf(str,fmt,argptr);
	va_end(argptr);
	if(PrintY+20>Ymax)
    {
        PrintY=PrintY-paso;
        BitBlt(MemDC,0,0,Xmax,Ymax,MemDC,0,paso,SRCCOPY);
    }
	TextOut(MemDC,PrintX,PrintY,str,strlen(str));
	PrintY=PrintY+paso;
	BitBlt(hdc,0,0,Xmax,Ymax,MemDC,0,0,SRCCOPY);
	ReleaseDC(hWnd,hdc);
}
/*****************  Window::printf()    **************************************/
void Window::printf(int Px,int Py,char *fmt,...)
{
	BringWindowToTop(hWnd);
	if(SHOW==FALSE)
        Show();
	HDC hdc=GetDC(hWnd);
	va_list argptr;
	char str[140];
	va_start(argptr,fmt);
	vsprintf(str,fmt,argptr);
	va_end(argptr);
	TextOut(MemDC,Px,Py,str,strlen(str));
	TextOut(hdc,Px,Py,str,strlen(str));
	ReleaseDC(hWnd,hdc);
}
/*****************  Window::printf()    **************************************/
void Window::printf(int Px,int Py,const char *fmt,...)
{
	BringWindowToTop(hWnd);
	if(SHOW==FALSE)
        Show();
	HDC hdc=GetDC(hWnd);
	va_list argptr;
	char str[140];
	va_start(argptr,fmt);
	vsprintf(str,fmt,argptr);
	va_end(argptr);
	TextOut(MemDC,Px,Py,str,strlen(str));
	TextOut(hdc,Px,Py,str,strlen(str));
	ReleaseDC(hWnd,hdc);
}
/*****************  Window::printf()   ***************************************/
char str[80];
char *Printf(char *fmt,...)
{
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf(str,fmt,argptr);
	va_end(argptr);
	return str;
}
/*****************************************************************************/
void Window::line(int x1,int y1,int x2,int y2)
{
    if(!SHOW)
      Show();
    BringWindowToTop(hWnd);
    HDC hdc=GetDC(hWnd);
    MoveToEx(MemDC,x1,y1,NULL);LineTo(MemDC,x2,y2);
    MoveToEx(hdc,x1,y1,NULL);LineTo(hdc,x2,y2);
    ReleaseDC(hWnd,hdc);
}
/*****************************************************************************/
void Window::GetPixelColor(int x,int y,COLORREF *col)
{
    if(!SHOW)
      Show();
    BringWindowToTop(hWnd);
    HDC hdc=GetDC(hWnd);
     *col = GetPixel(hdc,x,y);
    ReleaseDC(hWnd,hdc);
}
/*****************************************************************************/
void Window::Gtext(int x1,int y1,char *text)
{
    if(!SHOW)
        Show();
    BringWindowToTop(hWnd);
    HDC hdc=GetDC(hWnd);
    MoveToEx(MemDC,x1,y1,NULL);
    TextOut(MemDC,x1,y1,text,strlen(text)) ;
    MoveToEx(hdc,x1,y1,NULL);
    TextOut(hdc,x1,y1,text,strlen(text)) ;
    ReleaseDC(hWnd,hdc);
}
/*****************************************************************************/
void Window::line(int x1,int y1,int x2,int y2,COLORREF Color,int Size)
{
    HPEN OldhPen,hPen=CreatePen(PS_SOLID,Size,Color);
    OldhPen=(HPEN) SelectObject(MemDC,hPen);
    Window::line(x1,y1,x2,y2);
    SelectObject(MemDC,OldhPen);
    DeleteObject(hPen);
}
/*****************************************************************************/
void Window::PutPixel(int x,int y,int Size,COLORREF Color)
{
    //this->line(x,y,x,y,Color,Size);
    Window::line(x,y,x,y,Color,Size);
}
/********************* Para Graficar con linea escondida **********************/
void Window::Grafica(Matrix<float> &F,int x0,int y0,float Tam)
{
  Tam=Tam*140.0/((float)F.Rows());// Tam modify the Size of the graph
  HDC hdc=GetDC(hWnd);
  this->Show();
  int i,j,C=F.Rows(),R=F.Cols();
  POINT P[1000];
  for(j=0;j<R-1;j++)
  {
	for(i=0;i<C;i++)
    {
        P[i].x  =(LONG)(x0+0.8*(-j        +2.0*i)*Tam      +50);
        P[i].y  =(LONG)(y0-F(i,j)+Tam*j);
        P[i+C].x=(LONG)(x0+0.8*(-j-1+2.0*(C-1-i))*Tam+50);
        P[i+C].y=(LONG)(y0-F(C-1-i,j+1)+Tam*(j+1));
    }
	Polygon(MemDC,P,2*C);
	Polygon(hdc,P,2*C);
  }
  ReleaseDC(hWnd,hdc);
}
//------------------------------------------------------------------------------
/********************* Para Graficar con linea escondida **********************/
void Window::Grafica3D(float **F,int x0,int y0,float Tam,int nr,int nc,float fact)
{
  Tam=Tam*140.0/((float)nr);// Tam modify the Size of the graph
  HDC hdc=GetDC(hWnd);
  this->Show();
  int i,j,C=nr,R=nc;
  POINT P[1000];
  for(j=0;j<R-1;j++)
  {
	for(i=0;i<C;i++)
	 {
		  P[i].x  =(LONG)(x0+0.8*(-j        +2.0*i)*Tam      +50);
          P[i].y  =(LONG)(y0-F[j][i]*fact+Tam*j);
		  P[i+C].x=(LONG)(x0+0.8*(-j-1+2.0*(C-1-i))*Tam+50);
          P[i+C].y=(LONG)(y0-F[j+1][C-1-i]*fact+Tam*(j+1));
	 }
	Polygon(MemDC,P,2*C);
	Polygon(hdc,P,2*C);
  }
  ReleaseDC(hWnd,hdc);
}
//------------------------------------------------------------------------------
void Window::Grafica(Matrix<unsigned char> &B,int x0,int y0,float Tam)
{
	  Matrix<float> F(B.Rows(),B.Cols());
	  for(int i=0;i<B.Rows();i++)
	  for(int j=0;j<B.Cols();j++) F(i,j)=B(i,j);
	  Grafica(F,x0,y0,Tam);
}
/**************** GetOpenFileName () ******************************************/
char *GetFileName(char *Type)
 {
  char szFileTitle[256];
  char szFilter[]="Data Files (*.dat)\0*.dat\0Text Files (*.hol)\0*.hol\0";
  char *szFile=Type;
  OPENFILENAME ofn;

  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if(!GetOpenFileName(&ofn)) {return ofn.lpstrFileTitle;}
  else return NULL;
}
/******************* Create a Logical Pallete for gray displaying ************/
void Window::SetPalette(int *R,int *G,int *B)
{
   LOGPALETTE *plgpl;
   plgpl=(LOGPALETTE *) malloc(sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*255);
   plgpl->palNumEntries = 255;
   plgpl->palVersion = 0x300;
   for (int i=0;i<255;i++)
   {
       plgpl->palPalEntry[i].peRed  = LOBYTE(R[i]);
       plgpl->palPalEntry[i].peGreen= LOBYTE(G[i]);
       plgpl->palPalEntry[i].peBlue = LOBYTE(B[i]);
       plgpl->palPalEntry[i].peFlags= PC_RESERVED;
   }
   hpal = CreatePalette(plgpl);
   free((NPLOGPALETTE) plgpl);
   if(SHOW){
      Show();
      Paint();
   }
}
/*****************************************************************************/
//void Window::Bitmap(Matrix<unsigned char> &B,float zoom,int x0,int y0)
void Window::Bitmap(MByte &B,float zoom,int x0,int y0)
{
    int i,j,tx=B.Cols(),ty=B.Rows(),lx=(int)(zoom*((float)tx)),ly=(int)(zoom*((float)ty));
	 Vector<unsigned char> b(tx*ty);
    for(i=0;i<ty;i++)
        for(j=0;j<tx;j++)
        {
            //if(B(i,j)<10 )  B(i,j)=0;
            //if(B(i,j)>246) B(i,j)=255;
            b(i*tx+j)=B(i,j);
        }
    HDC hdc = GetDC(hWnd);
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hbit = CreateBitmapGray(hMemDC, tx, ty, b.Ptr());
    SelectObject(hMemDC,hbit);
    StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,tx,ty,SRCCOPY); //Copia Bitmap a MemDC
    DeleteObject(hbit);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd,hdc);
    Show();
    Paint();
}
/*****************************************************************************/
//void Window::Bitmap(Matrix<unsigned char> &B,float zoom,int x0,int y0)
void Window::PlotBitmap(float **f,int nr,int nc,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp%4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
for (i = 0 ; i < nr ; i++)
	for (j = 0 ; j < nc ; j++)
		{
			if( f[i][j] > aux) aux = f[i][j] ;
			if ( f[i][j] < auxm) auxm = f[i][j] ;
		}
		if (aux != auxm)
				aux = 230.0/(aux-auxm) ;
		else aux = 1 ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  bptr1[i*ncp+j] = (unsigned char)( (f[i][j] - auxm)*aux + 10)  ;
						else bptr1[i*ncp+j] = 255 ;
						}
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
    HDC hdc = GetDC(hWnd);
    HDC hMemDC = CreateCompatibleDC(hdc);
	 //HBITMAP hbit = CreateBitmapGray(hMemDC, tx, ty, b.Ptr());
	 HBITMAP hbit = CreateBitmapGray(hMemDC, ncp,nrp, bptr1);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
    DeleteObject(hbit);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd,hdc);
    Show();
	 Paint();
}
/*****************************************************************************/
void Window::PlotBitmap(unsigned char **f,int nr,int nc,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
for (i = 0 ; i < nr ; i++)
	for (j = 0 ; j < nc ; j++)
	  //if (f[i][j] < 0) f[i][j] = 0 ;
     if (f[i][j] > 230) f[i][j] = 230 ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp%4)
 nrp = ncp = nrp + 4 - (nrp%4)  ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  bptr1[i*ncp+j] =  (f[i][j]) ;
						else bptr1[i*ncp+j] = 255 ;
						}
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 //HBITMAP hbit = CreateBitmapGray(hMemDC, tx, ty, b.Ptr());
	 HBITMAP hbit = CreateBitmapGray(hMemDC, ncp,nrp, bptr1);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
void Window::PlotCBitmap(unsigned char **R,unsigned char **B,unsigned char **G,int nr,int nc,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr1[3*i*ncp+3*j]=G[i][j];
						  bptr1[3*i*ncp+3*j+1]=B[i][j];
						  bptr1[3*i*ncp+3*j+2]=R[i][j];
						  }
						else
						  {
						  bptr1[3*i*ncp+3*j]=255;
						  bptr1[3*i*ncp+3*j+1]=255;
						  bptr1[3*i*ncp+3*j+2]=255;
						  }
						}
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 HBITMAP hbit = CreateBitmapColor(hMemDC, ncp, nrp, bptr1);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
void Window::PlotCBitmap(COLORREF **col,int nr,int nc,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr1[3*i*ncp+3*j]=GetBValue(col[i][j]);
						  bptr1[3*i*ncp+3*j+1]=GetGValue(col[i][j]);
						  bptr1[3*i*ncp+3*j+2]=GetRValue(col[i][j]);
						  }
						else
						  {
						  bptr1[3*i*ncp+3*j]=255;
						  bptr1[3*i*ncp+3*j+1]=255;
						  bptr1[3*i*ncp+3*j+2]=255;
						  }
						}
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 HBITMAP hbit = CreateBitmapColor(hMemDC, ncp, nrp, bptr1);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
void Window::PlotCBitmapF(unsigned char *bptr,int nrp,int ncp,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 HBITMAP hbit = CreateBitmapColor(hMemDC, ncp, nrp, bptr);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
/*****************************************************************************/
void Window::PlotCBitmap(unsigned char **R,unsigned char **B,unsigned char **G,int i0,int j0,int nr,int nc,int x0,int y0,float zoom)
{
int i,j,lx,ly;
float aux=-1.e10,auxm = 1.e10 ;
int nrp,ncp ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr1[3*i*ncp+3*j]=G[i+i0][j+j0];
						  bptr1[3*i*ncp+3*j+1]=B[i+i0][j+j0];
						  bptr1[3*i*ncp+3*j+2]=R[i+i0][j+j0];
						  }
						else
						  {
						  bptr1[3*i*ncp+3*j]=255;
						  bptr1[3*i*ncp+3*j+1]=255;
						  bptr1[3*i*ncp+3*j+2]=255;
						  }
						}
	 lx = (int)(zoom*ncp) ; ly = (int)(zoom*nrp) ;
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 HBITMAP hbit = CreateBitmapColor(hMemDC, ncp, nrp, bptr1);
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,ncp,nrp,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
//void Window::Bitmap(Matrix<unsigned char> &B,float zoom,int x0,int y0)
void Window::CBitmap(MByte &R,MByte &G,MByte &B,float zoom,int x0,int y0)
{
    int i,j,tx=B.Cols(),ty=B.Rows(),lx=(int)(zoom*((float)tx)),ly=(int)(zoom*((float)ty));
	 Vector<unsigned char> b(3*tx*ty);
    for(i=0;i<ty;i++)
        for(j=0;j<tx;j++)
        {
            //if(B(i,j)<10 )  B(i,j)=0;
            //if(B(i,j)>246) B(i,j)=255;
				b(3*i*tx+3*j)=R(i,j);
				b(3*i*tx+3*j+1)=G(i,j);
				b(3*i*tx+3*j+2)=B(i,j);
		  }
	 HDC hdc = GetDC(hWnd);
	 HDC hMemDC = CreateCompatibleDC(hdc);
	 HBITMAP hbit = CreateBitmapColor(hMemDC, tx, ty, b.Ptr());
	 SelectObject(hMemDC,hbit);
	 StretchBlt(MemDC,x0,y0,lx,ly,hMemDC,0,0,tx,ty,SRCCOPY); //Copia Bitmap a MemDC
	 DeleteObject(hbit);
	 DeleteDC(hMemDC);
	 ReleaseDC(hWnd,hdc);
	 Show();
	 Paint();
}
/*****************************************************************************/
void Window::Bitmap(Matrix<float> &F,float zoom,int x0,int y0)
{
    Matrix<unsigned char> B(F.Rows(),F.Cols());
    for(int i=0;i<F.Rows();i++)
	  for(int j=0;j<F.Cols();j++)
        B(i,j)=(unsigned char)(F(i,j));
    Bitmap(B,zoom,x0,y0);
}
/***************** MessageYesNo() *********************************************/
BOOL MessageYesNo(char *p)
 {
 int R=MessageBox(NULL,p,"Pregunta",MB_YESNO|MB_ICONINFORMATION);
 if(R==IDYES) return TRUE;
 else return FALSE;
 }
/***************** MessageYesNo() *********************************************/
BOOL MessageYesNo(const char *p)
 {
 int R=MessageBox(NULL,p,"Pregunta",MB_YESNO|MB_ICONINFORMATION);
 if(R==IDYES) return TRUE;
 else return FALSE;
 }
/***************** MessageOk() *********************************************/
void MessageOk(char *p)
		{ MessageBox(NULL,p,"Message Box",MB_OK); }
/**************************************************************************/
/***************** MessageOk() *********************************************/
void MessageOk(const char *p)
		{ MessageBox(NULL,p,"Message Box",MB_OK); }
/**************************************************************************/
int MouseHit(void)
  {
  MSG msg;
  PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  if(msg.message==WM_LBUTTONDOWN) return 1;
  if(msg.message==WM_RBUTTONDOWN) return 2;
  return 0;
  }
/**************************************************************************/
int MouseHit(int *x,int *y)
  {
  MSG msg;
  PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  if (msg.message==WM_MOUSEMOVE || msg.message==WM_LBUTTONDOWN)
  {
     *x=LOWORD(msg.lParam);
     *y=HIWORD(msg.lParam);
  }
  if(msg.message==WM_LBUTTONDOWN) return 1;
  if(msg.message==WM_RBUTTONDOWN) return 2;
  return 0;
  }
/******************************************************************************/
void MinimizeAll()
	 {
	 for(int i=0;i<WindowsCount;i++) GetWinPtr(WinHand[i])->Hide();
	 if(hWndButt>0){ShowWindow(hWndButt,SW_RESTORE);BringWindowToTop(hWndButt);}
	 }
void RestoreAll()
	 {
	 for(int i=0;i<WindowsCount;i++) GetWinPtr(WinHand[i])->Show();
	 }
/******************************************************************************/
POINT ButtPos[20];
void CreateButtonWindow()
	 {
	 ButtonsWindow=TRUE;
	 int Count=0;
	 for(int i=0;i<=4;i++)
	 for(int j=0;j<=3;j++){ButtPos[Count].x=j;ButtPos[Count++].y=i;}

	 WNDCLASS wndclass;
	 memset(&wndclass, 0, sizeof(WNDCLASS));
	 wndclass.lpfnWndProc     = (WNDPROC) ButtonWndProc;
	 wndclass.hCursor         = LoadCursor(NULL,IDC_ARROW);
	 wndclass.hbrBackground   = (HBRUSH) CreateSolidBrush(150);
	 wndclass.lpszClassName   = "Buttons";
	 RegisterClass(&wndclass);
	 hWndButt = CreateWindow("buttons","Buttons Window",
									  WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,50,20,500,110,
									  NULL,NULL,NULL,NULL);
	 HMENU hMenu=CreateMenu();
	 AppendMenu(hMenu,MF_STRING,CM_EXIT,"E&xit");
	 AppendMenu(hMenu,MF_STRING,CM_MINIMIZE,"&Minimize-All");
	 AppendMenu(hMenu,MF_STRING,CM_RESTORE,"&Restore-All");
	 SetMenu(hWndButt,hMenu);
	 }
/*****************************************************************************/
void B(char *name,void (*ButtFunc) ())
{
    if(ButtonsWindow==FALSE)
        CreateButtonWindow();
    if(ButtonsCount>=20)
    {
        MessageOk((char *)"ERROR! no more than 20 Buttons allowed");
        return;
    }
    /*HWND hWndB=*/
    CreateWindow("Button",name,WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
                              10+ 150*ButtPos[ButtonsCount].x,
                              4 + 26 *ButtPos[ButtonsCount].y,
                              149,25,hWndButt,(HMENU) ButtonsCount,NULL,NULL);
    ButtFuncion[ButtonsCount]=ButtFunc;
    ButtonsCount=ButtonsCount+1;
}
/******************************************************************************/
int MainLoop()
{
	MSG msg;
	MoveWindow(hWndButt,50,50,620,35+25*ButtPos[ButtonsCount-1].y+45,TRUE);
	ShowWindow(hWndButt,SW_SHOW);
	while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return msg.wParam;
}
/******************************************************************************/
LONG WINAPI ButtonWndProc(HWND hWnd,UINT message,UINT wParam,LONG lParam)
	  {
		switch (message)
			{
			case WM_COMMAND:	// message: command from application menu
				switch(LOWORD(wParam))
					{
					case CM_EXIT:     exit(0); 		break;
					case CM_RESTORE:  RestoreAll();  break;
					case CM_MINIMIZE: MinimizeAll(); break;
					case 0:  ButtFuncion[0] (); break;
					case 1:  ButtFuncion[1] (); break;
					case 2:  ButtFuncion[2] (); break;
					case 3:  ButtFuncion[3] (); break;
					case 4:  ButtFuncion[4] (); break;
					case 5:  ButtFuncion[5] (); break;
					case 6:  ButtFuncion[6] (); break;
					case 7:  ButtFuncion[7] (); break;
					case 8:  ButtFuncion[8] (); break;
					case 9:  ButtFuncion[9] (); break;
					case 10: ButtFuncion[10](); break;
					case 11: ButtFuncion[11](); break;
					case 12: ButtFuncion[12](); break;
					case 13: ButtFuncion[13](); break;
					case 14: ButtFuncion[14](); break;
					case 15: ButtFuncion[15](); break;
					case 16: ButtFuncion[16](); break;
					case 17: ButtFuncion[17](); break;
					case 18: ButtFuncion[18](); break;
					case 19: ButtFuncion[19](); break;
					} break;
			case WM_QUIT:
			case WM_DESTROY:exit(0);break;
			default: return (DefWindowProc(hWnd, message, wParam, lParam));
			}
		return (0);
		}
/******************************************************************************/
int  x=0,y=0,MouseX=0,MouseY=0,Cx=0,Cy=0,Newx=0,Newy=0;
BOOL Drag=FALSE;
void Rec(int sx,int sy,int cx,int cy,HWND hWnd)
  {
  HDC hdc=GetDC(hWnd);
  SetROP2(hdc,R2_NOT);
  MoveToEx(hdc,sx,sy,NULL);
  LineTo(hdc,sx,cy);LineTo(hdc,cx,cy);
  LineTo(hdc,cx,sy);LineTo(hdc,sx,sy);
  ReleaseDC(hWnd,hdc);
  }
///////////////////////////////////////////////////////////////////////////////
void MouseMove(LPARAM lParam,HWND hWnd)
	{
	if(Drag)
		 {
		 Newx=LOWORD(lParam);Newy=HIWORD(lParam);
		 if((Newx!=MouseX)||(Newy!=MouseY))
			 {
			 Rec(MouseX,MouseY,Cx,Cy,hWnd);
			 Cx=Newx;Cy=Newy;
			 Rec(MouseX,MouseY,Cx,Cy,hWnd);
			 }
		 }
	 }
/******************************************************************************/
void CopiarOld(int xi,int yi,int xf,int yf,HWND hWnd)
{
  int xx=abs(xf-xi);
  int yy=abs(yf-yi);
  HDC hdc = GetDC(hWnd);        // Crear una imagen en memoria de las ventanas
  HDC MemDC = CreateCompatibleDC(hdc);
  //HBITMAP hbit = CreateBitmapGray(MemDC, xx, yy);
  HBITMAP hbit = CreateBitmapColor(MemDC, xx, yy);
  HBITMAP OldHbit = (HBITMAP)SelectObject(MemDC,hbit);
  if(xi>xf) xi=xf;
  if(yi>yf) yi=yf;
  BitBlt(MemDC, 0, 0, xx, yy, hdc, xi, yi, SRCCOPY);

  //hMemoryDC=CreateCompatibleDC(hdc);
  //hBitmap=CreateCompatibleBitmap(hdc,xx,yy);// Crea un DC en hMemoryDC
  //hOldBitmap=(HBITMAP) SelectObject(hMemoryDC,hBitmap);
  //if(xi>xf) xi=xf;if(yi>yf) yi=yf;
  //BitBlt(hMemoryDC,0,0,xx,yy,hdc,xi,yi,SRCCOPY);//Copia de hdc hacia hBitmap

  OpenClipboard(hWnd);                  //Abre Clipboard
  EmptyClipboard();
  SetClipboardData(CF_BITMAP,hbit);
  CloseClipboard();
  SelectObject(MemDC,OldHbit);
  DeleteDC(MemDC);
  ReleaseDC(hWnd,hdc);

  //SetClipboardData(CF_BITMAP,hBitmap);
  //CloseClipboard();
  //SelectObject(hMemoryDC,hOldBitmap);
  //DeleteDC(hMemoryDC);
  //ReleaseDC(hWnd,hdc);
}
///////////////////////////////////////////////////////////////////////////////

void Copiar(int xi,int yi,int xf,int yf,HWND hWnd)
{
  int xx=abs(xf-xi);
  int yy=abs(yf-yi);
  HDC hdc = GetDC(hWnd);  // Crear una imagen en memoria de las ventanas


  HDC MemDC = CreateCompatibleDC(hdc);
  HBITMAP hbit = CreateCompatibleBitmap(hdc, xx, yy);
  HBITMAP OldHbit = (HBITMAP)SelectObject(MemDC,hbit);
  if(xi>xf) xi=xf;
  if(yi>yf) yi=yf;
  BitBlt(MemDC, 0, 0, xx, yy, hdc, xi, yi, SRCCOPY);


  OpenClipboard(hWnd);
  EmptyClipboard();
  SetClipboardData(CF_BITMAP,hbit);
  CloseClipboard();
  SelectObject(MemDC,OldHbit);
  DeleteDC(MemDC);
  ReleaseDC(hWnd,hdc);
  DeleteObject(OldHbit);
  DeleteObject(hbit);
}

//********************************************************************
void HandlePoupMenu(HWND hWnd,POINT point)
	  {
	  HMENU hPopupMenu=CreatePopupMenu();
	  AppendMenu(hPopupMenu,MF_STRING,CM_EXIT,"E&xit");
	  AppendMenu(hPopupMenu,MF_STRING,CM_CORTAR,"&Copiar");
	  AppendMenu(hPopupMenu,MF_STRING,CM_MINIMIZETHIS,"&Minimize-This");
	  AppendMenu(hPopupMenu,MF_STRING,CM_MINIMIZE,"&Minimize-All");
	  AppendMenu(hPopupMenu,MF_STRING,CM_RESTORE,"&Restore-All");
	  ClientToScreen(hWnd,(LPPOINT) &point);
	  TrackPopupMenu(hPopupMenu,0,point.x,point.y,0,hWnd,NULL);
	  DestroyMenu(hPopupMenu);
	  }
//////////////////////////////////////////////////////////////////////////////
LONG WINAPI ChildWndProc(HWND hWnd,UINT message,UINT wParam,LONG lParam)
	  {
		PAINTSTRUCT ps;	// Paint Struct for BeginPaint call
		RECT rc;
		switch (message)
			{
			case WM_RBUTTONDOWN:
						  GetClientRect(hWnd,(LPRECT) &rc);
						  POINT pt;
						  pt.x=LOWORD(lParam);pt.y=HIWORD(lParam);
						  if(PtInRect((LPRECT) &rc,pt))HandlePoupMenu(hWnd,pt );
						  break;
			case WM_LBUTTONDOWN: GetWinPtr(hWnd)->Paint();
										Cx=MouseX=LOWORD(lParam);Cy=MouseY=HIWORD(lParam);
										Drag=TRUE;
										break;
			case WM_LBUTTONUP:   Drag=FALSE;  break;
			case WM_MOUSEMOVE:   MouseMove(lParam,hWnd);  break;
			case WM_PAINT:       BeginPaint(hWnd,&ps);
										GetWinPtr(hWnd)->Paint();
										EndPaint(hWnd,&ps);
										break;
			case WM_COMMAND:
				switch(LOWORD(wParam))
					{
					case CM_CORTAR:	Rec(MouseX,MouseY,Cx,Cy,hWnd);
											Copiar(MouseX,MouseY,Cx,Cy,hWnd);
											MouseX=0;MouseY=0;Cx=0;Cy=0;//Resetear valores
											break;
					case CM_RESTORE:      RestoreAll();            break;
					case CM_MINIMIZE:     MinimizeAll();           break;
					case CM_MINIMIZETHIS: GetWinPtr(hWnd)->Hide(); break;
					case CM_EXIT:         exit(0);                 break;
					} break;
			default: return (DefWindowProc(hWnd, message, wParam, lParam));
			}
		return (0);
		}
/************************ Dialog: FloatDiaProc *************************/
char OutDlgText[100],InDlgText[100];
#pragma argsused
BOOL WINAPI InTextDiaProc(HWND hdlg,UINT messg,UINT wParam,LONG lParam)
{
switch(messg)
  {
  case WM_INITDIALOG:
			CreateWindow("edit",NULL,WS_BORDER|ES_LEFT|WS_CHILD|WS_VISIBLE,
										10,24,500,22,hdlg,(HMENU) IDD_IN_TEXT,NULL,NULL);
			CreateWindow("static",OutDlgText,ES_LEFT|WS_CHILD|WS_VISIBLE,
										10,2,500,16,hdlg,NULL,NULL,NULL);
			CreateWindow("button","Ok",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,
										180,54,68,24,hdlg,(HMENU) IDOK,NULL,NULL);
			CreateWindow("button","Cancel",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE,
										280,54,68,24,hdlg,(HMENU) IDOK,NULL,NULL);
			return FALSE;
  case WM_COMMAND:
	 switch(LOWORD(wParam))
		{
		case IDOK:      GetDlgItemText(hdlg,IDD_IN_TEXT,(LPTSTR) InDlgText,50);
							 EndDialog(hdlg,TRUE);
							 break;
		case IDCANCEL:  EndDialog(hdlg,FALSE);
							 break;
		default:	return FALSE;
		} break;
	default: return FALSE;
	}
return TRUE;
}
/**************************************************************************/
char *InDialog(char *OutText)
	 {
	 DLGTEMPLATE *T;
	 memcpy(OutDlgText,OutText,50);
	 T=(LPDLGTEMPLATE) calloc(100,1);
	 T->style=WS_CAPTION|WS_DLGFRAME|WS_POPUP|WS_VISIBLE;
	 T->dwExtendedStyle=0;
	 T->cdit=0;T->x=10,T->y=10;T->cx=260;T->cy=42;
	 DialogBoxIndirect(NULL,T,hWndButt,InTextDiaProc);
	 free(T);
	 return InDlgText;
	 }
/************* Funcion para reservar una tabla de datos **********************/
void** Tabla(unsigned long rows,unsigned long cols,unsigned long sizeElem)
  {
  void** p;
  if((p=(void **) calloc(rows,sizeof(void *)))==NULL) return NULL;
  for(unsigned i=0;i<rows;i++)
	 if((p[i]=calloc(cols,sizeElem))==NULL) return NULL;
  return p;
  }
/************* Funcion para liberar una tabla de datos ***********************/
void freetabla(void **tabla,unsigned long rows)
  {  while(rows--) free(tabla[rows]); free(tabla);  }
/**************** Read a table  *****************************/
BOOL ReadFile(void **data,int x,int y,int size,char *fname)
 {
 OFSTRUCT ofStruct1;

 if(data == NULL) return FALSE;
 HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
 if(hFile1==HFILE_ERROR) return FALSE;
 for(int i=0; i < y; i++) _lread (hFile1, data[i], size*x);
 _lclose(hFile1);
 return TRUE;
 }
/************************ Write a table **************************************/
BOOL WriteFile(void **data,int x,int y,int size,char *fname)
	{
	OFSTRUCT ofStruct1;

	if(data == NULL) return FALSE;
	HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_CREATE);
	if(hFile1==HFILE_ERROR) return FALSE;
	for (int i=0; i<y; i++)  _lwrite(hFile1,(const char *) data[i], size*x);
	_lclose(hFile1);
	return TRUE;
	}
/**********************Lee imagen en formato *.bmp ***************************/
void Read256GrayBitmapFile(float **M,int nr,int nc,char *fname){
 OFSTRUCT ofStruct1;
 long i,j,x=nr,y=nc;

 HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
 unsigned char *p2=new unsigned char[x*y];//apuntador para los datos
 int
siz=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
 _lread(hFile1,p2,siz);// lee encabezado del DIB-BITMAP **********/
 _lread (hFile1,p2,x*y);// lee los Datos del DIB */
 _lclose(hFile1);
 for(i=0;i<x;i++)
 for(j=0;j<y;j++) M[y-1-j][i]=p2[i+j*x];

 delete[]p2;
 }
/*****************************************************************************/
void LeeBmp(float **M,char *fname)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;
     long i,j,x,y;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     x = BitInf->biWidth ;
     y = BitInf->biHeight ;
     unsigned char *p2=new unsigned char[x*y];//apuntador para los datos
     _lread (hFile1,p2,x*y);// lee los Datos del DIB */
     _lclose(hFile1);
     for(i=0;i<x;i++)
     for(j=0;j<y;j++) M[y-1-j][i]=p2[i+j*x];
     delete(BitHdr);delete(BitInf);
     delete[]p2;
     delete[]p1;
}
/*****************************************************************************/
void LeeBmp(float **M,const char *fname)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;
     long i,j,x,y;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     x = BitInf->biWidth ;
     y = BitInf->biHeight ;
     unsigned char *p2=new unsigned char[x*y];//apuntador para los datos
     _lread (hFile1,p2,x*y);// lee los Datos del DIB */
     _lclose(hFile1);
     for(i=0;i<x;i++)
     for(j=0;j<y;j++) M[y-1-j][i]=p2[i+j*x];
     delete(BitHdr);delete(BitInf);
     delete[]p2;
     delete[]p1;
}
/*****************************************************************************/
void LeeBmp(unsigned char **M,char *fname)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;
     long i,j,x,y;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     x = BitInf->biWidth ;
     y = BitInf->biHeight ;
     unsigned char *p2=new unsigned char[x*y];//apuntador para los datos
     _lread (hFile1,p2,x*y);// lee los Datos del DIB */
     _lclose(hFile1);
     for(i=0;i<x;i++)
     for(j=0;j<y;j++) M[y-1-j][i]=p2[i+j*x];
     delete(BitHdr);delete(BitInf);
     delete[]p2;
     delete[]p1;
}
/*****************************************************************************/
void LeeBmp(unsigned char **M,const char *fname)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;
     long i,j,x,y;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     x = BitInf->biWidth ;
     y = BitInf->biHeight ;
     unsigned char *p2=new unsigned char[x*y];//apuntador para los datos
     _lread (hFile1,p2,x*y);// lee los Datos del DIB */
     _lclose(hFile1);
     for(i=0;i<x;i++)
     for(j=0;j<y;j++) M[y-1-j][i]=p2[i+j*x];
     delete(BitHdr);delete(BitInf);
     delete[]p2;
     delete[]p1;
}
/*************************************************/
int LeeBmpColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre)
{
unsigned char *pixels;
struct tipoMetadatosBMP{
    int tamano;
    int reservado;
    int offset;
    int tamanoMetadatos;
    int ancho;
    int alto;
    short numeroPlanos;
    short profundidadColor;
    int tipoCompresion;
    int tamanoEstructura;
    int pxmv;
    int pxmh;
    int coloresUsados;
    int coloresImportantes;
}roca;
	FILE *bmp; 	int i,j,k,w,h; char head[3]={0};
	bmp=fopen(nombre,"rb");
	if(!bmp) return 0;
	fread(head,1,2,bmp);
	if(strcmp(head,"BM")!=0) return 0;
	fread(&roca,sizeof(roca),1,bmp);
	w=roca.ancho+((4-(roca.ancho%4))%4);
	h=(roca.alto);
	pixels=(unsigned char*) malloc(sizeof(unsigned char) * w * h *3) ;
	fread(pixels,sizeof(pixels[0]),w*h*3,bmp);
	fclose(bmp);
    h=roca.alto; w=roca.ancho;
	for(i=0,k=0;i<h;i++){
		for(j=0;j<w;j++,k+=3){
			R[h-i-1][j]=pixels[k+2];
			G[h-i-1][j]=pixels[k+1];
			B[h-i-1][j]=pixels[k+0];
		}
		for(;k%4;k++);
	}
	return 1;
}
//*****************************************************
int LeeBmpColor(COLORREF **col,const char *nombre)
{
unsigned char *pixels;
struct tipoMetadatosBMP{
    int tamano;
    int reservado;
    int offset;
    int tamanoMetadatos;
    int ancho;
    int alto;
    short numeroPlanos;
    short profundidadColor;
    int tipoCompresion;
    int tamanoEstructura;
    int pxmv;
    int pxmh;
    int coloresUsados;
    int coloresImportantes;
}roca;
	FILE *bmp; 	int i,j,k,w,h; char head[3]={0};
	bmp=fopen(nombre,"rb");
	if(!bmp) return 0;
	fread(head,1,2,bmp);
	if(strcmp(head,"BM")!=0) return 0;
	fread(&roca,sizeof(roca),1,bmp);
	w=roca.ancho+((4-(roca.ancho%4))%4);
	h=(roca.alto);
	pixels=(unsigned char*) malloc(sizeof(unsigned char) * w * h *3) ;
	fread(pixels,sizeof(pixels[0]),w*h*3,bmp);
	fclose(bmp);
    h=roca.alto; w=roca.ancho;
	for(i=0,k=0;i<h;i++){
		for(j=0;j<w;j++,k+=3){
			col[h-i-1][j]=RGB(pixels[k+2],pixels[k+1],pixels[k+0]) ;
		}
		for(;k%4;k++);
	}
	free(pixels) ;
	return 1;
}
//*****************************************************
/*****************************************************************************/
void DimensionesBmp(char *fname,int *pnr,int *pnc)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     *pnc = BitInf->biWidth ;
     *pnr = BitInf->biHeight ;
     _lclose(hFile1);
     delete(BitHdr);delete(BitInf);
     delete[]p1;
 }
/*****************************************************************************/
void DimensionesBmp(const char *fname,int *pnr,int *pnc)
{
     OFSTRUCT ofStruct1;
        BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
        BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;

     HFILE hFile1 = OpenFile(fname, &ofStruct1, OF_READ);
     int siz=256*sizeof(RGBQUAD);
     unsigned char *p1=new unsigned char[siz];//apuntador para los datos
     _lread(hFile1,BitHdr,sizeof(BITMAPFILEHEADER)) ;
     _lread(hFile1,BitInf,sizeof(BITMAPINFOHEADER)) ;
     _lread(hFile1,p1,siz);// lee encabezado del DIB-BITMAP **********/
     *pnc = BitInf->biWidth ;
     *pnr = BitInf->biHeight ;
     _lclose(hFile1);
     delete(BitHdr);delete(BitInf);
     delete[]p1;
 }
/******************************************************************/
/*
	La siguiente funcion escribe un objeto Float (Mtrix<float>) de tamano
M.Rows*M.Cols en un bitmap de 256 niveles de gris en un bitmap cuyo nombre
es "name"
*/
/********************Escribe *.bmp************************************/
void Write256GrayBitmapFile(float **M,int nr,int nc,char *name)
{
	OFSTRUCT ofStruct1;
	BITMAPFILEHEADER *BitHdr=new BITMAPFILEHEADER;
	BITMAPINFOHEADER *BitInf=new BITMAPINFOHEADER;
	RGBQUAD *rgb =new RGBQUAD[256];
	long x=nr,y=nc,i;
	char *p=new char[x*y];

	BitHdr->bfType     =0x4d42;
	BitHdr->bfSize     =(DWORD)(sizeof(BITMAPFILEHEADER)
                                +sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nr*nc)/4;
	BitHdr->bfReserved1=0;
	BitHdr->bfReserved2=0;
	BitHdr->bfOffBits   =sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
							  256*sizeof(RGBQUAD);
	BitInf->biSize         =sizeof(BITMAPINFOHEADER);
	BitInf->biWidth        =x;
	BitInf->biHeight       =y;
	BitInf->biPlanes       =1;
	BitInf->biBitCount     =8;
	BitInf->biCompression   =BI_RGB;
	BitInf->biSizeImage    =(y+7)*x;
	BitInf->biXPelsPerMeter=0;
	BitInf->biYPelsPerMeter=0;
	BitInf->biClrUsed      =0;
	BitInf->biClrImportant =0;
	for(i=0;i<255;i++){ // llenado de los 256 RGBQUAD's
	  rgb[i].rgbRed=rgb[i].rgbGreen=rgb[i].rgbBlue=LOBYTE (i);
	  rgb[i].rgbReserved=0;
	}
	for(i=0;i<x;i++)
	    for(int j=0;j<y;j++)
            p[i+j*x]=(char)(M[i][y-1-j]);//buffer de pixeles
	HFILE hFile = OpenFile(name,&ofStruct1,OF_CREATE|OF_WRITE);
	_lwrite(hFile,(LPCSTR) BitHdr,sizeof(BITMAPFILEHEADER));
	_lwrite(hFile,(LPCSTR) BitInf,sizeof(BITMAPINFOHEADER));
	_lwrite(hFile,(LPCSTR) rgb,256*sizeof(RGBQUAD));
	_lwrite(hFile,p,x*y);
	_lclose(hFile);
	delete(BitHdr);delete(BitInf);delete[](rgb);delete(p);
}
/******************************************************************************/
/************************** Fin del Archivo ***********************************/
/******************************************************************************/


#define STRICT
#include <windows.h>
#include <math.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "rbshell.h"
/*****************************************************************************/
DWORD dwColor [16] = {RGB(0,  0,  0),    // negro
                      RGB(255,0,  0),    // rojo
                      RGB(0,  255,0),    // verde
                      RGB(0,  0,  255),  // azul
                      RGB(0,  255,255),  // cian
                      RGB(255,0,  255),  // magenta
                      RGB(255,255,0),    // amarillo
                      RGB(80, 80, 80),   // gris1
                      RGB(170,170,170),  // gris2
                      RGB(255,255,255),  // blanco
                      // estos no se mapean, solo los primeros 10 (reservados)
                      RGB(80, 80, 0),    // color2
                      RGB(0,  80, 80),   // color3
                      RGB(80, 0,  80),   // color4
                      RGB(80, 0,  0),    // color5
                      RGB(0,  80, 0),    // color6
                      RGB(0,  0,  80)    // color7
                      };
/*****************************************************************************/
#define BLACK   dwColor [0]    //RGB(0,0,0)
#define RED     dwColor [1]    //RGB(255,0,0)
#define GREEN   dwColor [2]    //RGB(0,255,0)
#define BLUE    dwColor [3]    //RGB(0,0,255)
#define CYAN    dwColor [4]    //RGB(0,255,255)
#define MAGNETA dwColor [5]    //RGB(255,0,255)
#define YELLOW  dwColor [6]    //RGB(255,255,0)
#define GRAY1   dwColor [7]    //RGB(255,255,0)
#define GRAY2   dwColor [8]    //RGB(255,255,0)
#define WHITE   dwColor [9]    //RGB(255,255,255)

#define MByte    Matrix<unsigned char>
#define MFloat   Matrix<float>
#define MComplex Matrix<fcomplex>
#define VByte    Vector<unsigned char>
#define VFloat   Vector<float>
#define VComplex Vector<fcomplex>

/*****************************************************************************/
void DimensionesBmp(char *fname,int *pnr,int *pnc);
void LeeBmp(float **M,char *fname) ;
void DimensionesBmp(const char *fname,int *pnr,int *pnc);
void LeeBmp(float **M,const char *fname) ;
int LeeBmpColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre) ;
int LeeBmpColor(COLORREF **col,const char *nombre) ;
void LeeBmp(unsigned char **M,char *fname) ;
void Read256GrayBitmapFile(float **M,int nr,int nc,char *fname);
void Write256GrayBitmapFile(float **M,int nr,int nc,char *name);
void **Tabla(unsigned long, unsigned long, unsigned long);
void freetabla(void **tabla,unsigned long rows);
BOOL WriteFile(void **data,int x,int y,int size,char *fname);
BOOL ReadFile (void **data,int x,int y,int size,char *fname);
void MessageOk(char *);
void MessageOk(const char *);
char *Printf(char *fmt,...);
char *GetFileName(char *Type="*.dat");
int  MainLoop();
BOOL MessageYesNo(char *);
BOOL MessageYesNo(const char *);
int MouseHit();
int MouseHit(int *x,int *y);
BOOL Get_xy(int *x, int *y) ;
BOOL Kbhit(unsigned int *x) ;
BOOL Get_xy_Move(int *x, int *y) ;
char *InDialog(char *OutText="Input Text Dialog Box");
int random(int k) ;
void randomize() ;
float randfloat() ;
unsigned char  *FillBptr(unsigned char **R,unsigned char **B,unsigned char **G,int nr,int nc);
unsigned char  *FillBptr(COLORREF **col,int nr,int nc) ;
/*****************************************************************************/
template<class T> class Vector{
	 T *Vpt;
	 int size;
  public:
	 Vector(int S) {size=S; Vpt=(T *)calloc(size,sizeof(T));}
	 ~Vector(){free(Vpt);}
	 T & operator () (int Elem) { return Vpt[Elem]; }
	 T *Ptr(){return Vpt;}
	 int Size(){return size;}
};
/******************************************************************************/
template<class T> class Matrix{
	 int rows,cols;
	 T **Mpt;
  public:
	 Matrix(int R,int C){rows=R;cols=C;Mpt=(T **) Tabla(rows,cols,sizeof(T));}
	 ~Matrix(){freetabla((void **) Mpt,rows);}
	 int Rows() { return rows;}
	 int Cols() { return cols;}
	 T & operator () (int R,int C) { return Mpt[R][C]; }
	 T *Row(int R) {return Mpt[R];}
	 T **Ptr() {return Mpt;}
	 BOOL ReadFile(char *fname)
	  {return ::ReadFile( (void**) Mpt,cols,rows,sizeof(T),fname);}
	 BOOL WriteFile(char *fname)
	  {return ::WriteFile((void**) Mpt,cols,rows,sizeof(T),fname);}
};
/*****************************************************************************/
void B(char *ButtonName,void (*ButtFunc) ());
HBITMAP CreateBitmapGray   (HDC hdc, int Xmax, int Ymax);
HBITMAP CreateBitmapGray   (HDC hdc, int Xmax, int Ymax, const void *lpvBits);
/*****************************************************************************/
class Window{
	 BOOL     SHOW;
	 HWND     hWnd;
	 HDC      MemDC;
	 HBITMAP  hbit;
	 HPALETTE hpal;
	 int      Xmax,Ymax;
	 int      PrintX,PrintY;

  public:
	 Window(char *N="Window", int xmax=300,int ymax=280);
	 Window(const char *N="Window", int xmax=300,int ymax=280);
	 ~Window();
	 HWND Handle()  { return hWnd;  }
     HDC  HandleMemDC() { return MemDC;}
     int GetXMax()  { return Xmax;}
     int GetYMax()  { return Ymax;}
	 void Gotoxy(int x,int y)  { PrintX=x; PrintY=y;  }
	 void printf(char *, ... );
	 void printf(int Px,int Py,char *fmt,...);
	 void printf(const char *, ... );
	 void printf(int Px,int Py,const char *fmt,...);
	 void line(int x0,int y0,int x1,int y1);
    void Gtext(int x1,int y1, char *text) ;
	 void line(int x0,int y0,int x1,int x2,COLORREF Color,int Wide=1);
	 void Bitmap(MByte  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
	 //void PlotBitmap(unsigned char *bptr,int nr,int nc,float zoom=1.0,int OffsetX=10,int OffsetY=10);
	 void PlotBitmap(float **f,int nr,int nc,int x0,int y0,float zoom) ;
	 void PlotBitmap(unsigned char **f,int nr,int nc,int x0,int y0,float zoom) ;
	 void PlotCBitmap(unsigned char **r,unsigned char **g,unsigned char **b,int nr,int nc,int x0,int y0,float zoom) ;
     void PlotCBitmap(unsigned char **R,unsigned char **B,unsigned char **G,int i0,int j0,int nr,int nc,int x0,int y0,float zoom);
     void PlotCBitmap(COLORREF **col,int nr,int nc,int x0,int y0,float zoom) ;
     void PlotCBitmapF(unsigned char *bptr,int nr,int nc,int x0,int y0,float zoom);
	 void CBitmap(MByte  &R,MByte &G,MByte &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
	 //void Bitmap(Matrix<unsigned char>  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
	 void Bitmap(MFloat &F,float zoom=1.0,int OffsetX=10,int OffsetY=10);
	 void Grafica(MByte  &B,int OffsetX=0,int OffsetY=0,float Tam=1.0);
	 void Grafica(MFloat &F,int OffsetX=0,int OffsetY=0,float Tam=1.0);
	 void Show(){SHOW=TRUE;ShowWindow(hWnd,SW_RESTORE);BringWindowToTop(hWnd);}
	 void Hide(){SHOW=FALSE;ShowWindow(hWnd,SW_SHOWMINIMIZED);}
	 void SetPalette(int *R,int *G,int *B);
	 void Cls(){PatBlt(MemDC,0,0,Xmax,Ymax,WHITENESS);Paint();}
	 void PutPixel(int x,int y,int Size=3,COLORREF Color=0);
	 void GetPixelColor(int x,int y,COLORREF *col) ;
	 void Paint();
    void Grafica3D(float **f,int x0,int y0,float tam,int nr,int nc,float fact) ;
};
/************ Fin de archivo  ****************************************/









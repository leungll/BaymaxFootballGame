#include <GL/gl.h>
#include <GL/glut.h>
#include <windows.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#define ID_TIMER    1                               // 自动旋转时钟

//图像数据在内存块中的偏移量
#define BMP_Header_Length 54
GLuint tex1;

GLfloat rotStep = 0.20f;
GLfloat moveStep = 0.05f;
GLfloat moveStepFootball = 2.0f;

GLfloat xRot;                                       // 设置在X方向的旋转的速度定义
GLfloat yRot;                                       // 设置在Y方向的旋转的速度定义
GLfloat zRot;                                       // 设置在Z方向的旋转的速度定义

GLfloat xPosition;                                  // X方向的位置定义
GLfloat yPosition;                                  // Y方向的位置定义
GLfloat zPosition;                                  // Z方向的位置定义

GLfloat xPosFootbal;                                  // X方向的位置定义
GLfloat yPosFootbal;                                  // Y方向的位置定义
GLfloat zPosFootbal;                                  // Z方向的位置定义

GLboolean keys[256];                                // 用于键盘例程的数组
static GLboolean mouseFlag = FALSE;

static GLfloat colors[8][3] = {
	{ 1.0, 1.0, 1.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 1.0 },
	{ 0.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0 }
};

GLvoid Init();
GLvoid ChangeSize(GLsizei, GLsizei);
GLvoid Draw3D();
GLvoid SetupRC();
GLuint KeyPress();
GLuint MainWndMenu();
GLvoid CALLBACK timerProc(HWND, UINT, UINT, DWORD);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // 窗体过程函数声明
int commandBtns(HWND);
int commandBtnsLeft(HWND);

HINSTANCE hInstance;                                         // 系统实例句柄
HWND      hWndMain;                                          // 主窗体句柄
HMENU     hPopup;
HWND      hWnd;                                              // 窗体句柄变量定义
HDC       hDC;                                               // 设备描述表变量定义
PIXELFORMATDESCRIPTOR pfd;                                   // 像素格式结构变量定义
HGLRC     hGLRC;                                             // OpenGL渲染描述表变量定义

HMENU hMenuMain;                                             // 主窗体菜单句柄
HMENU hMenuWindow;
#define WINDOW_MENU_POS    1000
#define IDM_CTRL           2001
#define IDM_AUTO           2002
#define IDM_STOP           2003
#define IDM_RED            2004
#define IDM_YELLOW         2005
#define IDM_PINK           2006
#define IDM_WHITE          2007

HWND    hLabel;
HWND    hCmdBtns;
HWND    hCmdBtnsLeft;

#define IDC_FRAME          10000                // 框架控件唯一标识
#define IDC_RotateX1       10001                // 按钮控件唯一标识
#define IDC_RotateX2       10002                // 按钮控件唯一标识
#define IDC_RotateY1       10003                // 按钮控件唯一标识
#define IDC_RotateY2       10004                // 按钮控件唯一标识
#define IDC_RotateZ1       10005                // 按钮控件唯一标识
#define IDC_RotateZ2       10006                // 按钮控件唯一标识

#define IDC_TranslateX1    10011                // 按钮控件唯一标识
#define IDC_TranslateX2    10012                // 按钮控件唯一标识
#define IDC_TranslateY1    10013                // 按钮控件唯一标识
#define IDC_TranslateY2    10014                // 按钮控件唯一标识
#define IDC_TranslateZ1    10015                // 按钮控件唯一标识
#define IDC_TranslateZ2    10016                // 按钮控件唯一标识

#define IDC_FRAME_Football          10023       // 框架控件唯一标识
#define IDC_TranslateX1_Football    10017       // 按钮控件唯一标识
#define IDC_TranslateX2_Football    10018       // 按钮控件唯一标识
#define IDC_TranslateY1_Football    10019       // 按钮控件唯一标识
#define IDC_TranslateY2_Football    10020       // 按钮控件唯一标识
#define IDC_TranslateZ1_Football    10021       // 按钮控件唯一标识
#define IDC_TranslateZ2_Football    10022       // 按钮控件唯一标识

//********************************************************************
//*  打开OpenGL事件定义                                              *
//********************************************************************
//
GLuint Open(HWND phWnd) {
    hWnd = phWnd;

    hDC = GetDC(hWnd);

    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);                          // 格式描述符的大小
    pfd.nVersion        = 1;                       // 版本号
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; // 支持窗口双缓冲OPENGL
    pfd.dwLayerMask     = PFD_MAIN_PLANE;          //
    pfd.iPixelType      = PFD_TYPE_RGBA;           // RGBA格式
    pfd.cColorBits      = 24;                      // 色彩深度
    pfd.cRedBits        = 0;                       // 忽略的色彩位
    pfd.cRedShift       = 0;                       // 忽略的色彩位
    pfd.cGreenBits      = 0;                       // 忽略的色彩位
    pfd.cGreenShift     = 0;                       // 忽略的色彩位
    pfd.cBlueBits       = 0;                       // 忽略的色彩位
    pfd.cBlueShift      = 0;                       // 忽略的色彩位
    pfd.cAlphaBits      = 0;                       // 无Alpha缓存
    pfd.cAlphaShift     = 0;                       // 忽略Shift Bit
    pfd.cAccumBits      = 0;                       // 无聚集缓存
    pfd.cAccumRedBits   = 0;                       // 忽略聚集位
    pfd.cAccumGreenBits = 0;                       // 忽略聚集位
    pfd.cAccumBlueBits  = 0;                       // 忽略聚集位
    pfd.cAccumAlphaBits = 0;                       // 忽略聚集位
    pfd.cDepthBits      = 24;                      // 24位 Z-缓存 (深度缓存)
    pfd.cStencilBits    = 0;                       // 无模板缓存
    pfd.cAuxBuffers     = 0;                       // 无辅助缓存
    pfd.iLayerType      = 0;                       // 主绘图层
    pfd.bReserved       = 0;                       // 保留
    pfd.iLayerType      = 0;                       // 忽略层遮罩
    pfd.dwVisibleMask   = 0;                       // 忽略层遮罩
    pfd.dwDamageMask    = 0;                       // 忽略层遮罩

    int pixelformat = ChoosePixelFormat(hDC, &pfd);

    SetPixelFormat(hDC, pixelformat, &pfd);

    hGLRC = wglCreateContext(hDC);

    wglMakeCurrent(hDC,hGLRC);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);          // 初始化背景为灰色

    return 0;
} // 打开OpenGL事件定义

/********************************************************************/
/*  主窗体类注册函数定义                                            */
/********************************************************************/
//
GLuint MainWndCls() {
    //主窗体类注册函数定义
    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wcex.lpfnWndProc   = (WNDPROC)WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance,(LPCTSTR)IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = TEXT("WndCls");
    wcex.hIconSm       = LoadIcon(NULL,(LPCTSTR)IDI_APPLICATION);

    if(!RegisterClassEx(&wcex))
        return 1;
    return 0;
} // 主窗体类注册函数

/********************************************************************/
/*  建立主窗体函数定义                                              */
/********************************************************************/
//
GLuint MainWndCreate() {
    //建立主窗体
    hWndMain = CreateWindowEx(WS_EX_CLIENTEDGE,
                              TEXT("WndCls"),
                              TEXT("欢迎来到 大白爱踢球 小游戏!"),
                              WS_OVERLAPPED   |   WS_SYSMENU   |WS_MINIMIZEBOX|WS_CLIPCHILDREN,
                              200, 100, 1000, 600,
                              NULL,
                              hMenuMain,
                              hInstance,
                              NULL);

    if(!hWndMain)
        return 1;

    ShowWindow(hWndMain,SW_SHOWNORMAL);
    UpdateWindow(hWndMain);

    Init();

    return 0;
} // 建立主窗体函数

//********************************************************************
//*  主窗体过程函数定义                                          *
//********************************************************************
//
LRESULT CALLBACK WndProc(HWND   hWnd,
                         UINT   message,
                         WPARAM wParam,
                         LPARAM lParam) {
    POINT pt;
    POINT ptCurr;
    PAINTSTRUCT ps;
    switch (message) {
    case WM_CREATE:                                  // 窗体创建讯息
        commandBtns(hWnd);
        commandBtnsLeft(hWnd);
        Open(hWnd);
        break;

    case WM_PAINT:                                   // 重新绘制讯息
        BeginPaint(hWnd, &ps);
        SetCursor (LoadCursor (NULL, IDC_HAND));
        Draw3D();
        SetupRC();
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        EndPaint(hWnd,&ps);
        break;

    case WM_SIZE:                                    // 改变窗体大小讯息
        ChangeSize(LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_KEYDOWN:                                 // 响应按键操作讯息
        keys[wParam] = TRUE;
        KeyPress();
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        break;

    case WM_KEYUP:                                   // 响应松开键操作讯息
        keys[wParam] = FALSE;
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        break;

    case WM_LBUTTONDOWN:                             // 响应按下鼠标左键讯息
        mouseFlag = TRUE;
        GetCursorPos(&ptCurr);
        break;

    case WM_MOUSEMOVE:
        if(mouseFlag) {
            xRot = (((int)(short)LOWORD(lParam)) - ptCurr.x) * 0.001;
            yRot = (((int)(short)HIWORD(lParam)) - ptCurr.y) * 0.001;
            SendMessage(hWnd, WM_PAINT, 0, 0L);
        }
        break;

    case WM_LBUTTONUP:                               // 抬起鼠标左键讯息
        mouseFlag = FALSE;
        xRot = 0;
        yRot = 0;
        break;

    case WM_RBUTTONDOWN:                               // 抬起鼠标左键讯息
        GetCursorPos(&pt);
        TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, pt.x, pt.y, (DWORD)NULL, hWnd, NULL);
        break;

    case WM_MBUTTONDOWN:
        SetTimer(hWnd, ID_TIMER, 100, timerProc);
        break;

    case WM_MBUTTONUP:
        KillTimer(hWnd, ID_TIMER);
        break;

    case WM_COMMAND:
        switch(wParam) {
        case IDM_AUTO:
            SetTimer(hWnd, ID_TIMER, 100, timerProc);
            break;

        case IDM_STOP:
            KillTimer(hWnd, ID_TIMER);
            break;

        case IDM_RED:
            colors[7][0] = colors[1][0];
            colors[7][1] = colors[1][1];
            colors[7][2] = colors[1][2];
            break;

        case IDM_YELLOW:
            colors[7][0] = colors[6][0];
            colors[7][1] = colors[6][1];
            colors[7][2] = colors[6][2];
            break;

        case IDM_PINK:
            colors[7][0] = colors[3][0];
            colors[7][1] = colors[3][1];
            colors[7][2] = colors[3][2];
            break;

        case IDM_WHITE:
            colors[7][0] = colors[0][0];
            colors[7][1] = colors[0][1];
            colors[7][2] = colors[0][2];
            break;

        case IDC_RotateX1:
            xRot += rotStep;
            break;

        case IDC_RotateX2:
            xRot -= rotStep;
            break;

        case IDC_RotateY1:
            yRot += rotStep;
            break;

        case IDC_RotateY2:
            yRot -= rotStep;
            break;

        case IDC_RotateZ1:
            zRot += rotStep;
            break;

        case IDC_RotateZ2:
            zRot -= rotStep;
            break;

        case IDC_TranslateX1:
            xPosition -= moveStep;
            break;

        case IDC_TranslateX2:
            xPosition += moveStep;
            break;

        case IDC_TranslateY1:
            yPosition += moveStep;
            break;

        case IDC_TranslateY2:
            yPosition -= moveStep;
            break;

        case IDC_TranslateZ1:
            zPosition += moveStep;
            break;

        case IDC_TranslateZ2:
            zPosition -= moveStep;
            break;

        case IDC_TranslateX1_Football:
            xPosFootbal -= moveStepFootball;
            break;

        case IDC_TranslateX2_Football:
            xPosFootbal += moveStepFootball;
            break;

        case IDC_TranslateY1_Football:
            yPosFootbal += moveStepFootball;
            break;

        case IDC_TranslateY2_Football:
            yPosFootbal -= moveStepFootball;
            break;

        case IDC_TranslateZ1_Football:
            zPosFootbal += moveStepFootball;
            break;

        case IDC_TranslateZ2_Football:
            zPosFootbal -= moveStepFootball;
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        SetFocus(hWnd);
        break;

    case WM_DESTROY:                                 // 退出程序
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}// 窗体过程定义

//********************************************************************
//*  主控函数                                                        *
//********************************************************************
//
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow) {
    MSG msg;                                            // 定义消息

    if(MainWndCls())
        return 1;                                       // 主窗体类注册

    MainWndMenu();                                      // 显示主窗体菜单

    if(MainWndCreate())
        return 2;                                       // 建立主窗体

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);                         // 转换消息
        DispatchMessage(&msg);                          // 分发消息
    }

    return (int)msg.wParam;
}// 主控函数

//********************************************************************
//*  OpenGL初始化场景事件                                            *
//********************************************************************
//
GLvoid Init() {
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);                       // 使用调和
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			        // 设置背景色为黑色
    glClearDepth(1.0);					                    // 清除深度缓存
    glDepthFunc(GL_LESS);					                // 设置为LESS
    glEnable(GL_DEPTH_TEST);				                // 深度缓存可用
    glShadeModel(GL_SMOOTH);				                // Enables Smooth Color Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	    // 呈现Niceset
} //OpenGL初始化场景事件

//********************************************************************
//*  使场景随着窗口的大小变化而变化事件定义                          *
//********************************************************************
//
GLvoid ChangeSize(GLsizei width, GLsizei height) {
    float ar = (float) (width) / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}// 使场景随着窗口的大小变化而变化事件定义

//函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n) {
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name) {
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID=0, texture_ID = 0;

    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 ) {
        fclose(pFile);
        return 0;
    }

    // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width) || !power_of_two(height) || width > max || height > max ) {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 ) {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            // 进行像素缩放
            gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID=last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,0x80E0, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}

//********************************************************************
//*  对图形进行绘制(渲染)事件                                        *
//********************************************************************
//
GLvoid Draw3D() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //旋转
    glTranslated(0, 1.5, -50);
    glRotatef(xRot, 1, 0, 0);
    glTranslated(0, -1.5, 50);

    glTranslated(0, 1.5, -50);
    glRotated(yRot, 0, 1, 0);
    glTranslated(0, -1.5, 50);

    glTranslated(0, 1.5, -50);
    glRotated(zRot, 0, 1, 0);
    glTranslated(0, -1.5, 50);

    //平移
    glTranslated(xPosition, yPosition, zPosition);

    //大白左手
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-11, 1.5, -50);
        glRotatef(-30, 0.0, 0.0, 1.0);
        glScalef(1.9, 4, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //大白右手
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(7, 1.5, -50);
        glRotatef(30, 0.0, 0.0, 1.0),
        glScalef(1.9, 4, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //大白脑袋
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-2.1, 14.5, -50);
        glScalef(3, 2.5, 2.5);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //大白左眼睛
    glPushMatrix();
        glColor3f(colors[5][0], colors[5][1], colors[5][2]);
        glTranslated(-4.3, 14, -45);
        glScalef(1.5, 1.5, 2);
        glutSolidSphere(0.5, 200, 200);
    glPopMatrix();

    //大白右眼睛
    glPushMatrix();
        glColor3f(colors[5][0], colors[5][1], colors[5][2]);
        glTranslated(0.6, 14, -45);
        glScalef(1.5, 1.5, 2);
        glutSolidSphere(0.5, 200, 200);
    glPopMatrix();

    //大白笑脸
    glPushMatrix();
        glColor3f(colors[5][0], colors[5][1], colors[5][2]);
        glEnable(GL_LINE_SMOOTH);
        glBegin(GL_LINE_STRIP);
        glVertex3f(-4.3, 14, -45);
        glVertex3f(-1.8, 12.8, -45);
        glVertex3f(0.6, 14, -45);
        glEnd();
        glDisable(GL_LINE_SMOOTH);
    glPopMatrix();

    //大白肚子
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-2.1, -1, -50);
        glScalef(4, 4, 2.5);
        glutSolidSphere(3.0, 200, 200);
    glPopMatrix();

    //大白左腿
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-6, -10, -50);
        glScalef(2, 5, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //大白右腿
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(1.8, -10, -50);
        glRotatef(0, 0.0, 0.0, 1.0),
        glScalef(2, 5, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //足球
    glPushMatrix();
        //开启纹理
        glEnable(GL_TEXTURE_2D);
        //加载纹理
        tex1 = load_texture("data\\box.bmp");
        //选择纹理
        glBindTexture(GL_TEXTURE_2D, tex1);
        GLUquadricObj * sphere = gluNewQuadric();
        gluQuadricOrientation(sphere, GLU_OUTSIDE);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluQuadricTexture(sphere, GL_TRUE);

        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(15 + xPosFootbal, -15 + yPosFootbal, -45 + zPosFootbal);
        glScalef(2.5, 2.5, 2.5);

        gluSphere(sphere, 2, 50, 50);
        gluDeleteQuadric(sphere);
        //关闭纹理
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    SwapBuffers(hDC);                              // 交换缓存(双缓存)
} //对图形进行绘制(渲染)事件

GLvoid SetupRC() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    //环境光一般设置为黑色
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    //漫反射反映物体整体影响和环境光：与物体本身颜色有叠加（物体本身有颜色，漫反射就设置为白色）
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    //镜面反射；环境光+漫反射+镜面反射叠加
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //光源：找不到的部分认为环境光对其有影响，用环境光颜色填充
    GLfloat light_position[] = { 2.1f, 1.0f, 50.0f, 0.0f };
    GLfloat light_position2[] = { -2.0f, -5.0f, -5.0f, 0.0f };
    GLfloat light_position3[] = { -2.1f, -2.1f, -50.0f, 0.0f };

    GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat high_shininess[] = { 100.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

//********************************************************************
//*  键盘控制图形事件定义                                            *
//********************************************************************
//
GLuint KeyPress() {

    //旋转
    if (keys[VK_UP])           xRot -= rotStep;                     // 按上箭头键
    if (keys[VK_DOWN])         xRot += rotStep;                     // 按下箭头键
    if (keys[VK_LEFT])         yRot -= rotStep;                     // 按左箭头键
    if (keys[VK_RIGHT])        yRot += rotStep;                     // 按右箭头键
    if (keys[VK_HOME])         zRot += rotStep;                     // 按HOME键
    if (keys[VK_END])          zRot -= rotStep;                     // 按END键

    //平移
    if (keys['A'])             xPosition -= moveStep;               // 按A键
    if (keys['D'])             xPosition += moveStep;               // 按D键
    if (keys['W'])             yPosition += moveStep;               // 按W键
    if (keys['S'])             yPosition -= moveStep;               // 按S键
    if (keys['Q'])             zPosition -= moveStep;               // 按Q键
    if (keys['E'])             zPosition += moveStep;               // 按E键

    if (keys[VK_ESCAPE])                                            // 按ESC键
        SendMessage(hWnd, WM_DESTROY, 0, 0);

    return 0;
} // 键盘控制图形事件定义

//********************************************************************
//*  计时器函数定义                                          *
//********************************************************************
//
GLvoid CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
    keys[VK_LEFT] = true;
    KeyPress();
    SendMessage(hWnd, WM_PAINT, 0, 0L);
}

/********************************************************************/
/*  主窗体菜单定义函数定义                                          */
/********************************************************************/
//
GLuint MainWndMenu() {
    hMenuMain = CreateMenu();
    hPopup = CreatePopupMenu();
    AppendMenu(hPopup, MF_STRING, IDM_AUTO, TEXT("Auto-Rotate"));
    AppendMenu(hPopup, MF_STRING, IDM_STOP, TEXT("Stop"));
    AppendMenu(hPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hPopup, MF_STRING, IDM_RED, TEXT("点我大白会变红色！"));
    AppendMenu(hPopup, MF_STRING, IDM_YELLOW, TEXT("点我大白会变黄色！"));
    AppendMenu(hPopup, MF_STRING, IDM_PINK, TEXT("点我大白会变粉色！"));
    AppendMenu(hPopup, MF_STRING, IDM_WHITE, TEXT("点我大白会变白色！"));

    AppendMenu(hMenuMain,MF_POPUP,(int)hPopup,TEXT("CONTROL"));

    return 0;
} // 主窗体菜单定义函数

/********************************************************************/
/*  建立窗体控件函数定义                                            */
/********************************************************************/
int commandBtns(HWND hWndParent) {
    int diff = 810;
    hCmdBtns = CreateWindowEx((DWORD)NULL,
                              "BUTTON",
                              "",
                              WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                              diff + 0, 0, 180, 600,
                              hWndParent,
                              (HMENU)IDC_FRAME,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 1;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "x-",
                              WS_CHILD|WS_VISIBLE,
                              diff + 20, 100, 42, 32,
                              hWndParent,
                              (HMENU)IDC_RotateX1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 2;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "x+",
                              WS_CHILD|WS_VISIBLE,
                              diff + 120, 100, 42, 32,
                              hWndParent,
                              (HMENU)IDC_RotateX2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 3;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "y+",
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 60, 42, 32,
                              hWndParent,
                              (HMENU)IDC_RotateY1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 4;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "y-",
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 140, 42, 32,
                              hWndParent,
                              (HMENU)IDC_RotateY2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 5;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "z+",
                              WS_CHILD|WS_VISIBLE,
                              diff+120,60,42,32,
                              hWndParent,
                              (HMENU)IDC_RotateZ1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 6;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "z-",
                              WS_CHILD|WS_VISIBLE,
                              diff+20,140,42,32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 7;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建标签控件
                              "STATIC",
                              "控制大白踢球(旋转):",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 20, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 8;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建标签控件
                              "STATIC",
                              "控制大白踢球(平移):",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 260, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 9;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "<<",
                              WS_CHILD|WS_VISIBLE,
                              diff+20,340,42,32,
                              hWndParent,
                              (HMENU)IDC_TranslateX1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 10;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              ">>",
                              WS_CHILD|WS_VISIBLE,
                              diff+120,340,42,32,
                              hWndParent,
                              (HMENU)IDC_TranslateX2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 11;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              TEXT("UP"),
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 300, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateY1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 12;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "DN",
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 380, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateY2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 13;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "ZI",
                              WS_CHILD|WS_VISIBLE,
                              diff+120,300,42,32,
                              hWndParent,
                              (HMENU)IDC_TranslateZ1,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 14;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "ZO",
                              WS_CHILD|WS_VISIBLE,
                              diff+20,300,42,32,
                              hWndParent,
                              (HMENU)IDC_TranslateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 15;

    return 0;
}

/********************************************************************/
/*  建立窗体控件函数定义                                            */
/********************************************************************/
int commandBtnsLeft(HWND hWndParent) {
    int diff = 0;
    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,
                              "BUTTON",
                              "",
                              WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                              diff + 0, 0, 180, 600,
                              hWndParent,
                              (HMENU)IDC_FRAME_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 16;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建标签控件
                              "STATIC",
                              "控制足球（平移）:",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 190, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 17;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "<<",
                              WS_CHILD|WS_VISIBLE,
                              diff + 20, 270, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateX1_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 18;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              ">>",
                              WS_CHILD|WS_VISIBLE,
                              diff + 120, 270, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateX2_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 19;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              TEXT("UP"),
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 230, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateY1_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 20;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "DN",
                              WS_CHILD|WS_VISIBLE,
                              diff + 72, 310, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateY2_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 21;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "ZI",
                              WS_CHILD|WS_VISIBLE,
                              diff + 120, 230, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateZ1_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 22;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                              "BUTTON",
                              "ZO",
                              WS_CHILD|WS_VISIBLE,
                              diff + 20, 230, 42, 32,
                              hWndParent,
                              (HMENU)IDC_TranslateZ2_Football,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 23;

    return 0;
}

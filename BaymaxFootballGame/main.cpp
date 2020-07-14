#include <GL/gl.h>
#include <GL/glut.h>
#include <windows.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#define ID_TIMER    1                               // �Զ���תʱ��

//ͼ���������ڴ���е�ƫ����
#define BMP_Header_Length 54
GLuint tex1;

GLfloat rotStep = 0.20f;
GLfloat moveStep = 0.05f;
GLfloat moveStepFootball = 2.0f;

GLfloat xRot;                                       // ������X�������ת���ٶȶ���
GLfloat yRot;                                       // ������Y�������ת���ٶȶ���
GLfloat zRot;                                       // ������Z�������ת���ٶȶ���

GLfloat xPosition;                                  // X�����λ�ö���
GLfloat yPosition;                                  // Y�����λ�ö���
GLfloat zPosition;                                  // Z�����λ�ö���

GLfloat xPosFootbal;                                  // X�����λ�ö���
GLfloat yPosFootbal;                                  // Y�����λ�ö���
GLfloat zPosFootbal;                                  // Z�����λ�ö���

GLboolean keys[256];                                // ���ڼ������̵�����
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
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // ������̺�������
int commandBtns(HWND);
int commandBtnsLeft(HWND);

HINSTANCE hInstance;                                         // ϵͳʵ�����
HWND      hWndMain;                                          // ��������
HMENU     hPopup;
HWND      hWnd;                                              // ��������������
HDC       hDC;                                               // �豸�������������
PIXELFORMATDESCRIPTOR pfd;                                   // ���ظ�ʽ�ṹ��������
HGLRC     hGLRC;                                             // OpenGL��Ⱦ�������������

HMENU hMenuMain;                                             // ������˵����
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

#define IDC_FRAME          10000                // ��ܿؼ�Ψһ��ʶ
#define IDC_RotateX1       10001                // ��ť�ؼ�Ψһ��ʶ
#define IDC_RotateX2       10002                // ��ť�ؼ�Ψһ��ʶ
#define IDC_RotateY1       10003                // ��ť�ؼ�Ψһ��ʶ
#define IDC_RotateY2       10004                // ��ť�ؼ�Ψһ��ʶ
#define IDC_RotateZ1       10005                // ��ť�ؼ�Ψһ��ʶ
#define IDC_RotateZ2       10006                // ��ť�ؼ�Ψһ��ʶ

#define IDC_TranslateX1    10011                // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateX2    10012                // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateY1    10013                // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateY2    10014                // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateZ1    10015                // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateZ2    10016                // ��ť�ؼ�Ψһ��ʶ

#define IDC_FRAME_Football          10023       // ��ܿؼ�Ψһ��ʶ
#define IDC_TranslateX1_Football    10017       // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateX2_Football    10018       // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateY1_Football    10019       // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateY2_Football    10020       // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateZ1_Football    10021       // ��ť�ؼ�Ψһ��ʶ
#define IDC_TranslateZ2_Football    10022       // ��ť�ؼ�Ψһ��ʶ

//********************************************************************
//*  ��OpenGL�¼�����                                              *
//********************************************************************
//
GLuint Open(HWND phWnd) {
    hWnd = phWnd;

    hDC = GetDC(hWnd);

    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);                          // ��ʽ�������Ĵ�С
    pfd.nVersion        = 1;                       // �汾��
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; // ֧�ִ���˫����OPENGL
    pfd.dwLayerMask     = PFD_MAIN_PLANE;          //
    pfd.iPixelType      = PFD_TYPE_RGBA;           // RGBA��ʽ
    pfd.cColorBits      = 24;                      // ɫ�����
    pfd.cRedBits        = 0;                       // ���Ե�ɫ��λ
    pfd.cRedShift       = 0;                       // ���Ե�ɫ��λ
    pfd.cGreenBits      = 0;                       // ���Ե�ɫ��λ
    pfd.cGreenShift     = 0;                       // ���Ե�ɫ��λ
    pfd.cBlueBits       = 0;                       // ���Ե�ɫ��λ
    pfd.cBlueShift      = 0;                       // ���Ե�ɫ��λ
    pfd.cAlphaBits      = 0;                       // ��Alpha����
    pfd.cAlphaShift     = 0;                       // ����Shift Bit
    pfd.cAccumBits      = 0;                       // �޾ۼ�����
    pfd.cAccumRedBits   = 0;                       // ���Ծۼ�λ
    pfd.cAccumGreenBits = 0;                       // ���Ծۼ�λ
    pfd.cAccumBlueBits  = 0;                       // ���Ծۼ�λ
    pfd.cAccumAlphaBits = 0;                       // ���Ծۼ�λ
    pfd.cDepthBits      = 24;                      // 24λ Z-���� (��Ȼ���)
    pfd.cStencilBits    = 0;                       // ��ģ�建��
    pfd.cAuxBuffers     = 0;                       // �޸�������
    pfd.iLayerType      = 0;                       // ����ͼ��
    pfd.bReserved       = 0;                       // ����
    pfd.iLayerType      = 0;                       // ���Բ�����
    pfd.dwVisibleMask   = 0;                       // ���Բ�����
    pfd.dwDamageMask    = 0;                       // ���Բ�����

    int pixelformat = ChoosePixelFormat(hDC, &pfd);

    SetPixelFormat(hDC, pixelformat, &pfd);

    hGLRC = wglCreateContext(hDC);

    wglMakeCurrent(hDC,hGLRC);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);          // ��ʼ������Ϊ��ɫ

    return 0;
} // ��OpenGL�¼�����

/********************************************************************/
/*  ��������ע�ắ������                                            */
/********************************************************************/
//
GLuint MainWndCls() {
    //��������ע�ắ������
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
} // ��������ע�ắ��

/********************************************************************/
/*  ���������庯������                                              */
/********************************************************************/
//
GLuint MainWndCreate() {
    //����������
    hWndMain = CreateWindowEx(WS_EX_CLIENTEDGE,
                              TEXT("WndCls"),
                              TEXT("��ӭ���� ��װ����� С��Ϸ!"),
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
} // ���������庯��

//********************************************************************
//*  ��������̺�������                                          *
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
    case WM_CREATE:                                  // ���崴��ѶϢ
        commandBtns(hWnd);
        commandBtnsLeft(hWnd);
        Open(hWnd);
        break;

    case WM_PAINT:                                   // ���»���ѶϢ
        BeginPaint(hWnd, &ps);
        SetCursor (LoadCursor (NULL, IDC_HAND));
        Draw3D();
        SetupRC();
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        EndPaint(hWnd,&ps);
        break;

    case WM_SIZE:                                    // �ı䴰���СѶϢ
        ChangeSize(LOWORD(lParam),HIWORD(lParam));
        break;

    case WM_KEYDOWN:                                 // ��Ӧ��������ѶϢ
        keys[wParam] = TRUE;
        KeyPress();
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        break;

    case WM_KEYUP:                                   // ��Ӧ�ɿ�������ѶϢ
        keys[wParam] = FALSE;
        SendMessage(hWnd, WM_PAINT, 0, 0L);
        break;

    case WM_LBUTTONDOWN:                             // ��Ӧ����������ѶϢ
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

    case WM_LBUTTONUP:                               // ̧��������ѶϢ
        mouseFlag = FALSE;
        xRot = 0;
        yRot = 0;
        break;

    case WM_RBUTTONDOWN:                               // ̧��������ѶϢ
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

    case WM_DESTROY:                                 // �˳�����
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}// ������̶���

//********************************************************************
//*  ���غ���                                                        *
//********************************************************************
//
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow) {
    MSG msg;                                            // ������Ϣ

    if(MainWndCls())
        return 1;                                       // ��������ע��

    MainWndMenu();                                      // ��ʾ������˵�

    if(MainWndCreate())
        return 2;                                       // ����������

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);                         // ת����Ϣ
        DispatchMessage(&msg);                          // �ַ���Ϣ
    }

    return (int)msg.wParam;
}// ���غ���

//********************************************************************
//*  OpenGL��ʼ�������¼�                                            *
//********************************************************************
//
GLvoid Init() {
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);                       // ʹ�õ���
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			        // ���ñ���ɫΪ��ɫ
    glClearDepth(1.0);					                    // �����Ȼ���
    glDepthFunc(GL_LESS);					                // ����ΪLESS
    glEnable(GL_DEPTH_TEST);				                // ��Ȼ������
    glShadeModel(GL_SMOOTH);				                // Enables Smooth Color Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	    // ����Niceset
} //OpenGL��ʼ�������¼�

//********************************************************************
//*  ʹ�������Ŵ��ڵĴ�С�仯���仯�¼�����                          *
//********************************************************************
//
GLvoid ChangeSize(GLsizei width, GLsizei height) {
    float ar = (float) (width) / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}// ʹ�������Ŵ��ڵĴ�С�仯���仯�¼�����

//����power_of_two�����ж�һ�������ǲ���2����������
int power_of_two(int n) {
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name) {
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID=0, texture_ID = 0;

    // ���ļ������ʧ�ܣ�����
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

    // ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // �����������ֽ��������ڴ�
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 ) {
        fclose(pFile);
        return 0;
    }

    // ��ȡ��������
    if( fread(pixels, total_bytes, 1, pFile) <= 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
    // ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width) || !power_of_two(height) || width > max || height > max ) {
            const GLint new_width = 256;
            const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            // ����ÿ����Ҫ���ֽ��������ֽ���
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            // �����ڴ�
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 ) {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            // ������������
            gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    // ����һ���µ�������
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    // ���µ������������������������
    // �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
    GLint lastTextureID=last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,0x80E0, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
    free(pixels);
    return texture_ID;
}

//********************************************************************
//*  ��ͼ�ν��л���(��Ⱦ)�¼�                                        *
//********************************************************************
//
GLvoid Draw3D() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //��ת
    glTranslated(0, 1.5, -50);
    glRotatef(xRot, 1, 0, 0);
    glTranslated(0, -1.5, 50);

    glTranslated(0, 1.5, -50);
    glRotated(yRot, 0, 1, 0);
    glTranslated(0, -1.5, 50);

    glTranslated(0, 1.5, -50);
    glRotated(zRot, 0, 1, 0);
    glTranslated(0, -1.5, 50);

    //ƽ��
    glTranslated(xPosition, yPosition, zPosition);

    //�������
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-11, 1.5, -50);
        glRotatef(-30, 0.0, 0.0, 1.0);
        glScalef(1.9, 4, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //�������
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(7, 1.5, -50);
        glRotatef(30, 0.0, 0.0, 1.0),
        glScalef(1.9, 4, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //����Դ�
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-2.1, 14.5, -50);
        glScalef(3, 2.5, 2.5);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //������۾�
    glPushMatrix();
        glColor3f(colors[5][0], colors[5][1], colors[5][2]);
        glTranslated(-4.3, 14, -45);
        glScalef(1.5, 1.5, 2);
        glutSolidSphere(0.5, 200, 200);
    glPopMatrix();

    //������۾�
    glPushMatrix();
        glColor3f(colors[5][0], colors[5][1], colors[5][2]);
        glTranslated(0.6, 14, -45);
        glScalef(1.5, 1.5, 2);
        glutSolidSphere(0.5, 200, 200);
    glPopMatrix();

    //���Ц��
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

    //��׶���
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-2.1, -1, -50);
        glScalef(4, 4, 2.5);
        glutSolidSphere(3.0, 200, 200);
    glPopMatrix();

    //�������
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(-6, -10, -50);
        glScalef(2, 5, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //�������
    glPushMatrix();
        glColor3f(colors[7][0], colors[7][1], colors[7][2]);
        glTranslated(1.8, -10, -50);
        glRotatef(0, 0.0, 0.0, 1.0),
        glScalef(2, 5, 2);
        glutSolidSphere(2.0, 200, 200);
    glPopMatrix();

    //����
    glPushMatrix();
        //��������
        glEnable(GL_TEXTURE_2D);
        //��������
        tex1 = load_texture("data\\box.bmp");
        //ѡ������
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
        //�ر�����
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    SwapBuffers(hDC);                              // ��������(˫����)
} //��ͼ�ν��л���(��Ⱦ)�¼�

GLvoid SetupRC() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    //������һ������Ϊ��ɫ
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    //�����䷴ӳ��������Ӱ��ͻ����⣺�����屾����ɫ�е��ӣ����屾������ɫ�������������Ϊ��ɫ��
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    //���淴�䣻������+������+���淴�����
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //��Դ���Ҳ����Ĳ�����Ϊ�����������Ӱ�죬�û�������ɫ���
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
//*  ���̿���ͼ���¼�����                                            *
//********************************************************************
//
GLuint KeyPress() {

    //��ת
    if (keys[VK_UP])           xRot -= rotStep;                     // ���ϼ�ͷ��
    if (keys[VK_DOWN])         xRot += rotStep;                     // ���¼�ͷ��
    if (keys[VK_LEFT])         yRot -= rotStep;                     // �����ͷ��
    if (keys[VK_RIGHT])        yRot += rotStep;                     // ���Ҽ�ͷ��
    if (keys[VK_HOME])         zRot += rotStep;                     // ��HOME��
    if (keys[VK_END])          zRot -= rotStep;                     // ��END��

    //ƽ��
    if (keys['A'])             xPosition -= moveStep;               // ��A��
    if (keys['D'])             xPosition += moveStep;               // ��D��
    if (keys['W'])             yPosition += moveStep;               // ��W��
    if (keys['S'])             yPosition -= moveStep;               // ��S��
    if (keys['Q'])             zPosition -= moveStep;               // ��Q��
    if (keys['E'])             zPosition += moveStep;               // ��E��

    if (keys[VK_ESCAPE])                                            // ��ESC��
        SendMessage(hWnd, WM_DESTROY, 0, 0);

    return 0;
} // ���̿���ͼ���¼�����

//********************************************************************
//*  ��ʱ����������                                          *
//********************************************************************
//
GLvoid CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
    keys[VK_LEFT] = true;
    KeyPress();
    SendMessage(hWnd, WM_PAINT, 0, 0L);
}

/********************************************************************/
/*  ������˵����庯������                                          */
/********************************************************************/
//
GLuint MainWndMenu() {
    hMenuMain = CreateMenu();
    hPopup = CreatePopupMenu();
    AppendMenu(hPopup, MF_STRING, IDM_AUTO, TEXT("Auto-Rotate"));
    AppendMenu(hPopup, MF_STRING, IDM_STOP, TEXT("Stop"));
    AppendMenu(hPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hPopup, MF_STRING, IDM_RED, TEXT("���Ҵ�׻���ɫ��"));
    AppendMenu(hPopup, MF_STRING, IDM_YELLOW, TEXT("���Ҵ�׻���ɫ��"));
    AppendMenu(hPopup, MF_STRING, IDM_PINK, TEXT("���Ҵ�׻���ɫ��"));
    AppendMenu(hPopup, MF_STRING, IDM_WHITE, TEXT("���Ҵ�׻���ɫ��"));

    AppendMenu(hMenuMain,MF_POPUP,(int)hPopup,TEXT("CONTROL"));

    return 0;
} // ������˵����庯��

/********************************************************************/
/*  ��������ؼ���������                                            */
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ǩ�ؼ�
                              "STATIC",
                              "���ƴ������(��ת):",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 20, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 8;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ǩ�ؼ�
                              "STATIC",
                              "���ƴ������(ƽ��):",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 260, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtns)
        return 9;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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
/*  ��������ؼ���������                                            */
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ǩ�ؼ�
                              "STATIC",
                              "��������ƽ�ƣ�:",
                              WS_CHILD|WS_VISIBLE,
                              diff + 8, 190, 160, 32,
                              hWndParent,
                              (HMENU)IDC_RotateZ2,
                              hInstance,
                              NULL);
    if(!hCmdBtnsLeft)
        return 17;

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

    hCmdBtnsLeft = CreateWindowEx((DWORD)NULL,                 // ������ť�ؼ�
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

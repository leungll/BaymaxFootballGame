# BaymaxFootballGame
BaymaxFootballGame for Software Development Based on Graphics Engine, Spring 2020,  NENU

![maven](https://img.shields.io/badge/C-passing-red)
![maven](https://img.shields.io/badge/MinGW-compile-green)
![maven](https://img.shields.io/badge/IDE-Code%3A%3ABlocks%20-yellow)
![maven](https://img.shields.io/badge/version-v1.0-orange)
![maven](https://img.shields.io/badge/License-Apache%202.0-blue)

## 开发环境
* System: Win10
* IDE: Code::Blocks 17.12
* Compiler: MinGW
* Frame: WinGUI
    * 注：WinGUI框架搭建模型来源于我的课程教师：[封子军老师](http://js.nenu.edu.cn/teacher/index.php?zgh=2005900028)

## 引入资源
* `glut.h & gl.h & windows.h`
* 可在仓库 `resource` 文件夹中引入

## 编译 & 运行
* `cd BaymaxFootballGame\BaymaxFootballGame\`
* 用 Code::Blocks 打开 `BaymaxFootballGame.cbp` 文件
* `Build and run`
* 注：查看足球贴图效果时，需进入路径 `BaymaxFootballGame\bin\Debug` ，打开 `BaymaxFootballGame.exe` 程序查看

## 设计思路
- 整个程序框架是在老师的 `WiuGUI` 框架下进行编码操作
- 首先在 `Draw3D` 中绘制 `3D` 版 `Baymax`，再绘制足球球体
- 其次加上光照材质及纹理映射效果，使物体呈现真实感与立体感
- 最后依次加上键盘、鼠标、菜单的变色，旋转，平移操作


## 实现功能
- 键盘操作
    - 旋转操作
        - `UP`、`DOWN` 键：物体绕 `X` 轴旋转
        - `LEFT`、`RIGHT` 键：物体绕 `Y` 轴旋转
        - `HOME`、`END` 键：物体绕 `Z` 轴旋转
    - 平移操作
        - `A`、`D` 键：物体沿 `X` 轴平移
        - `W`、`S` 键：物体沿 `Y` 轴平移
        - `Q`、`E` 键：物体沿 `Z` 轴平移
- 鼠标操作
    - 按住 `鼠标左键`，拖动物体旋转，松开停止旋转
        - 鼠标消息：`WM_MOUSEMOVE`
    - 按住 `鼠标中键`，物体自动旋转，松开停止旋转
        - 时间控件，定时器消息：
            ```
            VOID CALLBACK MyTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
            SetTimer(hwnd, ID_TIMER, 1000, MyTimerProc);
            ```
- 菜单操作
    - 主菜单 `hMenuMain`
    - 右键弹出式菜单 `hPopup`
    - 功能操作
        - 控制物体自动旋转：`Auto-Rotate`
        - 停止物体自动旋转：`Stop`
        - `Baymax` 变色
- 按钮操作
    - 左处窗体：控制足球平移操作
    - 右处窗体：控制物体旋转、平移操作
- 光照材质
    - 针对 `Baymax`
    - `glLightfv, glMaterialfv`
- 纹理映射
    - 针对球体 `gluSphere` 贴图
    - 函数 `load_texture` 读取 `BMP` 文件作为纹理

## 页面截图
>程序的灵感来源于游戏机，在窗体的中间是游戏界面，左右两处的面板则像游戏机的控制按钮。左处面板，控制足球运动；右处面板，可控制整个物体的运动。最后呈现出的效果为控制 `Baymax` 踢足球。

![Baymax1](https://cdn.jsdelivr.net/gh/leungll/ImgHosting/img/Baymax.jpg)
![Baymax2](https://cdn.jsdelivr.net/gh/leungll/ImgHosting/img/Baymax2.jpg)
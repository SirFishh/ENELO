#include <windows.h>
#include <gdiplus.h>
#include <filesystem>

//80% CODE IS НЕЙРОСЛОП

//THIS FUNCTION IS FULL НЕЙРОSLOP
bool RenderPngToWindow(HWND hwnd, const wchar_t* pngPath, int x, int y)
{

    Gdiplus::Bitmap bmp(pngPath);
    if (bmp.GetLastStatus() != Gdiplus::Ok) {
        MessageBoxA(hwnd, "НЕ ВИЖУ КАРТИНКИ", "ААА НИЧО НЕ ПОНИМАЮ", 2);
        return false;
    }
    int w = bmp.GetWidth();
    int h = bmp.GetHeight();

    HDC screen = GetDC(NULL);
    HDC memDC  = CreateCompatibleDC(screen);

    BITMAPINFO bi = {};
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = w;
    bi.bmiHeader.biHeight      = -h;
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    HBITMAP dib = CreateDIBSection(memDC, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
    HGDIOBJ oldBmp = SelectObject(memDC, dib);

    Gdiplus::Graphics g(memDC);
    g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.DrawImage(&bmp, 0, 0, w, h);

    POINT ptDst = { x, y };
    POINT ptSrc = { 0, 0 };
    SIZE  size  = { w, h };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    BOOL ok = UpdateLayeredWindow(hwnd, screen, &ptDst, &size,
                                  memDC, &ptSrc, 0, &blend, ULW_ALPHA);

    SelectObject(memDC, oldBmp);
    DeleteObject(dib);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);
    return ok == TRUE;
}

namespace fs = std::filesystem; //SLOP

//SLOP TOO
std::wstring ExeDir() {
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(NULL, buf, MAX_PATH);
    return fs::path(buf).parent_path().wstring();
}

std::wstring path = (fs::path(ExeDir()) / L"image.png").wstring();

// CALLBACKS НЕ СЛОП 
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            ReleaseCapture();
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
// 50/50 SLOP
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Gdiplus::GdiplusStartupInput  gdiplusStartupInput;
    ULONG_PTR gdiPlusToken;
    GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, NULL);

    
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };
    
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);

    
    //Копипаста с микрософтиков
    //Такие кривые коментарии A.I не оставит
    //А я оставлю

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_APPWINDOW | WS_EX_TOPMOST,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"",    // Window text
        WS_POPUP,        // Window style
        
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 200,
        
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    
    
    if (hwnd == NULL)
    {
        return -1;
    }
    
    if(!RenderPngToWindow(hwnd, path.c_str(), 200, 200)) return -1;
    
    ShowWindow(hwnd, nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiPlusToken);
    
    return 0;
}
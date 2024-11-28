#include <Deferred/pch.h>
#include <Deferred/Commons.h>

#include <scope_guard.hpp> // TODO: to be removed

static bool s_is_running{ true };
static bool s_did_resize{};

#define D_WINDOW_CLASS_NAME "d_window_class"
#define D_WINDOW_NAME "Deferred Renderer"
#define D_START_CLIENT_W 1280
#define D_START_CLIENT_H 720

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_CLOSE:
    {
        s_is_running = false;
    } break;
    case WM_KEYDOWN:
    {
        s_is_running = (wparam != VK_ESCAPE);
        result = DefWindowProc(hwnd, msg, wparam, lparam);
    } break;
    case WM_SIZE:
    {
        s_did_resize = true;
        result = DefWindowProc(hwnd, msg, wparam, lparam);
    } break;
    default:
    {
        result = DefWindowProc(hwnd, msg, wparam, lparam);
    } break;
    }
    return result;
}

int main()
{
    try
    {
        // NOTE: make window DPI aware
        D_Check(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE));

        // NOTE: register window class
        {
            WNDCLASSEX wndclass{};
            wndclass.cbSize = sizeof(wndclass);
            wndclass.style = CS_HREDRAW | CS_VREDRAW;
            wndclass.lpfnWndProc = &WindowProcedure;
            //wndclass.cbClsExtra = ;
            //wndclass.cbWndExtra = ;
            wndclass.hInstance = GetModuleHandle(0);
            wndclass.hIcon = LoadIcon(0, IDI_APPLICATION);
            wndclass.hCursor = LoadCursor(0, IDC_ARROW);
            //wndclass.hbrBackground = ;
            //wndclass.lpszMenuName = ;
            wndclass.lpszClassName = D_WINDOW_CLASS_NAME;
            wndclass.hIconSm = LoadIcon(0, IDI_APPLICATION);
            D_Check(RegisterClassEx(&wndclass));
        }

        // NOTE: create window
        HWND hwnd{};
        auto hwnd_cleanup{ sg::make_scope_guard([]() {}) };
        {
            RECT rect{ 0, 0, D_START_CLIENT_W, D_START_CLIENT_H };
            D_Check(AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, 0, WS_EX_OVERLAPPEDWINDOW));
            LONG w{ rect.right - rect.left };
            LONG h{ rect.bottom - rect.top };
            hwnd = CreateWindowEx(
                WS_EX_OVERLAPPEDWINDOW,
                D_WINDOW_CLASS_NAME,
                D_WINDOW_NAME,
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT,
                w, h,
                0, 0, GetModuleHandle(NULL), 0);
            D_Check(hwnd);
            hwnd_cleanup = sg::make_scope_guard([=]() { DestroyWindow(hwnd); });
        }

    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

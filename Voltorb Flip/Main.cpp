#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "resource.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Game.h"

/*
The window-displaying class.  The code was modeled after the MSDN tutorial
http://msdn.microsoft.com/en-us/library/vstudio/bb384843.aspx

as well as theForger's winapi tutorial
http://www.winprog.org/tutorial/.
*/

const char szWindowClass[] = "MainWindow";
Game game;
bool clicks = true; // determines whether to recognize the user's mouseclicks

HBITMAP g_card1 = NULL;
HBITMAP g_card2 = NULL;
HBITMAP g_card3 = NULL;
HBITMAP g_cardv = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void handleLeftClick(int, int, HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
    /* set up game */
    game = Game();

    /* create window class structure */
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONS));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = CreatePatternBrush(LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BACKGROUND)));
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONS));//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    /* register the window class */
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    /* create the window */
    int x = 256, y = 384; RECT r = {0, 0, x, y};
    if (AdjustWindowRect(&r, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE)) {
        x = r.right-r.left;
        y = r.bottom-r.top;
    }

    HWND hwnd = CreateWindow(
        szWindowClass,
        "Voltorb Flip",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,//WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        x, y,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hwnd)
    {
        MessageBox(NULL, "Call to CreateWindow failed!", "Error", NULL);
        return 1;
    }

    /* display the window */
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* message loop */
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_card1 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CARD1));
        if(g_card1 == NULL)
				MessageBox(hwnd, "Could not load the 1 card!", "Error", MB_OK | MB_ICONEXCLAMATION);
        g_card2 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CARD2));
        if(g_card2 == NULL)
				MessageBox(hwnd, "Could not load the 2 card!", "Error", MB_OK | MB_ICONEXCLAMATION);
        g_card3 = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CARD3));
        if(g_card2 == NULL)
				MessageBox(hwnd, "Could not load the 3 card!", "Error", MB_OK | MB_ICONEXCLAMATION);
        g_cardv = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CARDV));
        if(g_cardv == NULL)
				MessageBox(hwnd, "Could not load the Voltorb card!", "Error", MB_OK | MB_ICONEXCLAMATION);
        break;
    case WM_PAINT:
        {
        BITMAP c1, c2, c3, cv;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);

        HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, g_card1);
        
        GetObject(g_card1, sizeof(c1), &c1);

        /* draw card 1 */
        for(int i = 0; i < 5; i++) 
            for (int j = 0; j < 5; j++) 
                if (game.get_board_state(i, j).flipped && game.get_board_state(i, j).points == 1)
                    BitBlt(hdc, 8 + j * 32, 200 + i * 32, c1.bmWidth, c1.bmHeight, hdcMem, 0, 0, SRCCOPY);

        /* draw card 2 */
        SelectObject(hdcMem, g_card2);
        GetObject(g_card2, sizeof(c2), &c2);
        for(int i = 0; i < 5; i++) 
            for (int j = 0; j < 5; j++) 
                if (game.get_board_state(i, j).flipped && game.get_board_state(i, j).points == 2)
                    BitBlt(hdc, 8 + j * 32, 200 + i * 32, c2.bmWidth, c2.bmHeight, hdcMem, 0, 0, SRCCOPY);

        /* draw card 3 */
        SelectObject(hdcMem, g_card3);
        GetObject(g_card3, sizeof(c3), &c3);
        for(int i = 0; i < 5; i++) 
            for (int j = 0; j < 5; j++) 
                if (game.get_board_state(i, j).flipped && game.get_board_state(i, j).points == 3)
                    BitBlt(hdc, 8 + j * 32, 200 + i * 32, c3.bmWidth, c3.bmHeight, hdcMem, 0, 0, SRCCOPY);

        /* draw card voltorb */
        SelectObject(hdcMem, g_cardv);
        GetObject(g_cardv, sizeof(cv), &cv);
        for(int i = 0; i < 5; i++) 
            for (int j = 0; j < 5; j++) 
                if (game.get_board_state(i, j).flipped && game.get_board_state(i, j).points == -1)
                    BitBlt(hdc, 8 + j * 32, 200 + i * 32, cv.bmWidth, cv.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        /* draw hints */
        SetBkMode(hdc, TRANSPARENT);
        std::stringstream srp, scp, srv, scv;
        for (int k = 0; k < 5; k++) {
            srp.str(std::string()); scp.str(std::string());
            srv.str(std::string()); scv.str(std::string());
            srp << std::setfill('0') << std::setw(2) << game.get_row_info(k).points_sum;
            scp << std::setfill('0') << std::setw(2) << game.get_col_info(k).points_sum;
            srv << game.get_row_info(k).voltorbs_sum;
            scv << game.get_col_info(k).voltorbs_sum;
            TextOut(hdc, 16 + k * 32, 196 + 5 * 32, scp.str().c_str(), 2);
            TextOut(hdc, 24 + k * 32, 210 + 5 * 32, scv.str().c_str(), 2);
            TextOut(hdc, 16 + 5 * 32, 196 + k * 32, srp.str().c_str(), 2);
            TextOut(hdc, 24 + 5 * 32, 210 + k * 32, srv.str().c_str(), 2);
        }

        // reuse srp for level, player score, round points
        srp.str(std::string());
        srp << game.get_level();
        TextOut(hdc, 168, 8, srp.str().c_str(), 1);
        
        srp.str(std::string());
        srp << std::setfill('0') << std::setw(6) << game.get_round_score();
        TextOut(hdc, 188, 120, srp.str().c_str(), 6);

        srp.str(std::string());
        srp << std::setfill('0') << std::setw(6) << game.get_player_score();
        TextOut(hdc, 188, 160, srp.str().c_str(), 6);

        EndPaint(hwnd, &ps);
        }
    break;

    case WM_LBUTTONDOWN:
        if (clicks)
            handleLeftClick(LOWORD(lParam), HIWORD(lParam), hwnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_HELP_RULES:
            if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_RULES), hwnd, DlgProc) == -1)
			    MessageBox(hwnd, "Failed to open rules!", "Error", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_HELP_ABOUT:
            if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgProc) == -1)
			    MessageBox(hwnd, "Failed to open About dialog!", "Error", MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        DeleteObject(g_card1);
        DeleteObject(g_card2);
        DeleteObject(g_card3);
        DeleteObject(g_cardv);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:

		    return TRUE;
		case WM_COMMAND:
            EndDialog(hwnd, IDOK);
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

/*
Connects with the Game class to modify GameState and Player attributes.
Click recognition is turned off until a move and any subsequent notification
is copmleted.
*/
void handleLeftClick(int xPos, int yPos, HWND hwnd)
{
    if ( xPos < 9 || yPos < 201 || xPos > 158 || yPos > 350 ||
        (xPos - 9) % 32 > 21 || (yPos - 201) % 32 > 21)
        return;
    
    clicks = false;

    int i = (yPos - 201) / 32;
    int j = (xPos - 9) / 32;

    std::stringstream s;
    //s << "Flipping card at " << i << ", " << j << ".  Score of " << game.get_board_state(i, j).points;
    //MessageBox(NULL, s.str().c_str(), "Action", MB_ICONEXCLAMATION | MB_OK);
    //s.str(std::string());

    bool refresh = game.handle_input(i, j);

    /*refresh board*/
    RECT r = {0, 0, 256, 192*2};
    InvalidateRect(hwnd, &r, TRUE);
    UpdateWindow(hwnd);
    
    if (refresh) {
        if (game.refresh_game_state()) { // refresh was for a win
            s << "Level cleared! Next round is level " << game.get_level() << ".";
            MessageBox(NULL, s.str().c_str(), "Action", MB_ICONEXCLAMATION | MB_OK);
            InvalidateRect(hwnd, &r, TRUE);
        } else {
            s << "You lost! Next round is level " << game.get_level() << ".";
            MessageBox(NULL, s.str().c_str(), "Action", MB_ICONEXCLAMATION | MB_OK);
            InvalidateRect(hwnd, &r, TRUE);
        }
    }
    clicks = true;
}
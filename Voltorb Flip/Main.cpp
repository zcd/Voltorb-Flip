#include "Main.h"

/*
The window-displaying class.  The code was modeled after the MSDN tutorial
http://msdn.microsoft.com/en-us/library/vstudio/bb384843.aspx

as well as theForger's winapi tutorial
http://www.winprog.org/tutorial/.
*/

const char szWindowClass[] = "MainWindow";
Game game;
bool board_lclicks = true; // determines whether to recognize the user's mouseclicks
int memo_type = Voltorb;

HBITMAP g_cards = NULL;
HBITMAP g_memos = NULL;
HBITMAP g_marks = NULL;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = CreatePatternBrush(LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BACKGROUND)));
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

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
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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
        g_cards = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CARDS));
        if(g_cards == NULL)
            MessageBox(hwnd, "Could not load the cards!", "Error", MB_OK | MB_ICONEXCLAMATION);
        g_memos = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MEMOS));
        if(g_memos== NULL)
            MessageBox(hwnd, "Could not load the memos!", "Error", MB_OK | MB_ICONEXCLAMATION);
        g_marks = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MARKS));
        if(g_marks == NULL)
            MessageBox(hwnd, "Could not load the markings!", "Error", MB_OK | MB_ICONEXCLAMATION);
        break;
    case WM_PAINT:
        {
            int memo_src_x, memo_src_y;
            int memo_dst_x, memo_dst_y;
            int card_src_x, card_src_y;

            BITMAP card_bmp, memo_bmp, mark_bmp;
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdcMem = CreateCompatibleDC(hdc);

            /* temp stash the hdcMem*/
            HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, g_cards);

            /* draw cards */
            for(int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (game.get_board_state(i, j).flipped) {
                        SelectObject(hdcMem, g_cards);
                        GetObject(g_cards, sizeof(card_bmp), &card_bmp);
                        switch(game.get_board_state(i, j).points) 
                        {
                        case One:
                            card_src_x = 24; card_src_y = 0;
                            break;
                        case Two:
                            card_src_x = 0; card_src_y = 24;
                            break;
                        case Three:
                            card_src_x = 24; card_src_y = 24;
                            break;
                        case Voltorb:
                            card_src_x = 0; card_src_y = 0;
                            break;
                        }
                        BitBlt(hdc, 8 + j * 32, 200 + i * 32, card_bmp.bmWidth/2, card_bmp.bmHeight/2, 
                               hdcMem, card_src_x, card_src_y, SRCCOPY);
                    } else {
                        SelectObject(hdcMem, g_marks);
                        GetObject(g_marks, sizeof(mark_bmp), &mark_bmp);
                        if (game.markers_1[i][j]) 
                            BitBlt(hdc, 25 + j * 32, 202 + i * 32, mark_bmp.bmWidth/2, mark_bmp.bmHeight/2, 
                                   hdcMem, 5, 0, SRCCOPY);
                        if (game.markers_2[i][j]) 
                            BitBlt(hdc, 9 + j * 32, 217 + i * 32, mark_bmp.bmWidth/2, mark_bmp.bmHeight/2, 
                                   hdcMem, 0, 5, SRCCOPY);
                        if (game.markers_3[i][j]) 
                            BitBlt(hdc, 25 + j * 32, 217 + i * 32, mark_bmp.bmWidth/2, mark_bmp.bmHeight/2, 
                                   hdcMem, 5, 5, SRCCOPY);
                        if (game.markers_v[i][j]) 
                            BitBlt(hdc, 10 + j * 32, 202 + i * 32, mark_bmp.bmWidth/2, mark_bmp.bmHeight/2, 
                                   hdcMem, 0, 0, SRCCOPY);
                    }
                }
            }

            /* draw memo */
            switch(memo_type)
            {
            case One:
                memo_src_x = 24; memo_src_y = 0;
                memo_dst_x = 224; memo_dst_y = 208;
                break;
            case Two:
                memo_src_x = 0; memo_src_y = 24;
                memo_dst_x = 200; memo_dst_y = 232;
                break;
            case Three:
                memo_src_x = 24; memo_src_y = 24;
                memo_dst_x = 224; memo_dst_y = 232;
                break;
            case Voltorb:
                memo_src_x = 0; memo_src_y = 0;
                memo_dst_x = 200; memo_dst_y = 208;
                break;
            }
            SelectObject(hdcMem, g_memos);
            GetObject(g_memos, sizeof(memo_bmp), &memo_bmp);
            BitBlt(hdc, memo_dst_x, memo_dst_y, memo_bmp.bmWidth/2, memo_bmp.bmHeight/2, 
                   hdcMem, memo_src_x, memo_src_y, SRCCOPY);

            /* reselect previous object and release hdcMem */
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
            TextOut(hdc, 168, 7, srp.str().c_str(), 1);

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
        if (board_lclicks || !in_rect(LOWORD(lParam), HIWORD(lParam), board_rect))
            handleLeftClick(LOWORD(lParam), HIWORD(lParam), hwnd);
        break;
    case WM_RBUTTONDOWN:
        handleRightClick(LOWORD(lParam), HIWORD(lParam), hwnd);
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
        DeleteObject(g_cards);
        DeleteObject(g_memos);
        DeleteObject(g_marks);

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
    if ( xPos > 200 && xPos < 223) {
        if ( yPos > 208 && yPos < 231) {
            memo_type = Voltorb;
        } else if ( yPos > 232 && yPos < 255) {
            memo_type = Two;
        }
        InvalidateRect(hwnd, &memo_rect, TRUE);
        return;
    } else if ( xPos > 224 && xPos < 247) {
        if ( yPos > 208 && yPos < 231) {
            memo_type = One;
        } else if ( yPos > 232 && yPos < 255) {
            memo_type = Three;
        }
        InvalidateRect(hwnd, &memo_rect, TRUE);
        return;
    }

    if ( xPos < 9 || yPos < 201 || xPos > 158 || yPos > 350 ||
        (xPos - 9) % 32 > 21 || (yPos - 201) % 32 > 21)
        return;

    board_lclicks = false;
    int i = (yPos - 201) / 32;
    int j = (xPos - 9) / 32;
    bool refresh = game.handle_input(i, j);

    /*refresh board*/
    InvalidateRect(hwnd, &screen_rect, TRUE);
    UpdateWindow(hwnd);

    if (refresh) {
        std::stringstream s;
        if (game.refresh_game_state()) { // refresh was for a win
            s << "Level cleared! Next round is level " << game.get_level() << ".";
            MessageBox(NULL, s.str().c_str(), "Action", MB_ICONEXCLAMATION | MB_OK);
            InvalidateRect(hwnd, &screen_rect, TRUE);
        } else {
            s << "You lost! Next round is level " << game.get_level() << ".";
            MessageBox(NULL, s.str().c_str(), "Action", MB_ICONEXCLAMATION | MB_OK);
            InvalidateRect(hwnd, &screen_rect, TRUE);
        }
    }
    board_lclicks = true;
}

void handleRightClick(int xPos, int yPos, HWND hwnd)
{
    if ( xPos > 200 && xPos < 223) {
        if ( yPos > 208 && yPos < 231) {
            memo_type = Voltorb;
        } else if ( yPos > 232 && yPos < 255) {
            memo_type = Two;
        }
        InvalidateRect(hwnd, &memo_rect, TRUE);
        return;
    } else if ( xPos > 224 && xPos < 247) {
        if ( yPos > 208 && yPos < 231) {
            memo_type = One;
        } else if ( yPos > 232 && yPos < 255) {
            memo_type = Three;
        }
        InvalidateRect(hwnd, &memo_rect, TRUE);
        return;
    }
    if ( xPos < 9 || yPos < 201 || xPos > 158 || yPos > 350 ||
        (xPos - 9) % 32 > 21 || (yPos - 201) % 32 > 21)
        return;
    int i = (yPos - 201) / 32;
    int j = (xPos - 9) / 32;
    if (game.get_board_state(i, j).flipped) 
        return;

    switch (memo_type)
    {
    case One:
        game.markers_1[i][j] = !game.markers_1[i][j];
        break;
    case Two:
        game.markers_2[i][j] = !game.markers_2[i][j];
        break;
    case Three:
        game.markers_3[i][j] = !game.markers_3[i][j];
        break;
    case Voltorb:
        game.markers_v[i][j] = !game.markers_v[i][j];
        break;
    }
    InvalidateRect(hwnd, &board_rect, TRUE);
    return;
}

bool in_rect(int xPos, int yPos, RECT r)
{
    return xPos >= r.left && yPos >= r.top && xPos <= r.right && yPos <= r.bottom;
}

RECT card_to_rect(int i, int j)
{
    RECT r = {8 + j * 32, 200 + i * 32, 23 + 8 + j * 32, 23 + 200 + i * 32};
    return r;
}
#include <fcntl.h>
#ifdef _WIN32
    #include <io.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void Sleep(int milliseconds) { usleep(milliseconds * 1000); }

void Beep(int frequency, int duration) {
    // No cross-platform equivalent for Beep, so this is a no-op
}
#endif

//* Colors
#define Color_Reset "\033[0m"
#define Color_Red "\033[0;31m"
#define Color_Red_Dark "\033[38;2;0;255;255m"
#define Color_Green "\033[0;32m"
#define Color_Yellow "\033[0;33m"
#define Color_Yellow_Dark "\033[38;2;200;200;0m"
#define Color_Blue "\033[0;34m"
#define Color_Purple "\033[0;35m"
#define Color_Aqua "\033[38;2;0;255;255m"
#define Color_Orange "\033[38;2;255;80;0m"
#define Color_Gray "\033[38;2;128;128;128m"
#define Color_Deep_Pink "\033[38;2;250;40;155m"
#define Color_Green_Blue "\033[38;2;0;200;80m"

struct Player {
        char Username[51];
        int id;
        int Loss;
        int Win;
        int Cur_Streak;
        int Win_Streak;
};

#define CHUNK_SIZE 10

//* functions
void Print_Player_Info(struct Player* in);
void Bar_Status(struct Player* User, int login);
void Delete_Player(int id, struct Player* Users);
void Change_Player_UserName(struct Player* User, struct Player* Users);

void Admin_Panel_Login(struct Player* Admin, struct Player* Users);
void Admin_Panel(struct Player* Admin, struct Player* Users);

void Main_Menu(struct Player* User, struct Player* Users);

int Board_Zero_Open(int** Board, int Size_Board, int x, int y, int Space_B);
void Print_Game(int** Board, struct Player* User, int Size_Board, int Boob_C,
                int Is_Loss, int Flag_Left, int Debug_Mode);
void Play_Game(struct Player* User, int Size_Board, int Boob_C,
               int Max_Column_Size);
void Create_Game(int** Board, int Size_Board, int Boob_C, int Max_Column_Size);
void Print_Char_Color_Board(int num, int len_level);
void Game_Menu(struct Player* User);

void Error_Management(int code);

int User_Input_Number_Range(int start, int end);
int Len_Number(int num);

//* cls Command
int run_cls = 1;
#define RUN_CLS \
    if (run_cls) system(CLEAR_SCREEN)

int Users_Counter = 1;

int main() {

    //! Header
    RUN_CLS;

    printf("Warning:\n");
    printf(
        "Ansi Code is used in this program. For a better experience, use a "
        "terminal that supports Ansi Code.\n");
    printf("I hope you like the Game :)\n");
    Sleep(3000);

    RUN_CLS;
    printf("%s(-: %sWelcome To %sMr Minesweeper%s :-)%s\n", Color_Yellow,
           Color_Green, Color_Red, Color_Yellow, Color_Reset);

    //* list of player :/
    struct Player* Users =
        (struct Player*)malloc(CHUNK_SIZE * sizeof(struct Player));
    int Users_Size = CHUNK_SIZE;

    // Admin Account Info
    strcpy(Users[0].Username, "admin");
    Users[0].id = 0;
    Users[0].Loss = 49;
    Users[0].Win = 49;
    Users[0].Cur_Streak = 49;
    Users[0].Win_Streak = 49;

    char User_Input[1001];

    // main loop
    while (1) {

        struct Player* Userptr = &Users[Users_Counter];

        // get UserName loop
        while (1) {

            //* Get Player Name
            Bar_Status(Userptr, 0);
            printf("Enter your Username (/ ~ Exit): ");

            // gets(User_Input);
            fgets(User_Input, sizeof(User_Input), stdin);
            User_Input[strcspn(User_Input, "\n")] =
                0;  // Remove newline character

            fflush(stdin);

            // exit
            if (strcmp(User_Input, "/") == 0) {
                Bar_Status(Userptr, 0);
                printf("%sExit%s :) %sBye Bye.\n", Color_Red, Color_Yellow,
                       Color_Reset);
                Sleep(3000);
                exit(0);
            }

            if (strlen(User_Input) > 51) {
                Bar_Status(Userptr, 0);
                Error_Management(21);
                continue;
            }

            if (strlen(User_Input) < 2) {
                Bar_Status(Userptr, 0);
                Error_Management(22);
                continue;
            }

            break;
        }

        // Admin Panel Login
        if (strcmp(User_Input, "admin") == 0) {

            Admin_Panel_Login(&Users[0], Users);
            continue;
        }

        int flag_login = 0;
        for (int i = 0; i < Users_Counter; i++) {

            if (strcmp(User_Input, Users[i].Username) == 0) {

                flag_login = 1;
                Userptr = &Users[i];
                break;
            }
        }

        // New User :)
        if (flag_login == 0) {

            if (Users_Counter == Users_Size) {
                struct Player* temp = realloc(
                    Users, (Users_Size + CHUNK_SIZE) * sizeof(struct Player));
                if (temp == NULL) {
                    free(Users);
                    Bar_Status(Userptr, 0);
                    printf("Memory Allocation failed, try again\n");
                    Sleep(3000);
                    continue;
                    // exit(EXIT_FAILURE);
                } else {
                    Users_Size += CHUNK_SIZE;
                    Users = temp;
                }
            }

            strcpy(Userptr->Username, User_Input);
            Userptr->id = Users_Counter;
            Userptr->Loss = 0;
            Userptr->Win = 0;
            Userptr->Cur_Streak = 0;
            Userptr->Win_Streak = 0;

            Users_Counter++;
        }

        Bar_Status(Userptr, 1);
        printf("Login was Successful :)\n");
        Print_Player_Info(Userptr);

        Main_Menu(Userptr, Users);

    }  // Main While End

    return 0;
}

void Print_Player_Info(struct Player* in) {

    printf("\n    %sPlayer:%s %s %s\n", Color_Blue, Color_Purple, in->Username,
           Color_Reset);
    printf("    %sWin: %s%d", Color_Green, Color_Reset, in->Win);
    printf(" %s~ %sLoss: %s%d\n\n", Color_Aqua, Color_Red, Color_Reset,
           in->Loss);
    printf("    %sCurrent Streak: %s%d\n", Color_Yellow, Color_Reset,
           in->Cur_Streak);
    printf("    %sWinning Streak: %s%d\n", Color_Yellow, Color_Reset,
           in->Win_Streak);
    Sleep(3000);
}

void Change_Player_UserName(struct Player* User, struct Player* Users) {

    char in_n_user[1001];

    while (1) {

        Bar_Status(User, 1);
        printf("Enter Your New UserName (/ ~ Back): ");

        // gets(in_n_user);
        fgets(in_n_user, sizeof(in_n_user), stdin);
        in_n_user[strcspn(in_n_user, "\n")] = 0;  // Remove newline character

        if (strcmp(in_n_user, "/") == 0) {
            Bar_Status(User, 1);
            printf("Username did not Change :(\n");
            Sleep(500);
            return;
        }

        int is_duplicate = 0;
        for (int i = 0; i < Users_Counter; i++) {

            if (strcmp(in_n_user, Users[i].Username) == 0) {

                Error_Management(20);
                is_duplicate = 1;
                break;
            }
        }

        if (is_duplicate) continue;

        if (strlen(in_n_user) > 51) {
            Bar_Status(User, 1);
            Error_Management(21);
            continue;
        }

        if (strlen(in_n_user) < 2) {
            Bar_Status(User, 1);
            Error_Management(22);
            continue;
        }

        break;
    }

    strcpy(User->Username, in_n_user);

    Bar_Status(User, 1);
    printf("Username Changed Successfully.\n");
    Sleep(500);

    return;
}

void Delete_Player(int id, struct Player* Users) {

    for (int i = id; i < Users_Counter - 1; i++) {

        strcpy(Users[i].Username, Users[i + 1].Username);
        // Users[i].id = i;
        Users[i].Loss = Users[i + 1].Loss;
        Users[i].Win = Users[i + 1].Win;
        Users[i].Win_Streak = Users[i + 1].Win_Streak;
        Users[i].Cur_Streak = Users[i + 1].Cur_Streak;
    }

    strcpy(Users[Users_Counter - 1].Username, "NULL");
    Users[Users_Counter - 1].Loss = 0;
    Users[Users_Counter - 1].Win = 0;
    Users[Users_Counter - 1].Win_Streak = 0;
    Users[Users_Counter - 1].Cur_Streak = 0;

    Users_Counter--;
}

void Bar_Status(struct Player* User, int login) {

    if (login == 0) {
        printf("\n%s! Login ! %s>>> %s", Color_Yellow, Color_Aqua, Color_Reset);
    }

    // login as player
    else if (login == 1) {
        printf("\n%s#%d%s %s %s>>> %s", Color_Blue, User->id, Color_Green,
               User->Username, Color_Aqua, Color_Reset);
    }

    // login as admin
    else if (login == 2) {
        printf("\n%s#%d%s %s %s>>> %s", Color_Blue, User->id, Color_Red,
               User->Username, Color_Aqua, Color_Reset);
    }
}

void Main_Menu(struct Player* User, struct Player* Users) {

    while (1) {

        Bar_Status(User, 1);
        printf("Game Menu:\n\n");
        printf("    %s1 %s> %sPlay Game\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sPlayer Info\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sChange Name\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sDelete Account\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sLogout\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s6 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(User, 1);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 6);

        switch (in_code) {

            // ctrl + c exit code
            case -2:
                printf("%sExit.%s", Color_Red, Color_Reset);
                Sleep(3000);
                exit(0);
                break;

            case -1:  // Error input
                break;

            case 1:
                Game_Menu(User);
                break;

            case 2:
                Bar_Status(User, 1);
                printf("Player Info:\n");
                Print_Player_Info(User);
                break;

            case 3:
                Change_Player_UserName(User, Users);
                break;

            case 4:
                Bar_Status(User, 1);
                printf(
                    "%sWarning:%s Press Ctrl+C to Delete (To Cancel, Press "
                    "Another Button). ",
                    Color_Yellow, Color_Reset);

                int x = getch();

                if (x == 3) {
                    Delete_Player(User->id, Users);
                    printf("%sDeleted\n%s", Color_Yellow, Color_Reset);
                    Sleep(1500);
                    RUN_CLS;
                    return;
                }

                else
                    printf("%sCancelled\n%s", Color_Green, Color_Reset);
                break;

            case 5:
                Bar_Status(User, 1);
                printf("logout Successful.\n");
                Sleep(1500);
                RUN_CLS;
                return;
                break;

            case 6:
                Bar_Status(User, 1);
                printf("%sExit%s :) %sBye Bye.\n", Color_Red, Color_Yellow,
                       Color_Reset);
                Sleep(3000);
                exit(0);
                return;
                break;

        }  // switch end

    }  // while end
}

void Game_Menu(struct Player* User) {

    while (1) {

        Bar_Status(User, 1);
        printf("Choose Your desired Size Board:\n\n");
        printf("    %s1 %s> %sEasy (9*9 ~ 10)\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sMedium (12*12 ~ 20)\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sHard (25*25 ~ 100)\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sCustom Game\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sBack to Menu\n", Color_Yellow, Color_Aqua,
               Color_Reset);

        Sleep(500);

        Bar_Status(User, 1);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 5);

        switch (in_code) {

            // ctrl + c exit code
            case -2:
                printf("%sExit.%s", Color_Red, Color_Reset);
                Sleep(3000);
                exit(0);
                break;

            case 1:
                Play_Game(User, 9, 10, 3);
                break;

            case 2:
                Play_Game(User, 12, 20, 3);
                break;

            case 3:
                Play_Game(User, 25, 100, 9);
                break;

            case 4:
                //* Custom Game
                int Exit_Code_Custom = 0;

                // Get Board Size
                int Size_Custom;
                while (1) {

                    Bar_Status(User, 1);
                    printf("Enter Size of Board: (3 ~ 30) (Ctrl+C ~ Back): ");

                    Size_Custom = User_Input_Number_Range(1, 30);

                    if (Size_Custom == -1) continue;
                    if (Size_Custom == -2) {
                        printf("Back.\n");
                        Exit_Code_Custom = 1;
                        Sleep(500);
                    }

                    break;
                }

                if (Exit_Code_Custom) break;

                // Get Number Of Bomb
                int Boob_C_Custom;
                while (1) {

                    Bar_Status(User, 1);
                    printf("Enter the Number of Bombs: (1 ~ %d): ",
                           Size_Custom * Size_Custom);

                    Boob_C_Custom =
                        User_Input_Number_Range(1, Size_Custom * Size_Custom);

                    if (Boob_C_Custom == -1) continue;
                    if (Boob_C_Custom == -2) {
                        printf("Back.\n");
                        Exit_Code_Custom = 1;
                        Sleep(500);
                    }

                    break;
                }

                if (Exit_Code_Custom) break;

                // Get Max Number of Bombs per Column
                int Max_Boob_col_Custom;
                int min_boob_custom = Boob_C_Custom / Size_Custom + 1;

                if (min_boob_custom > Size_Custom)
                    min_boob_custom = Size_Custom;

                while (1) {
                    Bar_Status(User, 1);
                    printf("Enter Max Number of Bombs per Column (%d ~ %d): ",
                           min_boob_custom, Size_Custom);

                    Max_Boob_col_Custom =
                        User_Input_Number_Range(min_boob_custom, Size_Custom);

                    if (Max_Boob_col_Custom == -1) continue;
                    if (Max_Boob_col_Custom == -2) {
                        printf("Back.\n");
                        Exit_Code_Custom = 1;
                        Sleep(500);
                    }

                    break;
                }

                if (Exit_Code_Custom) break;

                // Play Game:)
                Play_Game(User, Size_Custom, Boob_C_Custom,
                          Max_Boob_col_Custom);
                break;

            case 5:
                Bar_Status(User, 1);
                printf("Back To Menu.\n");
                Sleep(500);
                return;

        }  // Switch end

    }  // While end
}

void Print_Game(int** Board, struct Player* User, int Size_Board, int Boob_C,
                int Is_Loss, int Flag_Left, int Debug_Mode) {

    /*
    0 ~ 9 => Board Data
    10 ~ 18 => User Data
    20 ~ 29 => Flag Data
    */

    if (!Debug_Mode) RUN_CLS;

    Bar_Status(User, 1);
    printf("Flags: %d \n", Flag_Left);

    int Len_Level = Len_Number(Size_Board);

    //** bar number
    printf("%s| ", Color_Gray);

    for (int i = 0; i < Len_Level; i++) printf("#");
    printf(" ");

    for (int i = 0; i < Size_Board; i++) {

        printf("| ");

        int c = Len_Number(i + 1);
        for (; c < Len_Level; c++) printf(" ");

        printf("%d ", i + 1);
    }

    printf("|%s\n", Color_Reset);
    //** bar number end:')

    // board :')
    for (int i = 0; i < Size_Board; i++) {

        // board bar
        printf("%s| ", Color_Gray);
        int c = Len_Number(i + 1);
        for (; c < Len_Level; c++) printf(" ");
        printf("%d |%s", i + 1, Color_Reset);

        for (int j = 0; j < Size_Board; j++) {

            if (Debug_Mode) {
                Print_Char_Color_Board(Board[i][j] % 10, Len_Level);
                continue;
            }

            if (Is_Loss) {

                if (Board[i][j] >= 10 && Board[i][j] <= 18) {
                    Print_Char_Color_Board(Board[i][j] - 10, Len_Level);
                }

                // flags
                else if (Board[i][j] >= 20) {

                    // bad flag
                    if (Board[i][j] - 20 != 9)
                        Print_Char_Color_Board(-3, Len_Level);

                    else
                        Print_Char_Color_Board(-2, Len_Level);

                }

                else if (Board[i][j] == 9) {
                    Print_Char_Color_Board(Board[i][j], Len_Level);

                    srand(time(NULL));
                    int beep_in = (rand() % 1401) + 400;
                    // 400 ~ 1800

                    Beep(beep_in, 50);
                }

                else {
                    Print_Char_Color_Board(-1, Len_Level);
                }

                continue;
            }

            // if not loss :)
            if (Board[i][j] >= 0 && Board[i][j] <= 9) {
                // closed board ~ code -1
                Print_Char_Color_Board(-1, Len_Level);
            }

            else if (Board[i][j] >= 10 && Board[i][j] <= 18) {
                Print_Char_Color_Board(Board[i][j] - 10, Len_Level);
            }

            else if (Board[i][j] >= 20 && Board[i][j] <= 29) {
                Print_Char_Color_Board(-2, Len_Level);
            }
        }

        printf("\n");
    }
}

void Print_Char_Color_Board(int num, int len_level) {

    for (int i = 0; i < len_level - 1; i++) printf(" ");

    //?* _O_U8TEXT == 0x00040000

    switch (num) {
        case -3:  // bad flag
            printf("%s", Color_Red_Dark);
#ifdef _WIN32
            _setmode(_fileno(stdout), 0x00040000);
            wprintf(L" \u00D7 ");
            _setmode(_fileno(stdout), _O_TEXT);
#else
            printf(" \u00D7 ");
#endif
            printf("%s", Color_Reset);
            break;

        case -2:  // flag
            printf("%s", Color_Deep_Pink);
#ifdef _WIN32
            _setmode(_fileno(stdout), 0x00040000);
            wprintf(L" \u2691 ");
            _setmode(_fileno(stdout), _O_TEXT);
#else
            printf(" \u2691 ");
#endif
            printf("%s", Color_Reset);
            break;

        case -1:  // closed board
#ifdef _WIN32
            _setmode(_fileno(stdout), 0x00040000);
            wprintf(L" \u2BD1 ");
            _setmode(_fileno(stdout), _O_TEXT);
#else
            printf(" \u2BD1 ");
#endif
            break;

        case 0:
            printf("   ");
            break;

        case 1:
            printf(" %s%d%s ", Color_Blue, num, Color_Reset);
            break;

        case 2:
            printf(" %s%d%s ", Color_Green, num, Color_Reset);
            break;

        case 3:
            printf(" %s%d%s ", Color_Red, num, Color_Reset);
            break;

        case 4:
            printf(" %s%d%s ", Color_Purple, num, Color_Reset);
            break;

        case 5:
            printf(" %s%d%s ", Color_Yellow, num, Color_Reset);
            break;

        case 6:
            printf(" %s%d%s ", Color_Aqua, num, Color_Reset);
            break;

        case 7:
            printf(" %s%d%s ", Color_Orange, num, Color_Reset);
            break;

        case 8:
            printf(" %s%d%s ", Color_Green_Blue, num, Color_Reset);
            break;

        case 9:
            // Boob
            // random color :)
            srand(time(NULL));

            printf("\033[38;2;%d;%d;%dm", rand() % 256, rand() % 256,
                   rand() % 256);
#ifdef _WIN32
            _setmode(_fileno(stdout), 0x00040000);
            wprintf(L" \u01A0 ");
            _setmode(_fileno(stdout), _O_TEXT);
#else
            printf(" \u01A0 ");
#endif
            printf("%s", Color_Reset);
            break;
    }

    printf("%s|%s", Color_Gray, Color_Reset);
}

void Create_Game(int** Board, int Size_Board, int Boob_C, int Max_Column_Size) {

    int* Max_Column = (int*)malloc(Size_Board * sizeof(int));

    for (int i = 0; i < Size_Board; i++) {

        for (int j = 0; j < Size_Board; j++) {

            Board[i][j] = 0;
        }

        Max_Column[i] = 0;
    }

    srand(time(NULL));

    while (Boob_C) {

        int i = rand() % Size_Board;
        int j = rand() % Size_Board;

        if (Board[i][j] != 9 && Max_Column[j] < Max_Column_Size) {
            srand(time(NULL));
            Board[i][j] = 9;
            Max_Column[j] += 1;
            Boob_C--;
        }
    }

    free(Max_Column);

    for (int i = 0; i < Size_Board; i++) {

        for (int j = 0; j < Size_Board; j++) {

            if (Board[i][j] == 9) continue;

            if (i + 1 < Size_Board && Board[i + 1][j] == 9) Board[i][j]++;

            if (i - 1 >= 0 && Board[i - 1][j] == 9) Board[i][j]++;

            if (j + 1 < Size_Board && Board[i][j + 1] == 9) Board[i][j]++;

            if (j - 1 >= 0 && Board[i][j - 1] == 9) Board[i][j]++;

            if (j + 1 < Size_Board && i + 1 < Size_Board &&
                Board[i + 1][j + 1] == 9)
                Board[i][j]++;

            if (j - 1 >= 0 && i - 1 >= 0 && Board[i - 1][j - 1] == 9)
                Board[i][j]++;

            if (j - 1 >= 0 && i + 1 < Size_Board && Board[i + 1][j - 1] == 9)
                Board[i][j]++;

            if (i - 1 >= 0 && j + 1 < Size_Board && Board[i - 1][j + 1] == 9)
                Board[i][j]++;
        }
    }
}

void Play_Game(struct Player* User, int Size_Board, int Boob_C,
               int Max_Column_Size) {

    Bar_Status(User, 1);
    printf("%sBoard:%s %d*%d%s, ", Color_Green, Color_Gray, Size_Board,
           Size_Board, Color_Reset);
    printf("%sBoob:%s %d%s,", Color_Red, Color_Gray, Boob_C, Color_Reset);
    printf(" It Will Start in 3 Seconds.\n");
    // Sleep(3000);

    RUN_CLS;

    // Create Board

    int** Board = (int**)malloc(Size_Board * sizeof(int*));
    // int **Board = malloc(Size_Board * sizeof(int*));

    for (int i = 0; i < Size_Board; i++) {
        Board[i] = (int*)malloc(Size_Board * sizeof(int));
        // Board[i] = malloc(Size_Board * sizeof(int));
    }

    int Space_B = Size_Board * Size_Board - Boob_C;  // need for win
    int Flag_Left = Boob_C;
    int loss = 0;

    Create_Game(Board, Size_Board, Boob_C, Max_Column_Size);

    Print_Game(Board, User, Size_Board, Boob_C, loss, Flag_Left, 0);

    int i = 0, j = 0, in_c = 0;
    int ctrl_c_exit_game = 0;
    int debug_input_esc = 0;

    while (Space_B != 0 || Flag_Left != 0) {

        ctrl_c_exit_game = 0;

        if (i == -2 || j == -2 || in_c == 3) {
            // ctrl+c ~ exit code
            printf("\n");
            Bar_Status(User, 1);
            printf(
                "Exit Code (Press Ctrl+C To Exit ~ Press Another Button to "
                "Cancel)?  ");
            int exit_game = getch();

            if (exit_game == 3) {

                printf("%sExit%s\n", Color_Yellow, Color_Reset);

                // free the board
                for (int k = 0; k < Size_Board; k++) {
                    free(Board[k]);
                }

                free(Board);

                return;
            }

            printf("%sCanceled%s\n", Color_Green, Color_Reset);
        }

        // get column (x)
        while (1) {
            Bar_Status(User, 1);
            printf("Enter Column: ");
            j = User_Input_Number_Range(1, Size_Board);

            if (j == -1) continue;

            if (j == -2) ctrl_c_exit_game = 1;

            break;
        }

        // Return to the beginning of the While
        if (ctrl_c_exit_game) continue;

        // get row (y)
        while (1) {
            Bar_Status(User, 1);
            printf("Enter Row: ");
            i = User_Input_Number_Range(1, Size_Board);

            if (i == -1) continue;

            if (i == -2) ctrl_c_exit_game = 1;

            break;
        }

        // Return to the beginning of the While
        if (ctrl_c_exit_game) continue;

        // get char
        debug_input_esc = 0;

        while (1) {
            Bar_Status(User, 1);
            printf("Plz Enter R (flag) ~ L (reveal): ");

            in_c = getch();
            printf("%c\n", in_c);

            if (in_c == 3) {
                ctrl_c_exit_game = 1;
            }

            else if (in_c == 27) {
                // debug mode ~ esc (back door)
                Print_Game(Board, User, Size_Board, Boob_C, loss, Flag_Left, 0);
                Print_Game(Board, User, Size_Board, Boob_C, loss, Flag_Left, 1);
                debug_input_esc = 1;
            }

            else if (in_c != 'R' && in_c != 'r' && in_c != 'L' && in_c != 'l')
                continue;

            break;
        }

        // Return to the beginning of the While
        if (ctrl_c_exit_game) continue;

        // back to get data
        if (debug_input_esc) continue;

        i--;
        j--;

        if (in_c == 'R' || in_c == 'r') {

            if (Board[i][j] >= 20) {
                Board[i][j] -= 20;
                Flag_Left++;
            }

            else if (Flag_Left == 0) {
                Bar_Status(User, 1);
                printf("There are no Flags left for You :')\n");
                Sleep(2000);
            }

            else if (Board[i][j] >= 10) {
                Bar_Status(User, 1);
                printf("Why do you Want to put a Flag on the Number? :/\n");
                Sleep(2000);
            }

            else {
                Board[i][j] += 20;
                Flag_Left--;
            }

        }

        else if (in_c == 'L' || in_c == 'l') {

            // player loss
            if (Board[i][j] == 9) {
                loss = 1;
            }

            else if (Board[i][j] >= 20) {
                Bar_Status(User, 1);
                printf("You Cannot Remove the Flag\n");
                Sleep(2000);
            }

            else if (Board[i][j] >= 10) {
                Bar_Status(User, 1);
                printf("You have Already Opened here. :/\n");
                Sleep(2000);
            }

            else if (Board[i][j] == 0) {
                Space_B = Board_Zero_Open(Board, Size_Board, i, j, Space_B);
            }

            else {
                Board[i][j] += 10;
                Space_B--;
            }
        }

        Print_Game(Board, User, Size_Board, Boob_C, loss, Flag_Left, 0);

        if (loss) break;

    }  // main while end

    if (loss) {
        User->Loss += 1;

        if (User->Win_Streak < User->Cur_Streak) {
            User->Win_Streak = User->Cur_Streak;
        }

        User->Cur_Streak = 0;

        Bar_Status(User, 1);
        printf("You loss :(\n");
        Sleep(5000);
    }

    else {
        User->Win += 1;
        User->Cur_Streak += 1;

        Bar_Status(User, 1);
        printf("You Won :)\n");
        Sleep(5000);
    }

    for (int k = 0; k < Size_Board; k++) {
        free(Board[k]);
    }

    free(Board);

    Bar_Status(User, 1);
    printf("Back to Game Menu.\n");
    Sleep(1000);
}

int Board_Zero_Open(int** Board, int Size_Board, int x, int y, int Space_B) {

    if (x >= Size_Board || y >= Size_Board || x < 0 || y < 0) return Space_B;

    Board[x][y] += 10;
    Space_B--;

    if (Board[x][y] > 10) return Space_B;

    if (x + 1 < Size_Board && Board[x + 1][y] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x + 1, y, Space_B);
    }

    if (x - 1 >= 0 && Board[x - 1][y] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x - 1, y, Space_B);
    }

    if (y + 1 < Size_Board && Board[x][y + 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x, y + 1, Space_B);
    }

    if (y - 1 >= 0 && Board[x][y - 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x, y - 1, Space_B);
    }

    if (x + 1 < Size_Board && y + 1 < Size_Board && Board[x + 1][y + 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x + 1, y + 1, Space_B);
    }

    if (x - 1 >= 0 && y - 1 >= 0 && Board[x - 1][y - 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x - 1, y - 1, Space_B);
    }

    if (x - 1 >= 0 && y + 1 < Size_Board && Board[x - 1][y + 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x - 1, y + 1, Space_B);
    }

    if (x + 1 < Size_Board && y - 1 >= 0 && Board[x + 1][y - 1] < 9) {
        Space_B = Board_Zero_Open(Board, Size_Board, x + 1, y - 1, Space_B);
    }

    return Space_B;
}

void Admin_Panel_Login(struct Player* Admin, struct Player* Users) {

    while (1) {

        Bar_Status(Admin, 2);
        printf("Enter Admin PassWord (Ctrl+C ~ Back): ");

        //* Hack :)
        int Pass[4] = { Admin->Loss, Admin->Win, Admin->Cur_Streak,
                        Admin->Win_Streak };
        int Pass_In[4];
        int NOT_Login = 0;

        for (int i = 0; i < 4; i++) {

            int x = getch();

            // exit code Ctrl + C
            if (x == 3) {
                printf("%sExit.%s\n", Color_Yellow, Color_Reset);
                Sleep(1500);
                RUN_CLS;
                return;
            }

            Pass_In[i] = x;

            printf("%s* %s", Color_Gray, Color_Reset);

            if (Pass[i] != Pass_In[i]) NOT_Login = 1;
        }

        if (NOT_Login) {
            printf("%sPassword is Wrong.%s\n", Color_Red, Color_Reset);
            Sleep(500);
            continue;
        }

        printf("%sLogin was Successful.%s\n", Color_Green, Color_Reset);
        Sleep(1000);

        Admin_Panel(Admin, Users);
        return;
    }
}

void Admin_Panel(struct Player* Admin, struct Player* Users) {

    while (1) {

        Bar_Status(Admin, 2);
        printf("Admin Panel:\n\n");
        printf("    %s1 %s> %sAll Player Info\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sChange PassWord\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sDelete Player\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sEdit Player Info\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sLogout\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s6 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(Admin, 2);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 6);

        switch (in_code) {

            // ctrl + c exit code
            case -2:
                printf("%sExit.%s", Color_Red, Color_Reset);
                Sleep(3000);
                exit(0);
                break;

            case 1:
                printf(
                    "\n     %sID | UserName | Win | Loss | Current Streak | "
                    "Winning Streak%s\n",
                    Color_Gray, Color_Reset);

                for (int i = 0; i < Users_Counter; i++) {

                    printf("    ");
                    printf("%s %2d %s", Color_Yellow, Users[i].id, Color_Reset);
                    printf("%s|%s", Color_Gray, Color_Reset);
                    printf("%s %8s %s", Color_Blue, Users[i].Username,
                           Color_Reset);
                    printf("%s|%s", Color_Gray, Color_Reset);
                    printf("%s %3d %s", Color_Green,
                           (i != 0) ? (Users[i].Win) : (0), Color_Reset);
                    printf("%s|%s", Color_Gray, Color_Reset);
                    printf("%s %4d %s", Color_Red,
                           (i != 0) ? (Users[i].Loss) : (0), Color_Reset);
                    printf("%s|%s", Color_Gray, Color_Reset);
                    printf("%s %14d %s", Color_Yellow,
                           (i != 0) ? (Users[i].Cur_Streak) : (0), Color_Reset);
                    printf("%s|%s", Color_Gray, Color_Reset);
                    printf("%s %14d %s\n", Color_Yellow,
                           (i != 0) ? (Users[i].Win_Streak) : (0), Color_Reset);
                }

                break;

            case 2:
                int Pass_In[4];
                int Exit_Code = 0;
                Bar_Status(Admin, 2);
                printf("Enter New PassWord (Ctrl+C ~ Back): ");

                for (int i = 0; i < 4; i++) {

                    int x = getch();

                    // exit code Ctrl + C
                    if (x == 3) {
                        printf("%sBack.%s\n", Color_Yellow, Color_Reset);
                        Exit_Code = 1;
                        Sleep(500);
                        break;
                    }

                    Pass_In[i] = x;

                    printf("%s* %s", Color_Gray, Color_Reset);
                }

                if (Exit_Code) break;

                printf("\n");

                // if Confirm PassWord is Wrong back here.
                while (1) {

                    int Pass_In_Con[4];
                    int Flag = 0;
                    Bar_Status(Admin, 2);
                    printf("Confirm New PassWord (Ctrl+C ~ Back): ");

                    for (int i = 0; i < 4; i++) {

                        int x = getch();

                        // exit code Ctrl + C
                        if (x == 3) {
                            printf("%sBack.%s\n", Color_Yellow, Color_Reset);
                            Exit_Code = 1;
                            Sleep(500);
                            break;
                        }

                        Pass_In_Con[i] = x;

                        // PassWord is Wrong.
                        if (Pass_In[i] != Pass_In_Con[i]) Flag = 1;

                        printf("%s* %s", Color_Gray, Color_Reset);
                    }

                    if (Exit_Code) break;

                    if (Flag) {
                        printf("%sPassWord Don't Match.%s\n", Color_Red,
                               Color_Reset);
                        Sleep(500);
                        continue;
                    }

                    break;
                }

                if (Exit_Code) break;

                Admin->Loss = Pass_In[0];
                Admin->Win = Pass_In[1];
                Admin->Cur_Streak = Pass_In[2];
                Admin->Win_Streak = Pass_In[3];

                printf("%sPassWord Change.%s\n", Color_Green, Color_Reset);
                Sleep(500);
                break;

            case 3:

                int del_in;

                while (1) {

                    Bar_Status(Admin, 2);
                    printf("Enter A User ID (0 ~ %d) (Ctrl+C ~ Back): ",
                           Users_Counter - 1);

                    del_in = User_Input_Number_Range(0, Users_Counter - 1);

                    if (del_in == -1) continue;

                    break;
                }

                if (del_in == -2) {
                    printf("%sBack. \n%s", Color_Yellow, Color_Reset);
                    break;
                }

                else if (del_in == 0) {
                    Error_Management(30);
                    break;
                }

                Delete_Player(del_in, Users);

                Bar_Status(Admin, 2);
                printf("The User With ID %d was Deleted.\n", del_in);
                Sleep(500);
                break;

            case 4:

                int id_edit_in;
                // get id user to edit
                while (1) {

                    Bar_Status(Admin, 2);
                    printf("Enter A User ID (0 ~ %d) (Ctrl+C ~ Back): ",
                           Users_Counter - 1);

                    id_edit_in = User_Input_Number_Range(0, Users_Counter - 1);

                    if (id_edit_in == -1) continue;

                    break;
                }

                if (id_edit_in == -2) {
                    printf("%sBack. \n%s", Color_Yellow, Color_Reset);
                    break;
                }

                else if (id_edit_in == 0) {
                    Error_Management(31);
                    break;
                }

                int edit_in;
                // get itme user to edit
                while (1) {

                    Bar_Status(Admin, 2);
                    printf(
                        "Edit (Win ~ 1)(Loss ~ 2)(Cur Streak ~ 3)(Win Streak ~ "
                        "4).\n");

                    Bar_Status(Admin, 2);
                    printf("Select one More (Ctrl+C ~ Back): ");

                    edit_in = User_Input_Number_Range(1, 4);

                    if (edit_in == -1) continue;

                    break;
                }

                if (edit_in == -2) {
                    printf("%sBack. \n%s", Color_Yellow, Color_Reset);
                    break;
                }

                int edit_value_in;
                // get value of itme to edit
                while (1) {

                    Bar_Status(Admin, 2);
                    printf("Specify its Value (0 ~ 999) (Ctrl+C ~ Back): ");

                    edit_value_in = User_Input_Number_Range(0, 999);

                    if (edit_value_in == -1) continue;

                    break;
                }

                if (edit_value_in == -2) {
                    printf("%sBack. \n%s", Color_Yellow, Color_Reset);
                    break;
                }

                // edit info
                switch (edit_in) {
                    case 1:
                        Users[id_edit_in].Win = edit_value_in;
                        break;
                    case 2:
                        Users[id_edit_in].Loss = edit_value_in;
                        break;
                    case 3:
                        Users[id_edit_in].Cur_Streak = edit_value_in;
                        break;
                    case 4:
                        Users[id_edit_in].Win_Streak = edit_value_in;
                        break;
                }

                // done
                Bar_Status(Admin, 2);
                printf("Editing was done Successfully :)\n");
                Sleep(500);

                break;

            case 5:
                Bar_Status(Admin, 2);
                printf("logout Successful.\n");
                Sleep(1500);
                RUN_CLS;
                return;
                break;

            case 6:
                Bar_Status(Admin, 2);
                printf("%sExit%s :) %sBye Bye.\n", Color_Red, Color_Yellow,
                       Color_Reset);
                Sleep(3000);
                exit(0);
                return;
                break;
        }
    }
}

void Error_Management(int code) {

    printf("\n    %sError: %s", Color_Red, Color_Reset);

    //! Error
    //? 10 - 19 Number
    //? 20 - 29 User Error
    //? 30 - 39 Admin Error

    switch (code) {
        case 10:
            printf("%s#Code 1-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number.\n", Color_Gray);
            printf("      Please Enter a Number in the given Range.%s\n",
                   Color_Reset);
            break;

        case 11:
            printf("%s#Code 1-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number.\n", Color_Gray);
            printf("      Please Enter a NUMBER :/%s\n", Color_Reset);
            break;

        case 12:  // very out of range
            printf("%s#Code 1-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number. Keep Calm :)\n",
                   Color_Gray);
            printf("      Everything is Under Control :)\n");
            printf("      Please Enter a Number in the given Range.%s\n",
                   Color_Reset);
            break;

        case 20:
            printf("%s#Code 2-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Please do not Enter a duplicate UserName.%s\n",
                   Color_Reset);
            break;

        case 21:
            printf("%s#Code 2-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Username length should not be more than 50.%s\n",
                   Color_Reset);
            break;

        case 22:
            printf("%s#Code 2-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Username length must be more than 1 Character.%s\n",
                   Color_Reset);
            break;

        case 30:  // delete admin :/
            printf("%s#Code 3-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Deleting Admin.\n", Color_Gray);
            printf("      Why do you want to destroy yourself ? :/%s\n",
                   Color_Reset);
            break;

        case 31:  // edit admin :/
            printf("%s#Code 3-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Editing Admin.\n", Color_Gray);
            printf("      Why do you want to edit the admin :/%s\n",
                   Color_Reset);
            break;

        default:
            printf("%s¯\\_(ツ)_/¯%s\n", Color_Yellow, Color_Reset);
            break;
    }

    Sleep(3000);

    //* Spam Protection :))))))
    printf("    %sPress Ctrl+C to Continue. %s", Color_Green, Color_Reset);

    int x = -1;
    int c = 0, cc = 0;

    while (x != 3) {

        x = getch();
        c++;

        if (c > 10) {
            printf("%s!%s", Color_Red, Color_Reset);
            c -= 10;
            cc++;
        }

        // Emergency EXIT :/
        if (cc >= 10) {
            printf("%s WTF :/%s", Color_Yellow, Color_Reset);
            Sleep(1000);
            exit(0);
        }
    }

    printf("\n");
}

int User_Input_Number_Range(int start, int end) {

    /*
        -1 Error input
        -2 Ctrl+C Exit Code
    */

    int final = 0, c = 0, tmp = 0, end_c = 0;

    tmp = end;
    while (tmp > 0) {
        tmp /= 10;
        end_c++;
    }

    int x = getch();

    // Ctrl+C Exit Code
    if (x == 3) {
        printf("%sCtrl+C %s", Color_Gray, Color_Reset);
        return -2;
    }

    if (x == 13 || x == 8) {
        printf("\n");
        Error_Management(11);
        return -1;
    }

    // enter code ~ 13
    while (x != 13) {

        // Ctrl+C Exit Code
        if (x == 3) {
            printf("%sCtrl+C %s", Color_Gray, Color_Reset);
            return -2;
        }

        printf("%c", (char)x);

        x -= 48;

        // enter a char or chert & pert :/
        if (!(x >= 0 && x <= 9)) {

            // backspace code ~ 8
            if (x == -40) {

                // back cursor
                printf(" ");
                printf("%c", (char)8);

                c -= 2;
                final /= 100;
                x = 0;

            }

            else {
                printf("\n");
                Error_Management(11);
                return -1;
            }
        }

        final += x;
        final *= 10;
        c++;

        if (end_c + 2 < c) {
            printf("\n");
            Error_Management(12);
            return -1;
        }

        x = getch();
    }

    final /= 10;

    printf("\n");

    if (!(final >= start && final <= end)) {

        Error_Management(10);
        return -1;
    }

    return final;
}

int Len_Number(int num) {

    if (num == 0) num++;

    int Len = 0;

    while (num != 0) {
        num /= 10;
        Len++;
    }

    return Len;
}

// END GAME ¯\_(ツ)_/¯

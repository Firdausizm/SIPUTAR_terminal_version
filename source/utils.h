#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

using namespace std;

// Kode warna ANSI
const string RESET      = "\033[0m";
const string BOLD       = "\033[1m";
const string DIM        = "\033[2m";
const string UNDERLINE  = "\033[4m";

const string RED        = "\033[31m";
const string GREEN      = "\033[32m";
const string YELLOW     = "\033[33m";
const string BLUE       = "\033[34m";
const string MAGENTA    = "\033[35m";
const string CYAN       = "\033[36m";
const string WHITE      = "\033[37m";

const string BOLD_RED     = "\033[1;91m";
const string BOLD_GREEN   = "\033[1;32m";
const string BOLD_YELLOW  = "\033[1;33m";
const string BOLD_BLUE    = "\033[1;34m";
const string BOLD_MAGENTA = "\033[1;35m";
const string BOLD_CYAN    = "\033[1;36m";
const string BOLD_WHITE   = "\033[1;37m";

const string BG_BLUE    = "\033[44m";
const string BG_GREEN   = "\033[42m";
const string BG_RED     = "\033[41m";
const string BG_CYAN    = "\033[46m";

// Clear screen (cross-platform)
inline void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pause screen
inline void pauseScreen() {
    cout << endl;
    cout << DIM << "  Tekan Enter untuk melanjutkan..." << RESET;
    cin.ignore();
    cin.get();
}

// Pause screen tanpa ignore (saat sebelumnya sudah getline)
inline void pauseScreenNoIgnore() {
    cout << endl;
    cout << DIM << "  Tekan Enter untuk melanjutkan..." << RESET;
    cin.get();
}

// Print garis pemisah
inline void printLine(int length = 56) {
    cout << CYAN;
    for (int i = 0; i < length; i++) cout << "═";
    cout << RESET << endl;
}

// Print garis tipis
inline void printThinLine(int length = 56) {
    cout << DIM;
    for (int i = 0; i < length; i++) cout << "─";
    cout << RESET << endl;
}

// Print header SIPUTAR
inline void printHeader() {
    const int innerWidth = 52;
    const string title = "★  SIPUTAR  ★";
    const string subtitle = "Sistem informasi Puskesmas Pintar";

    auto printBorderLine = [&](const string& left, const string& fill, const string& right) {
        cout << BOLD_CYAN << left;
        for (int i = 0; i < innerWidth; i++) cout << fill;
        cout << right << RESET << endl;
    };

    auto centerText = [&](const string& text) {
        int paddingLeft = (innerWidth - static_cast<int>(text.length())) / 2;
        int paddingRight = innerWidth - paddingLeft - static_cast<int>(text.length());
        return string(paddingLeft, ' ') + text + string(paddingRight, ' ');
    };

    cout << endl;
    printBorderLine("  ╔", "═", "╗");
    cout << BOLD_CYAN << "  ║" << RESET;
    cout << BOLD_WHITE << centerText(title) << RESET;
    cout << BOLD_CYAN << "║" << RESET << endl;
    cout << BOLD_CYAN << "  ║" << RESET;
    cout << DIM << centerText(subtitle) << RESET;
    cout << BOLD_CYAN << "║" << RESET << endl;
    printBorderLine("  ╚", "═", "╝");
    cout << endl;
}

// Print sub-header
inline void printSubHeader(const string& title) {
    const int innerWidth = 52;
    int titleLen = title.length();
    int paddingLeft = (innerWidth - titleLen - 2) / 2;  // -2 untuk spasi kiri kanan
    int paddingRight = innerWidth - titleLen - 2 - paddingLeft;
    
    cout << endl;
    cout << BOLD_BLUE << "  ┌";
    for (int i = 0; i < innerWidth; i++) cout << "─";
    cout << "┐" << RESET << endl;
    
    cout << BOLD_BLUE << "  │" << RESET;
    cout << string(paddingLeft, ' ');
    cout << BOLD_WHITE << " " << title << " " << RESET;
    cout << string(paddingRight, ' ');
    cout << BOLD_BLUE << "│" << RESET << endl;
    
    cout << BOLD_BLUE << "  └";
    for (int i = 0; i < innerWidth; i++) cout << "─";
    cout << "┘" << RESET << endl;
    cout << endl;
}

// Print menu item
inline void printMenuItem(int nomor, const string& text) {
    cout << "    " << BOLD_CYAN << "[" << nomor << "]" << RESET << " " << text << endl;
}

// Print info label + value
inline void printInfo(const string& label, const string& value) {
    cout << "    " << DIM << label << RESET << " : " << BOLD_WHITE << value << RESET << endl;
}

// Print pesan sukses
inline void printSuccess(const string& msg) {
    cout << endl << "  " << BOLD_GREEN << "✓ " << msg << RESET << endl;
}

// Print pesan error
inline void printError(const string& msg) {
    cout << endl << "  " << BOLD_RED << "✗ " << msg << RESET << endl;
}

// Print pesan warning
inline void printWarning(const string& msg) {
    cout << endl << "  " << BOLD_YELLOW << "⚠ " << msg << RESET << endl;
}

// Print prompt input
inline void printPrompt(const string& label) {
    cout << "    " << BOLD_YELLOW << "→ " << RESET << label << ": ";
}

#endif // UTILS_H

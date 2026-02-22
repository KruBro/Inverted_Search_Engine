#ifndef COLOR_H
#define COLOR_H

// Regular Text Colors
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"

// Bold Colors (Great for Headers)
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_CYAN    "\033[1;36m"
#define BOLD_WHITE   "\033[1;37m"

// High Intensity Colors (More "Vibrant")
#define H_RED     "\033[0;91m"
#define H_GREEN   "\033[0;92m"
#define H_YELLOW  "\033[0;93m"
#define H_BLUE    "\033[0;94m"
#define H_MAGENTA "\033[0;95m"
#define H_CYAN    "\033[0;96m"

// Background Colors
#define BG_RED    "\033[41m"
#define BG_GREEN  "\033[42m"
#define BG_BLUE   "\033[44m"

// Text Decoration & Reset
#define UNDERLINE "\033[4m"
#define RESET     "\033[0m"

#endif
//Regular text
#include <stdio.h>
#define ANSI_BLACK "\e[0;30m"
#define ANSI_RED "\e[0;31m"
#define ANSI_GREEN "\e[0;32m"
#define ANSI_YELLOW "\e[0;33m"
#define ANSI_BLUE "\e[0;34m"
#define ANSI_MAGENTA "\e[0;35m"
#define ANSI_CYAN "\e[0;36m"
#define ANSI_WHITE "\e[0;37m"

//Regular bold text
#define ANSI_B_BLACK "\e[1;30m"
#define ANSI_B_RED "\e[1;31m"
#define ANSI_B_GREEN "\e[1;32m"
#define ANSI_B_YELLOW "\e[1;33m"
#define ANSI_B_BLUE "\e[1;34m"
#define ANSI_B_MAGENTA "\e[1;35m"
#define ANSI_B_CYAN "\e[1;36m"
#define ANSI_B_WHITE "\e[1;37m"

//Regular underline text
#define ANSI_U_BLACK "\e[4;30m"
#define ANSI_U_RED "\e[4;31m"
#define ANSI_U_GREEN "\e[4;32m"
#define ANSI_U_YELLOW "\e[4;33m"
#define ANSI_U_BLUE "\e[4;34m"
#define ANSI_U_MAGENTA "\e[4;35m"
#define ANSI_U_CYAN "\e[4;36m"
#define ANSI_U_WHITE "\e[4;37m"

//Regular background
#define ANSI_BACK_BLACK "\e[40m"
#define ANSI_BACK_RED "\e[41m"
#define ANSI_BACK_GREEN "\e[42m"
#define ANSI_BACK_YELLOW "\e[43m"
#define ANSI_BACK_BLUE "\e[44m"
#define ANSI_BACK_MAGENTA "\e[45m"
#define ANSI_BACK_CYAN "\e[46m"
#define ANSI_BACK_WHITE "\e[47m"

//High intensty background 
#define ANSI_H_BACK_BLACK "\e[0;100m"
#define ANSI_H_BACK_RED "\e[0;101m"
#define ANSI_H_BACK_GREEN "\e[0;102m"
#define ANSI_H_BACK_YELLOW "\e[0;103m"
#define ANSI_H_BACK_BLUE "\e[0;104m"
#define ANSI_H_BACK_MAGENTA "\e[0;105m"
#define ANSI_H_BACK_CYAN "\e[0;106m"
#define ANSI_H_BACK_WHITE "\e[0;107m"

//High intensty text
#define ANSI_H_BLACK "\e[0;90m"
#define ANSI_H_RED "\e[0;91m"
#define ANSI_H_GREEN "\e[0;92m"
#define ANSI_H_YELLOW "\e[0;93m"
#define ANSI_H_BLUE "\e[0;94m"
#define ANSI_H_MAGENTA "\e[0;95m"
#define ANSI_H_CYAN "\e[0;96m"
#define ANSI_H_WHITE "\e[0;97m"

//Bold high intensity text
#define ANSI_BH_BLACK "\e[1;90m"
#define ANSI_BH_RED "\e[1;91m"
#define ANSI_BH_GREEN "\e[1;92m"
#define ANSI_BH_YELLOW "\e[1;93m"
#define ANSI_BH_BLUE "\e[1;94m"
#define ANSI_BH_MAGENTA "\e[1;95m"
#define ANSI_BH_CYAN "\e[1;96m"
#define ANSI_BH_WHITE "\e[1;97m"

//Reset
#define ANSI_RESET "\e[0m"

void setColor(char* color) {
    printf("%s", color);
}

void resetColor() {
    printf("%s", ANSI_RESET);
}
/**
 * CSI(Control Sequence Introducer/Initiator) sign
 * more information on https://en.wikipedia.org/wiki/ANSI_escape_code
*/

#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__


#define CSI_START           "\033[1;"    //high light
#define CSI_END             "\033[0m"

/* output log front color */
#define F_BLACK             "30m"
#define F_RED               "31m"
#define F_GREEN             "32m"
#define F_YELLOW            "33m"
#define F_BLUE              "34m"
#define F_MAGENTA           "35m"
#define F_CYAN              "36m"
#define F_WHITE             "37m"

#define COLOR_OFST__BLACK   0
#define COLOR_OFST__RED     1
#define COLOR_OFST__GREEN   2
#define COLOR_OFST__YELLOW  3
#define COLOR_OFST__BLUE    4
#define COLOR_OFST__MAGENTA 5
#define COLOR_OFST__CYAN    6
#define COLOR_OFST__WHITE   7


#define PRINTF(FORMAT,...)   printf(FORMAT,##__VA_ARGS__)
#define PUTS(s)     {puts(s)}
#define LOG_D(FORMAT,...) printf(FORMAT,##__VA_ARGS__)
#define LOG_I(FORMAT,...) printf(CSI_START""F_GREEN""FORMAT""CSI_END,##__VA_ARGS__)
#define LOG_W(FORMAT,...) printf(CSI_START""F_YELLOW""FORMAT""CSI_END,##__VA_ARGS__)
#define LOG_E(FORMAT,...) printf(CSI_START""F_RED""FORMAT""CSI_END,##__VA_ARGS__)
#define PRINTF_R(FORMAT,...)


#define PRINT_XML_PARSE_LOG 0
#define PRINT_HTTP_LOG      0
#define PRINT_SE_LOG        0

#if PRINT_XML_PARSE_LOG
    #define PRINT_XML(something)     {something;}
#else
    #define PRINT_XML(something)
#endif

#if PRINT_HTTP_LOG
    #define PRINT_HTTP(something)     {something;}
#else
    #define PRINT_HTTP(something)
#endif

#if PRINT_SE_LOG
    #define PRINT_SE(something)     {something;}
#else
    #define PRINT_SE(something)
#endif

#endif  //__DEBUG_LOG_H__

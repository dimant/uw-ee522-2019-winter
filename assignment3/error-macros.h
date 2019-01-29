#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#define FAIL_CODE 1

#define IF_THEN_FAIL(expr, code, message) { if(expr) { fprintf(stderr, message); return code; } }
#define IF_THEN_FAIL_FMT(expr, code, message, ...) { if(expr) { fprintf(stderr, message, ##__VA_ARGS__); return code; } }

#endif
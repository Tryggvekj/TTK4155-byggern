#include <stdio.h>

#pragma once

#define DEBUG_PRINT(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
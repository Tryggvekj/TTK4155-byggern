/** ***************************************************************************
 * @file debug.h
 * @author Magnus Carlsen Haaland
 * @brief Debug functionality
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025 Byggarane
 * 
*******************************************************************************/

#pragma once

#include <stdio.h>

// Set to 1 to enable debug prints, 0 to disable
#define DEBUG 1

#define DEBUG_PRINTF(fmt, ...) \
            do { if (DEBUG) printf(fmt, __VA_ARGS__); } while (0)

#define DEBUG_PRINT(str) \
            do { if (DEBUG) printf(str); } while (0)

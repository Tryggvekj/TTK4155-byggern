#!/bin/bash
# Stack usage analysis script

echo "=== Stack Usage Analysis ==="
echo "ATmega162 has 1024 bytes of SRAM"
echo "Current memory usage:"

if [ -f "build/a.out" ]; then
    avr-size build/a.out
    echo ""
    
    echo "Stack usage by function (if .su files exist):"
    if ls build/*.su 1> /dev/null 2>&1; then
        find build -name "*.su" -exec cat {} \; | sort -k2 -nr | head -10
    else
        echo "No .su files found. Add -fstack-usage to CFLAGS to generate them."
    fi
    
else
    echo "build/a.out not found. Run 'make' first."
fi
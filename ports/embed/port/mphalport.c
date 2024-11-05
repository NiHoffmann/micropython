/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022-2023 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include "py/mphal.h"
#include "mpconfigport.h"
#include "mphalport.h"

#ifdef ORB_ENABLE_MONITOR_STD_OUT
    #include "Monitor_C_Interface.h"
#endif


#ifdef ORB_ENABLE_MONITOR_STD_OUT
    #define WIDTH (31)
    #define HEIGHT (4)

    static char out[HEIGHT][WIDTH];
    static uint8_t pointer_width = 0;
    static uint8_t pointer_height = 0;
    static uint8_t current_length = 0;

    void reset_std_out(){
        pointer_width = 0;
        pointer_height = 0;
        current_length = 0;
    }
#endif
//Send string of given length to stdout, converting \n to \r\n.
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    #ifdef ORB_ENABLE_MONITOR_STD_OUT
        for(int i=0;i<len;i++){
            if(str[i] == '\n'){
                if(current_length > 0){
                    setMonitorText(pointer_height, (const char*)out[pointer_height], current_length);
                }
                //new line
                pointer_height = (pointer_height + 1) % HEIGHT;
                pointer_width = 0;
                current_length = 0;
                continue;
            }

            if(str[i] == '\r'){
                if(current_length > 0){
                    setMonitorText(pointer_height, (const char*)out[pointer_height], current_length);
                }
                //carrier return
                pointer_width = 0;
                current_length = 0;
                continue;
            }
            out[pointer_height][pointer_width] = str[i];
            current_length++;

            pointer_width = (pointer_width+1)%WIDTH;
            if(pointer_width == 0){
                setMonitorText(pointer_height, (const char*)out[pointer_height], current_length);
                pointer_height = (pointer_height + 1) % HEIGHT;
                current_length = 0;
            }
        }

        if(current_length > 0){
            setMonitorText(pointer_height, (const char*)out[pointer_height], current_length);
        }
    #else
        printf("%.*s", (int)len, str);
    #endif
}

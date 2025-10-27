#include "vofa.h"
#include <stdarg.h> 

void VOFA_SendByte(uint8_t byte)
{
    bsp_vofa_usart_transmit_byte(byte);
}

// ?????
void VOFA_SendString(const char *str)
{


 bsp_vofa_usart_transmit_string(str);


}


void VOFA_SendFloat(float f)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.3f", f); 
    VOFA_SendString(buffer);
}
void VOFA_SendMulti(const char *format, ...)
{
    char buffer[128]; 
    int offset = 0;

    va_list args;
    va_start(args, format);

    
    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == 'f') 
        {
            float val = (float)va_arg(args, double); // float ????? double
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%.1f,", val);
        }
        else if (format[i] == 'i') // ??
        {
            int val = va_arg(args, int);
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%d,", val);
        }
        else if (format[i] == 'u') // ?????
        {
            unsigned int val = va_arg(args, unsigned int);
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%u,", val);
        }
        
    }

    va_end(args);


    if (offset > 0)
    {
        buffer[offset - 1] = '\r';
        buffer[offset] = '\n';
        buffer[offset + 1] = '\0';
    }

   
    bsp_vofa_usart_transmit_string(buffer);
}


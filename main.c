#include <stdint.h>
#include <stdio.h>

// This function will check if the string is proper ASCII or not.
// @param: unsigned char --- so it can go from range [0,255] instead of regular [-128,127]
// @return: 0 : False, 1 : True.
//
int32_t is_ascii(unsigned char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        // If a character is not ASCII, immediate False.
        if (str[i] >= 128)
        {
            return 0;
        }
    }

    return 1;
}

// This function will capitalized all characters
// @param: a string 
// @return: how many characters capitalized
//
int32_t capitalize_ascii(char str[])
{
    int counter = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
        {
            str[i] = str[i] - 32;
            counter++;
        }
    }

    return counter;
}

int32_t getCodePoint(char c1, char c2)
{
    
}

/*
int32_t width_from_start_bytes(unsigned char start_byte)
{
    return is_ascii(start_byte);
}
*/

int main()
{
    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    //printf("Width: %d bytes\n", width_from_start_bytes(s[0])); // start byte 0xC3 indicates 2-byte sequence

    return 0;
}
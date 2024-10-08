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

/*
// This function will return code point the first half
// @param: a char 
// @return: the first half code point
//
int32_t getCodePointAsByte(unsigned char c)
{
    if (c & 0b00011111 >= 0b11000000)
    {
        return 1;
    }

    return 0;
}
*/

// This function will return the width of how many bytes a UTF-8 takes up
// @param: a char 
// @return: width of how bytes
//
int32_t width_from_start_bytes(char start_byte)
{
    //For ASCII, highest order byte is 0b0xxxxxxx
    if ((start_byte & 0b10000000) == 0b00000000) 
        return 1;

    //For 2 bytes, start byte is 0b11xxxxxxx
    else if ((start_byte & 0b11100000) == 0b11000000)
        return 2;

    //SAME THING 
    else if ((start_byte & 0b11110000) == 0b11100000)
        return 3;

    //Continuation bytes will always satrt with 0b10xxxxxx
    else if ((start_byte & 0b11000000) == 0b10000000)
        return -1;
}


int main()
{
    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_bytes(s[1])); // start byte 0xC3 indicates 2-byte sequence

    return 0;
}
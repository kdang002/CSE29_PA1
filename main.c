#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

// This function will return the width of how many bytes a UTF-8 takes up
// @param: a char 
// @return: width of how bytes
//
int32_t width_from_start_bytes(char start_byte)
{
    //For ASCII, isolate first bit and check start byte is 0b0xxxxxxx
    if ((start_byte & 0b10000000) == 0b00000000) 
    {
        //printf("ASCII\n");
        return 1;
    }

    //For 2 bytes, isolate first 3 bits and check start byte is 0b110xxxxxx
    else if ((start_byte & 0b11100000) == 0b11000000)
    {
        //printf("BYTE 2\n");
        return 2;
    }

    //For 3 bytes, isolate first 4 bits and check start byte is 0b1110xxxx
    else if ((start_byte & 0b11110000) == 0b11100000)
        return 3;

    //For 4 bytes, isolate first 5 bits and check start byte is 0b1110xxxx
    else if ((start_byte & 0b11111000) == 0b11110000)
        return 4;

    //Continuation bytes or multi bytes will always start with 0b10xxxxxx
    else if ((start_byte & 0b11000000) == 0b10000000)
        return -1;
}

// This function will return the length of the UTF-8 string CORRECTLY
// @param: UTF-8 string 
// @return: length of it
//
int32_t utf8_strlen(char str[])
{
    int counter = 0;
    int i = 0;

    // RECOMMENDED TO USE WHILE LOOP => EASIER TO KEEP TRACK WITH THE 'i' AND THE "width of bytes".
    while (str[i] != '\0')
    {
        counter++;
        i += width_from_start_bytes(str[i]);
    }

    return counter;
}

// This function will convert codepoint_index_to_byte_index
// @param: UTF-8 string, codepoint index (cpi)
// @return: int byte index
//
int32_t codepoint_index_to_byte_index(char str[], int32_t cpi)
{
    int codePointIterator = 0;
    int byteIterator = 0;
    while (str[byteIterator] != '\0')
    {

        if (codePointIterator == cpi)
        {
            return byteIterator;
        }

        if (width_from_start_bytes(str[byteIterator]) == -1)
            return -1;

        byteIterator += width_from_start_bytes(str[byteIterator]);
        codePointIterator++;
    }

    return 0;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[])
{
    int byteStart = codepoint_index_to_byte_index(str, cpi_start);
    int byteEnd = codepoint_index_to_byte_index(str, cpi_end);

    int index = 0;

    for (int i = byteStart; i != byteEnd; i++)
    {
        //CONCATNATE from cpi_start to cpi_end to result.
        result[index] = str[i];
        index++;
    }

    result[index] = '\0';

    /*
    // RECOMMENDED TO USE WHILE LOOP => EASIER TO KEEP TRACK WITH THE 'i' AND THE "width of bytes".

    while (str[index] != '\0')
    {
        if (i )
        i += width_from_start_bytes(str[i]);
    }
    */
}

int main()
{
    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("String: %s\nSubstring: %s", result); // these emoji are 4 bytes long


    //char str[5] = "HAHA";
    //printf("STRING: %s \n", strcat(result,str));

    return 0;
}
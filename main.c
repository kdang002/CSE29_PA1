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
int32_t width_from_start_bytes(unsigned char start_byte)
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
    {

        return -1;
    }

    return 0;
    

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
        int width = width_from_start_bytes((unsigned char)str[byteIterator]);
        if (width == -1)
            return -1;

       byteIterator += width;
       codePointIterator++;

        if (codePointIterator == cpi)
        {
            return byteIterator;
        }
    }

    return 0;
    
}

// This function will convert codepoint_index_to_byte_index
// @param: UTF-8 string, codepoint index (cpi)
// @return: int byte index
// FUNCTIONING NOW - DUE TO codepoint_index_to_byte_index()
void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[])
{
    int byteStart = codepoint_index_to_byte_index(str, cpi_start);
    int byteEnd = codepoint_index_to_byte_index(str, cpi_end);

    printf("BYTE START/END: %d %d \n", byteStart, byteEnd);

    if (byteStart == -1 || byteEnd == -1)
    {
        return;
    }

    if (cpi_start < 0 || cpi_end <= cpi_start)
        return;

    int index = 0;

    //This loop will copy the indicated bytes into result[]
    ///LOGIC ERROR: MEMORY CONTROLS
    for (int i = byteStart; i < byteEnd; i++)
    {
        result[index] = str[i];
        index++;
    }

    result[index] = '\0';
}


// Takes a UTF-8 encoded string and a codepoint index, 
// and returns a decimal representing the codepoint at that index.
// @param: string, codepointindex
// @return: decimal represent codepoint at index.
int32_t codepoint_at(char str[], int32_t cpi)
{
    int codePointIterator = 0;
    int byteIterator = 0;

    while (str[byteIterator] != '\0')
    {
        int width = width_from_start_bytes((unsigned char)str[byteIterator]);
        if (width == -1)
            return -1;

        byteIterator += width;
        codePointIterator++;

        if (codePointIterator == cpi)
        {
            //CHECK FOR THE CASE WHEN CPI is UTF, DOESN'T RETURN -1
            return str[byteIterator]; // SHOULD RETURN AT BYTE ITERATOR TO PREVENT WRONG MEMORY.
        }
    }

    return 0;
}

// Takes a UTF-8 encoded string and an codepoint index, and returns if the code point at that index is an animal emoji.
// @param: UTF-8 string, codePointIndex
// @return: 1 : TRUE, 0 : FALSE
// NOT WORKING
int is_animal_emoji_at(char str[], int32_t cpi)
{
    int codePoint = codepoint_at(str, cpi);

    ///LOGIC ERROR: THE CONDITION NEVER GETS EXECUTED
    return ((codePoint >= 0b00011111010000000000000000000000 && codePoint <= 0b00011111010000111111) ||  // NESTED LIST OF EMOJI BASE ON
        (codePoint >= 0b00011111001001100000 && codePoint <= 0b00011111001010111110))   ;                // WIKI'S UNICODE

}

int main()
{
    //substring() test
    char result[17];
    char newS[] = "🦀🦮🦮🦀🦀🦮🦮";
    utf8_substring(newS, 3, 7, result);
    printf("String: %s\nSubstring: %s\n", newS, result); // these emoji are 4 bytes long

    //animal_emoji() test
    char str[] = "🐀Rat";
    int32_t idx = 0;
    printf("Codepoint index %d of %s is ANIMAL: %d\n", idx, str, is_animal_emoji_at(str, idx));

    //codepoint_at() test
    char str1[] = "Joséph";
    idx = 1;
    printf("Codepoint at %d in %s is %d\n", idx, str1, codepoint_at(str1, idx)); // 'p' is the 4th codepoint

    return 0;
}
// VY KIET DANG (A18531908)
// CSE29_PA1
// 10/07/2024

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// This function will check if the string is proper ASCII or not.
// @param: unsigned char --- so it can go from range [0,255] instead of regular [-128,127]
// @return: 0 : False, 1 : True.
int32_t is_ascii(unsigned const char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        // If a character is not ASCII, immediate False.
        if (str[i] > 127)
        {
            return 0;
        }
    }

    return 1;
}

// This function will capitalized all characters
// @param: a string 
// @return: how many characters capitalized
int32_t capitalize_ascii(char str[])
{
    int counter = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - 32;
            counter++;
        }
    }

    return counter;
}

// This function will return the width of how many bytes a UTF-8 takes up
// @param: a char (ASCII OR UTF-8)
// @return: width of how bytes
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
// @return: codepoint-length of it
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

    return counter; // EXCLUDING NULL TERMINATOR
}

// This function will convert codepoint_index_to_byte_index
// @param: UTF-8 string, codepoint index (cpi)
// @return: int byte index
int32_t codepoint_index_to_byte_index(const char str[], int32_t cpi)
{
    
    int codePointIterator = 0;
    int byteIterator = 0;

    while (str[codePointIterator] != '\0')
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

// This function will return a substring
// @param: UTF-8 string, codepoint index (cpi)
// @return: int byte index
void utf8_substring(const char str[], int32_t cpi_start, int32_t cpi_end, char result[])
{    
    if (cpi_start < 0)
    {        
        return;
    }

    int32_t str_length = utf8_strlen(str);
    
    if (cpi_end > str_length)
    {
	cpi_end = str_length;
    }

    if (cpi_end <= cpi_start)
    {
	result[0] = '\0';
	return;
    }

    int byteStart = codepoint_index_to_byte_index(str, cpi_start);
    int byteEnd = codepoint_index_to_byte_index(str, cpi_end);

    //printf("BYTE START/END: %d %d \n", byteStart, byteEnd);

    if (byteStart == -1 || byteEnd == -1)
    {
        return;
    }


    int index = 0;

    //This loop will copy the indicated bytes into result[]
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
    int byteIndex = codepoint_index_to_byte_index(str, cpi);
    
    if (byteIndex == -1)
        return -1;

    // GET START BYTE
    unsigned char c1 = (unsigned char)str[byteIndex];
    int width = width_from_start_bytes(c1);

    /// REFER TO WEEK 2 MONDAY - ANNOTATED NOTES
    /// on how to convert to UTF-8 exact value.
    switch (width)
    {
        case 1:
            return c1;
        
        case 2:
        {
            unsigned char c2 = (unsigned char)str[byteIndex+1];
            return (c1 & 0b00011111) * 64 + (c2 & 0b00111111);
        }
            

        case 3:
        {
            unsigned char c2 = (unsigned char)str[byteIndex+1];
            unsigned char c3 = (unsigned char)str[byteIndex+2];

            return (c1 & 0b00001111) * 4096 + (c2 & 0b00111111) * 64 + (c3 & 0b00111111);
        }

        case 4:
        {
            unsigned char c2 = (unsigned char)str[byteIndex+1];
            unsigned char c3 = (unsigned char)str[byteIndex+2];
            unsigned char c4 = (unsigned char)str[byteIndex+3];

            return (c1 & 0b00000111) * 262144 + (c2 & 0b00111111) * 4096 
            + (c3 & 0b00111111) * 64 + (c4 & 0b00111111);
        }
            
        default:
            return -1;
    }
}


// Takes a UTF-8 encoded string and an codepoint index, and returns if the code point at that index is an animal emoji.
// @param: UTF-8 string, codePointIndex
// @return: 1 : TRUE, 0 : FALSE
char is_animal_emoji_at(char str[], int32_t cpi)
{
    // Let's say, codePoint somehow able to extract binary from UTF-8,
    // then this gonna work, hypothetically.
    int codePoint = codepoint_at(str, cpi);
    
    // NESTED LIST OF EMOJI BASE ON WIKI PAGE
    // Comparing the exact value to hex value (which will automatically converted)
    if ((codePoint >= 0x1F400 && codePoint <= 0x1F43F) || (codePoint >= 0x1F980 && codePoint <= 0x1F99E))
    {
        return 1;
    }

    return 0;
}


// This function will takes in a UTF-8 string,
// and return the bytes length.
// @param: UTF-8 string
// @return: int byte-length.
int utf8_bytelen(char str[])
{
    int i = 0;
    for (i; str[i] != '\0';)
    {
        int width = width_from_start_bytes(str[i]);
        i += width;
    }

    return i;
}


void next_utf8_char(char str[], int32_t cpi, char result[])
{
    int32_t next_char = codepoint_at(str, cpi) + 1; // Current code_point, add 1 to get next one

    result[0] = next_char;
}

int main()
{
    /// UTF-8 Analyzer
    printf("Enter a UTF-8 encoded string: \n");
    char input[50];
    fgets(input,50, stdin);

    // is_ascii()
    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");

   
    //Uppercase
    char capitalInput[strlen(input)];

    int i;
    for (i = 0; input[i] != '\0'; i++)
    {
        capitalInput[i] = input[i];
    }
    capitalInput[i] = '\0';

    capitalize_ascii(capitalInput);
    printf("Uppercased ASCII: \"%s\"\n", capitalInput);

    //utf8_bytelen()
    printf("Length in bytes: %d\n", utf8_bytelen(input));

    //utf_strlen()
    printf("Number of code points: %d\n", utf8_strlen(input));

    //width_from_start_bytes()
    printf("Bytes per code points: ");
    for (int i = 0; input[i] != '\0';)
    {
        int width = width_from_start_bytes(input[i]);
        printf("%d ", width);
        i += width;
    }
    printf("\n");

    //utf8_substring()
    char subString[strlen(input)];

    for (i = 0; i != '\0'; i++)
    {
        subString[i] = input[i];
    }
    subString[i] = '\0';

    utf8_substring(input, 0, 6, subString);
    printf("Substring of the first %d code points: \"%s\" \n", 6, subString);

    //codepoint_at()
    printf("Code points as decimal numbers: ");
    for (int i = 0; i < utf8_strlen(input); i++ )
    {
        int width = width_from_start_bytes(input[i]);
        int code = codepoint_at(input, i);
        printf("%d ", code);
        //i += width;
    }
    printf("\n");


    //is_animal_emoji_at()
    for (int i = 0; i < utf8_strlen(input); )
    {        
        int width = width_from_start_bytes(input[i]);
        int is_animal = is_animal_emoji_at(input, i);

        if (is_animal == 1)
        {
            printf("Animal emojis: ");

            // This will print out all BYTES of the emoji, 
            // where as they have 4 bytes.
            // Therefore we need a loop to iterate through
            // all of the bytes.
            for (int j = 0; j < width; j++)
            {
                printf("%c", input[i+j]);
            }
            printf("\n");
        }

        i += width;
    }



    char str[] = "Joséph";
    char result[100];
    int32_t idx = 3;
    next_utf8_char(str, idx, result);
    printf("Next Character of Codepoint at Index 3: %s\n", result);

    return 0;
}

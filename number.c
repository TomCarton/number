// number.c
//
// written by Thomas CARTON on 06/06/2017
//

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


// pathLastComponentName: return the last component of a given path
static char *pathLastComponentName(const char *path)
{
    char *lname = strrchr(path, '/');
    lname = lname == NULL ? (char *)path : lname + 1;

    char *name = malloc(strlen(lname) + 1);
    strcpy(name, lname);

    char *rname = strrchr(name, '.');
    if (rname)
    {
        *rname = '\0';
    }

    return name;
}

const char *b2a(int val)
{
    static char binaryString[] = "0b................................";

    unsigned int mask = 0x80000000;
    for (unsigned int i = 0; i < 32; ++i)
    {
        binaryString[i + 2] = val & mask ? '1' : '0';
        mask >>= 1;
    }

    return binaryString;
}

const unsigned int bitcount(int val)
{
    unsigned int bc = *((unsigned int *)&val);

    bc = ((bc & 0xAAAAAAAA) >> 1) + (bc & 0x55555555);
    bc = ((bc & 0xCCCCCCCC) >> 2) + (bc & 0x33333333);
    bc = ((bc & 0xF0F0F0F0) >> 4) + (bc & 0x0F0F0F0F);
    bc = ((bc & 0xFF00FF00) >> 8) + (bc & 0x00FF00FF);
    bc = ((bc & 0xFFFF0000) >> 16) + (bc & 0x0000FFFF);

    return bc;
}

const unsigned int bitsneeded(int val)
{
    if (val < 0) return 32;

    unsigned int bn = 1;

    if (val & 0xFFFF0000) val >>= 16, bn += 16;
    if (val & 0xFF00) val >>= 8, bn += 8;
    if (val & 0xF0) val >>= 4, bn += 4;
    if (val & 0xC) val >>= 2, bn += 2;
    if (val & 0x2) bn += 1;

    return bn;
}

// get the number from a string (binary, octal, decimal, hexadecimal)
static bool getNumber(const char *str, int *number)
{
    unsigned int p = 0;
    unsigned long slen = strlen(str);

    bool negative = false;
    int n = 0;

    if (str[p] == '+' || str[p] == '-')
    {
        negative = (str[p++] == '-');
    }

    if (str[p] == '0')
    {
        // binary
        if (str[p + 1] == 'b')
        {
            for (unsigned int i = p + 2; i < slen; ++i)
            {
                char c = str[i];

                if (c == '0' || c == '1')
                {
                    n = (n << 1) + c - '0';
                    continue;
                }

                return false;
            }

            goto end;
        }
        // octal
        else if (str[p + 1] == 'o')
        {
            for (unsigned int i = p + 2; i < slen; ++i)
            {
                char c = str[i];

                if (c >= '0' && c <= '7') continue;

                return false;
            }

            sscanf(&str[p + 2], "%o", &n);
            goto end;
        }
        // hexadecimal
        else if (str[p + 1] == 'x')
        {
            for (unsigned int i = p + 2; i < slen; ++i)
            {
                char c = str[i];

                if (c >= '0' && c <= '9') continue;
                if (c >= 'A' && c <= 'F') continue;

                return false;
            }

            sscanf(&str[p + 2], "%x", &n);
            goto end;
        }
        // decimal
        else if (str[p + 1] == 'd')
        {
            p += 2;
        }
    }

    // default to decimal
    for (unsigned int i = p; i < slen; ++i)
    {
        char c = str[i];

        if (c >= '0' && c <= '9') continue;

        return false;
    }

    sscanf(&str[p], "%d", &n);

end:
    *number = negative ? -n : n;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        goto usage;
    }

    printf("\n");

    // read parameters
    for (unsigned int i = 1; i < argc; ++i)
    {
        // help
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            goto usage;
        }
        // number
        else
        {
            int number;
            bool result = getNumber(argv[i], &number);
            if (result)
            {
                unsigned int bc = bitcount(number);
                unsigned int bn = bitsneeded(number);
                printf("> '%s' = %d, 0x%X, %s - %d/%d\n", argv[i], number, number, b2a(number), bc, bn);
            }
            else
            {
                printf("! unrecognized number (%s)\n", argv[i]);
            }
        }
    }

    printf("\n");
    return 0;

usage:

    fprintf(stderr, "\n");
    char *command = pathLastComponentName(argv[0]);

    fprintf(stderr, "   usage:\n");
    fprintf(stderr, "     %s [-h/--help] | number [number, ...]\n\n", command);

    fprintf(stderr, "   parameters:\n");
    fprintf(stderr, "     -h, --help     - display this\n");
    fprintf(stderr, "     number         - number to display details about\n\n");
    
    fprintf(stderr, "   example:\n");
    fprintf(stderr, "     ./%s 2323 0x1234\n", command);
    
    fprintf(stderr, "\n");
    
    free(command);
    
    return 0;
}

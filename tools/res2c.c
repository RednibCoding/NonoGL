#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//
// Converts a resource e.g. a png image to a resource_name.c and resource_name.h so the resource can be included into the binary
//
// To build it run:
//     gcc res2c.c -o res2c.exe -std=c99 -O3 -march=native -funroll-loops -flto -fomit-frame-pointer
//

// Function to replace '.' with '_' in filenames for valid C identifiers
void sanitizeFilename(char *filename)
{
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '.')
            filename[i] = '_';
    }
}

// Function to convert the arrayName to uppercase for the header guard
void toUpperCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Function to convert a file to a C array and generate corresponding header file
void convertFileToCArray(const char *inputFilePath)
{
    char outputFilePath[256], headerFilePath[256], arrayName[256];
    strncpy(outputFilePath, inputFilePath, sizeof(outputFilePath) - 2);
    strncpy(headerFilePath, inputFilePath, sizeof(headerFilePath) - 2);
    strncpy(arrayName, inputFilePath, sizeof(arrayName) - 1);
    sanitizeFilename(outputFilePath);
    sanitizeFilename(headerFilePath);
    sanitizeFilename(arrayName);

    strcat(outputFilePath, ".c");
    strcat(headerFilePath, ".h");

    FILE *inputFile = fopen(inputFilePath, "rb");
    FILE *outputFile = fopen(outputFilePath, "w");
    FILE *headerFile = fopen(headerFilePath, "w");
    unsigned char buffer[1024];
    size_t bytesRead;
    int firstLine = 1;
    unsigned int totalBytes = 0;

    if (inputFile == NULL || outputFile == NULL || headerFile == NULL)
    {
        fprintf(stderr, "Error opening files.\n");
        exit(1);
    }

    char arrayNameUpper[256];
    strncpy(arrayNameUpper, arrayName, sizeof(arrayNameUpper) - 1);
    toUpperCase(arrayNameUpper); // Convert arrayName to uppercase for header guard

    // Header file content
    fprintf(headerFile, "\n// This file is generated from '%s'\n", inputFilePath);
    fprintf(headerFile, "// Include this file: #include \"%s\" \n\n", headerFilePath);

    fprintf(headerFile, "#ifndef %s_H\n#define %s_H\n\n", arrayNameUpper, arrayNameUpper);
    fprintf(headerFile, "extern unsigned int %s_arr_len;\n", arrayName);
    fprintf(headerFile, "extern unsigned char %s_arr[];\n\n", arrayName);
    fprintf(headerFile, "#endif // %s_H\n", arrayNameUpper);

    // Output file content

    // Write a comment at the top of the file
    fprintf(outputFile, "\n// This file is generated from '%s'\n", inputFilePath);
    fprintf(outputFile, "// Include this file in your build: gcc ... %s\n\n", outputFilePath);

    fprintf(outputFile, "#include \"%s\"\n\n", headerFilePath);
    fprintf(outputFile, "unsigned int %s_arr_len;\n", arrayName);
    fprintf(outputFile, "unsigned char %s_arr[] = {\n", arrayName);

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0)
    {
        for (size_t i = 0; i < bytesRead; i++)
        {
            if (!firstLine)
            {
                fprintf(outputFile, ",");
            }
            if (i % 12 == 0)
            { // Formatting: new line after every 12 bytes
                fprintf(outputFile, "\n    ");
            }
            fprintf(outputFile, "0x%02X", buffer[i]);
            firstLine = 0;
            totalBytes++;
        }
    }
    fprintf(outputFile, "\n};\n");
    fprintf(outputFile, "unsigned int %s_arr_len = %u;\n", arrayName, totalBytes);

    fclose(inputFile);
    fclose(outputFile);
    fclose(headerFile);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input.png>\n", argv[0]);
        return 1;
    }

    convertFileToCArray(argv[1]);
    return 0;
}

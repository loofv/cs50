#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;
int BLOCK_SIZE = 512;

// Helper method to make code more readable. Checks for start of a JPEG by looking at the first 4 bytes.
bool is_start(BYTE buffer[])
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
           && (buffer[3] & 0xf0) == 0xe0;
}

int main(int argc, char *argv[])
{
    if (argv[1] == NULL || argv[2] != NULL)
    {
        printf("Usage: ./recover [filename]\n");
        return 1;
    }

    // just use a regular int, ++ it and then use %03d to get it as 3 values.
    // int zero = 99;
    // printf("zero: %03d\n", zero);
    // Use sprintf to convert int to string

    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s. \n", argv[1]);
        return 4;
    }

    // int buffer_size = sizeof(&raw_file);
    int buffer_size = BLOCK_SIZE;
    BYTE buffer[buffer_size];

    int file_count = 0;
    FILE *out_ptr = NULL;
    char outfile[8];

    while (fread(&buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (is_start(buffer))
        {
            if (out_ptr != NULL)
            {
                fclose(out_ptr);
            }
            sprintf(outfile, "%03i.jpg", file_count);
            out_ptr = fopen(outfile, "w");
            file_count++;
        }

        if (out_ptr != NULL)
        {
            fwrite(&buffer, 1, 512, out_ptr);
        }
    }
    fclose(out_ptr);
    fclose(raw_file);
}

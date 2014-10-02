/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "bin2array" program.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>


#define LINE_WIDTH  16

static void print_usage(void);


int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    int ch;
    int i;
    
    if (argc != 3)
    {
        print_usage();
        return 0;
    }
    
    fin = fopen(argv[1], "rb");
    if (fin == NULL)
    {
        printf("Could not open input file %s for reading!\n", argv[1]);
        return 1;
    }
    
    fout = fopen(argv[2], "w");
    if (fout == NULL)
    {
        printf("Could not open output file %s for writing!\n", argv[2]);
        if (fin != NULL) fclose(fin);
        return 2;
    }

    fprintf(fout, "unsigned char LR0[] = {\n\t");
    
    i = LINE_WIDTH; // first run
    while ((ch=fgetc(fin)) != EOF)
    {
        if (i == 0)
            fprintf(fout, ",\n\t");
        else if (i < LINE_WIDTH)
            fprintf(fout, ", ");
        
        i = (i + 1) & (LINE_WIDTH - 1);
        
        fprintf(fout, "0x%02X", (unsigned char)ch);
    }

    fprintf(fout, "\n};\n");
    
    fclose(fin);
    fclose(fout);
    return 0;
}


static void print_usage(void)
{
    printf("Usage: bin2array inputFileName outputFileName\n");
}

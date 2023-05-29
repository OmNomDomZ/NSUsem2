#include "encode.h"
#include "decode.h"
#include "global.h"

FILE *in;
FILE *out;

int main(int argc, char *argv[])
{
    // the direction of the archiver's work
    int option = 0;

    in = stdin;
    out = stdout;

    if (argc == 1) {
        option = getc(in);
        if (option == EOF) {
            return 0;
        }
    }
    else if (argc == 4) {
        option = argv[1][0];
        in = fopen(argv[2], "rb");
        if (in == NULL) {
            return 0;
        }
        out = fopen(argv[3], "wb");
        if (out == NULL) {
            fclose(in);
            return 0;
        }
    }

    switch (option)
    {
    case 'c':
        encode();
        break;
    case 'd':
        decode();
        break;
    default:
        return 0;
    }

    fclose(in);
    fclose(out);
    return 0;
}

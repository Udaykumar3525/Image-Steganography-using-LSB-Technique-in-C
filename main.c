#include <stdio.h>              //printf, scanf
#include <string.h>             // strcmp
#include "encode.h"            //Encoding functions
#include "decode.h"           // Decoding Functions
#include "types.h"            //enums & types
#include "common.h"

//Checks whether the user selected encoding (-e) or decoding (-d).
OperationType check_operation_type(char *argv[]){
    if (strcmp(argv[1],"-e") == 0)    //strcmp return 0 if equal , Non-zero if not equal
    return e_encode;

    else if (strcmp(argv[1],"-d") == 0)
    return e_decode;

    else return e_unsupported; 
}

int main(int argc,char *argv[])       //Entry point of the program
{
    EncodeInfo encInfo;           //Structure to store all encoding-related info
    DecodeInfo decInfo;          //Structure to store all decoding-related info

    if (argc < 2)
    {
        printf(RED"ERROR: No operation provided!\n"RESET);
        return e_failure;
    }

    int res = check_operation_type(argv); 
    if (res == e_encode){           //Encode operation
        printf(GREEN "--------> You Selected Encoding Operation:🔐 <--------\n" RESET);
        if (read_and_validate_encode_args(argc,argv,&encInfo) == e_success)
        {
            printf("Reading and validation of Arguments done Successfully ✅\n");
            if (do_encoding(&encInfo) == e_success)
                printf(GREEN "Encoding Completed Successfully 🔐 ✔️\n" RESET);
            else
                printf(RED "ERROR: Encoding Failed!\n" RESET);
        }
        else
            printf(RED "ERROR: Invalid Encoding Arguments!\n" RESET);
    }
    

    else if (res == e_decode)       // Decode operation
    {
        printf(YELLOW "--------> You Selected Decoding Operation:🔓 <--------\n" RESET);

        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
                printf(GREEN "Decoding Completed Successfully 🔓 ✔️\n" RESET);
            else
                printf(RED "ERROR: Decoding Failed!\n" RESET);
        }
        else
           printf(RED "ERROR: Invalid Decoding Arguments!\n" RESET);
    }

    else         // Invalid Operation 
    {
        printf(RED "ERROR: Unsupported operation '%s'\n" RESET, argv[1]);
        printf("Use -e for encoding, -d for decoding\n");
        return e_failure;
    }
    return e_success;

}
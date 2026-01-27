#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"

char str[50];   

Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo){
    //Validate arg count (must be 3 or 4)
    if (!(argc == 3 || argc == 4)) {
        printf(RED "Enter the correct number of arguments !" RESET);
        return e_failure;
    }

    // Validate .bmp file
    if (strstr(argv[2],".bmp") == NULL) {     
        printf(RED "No stego image provided ? \n" RESET);
        return e_failure;
    } 
    decInfo-> stego_image_fname=argv[2];    //destination image
    printf("Stego image set to: %s\n", decInfo->stego_image_fname);
    

    if(argv[3] == NULL){               // if output file not given, use default file name as .txt
        printf("Output File not mentioned. Creating output.txt as default\n");
        decInfo ->output_secret_fname="output.txt";   
        return e_success;
    } 
    if(strstr(argv[3],".txt")!=NULL){    // validate .txt extension
        printf(".txt is present\n");
        decInfo ->output_secret_fname=argv[3];
    }
    else decInfo ->output_secret_fname=argv[3];   // If user provides output file

    return e_success;
    
}

Status do_decoding(DecodeInfo *decInfo)
{
    //write function calls and write e_success and e_failure each time
    printf(MAGENTA "Starting Decoding Process 🚀.....\n" RESET);

    //step1: Open stego image
    printf("Opening required files 📂  : ");
    if (open_stego_file(decInfo) != e_success){
        printf("Failed to open\n");
        return e_failure;
    }
    printf(GREEN "Opened succssfullly \n" RESET);

    // Step 2: Skip BMP header
    printf("Skipping BMP Header (54Bytes)🧾  : ");
    if (skip_bmp_header(decInfo->fptr_stego_image) != e_success){
        printf("Failed to skip\n");
        return e_failure;
    }
    printf(GREEN "Skipped\n" RESET);

    // Step 3: Decode MAGIC STRING
    printf("Decoding Magic String ✨  : ");
    if (decode_magic_string(MAGIC_STRING, decInfo) != e_success){
        printf("decoding failed\n");
        return e_failure;
    }
    printf(GREEN "Decoded\n" RESET);

    // Step 4: Decode file extension
    printf("Decoding secret (.txt) File Extenstion 🏷️  : ");
    if (decode_secret_file_extn(decInfo) != e_success){
        printf("Failed to decode secret file extension!\n");
        return e_failure;
    }
    printf(GREEN "Decoded\n" RESET);

    // Step 5: Decode secret file size
    printf("Decoding secret File Size 📏  : ");
    if (decode_secret_file_size(decInfo) != e_success){
        printf("Failed to decode secret file size!\n");
        return e_failure;
    }
    printf(GREEN "Decoded\n" RESET);

    // Step 6: Decode the actual secret data
    printf("Decoding Actual secret Data from image 💾  : ");
    if (decode_secret_file_data(decInfo) != e_success){
        printf("Failed to decode secret file data!\n");
        return e_failure;
    }
    printf(GREEN "Decoded\n" RESET);

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->output_fptr_secret);
    
    return e_success;
}

Status open_stego_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)   // Validate filename pointer
    {
        printf("Unable to open stego image file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

Status skip_bmp_header(FILE *fptr_stego_image)
{
    //Skip the first 54 bytes (standard BMP header)
    if (fseek(fptr_stego_image, 54, SEEK_SET) != 0){
        printf("Failed to skip BMP header!\n");
        return e_failure;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)    //Build character from 8 LSBs
    {
        char bit = image_buffer[i] & 1;   // extract LSB (0 or 1)
        ch = (ch << 1) | bit;             // shift left and add bit, build byte from MSB -> LSB
    }
    *data = ch;            // store decoded character
    return e_success;
}

Status decode_size_from_lsb(long *size, char *image_buffer)
{
    long value = 0;
    for (int i = 0; i < 32; i++)   //Decode 32 bits (MSB -> LSB)
    {
        long bit = image_buffer[i] & 1;   // extract LSB of each byte
        value = (value << 1) | bit;       // shift left and add the bit
    }

    *size = value;     // return decoded size
    return e_success;
}


//Directly calling this function without calling decode_data_from_image
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char ch;
    char buffer[8];
    char temp[20];
    int len = strlen(magic_string);

    for (int i = 0; i < len; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1){
            printf("Failed to read 8 bytes for magic string decoding!\n");
            return e_failure;
        }

        if (decode_byte_from_lsb(&ch, buffer) != e_success){
            printf("Failed to decode byte from LSB for magic string!\n");
            return e_failure;
        }

        temp[i]=ch;
        if (ch != magic_string[i])
        {
            printf("Magic string mismatch, Not a valid stego image!\n");
            return e_failure;
        }
    }
    temp[len]='\0';
    //printf("Extracted Magic string from secret image is : %s\n", temp);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char ch;
    char buffer[8];
    int i;

    for (i = 0; i < MAX_FILE_SUFFIX; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1){           //Read 8 bytes that encode 1 character
            printf("Failed to read 8 bytes for extension decoding!\n");
            return e_failure;
        }

        if (decode_byte_from_lsb(&ch, buffer) != e_success){
            printf("Failed to decode byte for extension!\n");
            return e_failure;
        }
        decInfo->extn_secret_file[i] = ch;
    }
    decInfo->extn_secret_file[i] = '\0';   // NULL terminate string

    //printf("Final Decoded extension: %s\n", decInfo->extn_secret_file);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    long size = 0;

    // Read 32 bytes from stego image (these contain the file size bits)
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1){
        printf("Failed to read 32 bytes for secret file size!\n");
        return e_failure;
    }

    //Decode 32 bits from these 32 bytes -> reconstruct the original long file size
    if (decode_size_from_lsb(&size, buffer) != e_success){
        printf("Failed to decode secret file size from LSB!\n");
        return e_failure;
    }

    decInfo->size_secret_file = size;     // Store decoded size into structure
    //printf("Decoded Secret File Size: %ld bytes\n", size);

    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //Open output file to write decoded secret data
    decInfo->output_fptr_secret = fopen(decInfo->output_secret_fname, "w");
    if (decInfo->output_fptr_secret == NULL){
        printf("Unable to open output file\n");
        return e_failure;
    }

    char buffer[8];     // holds 8 bytes -> contains 1 hidden characte
    char ch;

    for (int i = 0; i < decInfo->size_secret_file; i++)    //Loop through all characters of secret file
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);  //Read 8 bytes from stego image
        decode_byte_from_lsb(&ch, buffer);              // Decode 1 character from those 8 bytes
        fputc(ch, decInfo->output_fptr_secret);         // Write decoded character to output file
    }
    return e_success;
}
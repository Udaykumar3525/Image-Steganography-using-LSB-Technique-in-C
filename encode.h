//This is a Encode opeartion , functions declaration file

#ifndef ENCODE_H
#define ENCODE_H

//Why Because: Status , OperationType ,uint are defined in types.h
#include "types.h"        // Contains user defined types
#include <string.h>

/* Why 1 - We encode 1 character at a time , 1 char = 1byte = 8 bits
   Why multiple of 8 - Each bit of secret data goes into one image byte ,So 1 secret byte - 8 image bytes
   Why 4 - Enough for .txt, .c, .h, etc. --> Includes dot .
*/
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4     

/* 
   Structure to store information required for encoding secret file to source Image
   Info about output and intermediate data is also stored 
*/
typedef struct _EncodeInfo
{
    // Source Image info 
    char *src_image_fname;     //File name of input image
    FILE *fptr_src_image;      //File pointer to image
    uint image_capacity;      //Image capacity - how many bytes available for hiding
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];   //Temporary buffer to read image data


    // Secret File Info 
    char *secret_fname;       //Name of secret file
    FILE *fptr_secret;       //File pointer to secret file
    //for finding the extension size -  .txt,.h,.csv anything so dont use strstr , use strchar - pass . then it will give address of . , the copy that to extn_secret_file array
    //strchr(filename, '.') gives address of .  --> Copy from there into extn_secret_file
    char extn_secret_file[MAX_FILE_SUFFIX];   
    char secret_data[MAX_SECRET_BUF_SIZE];    //Temporary buffer Holds 1 character at a time,to do operation on secret data
    long size_secret_file;                  //Stores total secret file size Needed during decoding
    // decleare variable for extension size or else use strlen() everywhere


    // Stego Image Info  - destination image
    char *stego_image_fname;
    FILE *fptr_stego_image;        //File pointer to encoded image

} EncodeInfo;


// Encoding functions prototype 

OperationType check_operation_type(char *argv[]);  //Check operation type

Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo);  // Read and validate Encode args from argv

Status do_encoding(EncodeInfo *encInfo);            //last function call after all validations in main - Perform the encoding  

Status open_files(EncodeInfo *encInfo);               // Get File pointers for i/p and o/p files

Status check_capacity(EncodeInfo *encInfo);          // check capacity  

uint get_image_size_for_bmp(FILE *fptr_image);      // Get image size

uint get_file_size(FILE *fptr);                   // Get file size 

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);          // Copy bmp image header

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);   // Store Magic String 

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);  // Encode secret file extenstion  

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);        // Encode secret file size

Status encode_secret_file_data(EncodeInfo *encInfo);                       // Encode secret file data

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);     // Encode function, which does the real encoding  

Status encode_byte_to_lsb(char data, char *image_buffer);     // Encode a byte into LSB of image data array

Status encode_size_to_lsb(long size, char *image_buffer);      // Encode a 32-bit integer (file size or extension size) into 32 image bytes

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);   // Copy remaining image bytes from src to stego image after encoding 


#endif
//This is a declaration file of Decode operation

#ifndef DECODE_H
#define DECODE_H

#include "types.h"        

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    // output File Info  -> output.txt 
    char *output_secret_fname;      
    FILE *output_fptr_secret;       
    char extn_secret_file[MAX_FILE_SUFFIX];   
    char secret_data[MAX_SECRET_BUF_SIZE];    
    long size_secret_file;           

    // Stego Image Info  - destination image
    char *stego_image_fname;
    FILE *fptr_stego_image;        //File pointer to decode image

} DecodeInfo;


// Decoding functions prototype 

OperationType check_operation_type(char *argv[]);  //Check operation type
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);  
Status do_decoding(DecodeInfo *decInfo);           

Status open_stego_file(DecodeInfo *decInfo);      
Status skip_bmp_header(FILE *fptr_stego_imagee);          

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);   
Status decode_secret_file_extn(DecodeInfo *decInfo);   

Status decode_secret_file_size(DecodeInfo *decInfo);        
Status decode_secret_file_data(DecodeInfo *decInfo);    

Status decode_data_from_image(const char *data, int size,FILE *fptr_stego_image);     
Status decode_byte_from_lsb(char *data, char *image_buffer);    
Status decode_size_from_lsb(long *data, char *image_buffer);  

#endif
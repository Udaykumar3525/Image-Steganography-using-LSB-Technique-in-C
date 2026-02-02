#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

// -----> Encode Functions Definitions 


//EncodeInfo *encInfo - this structure is used to store the file details
//Validates - Argument count, .bmp input, .txt secret file, Output file name
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo){
    if (argc < 4 || argc > 5){    //Validate argument count
        printf(RED "Invalid argument count for Encoding!\n" RESET);
        printf("Correct Way: %s -e <input.bmp> <secret_file> [output.bmp]\n", argv[0]);
        return e_failure;
    }

    if (strstr(argv[2],".bmp") == NULL) {      //Validate source BMP image (argv[2])
        printf(RED "Source image must be .BMP\n" RESET);
        return e_failure;
    }  
    //if it is false , store the argv[2] into the structure encInfo -> src_image_fname
    else{
        printf(".bmp is present in source file\n");
        encInfo->src_image_fname=argv[2];
    }


    //.txt --> check argv[3] has "." or not (or also check like this -> .csv,.sh,.jpeg,jpg, etc  )
    //is it is true store the file name , encInfo -> secret_fname , otherwise print error msg and exit
    if(strstr(argv[3],".") == NULL){
        printf(RED "Secret file must have an extension (.txt/.c/.jpg/etc)\n" RESET);   // .txt is valid
        return e_failure;
    }
    encInfo->secret_fname=argv[3];
    strcpy(encInfo->extn_secret_file, strchr(encInfo->secret_fname, '.'));

    printf("Secret file set to: %s\n", encInfo->secret_fname);

    //Stego file name (argv[4])
    if(argv[4] == NULL){
        encInfo->stego_image_fname="stego.bmp";       //Default output file
        return e_success;
    }    
    else{
        if (strstr(argv[4], ".bmp") == NULL)
        {
            printf("Output file not .bmp -> Defaulting to stego.bmp\n");
            encInfo->stego_image_fname = "stego.bmp";
        }
        else encInfo->stego_image_fname = argv[4];
        
        printf("Output stego file set to: %s\n", encInfo->stego_image_fname);
        
    }
    return e_success;
}


/*
    Function: do_encoding
    ontrols the entire encoding process step-by-step
    Steps:
    1. Open all required files
    2. Check whether image has enough capacity
    3. Copy BMP header (54 bytes)
    4. Encode MAGIC STRING ("#*")
    5. Encode secret file extension length + extension
    6. Encode secret file size
    7. Encode secret file actual data
    8. Copy remaining BMP data unchanged
*/
Status do_encoding(EncodeInfo *encInfo) 
{       
    printf(MAGENTA "Starting Encoding Process 🚀.....\n" RESET);

    //Step 1: Open 3 files 
    printf("Opening required files 📂 : ");
    if (open_files(encInfo) != e_success){
        printf(RED "not opened\n" RESET);
        return e_failure;
    }
    printf(GREEN "Opened successfullly \n" RESET);

    //Step 2: Check if source image has enough capacity to hide secret file data
    printf("Checking the Image has Enough space to hide secret data or not 🖼️  : ");
    if (check_capacity(encInfo) != e_success){
        printf(RED "No Enough space!\n" RESET);
        return e_failure;
    }
    printf(GREEN "Image has Enough Space to Hide \n" RESET);

    //Step 3: Copy header of .bmp file (first 54 bytes)
    printf("Copying first 54 bytes of Image Header🧾 : ");
    if (copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) != e_success){
        printf(RED "not Copied\n" RESET);
        return e_failure;
    }
    printf(GREEN "Header Copied successfully \n" RESET);

    //Step 4: Encode magic string
    printf("Encoding Magic String ✨ : ");
    if (encode_magic_string(MAGIC_STRING,encInfo) != e_success){
        printf(RED "Failed to Encode\n" RESET);
        return e_failure;
    }
    printf(GREEN "Encoded \n" RESET);
        
    //step5 : Encode secret file extension
    printf("Encoding secret (.txt) File Extenstion 🏷️  : ");
    if(encode_secret_file_extn(strchr(encInfo->secret_fname,'.'),encInfo) != e_success){
        printf(RED "Failed to Encode\n" RESET);
        return e_failure;
    }
    printf(GREEN "Encoded \n" RESET);
    
    //step6 : Encode secret file size into image 
    printf("Encoding secret File Size 📏 : ");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) != e_success){
        printf(RED "Failed to Encode\n" RESET);
        return e_failure;
    }
    printf(GREEN "Encoded\n" RESET);

    //step7 : Encode secrete File data
    printf("Encoding secret File Data 💾 : ");
    if (encode_secret_file_data(encInfo) != e_success){
        printf(RED "Failed to Encode!\n" RESET);
        return e_failure;
    }
    printf(GREEN "Encoded \n" RESET);

    //Step8 : Copy remaining image 
    printf("Copying Left Over Data 📥 : ");
    if (copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) !=  e_success){
        printf(RED "Error in Copying!" RESET);
        return e_failure;
    }
    printf(GREEN "Copied \n" RESET);

    //closing both source and destination files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_stego_image);

    //printf("Copied Remaining imgage Data Successfully 📥\n");

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Opnes Source Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

    if (encInfo->fptr_src_image == NULL)  // Do Error handling
    {
    	printf(RED "ERROR: Unable to open Source file %s\n" RESET, encInfo->src_image_fname);
    	return e_failure;
    }

    // Open Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    
    if (encInfo->fptr_secret == NULL)   // Do Error handling
    {
    	fprintf(stderr, "ERROR: Unable to open Secret file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    
    if (encInfo->fptr_stego_image == NULL)   // Do Error handling
    {
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // 3 files opened successfully - so return e_success
    return e_success;   
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;          //Width stored at offset 18, Height stored right after width
    
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);   //for width , 4bytes , 18+4 =22, so width is b/w 18 - 22

    fread(&width, sizeof(int), 1, fptr_image);     //Read the width (an int)

    fread(&height,sizeof(int), 1, fptr_image);   //Read the height (an int)
    
    // printf("Width              : %u pixels\n", width);
    // printf("Height             : %u pixels\n", height);

    // Return image capacity
    return width * height * 3;   //converting pixel to bits , why 3 means 1pixel=3bytes (BMP uses RGB)
}


uint get_file_size(FILE *fptr)
{
    // use fseek - to move file pointer to end 
    // and after use ftell to find the position of file pointer and then return that

    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/*
    size of src image > size of secret msg + magic string size 
    if 2 characters then size of magic string is --> 2* 8 bits = 16 bytes of data is needed for magic string
    magic string 2 char + size of extension 4 (int) + extension 2 char + secret file data size 4 int + size of secret file 16char
    (2+4+2+4+16) total 28 --> 28 * 8 = 224 bits --> so 224 bytes needed (each byte can store 1 bit only )
*/
Status check_capacity(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_src_image);             // Move pointer to start before checking size 
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    uint required_bytes = 0;   // stores total image bytes needed for encoding 

    //Encoding the magic string --> #* -> 2 chars -> 2 byte = 2 * 8(bits) = 16bytes
    //magic string size + extension size (int) - sizeof(int) * 8+ extension characters  + secret file size + actual data size in secret file
    //Actual layout -> magic string size + extension size (int) - sizeof(int) * 8  + extension characters  + secret file size + actual data size in secret file
    //IN our Project - magic "#*" + extension ".txt" + file size (32 bits) + data, 
    // No extension size stored, in My project it is fixed length , #define MAX_FILE_SUFFIX 4, so extension ALWAYS 4 bytes, just read 4 chars directly, No need for size
    required_bytes =  strlen(MAGIC_STRING) * 8 + strlen(encInfo->extn_secret_file) * 8 +
                      sizeof(long) * 8 + encInfo->size_secret_file * 8;

    printf("\nTotal required bytes       : %u bytes\n", required_bytes);
    printf("Total available bytes      : %u bytes\n", encInfo->image_capacity);

    if (encInfo->image_capacity >= required_bytes)     //Final capacity comparison
        return e_success;
    
    else{
        printf(RED "Image does NOT have enough capacity!\n" RESET);
        printf("Required: %u bytes\n", required_bytes);
        printf("Available: %u bytes\n\n", encInfo->image_capacity);
        return e_failure;
    }
}

//Reads the first 54 bytes from source BMP - Writes them into the stego BMP
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        before this move file pointer to starting position, 
        Because in check capacity we moved fptr to end of file using fseek, so we are getting back it to start to read 54 bytes
    */
    rewind(fptr_src_image);
    char buffer[54];

    // Read 54-byte to buffer from header of source image
    if (fread(buffer, 54, 1, fptr_src_image) != 1){
        printf(RED "Failed to read BMP header from source image!\n" RESET);
        return e_failure;
    }

    // Write 54-byte from buffer to destination
    if (fwrite(buffer, 54, 1, fptr_dest_image) != 1){
        printf(RED "Failed to write BMP header to stego image!\n" RESET);
        return e_failure;
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        Runs loop for 8 times 
            step1: Get the bit from data (MSB --> LSB) (7 --> 6 --> 5 like this)
            step2: clear the LSB bit of arr[i] , (each time one one bit get cleared)
            step3: set the getted bit into the arr[i]
        return e_success
    */

    int i;          // bit position from size
    int bit;        // stores extracted bit (0 or 1)

    for (i = 7; i >= 0; i--)    // Loop runs for 8 bits of one character
    {
        bit = (data >> i) & 1;   //Extract one bit from data (MSB to LSB)

        image_buffer[7 - i] &= 0xFE;     //Clear LSB of image byte

        image_buffer[7 - i] |= bit;     //Store extracted bit in LSB
    }

    return e_success;
}

Status encode_size_to_lsb(long size, char *image_buffer)
{
    /*
        Runs loop for 32 times 
            step1: Get the bit from data (MSB --> LSB) (31st --> 0th like this)
            step2: clear the LSB bit of arr[i] , (each time one one bit get cleared)
            step3: set the getted bit into the arr[i]
        return e_success
    */

    int i;          // to select bit position from size
    int bit;        // to store extracted bit (0 or 1)

    for (i = 31; i >= 0; i--)   // Loop runs for 32 bits of integer
    {
        //Take one bit from size (MSB to LSB) - Shifts required bit to last position, & 1 keeps only that bit
        bit = (size >> i) & 1;    
        //Clear the LSB of image byte - 0xFE (11111110) Makes last bit 0 Other bits stay same
        image_buffer[31 - i] &= 0xFE;  
        //Store the extracted bit in LSB , if bit = 1 --> LSB becomes 1 or If bit = 0 --> LSB remains 0
        image_buffer[31 - i] |= bit;   
    }

    return e_success;    // Encoding completed successfully
}


//why this is coommon function
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /*
       Magic stirng = #*
       for (size times)   --> runs 2 times
            step1: read 8bytes from fptr_src_image & store that data into buffer
            step2 : Function call - encode_byte_to_lsb(data[i],buffer) , passing each character of magic string , so data[i]
            (8 bytes - # encoded into that)
            step3 : store that 8bytes of buffer into destination 
             
            after this * get done
    */

    char buffer[8];  //Holds 8 bytes from source image
    for(int i=0;i<size;i++)
    {
        // Read 8 bytes from original image
        if (fread(buffer, 8, 1, fptr_src_image) != 1){          
            printf(RED "Failed to read 8 bytes from source image" RESET);
            return e_failure;
        }

        // Encode ONE byte of secret into 8 bytes of image
        if (encode_byte_to_lsb(data[i], (char *)buffer) != e_success){
            printf(RED "LSB encoding failed" RESET);
            return e_failure;
        }
        
        // Write modified bytes to stego image
        if (fwrite(buffer, 8, 1, fptr_stego_image) != 1){      
            printf(RED "Failed to write encoded bytes to stego image at index %d\n"RESET, i);
            return e_failure;
        }
    }
    return e_success;

}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //extension size , secret data size --> integer we are going to encode, means instead of 8bit we are encoding 32 bits

    if (encode_data_to_image((char*)magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo ->fptr_stego_image) == e_success){
        // printf("\nMagic string        : %s\n", magic_string);
        // printf("Characters count    : %lu\n", strlen(magic_string));
        // printf("Bits required       : %lu bits\n", strlen(magic_string) * 8);
        // printf("Image bytes needed  : %lu bytes\n", strlen(magic_string) * 8);

        return e_success;
    }
    else{
        printf(RED "Failed to encode magic string!\n" RESET);
        return e_failure;
    }
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success){
        // printf("Extension           : %s\n", file_extn);
        // printf("Length              : %lu characters\n", strlen(file_extn));
        // printf("Bytes needed        : %lu bytes\n", strlen(file_extn) * 8);

        return e_success;
    }
    printf(RED "Failed to encode secret file extension!\n" RESET);
    return e_failure;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];  //to store the readed 8 bytes
    /*
       operation done for size times 
            step1: read 32bytes from fptr_src_image & store that data into buffer -  fread(buffer,32,1,fptr)
            step2 : Function call - encode_size_to_lsb(data[i],buffer)
            step3 : store that 32bytes of buffer into fptr_stego_image
    */

    //Read 32 bytes from source image
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1){
        printf(RED "Failed to read 32 bytes from source BMP for file size encoding!\n" RESET);
        return e_failure;
    }

    //Encode size into those 32 bytes
    if (encode_size_to_lsb(file_size, (char *)buffer) != e_success){
        printf(RED "Failed to encode secret file size into LSBs!\n" RESET);
        return e_failure;
    }

    //Write these encoded bytes to stego image
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1){
        printf(RED "Failed to write encoded file size to stego image!\n" RESET);
        return e_failure;
    }

    // printf("Secret file size    : %ld bytes\n", file_size);
    // printf("Bits to encode      : 32 bits\n");
    // printf("Image bytes needed  : 32 bytes\n");

    return e_success;
}


// original data encoding - Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{ 
    /*
        (before this make the file pointer at start and then only read data)
        step1 : Read a secret file size bytes from secret file and store that into char buffer
        step2 : Now encode the data into image so call func  encode_data_to_image
        encode_data_to_image(buffer,encInfo -> size_secret_file,encInfo->fptr_src_image, encInfo->fptr_stego_image)

    */
    char buffer[encInfo->size_secret_file] ;
    FILE *fp = encInfo->fptr_secret;
    if(fp == NULL){
        printf(RED "Secret file pointer is NULL ?\n" RESET);
        return e_failure;
    }

    rewind(encInfo->fptr_secret);  // Move secret file pointer to beginning
    //reading the secret data from secret file and storing it in a temporary buffer
    if (fread(buffer,encInfo->size_secret_file,1,encInfo->fptr_secret) != 1){
        printf(RED "Failed to read secret data!\n" RESET);
        return e_failure;
    }
    if (encode_data_to_image(buffer,encInfo->size_secret_file,encInfo->fptr_src_image, encInfo->fptr_stego_image)!= e_success){  // calling the function to encode the secret data into the image
        printf(RED "encoding secret data to image is failed!\n" RESET);
        return e_failure;
    }

    // printf("Total characters    : %ld\n", encInfo->size_secret_file);
    // printf("Bits required       : %ld bits\n", encInfo->size_secret_file * 8);
    // printf("Image bytes needed  : %ld bytes\n", encInfo->size_secret_file * 8);

    return e_success;
}

//copying remaining bytes of the original BMP --> to the stego BMP.
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer;
    /*
        loop()
            step1: Read 1byte from fptr_src & store it in buffer
            step2 : Write 1byte of buffer into fptr_dest
    */

    while (fread(&buffer, 1, 1, fptr_src) == 1)
        fwrite(&buffer, 1, 1, fptr_dest);
    
    return e_success;
}
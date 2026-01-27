//Avoids redefinition errors,If types.h is included in multiple .c files, compiler includes it only once
#ifndef TYPES_H
#define TYPES_H

//unsigned int is long to write repeatedly , uint improves readability
typedef unsigned int uint;  

//Enums make return values readable ,Instead of returning 0 or -1, we return meaningful words
typedef enum
{
    e_success,
    e_failure
} Status;

//This represents what operation user wants
typedef enum
{
    e_encode,        //Encode - hide data
    e_decode,       //Decode - extract data
    e_unsupported  //Unsupported - wrong argument
} OperationType;

#endif

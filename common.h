#ifndef COMMON_H
#define COMMON_H

#define RESET   "\033[0m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


/* Magic string to identify whether stegged or not 
   A unique identifier , Stored inside image during encoding
   Decoder checks this to confirm: Yes, this image contains hidden data
   Why #*? , Rare combination - Low chance of accidental match*/
#define MAGIC_STRING "#*"

#endif

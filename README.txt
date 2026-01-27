                      ------------> LSB IMAGE STEGANOGRAPHY PROJECT <---------------

Name : Udaykumar Upputuri
Mail : udayupputuri3525@gmail.com

---------------------------------------------------------------------------------------------------------------------------

What is This Project?
This project hides secret text inside a BMP image using Least Significant Bit (LSB) technique.
During encoding, each bit of the secret message is stored inside the last bit of each pixel of the image.
During decoding, the program reads those bits and reconstructs the full hidden message.
The image looks the same before and after encoding.

What This Project Does (Features)
1.Hide secret text inside an image - The image looks exactly the same even after hiding the message.
2.Extract the secret text from the encoded image - Only the receiver with the decoding program can retrieve the hidden data.
3.Uses Least Significant Bit (LSB) technique - Only the last bit of each pixel is modified → invisible to the human eye.
4.Ensures message integrity - Magic string (#*) is used to verify correctness.
5.Supports variable-sized secret files - As long as the image has enough capacity.
6.Works without losing image quality - No distortion, no color change, no artifacts.

Why This Project is Useful
1. Secure communication - People can send hidden messages inside normal-looking images.
2. Data protection - Text remains hidden even if someone opens the image.
3. No suspicion - Unlike encryption, steganography does not reveal that a secret exists.
4. Useful in cybersecurity learning - Helps students understand binary data manipulation, bitwise encoding, and file structures.
5. Useful in digital watermarking - This technique is used to embed signatures or copyright marks inside images.

Where This Project Is Used in Real Life
1. Military and Intelligence Agencies - Used to pass hidden information secretly inside media files.
2. Digital Watermarking - Copyright owners embed secret IDs inside photos to prevent theft.
3. Secure Messaging Apps - Some advanced communication tools hide data inside images.
4. Steganography-based Authentication - Used to hide keys or codes inside innocuous image files.
5. Forensic Investigations - Officers hide tracking information inside images sent for analysis.
6. Anti-piracy Systems - embed invisible marks inside images or videos.

-------------------------------------------------------------------------------------------

1. HOW THE PROJECT WORKS

-----> Encoding Operation (Hiding text inside the image)
Command: ./a.out -e input.bmp secret.txt stego.bmp
Encoding Steps (in order):
1️⃣ Open required files
- Read original BMP image
- Read secret text file
- Open new output file for encoded image
2️⃣ Check image capacity - Make sure image has enough bytes to hide the secret text
3️⃣ Copy BMP header (54 bytes) - Keep the original header unchanged
4️⃣ Encode Magic String (#*) - Used as a marker to validate during decoding
5️⃣ Encode secret file extension - Example: .txt
6️⃣ Encode secret file size - Example: 25 bytes
7️⃣ Encode secret file data - Hide text, one character = 8 bits = 8 pixels
8️⃣ Copy remaining image data - Preserve rest of the original BMP
9️⃣ Close all files

---> Decoding Operation (Extracting text from encoded image)
Command: ./a.out -d stego.bmp output.txt
Decoding Steps (in order):

1️⃣ Open Stego image file
2️⃣ Skip BMP header (54 bytes)
3️⃣ Decode Magic String - Confirms the image is valid, If mismatch -> “Not a valid stego image”
4️⃣ Decode secret file extension
5️⃣ Decode secret file size - Know how many bytes to extract
6️⃣ Decode secret file data- Recover the hidden text
7️⃣ Write the decoded text to output file

2. PROJECT FILE STRUCTURE
main.c
encode.c
decode.c
encode.h
decode.h
types.h
common.h

3. FUNCTION EXPLANATION – ENCODE SIDE
- check_operation_type() - Checks whether user selected -e (encode) or -d (decode).
- read_and_validate_encode_args()
Validates:
Input image (.bmp), Secret file (.txt),  Output file name
- open_files()
Opens: Source image, Secret text, Output stego image

- check_capacity() -> Checks if image has enough space to hide secret file.
- copy_bmp_header() -> Copies first 54 bytes of BMP as it is.
- encode_magic_string() -> Encodes magic string #* into image bytes.
- encode_secret_file_extn() -> Encodes the secret file’s extension like .txt.
- encode_secret_file_size() -> Encodes file size (32 bits → 32 bytes).
- encode_byte_to_lsb() -> Encodes 1 character → 8 bits → 8 image bytes.
- encode_secret_file_data() -> Reads full text file and hides it.
- copy_remaining_img_data() -> Copies remaining bytes of the original BMP.

4. FUNCTION EXPLANATION – DECODE SIDE
- read_and_validate_decode_args() -> Validates .bmp file and output filename.
- open_files() -> Opens the stego image.
- skip_bmp_header() -> Skips first 54 bytes.
- decode_magic_string() -> Extracts #* and verifies authenticity.
- decode_secret_file_extn() -> Reads and reconstructs .txt.
- decode_secret_file_size() -> Extracts the 32-bit size of secret data.
- decode_byte_from_lsb() -> Extracts 1 character from 8 bytes.
- decode_secret_file_data() -> Extracts full text and writes to output.

5.INTERNAL LOGIC — TECHNICAL EXPLANATION
LSB (Least Significant Bit) of each pixel byte is replaced with
one bit of secret data. Since only the last bit is modified, the
human eye cannot see any change in the image.

Example:
Original Image Byte: 10011010
Secret Bit:          1
Modified Byte:       10011011

6. HOW TO COMPILE YOUR PROJECT  --> gcc *.c
7. HOW TO RUN THE PROJECT
Encoding
Default stego file: ./a.out -e beautiful.bmp secret.txt
Custom stego file: ./a.out -e beautiful.bmp secret.txt stego.bmp

Decoding
Default output file: ./a.out -d stego.bmp
Custom output: ./a.out -d stego.bmp out.txt

8.PROJECT SUMMARY
This project teaches:
    File handling in C
    Bitwise operations
    Encoding data in image pixels
    Extracting data from image pixels
    Struct usage
    Modular programming
    Command-line argument handling

9.PROJECT OUTPUT EXAMPLE
Encoding Output Example --> 
--------> You Selected Encoding Operation:🔐 <--------
.bmp is present in source file
Secret file set to: secret.txt
Reading and validation of Arguments done Successfully ✅
Starting Encoding Process 🚀.....
Opening required files 📂 : Opened successfullly 
Checking the Image has Enough space to hide secret data or not 🖼️  : 
Total required bytes       : 648 bytes
Total available bytes      : 613417032 bytes
Image has Enough Space to Hide 
Copying first 54 bytes of Image Header🧾 : Header Copied successfully 
Encoding Magic String ✨ : Encoded 
Encoding secret (.txt) File Extenstion 🏷️  :  Encoded 
Encoding secret File Size 📏 : Encoded
Encoding secret File Data 💾 : Encoded 
Copying Left Over Data 📥 : Copied 
Encoding Completed Successfully 🔐 ✔️

Decoding Output Example-->
--------> You Selected Decoding Operation:🔓 <--------
Stego image set to: stego.bmp
Output File not mentioned. Creating output.txt as default
Starting Decoding Process 🚀.....
Opening required files 📂  : Opened succssfullly 
Skipping BMP Header (54Bytes)🧾  : Skipped
Decoding Magic String ✨  : Decoded
Decoding secret (.txt) File Extenstion 🏷️  :  Decoded
Decoding secret File Size 📏  : Decoded
Decoding Actual secret Data from image 💾  : Decoded
Decoding Completed Successfully 🔓 ✔️
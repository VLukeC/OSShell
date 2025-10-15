#include "parser.h"

//Function to trim whitespace and ASCII control characters from buffer
//[Input] char* inputbuffer - input string to trim
//[Input] size_t bufferlen - size of input and output string buffers
//[Output] char* outputbuffer - output string after trimming 
//[Return] size_t - size of output string after trimming
size_t trimstring(char* outputbuffer, const char* inputbuffer, size_t bufferlen)
{   
    memcpy(outputbuffer, inputbuffer, bufferlen*sizeof(char));

    for(size_t ii = strlen(outputbuffer)-1; ii >=0; ii--){
        if(outputbuffer[ii] < '!') //In ASCII '!' is the first printable (non-control) character
        {
            outputbuffer[ii] = 0;
        }else{
            break;
        }    
    }

    return strlen(outputbuffer);
}

//Function to trim the input command to just be the first word
//[Input] char* inputbuffer - input string to trim
//[Input] size_t bufferlen - size of input and output string buffers
//[Output] char* outputbuffer - output string after trimming 
//[Return] size_t - size of output string after trimming
size_t firstword(char* outputbuffer, const char* inputbuffer, size_t bufferlen)
{
    memcpy(outputbuffer, inputbuffer, bufferlen * sizeof(char));
    for(size_t ii = 0; ii < bufferlen; ii++){
        for(size_t ii = 0; ii < bufferlen; ii++) {
        char c = outputbuffer[ii];

        
        if (c == ' ' || c == '\t' || c == '\0' || c < '!') {
            outputbuffer[ii] = '\0';
            return ii;              
        }
    }
    }
    outputbuffer[bufferlen - 1] = '\0';
    return bufferlen - 1;


}

//Function to test that string only contains valid ascii characters (non-control and not extended)
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] bool - true if no invalid ASCII characters present
bool isvalidascii(const char* inputbuffer, size_t bufferlen)
{
    if (inputbuffer == NULL || bufferlen == 0)
        return false;

    
    size_t stringlength = strlen(inputbuffer);
    size_t testlen = (stringlength < bufferlen) ? stringlength : bufferlen;

    for (size_t ii = 0; ii < testlen; ii++) {
        unsigned char c = inputbuffer[ii];
        
        if (c < ' ' || c > '~') {
            return false;
        }
    }

    return true;
}

//Function to find location of pipe character in input string
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] int - location in the string of the pipe character, or -1 pipe character not found
int findpipe(const char* inputbuffer, size_t bufferlen){
    for(int index = 0; index < bufferlen; index++){
        if(inputbuffer[index] == '|'){
            return index;
        }
    }
    return -1;
}


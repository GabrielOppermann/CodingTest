#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

int leading_zeros(const char* digest) {
  int count = 0;
  for (int i = 0; i < 40; i++) {
    if (!isxdigit(digest[i]))
      return -1;
    else if (digest[i] != '0')
      return count;
    else if (digest[i] == '0')
      count++;
  }
  return count;
}

bool file_to_SHA1_digest(const char* filename, char* digest) {
  char text[100000];
  int i = 0;
  char ch; 
  ifstream in(filename);
  if (in.fail()) {
    strcpy(digest, "error"); 
    return false;
  }
  in.get(ch);
  while(!in.eof()) {
    text[i] = ch;
    in.get(ch);
    i++;
  }
  in.close();
  text[i] = '\0';
  text_to_SHA1_digest(text, digest);
  digest[40] = '\0';
  return true;
}

bool make_header(const char* recipient, const char* filename, char*header) {
  int count = 1, index = 0;
  char text_digest[41], header_digest[41], number[9];
  file_to_SHA1_digest(filename, text_digest);
  strcpy(header, recipient);
  header[strlen(recipient)] = ':';
  index = strlen(recipient) + 1;
  strcpy(header + index, text_digest);
  index += 40;
  header[index] = ':';
  index++;

  do {
    if (count > 10000000)
      return false;
    sprintf(number, "%d", count);
    strcpy(header + index, number);
    text_to_SHA1_digest(header, header_digest);
    count++;
  } while (leading_zeros(header_digest) != 5);

  return true;
}

MessageStatus check_header(const char* email_address, const char* header, const char* filename) {
  int i = 0, index_colon1 = -1, index_colon2 = -1;
  while (header[i] != '\0') {
    if (index_colon1 == -1 && header[i] == ':')
      index_colon1 = i;
    else if (index_colon2 == -1 && header[i] == ':')
      index_colon2 = i;
    else if (header[i] == ':')
      return INVALID_HEADER;
    i++;
  }
  if (index_colon2 == -1 || index_colon2 == -1 || !isdigit(header[index_colon2+1]))
     return INVALID_HEADER;

  char copy_header[100], digest[41], header_digest[41];
  strcpy(copy_header, header);
  copy_header[index_colon1] = '\0';
  copy_header[index_colon2] = '\0';

  if (strcmp(email_address, copy_header))
    return WRONG_RECIPIENT;

  file_to_SHA1_digest(filename, digest);
  cout << digest << " " << copy_header + index_colon1 + 1 << endl;
  if (strcmp(digest, copy_header + index_colon1 + 1))
    return INVALID_MESSAGE_DIGEST;

  text_to_SHA1_digest(header, header_digest); 
  if (leading_zeros(header_digest) != 5)
    return INVALID_HEADER_DIGEST;  

  return VALID_EMAIL;
}
#pragma once

#include <stdint.h>
#include "aes.h" 
  
#define AES_CMAC_KEY_LENGTH     16
#define AES_CMAC_DIGEST_LENGTH  16
 
struct AES_CMAC_CTX 
{
  aes_context    rijndael;
  uint8_t        X[16];
  uint8_t        M_last[16];
  uint32_t       M_n;
};


void AES_CMAC_Init (struct AES_CMAC_CTX * ctx);
void AES_CMAC_SetKey (struct AES_CMAC_CTX * ctx, const uint8_t key[AES_CMAC_KEY_LENGTH]);
void AES_CMAC_Update (struct AES_CMAC_CTX * ctx, const uint8_t * data, uint32_t len);
void AES_CMAC_Final (uint8_t digest[AES_CMAC_DIGEST_LENGTH], struct AES_CMAC_CTX  * ctx);

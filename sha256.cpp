#include "sha256.h"
#include <cstdlib>
/**
* Inicializáljuk a használt konstansok értékeket:
* (első 32 bitje, az első 64 prím köbgyökének):
*/
const uint32_t K[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                     0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                     0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                     0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                     0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                     0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};


endian sha256::e = endian_check();

sha256::sha256(const String& _arg): digest(), arg(_arg){
      size_t arg_length = arg.getLength();
      size_t x = 0;
      bool eightzero = true;
      bool failed = false;
      /**
       * Inicializáljuk a hash értékeket:
       * (első 32 bitje, az első 8 prím négyzetgyökének):
       */
      uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                       0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
      /**
       * Pre-processzálás
       */
      for(size_t j = 0; j < (arg_length+8)/64 + 1; ++j){
        Vector<uint8_t> wa(64);
        Vector<uint32_t> wd(64);
        Vector<uint32_t> wv(8); //0=a, 1=b, 2=c, 3=d, 4=e, 5=f, 6=g, 7=h
        for(size_t i = 0; i < 8; ++i){
            wv[i] = h[i];
        }
        
         for(Vector<uint8_t>::iterator iter = wa.begin(); iter != wa.end(); ++iter){
          try{
            (*iter) = arg[x];
            x++;
          }
          catch(std::out_of_range const&){
            failed = true;
            break;
          }
        }
        size_t i = (eightzero)? (x % 64):0; 
        if(failed){
          if(eightzero) wa[i++] = 0x80;
          if(i <= 56){
            (*(uint64_t*)(&wa[56])) = arg_length*8;  
            if(e == little) reverse_byte_order(&wa[56], sizeof(uint64_t));
          }    
          while(i != 56 && i!= 64){
            wa[i++] = 0x00;
          }
          eightzero = false;
        }
        /**
         * 64db 8 bites adat feldolgozása mint 16db 32 bites adat
         */
        for(size_t i = 0; i < 16; ++i){
          if(e == little) reverse_byte_order(&wa[4*i], sizeof(uint32_t));
          wd[i] = *(uint32_t*)&wa[4*i];
        }
        /**
         * Hashelés folyamata
         */
        dir d = right;
        for(size_t i = 16; i < 64; ++i){
          uint32_t ro0 = (rotate(wd[i-15], sizeof(uint32_t), 7,d) ^ rotate(wd[i-15], sizeof(uint32_t), 18,d)) ^ (wd[i-15] >> 3);
          uint32_t ro1 = (rotate(wd[i-2], sizeof(uint32_t), 17,d) ^ rotate(wd[i-2], sizeof(uint32_t), 19,d)) ^ (wd[i-2] >> 10);
          wd[i] = ro0 + ro1 + wd[i-16] + wd[i-7];
        }
        for(size_t i = 0; i < 64; ++i){
          uint32_t sum0 = (rotate(wv[0], sizeof(uint32_t), 2,d) ^ rotate(wv[0], sizeof(uint32_t), 13,d)) ^ rotate(wv[0], sizeof(uint32_t), 22,d);
          uint32_t sum1 = (rotate(wv[4], sizeof(uint32_t), 6,d) ^ rotate(wv[4], sizeof(uint32_t), 11,d)) ^ rotate(wv[4], sizeof(uint32_t), 25,d);
          uint32_t choice = (wv[4] & wv[5]) ^ ((~wv[4]) & wv[6]);
          uint32_t majority = (wv[0] & wv[1]) ^ (wv[0] & wv[2]) ^ (wv[2] & wv[1]);
          uint32_t temp1 = wv[7] + sum1  + choice + K[i] + wd[i];
          uint32_t temp2 = sum0 + majority;
          
          for(size_t j = 7; j>0; --j){
            if(j != 4) wv[j] = wv[j-1];  
            else wv[j] = wv[j-1] + temp1;
          }
          wv[0] = temp1 + temp2;
        }
        
        for(size_t i = 0; i < 8; ++i){
            h[i] += wv[i];
        }
      }
      /**
       * Hash értékének kimentése Stringbe
       */
      for(size_t i = 0; i < 8; ++i){
          digest += h[i];
      }
}
void sha256::update(const String& _arg){
  *this = sha256(arg + _arg);
}
String sha256::hexdigest() const{
  return digest;
}


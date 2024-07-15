#include "cipher.h"
#include "string.h"
#include <cstdint>
#include <iostream>
XOR::XOR(const String& key): key(key){}
String XOR::decode(const Vector<uint8_t>& ciphertext) const{
  String res;
  size_t key_len = key.getLength();
  size_t text_len = ciphertext.size();
  char c;
  for(size_t x = 0; x<text_len; ++x){
    c = ciphertext[x]^key[x%key_len];
    res += c;
  }
  return res;
}
Vector<uint8_t> XOR::encode(const String& plaintext)const{
  Vector<uint8_t> res(plaintext.getLength());
  size_t key_len = key.getLength();
  size_t text_len = plaintext.getLength();
  for(size_t x = 0; x<text_len; ++x){
    res[x] = plaintext[x]^key[x%key_len];
  }
  return res;
}

Vigenere::Vigenere(const String& _key){
  if(!_key.isalpha()) throw std::invalid_argument("Csak alfanumerikus kulccsal működik!");
  key = _key;
  key.toUpper();
}
Vector<uint8_t> Vigenere::encode(const String& plaintext) const{
  if(!plaintext.isalpha()) throw std::invalid_argument("Csak alfanumerikus szöveggel működik!");
  String plainUp = plaintext;
  Vector<uint8_t> res(plaintext.getLength());
  size_t key_len = key.getLength();
  size_t text_len = plaintext.getLength();
  plainUp.toUpper();
  for(size_t x = 0; x<text_len; ++x){
    res[x] = 'A' + (plainUp[x]-'A' + key[x%key_len]-'A')%26;
  }
  return res;
}
String Vigenere::decode(const Vector<uint8_t>& ciphertext) const{
  String res;
  size_t key_len = key.getLength();
  size_t text_len = ciphertext.size();
  char c;
  for(size_t x = 0; x<text_len; ++x){
    c = 'A' + (ciphertext[x]-'A' + (26 - (key[x%key_len]-'A')))%26;
    res += c;
  }
  return res;
}
Bifid::Bifid(const String& _key){
  if(!_key.isalpha()) throw std::invalid_argument("Csak alfanumerikus kulccsal működik!");
  String tmp = _key;
  tmp.toUpper();
  String used('J');
  int x = 0;
  int y = 0;
  int len = _key.getLength();
  for(size_t i = 0; i < 5; ++i){
    for(size_t j = 0; j < 5; ++j){
        while(y < len && used == tmp[y]) y++;
        if(y < len){
            key[i][j] = tmp[y]; 
            used += tmp[y];
            y++;
         }
        else{
            while(used == ('A' + x)) x++;
            key[i][j] = 'A' + x; 
            used += 'A' + x;
            x++;
        }
        }
    }
 }
Bifid::Point Bifid::find_it(char c) const{
  for(size_t i = 0; i < 5; ++i){
    for(size_t j = 0; j < 5; ++j){
      if(key[i][j] == c) return Point(j,i);
    }
  } 
  return Point(5,5);
}
Vector<uint8_t> Bifid::encode(const String& plaintext) const{
  if(!plaintext.isalpha()) throw std::invalid_argument("Csak alfanumerikus szöveggel működik!");
  String plainUp = plaintext;
  Vector<uint8_t> res(plaintext.getLength());
  size_t text_len = plaintext.getLength();
  plainUp.toUpper();
  Vector<size_t> idx(text_len*2);
  Point tmp;
  for(size_t i = 0; i < text_len; ++i){
    tmp = find_it(plainUp[i]);
    idx[i] = tmp.y;
    idx[text_len+i] = tmp.x;
  }
  for(size_t i = 0; i < text_len; ++i){
    res[i] = key[idx[i*2]][idx[i*2+1]];
  }
  return res;
}
String Bifid::decode(const Vector<uint8_t>& ciphertext) const{
  String res;
  size_t text_len = ciphertext.size();
  char c;
  Vector<size_t> idx(text_len*2);
  Point tmp;
  for(size_t i = 0; i < text_len; ++i){
    tmp = find_it(ciphertext[i]);
    idx[i*2] = tmp.y;
    idx[i*2+1] = tmp.x;
  }
  for(size_t i = 0; i < text_len; ++i){
    c = key[idx[i]][idx[text_len+i]];
    res += c;
  }
  return res;
}

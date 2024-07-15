#include "string.h"
#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstring>

String::String(){
  data= new char[1];
  data[0] = '\0';
  length = 0;
  pos = 0;
}
String::String(const char *_data){
  length = strlen(_data);
  pos = 0;
  data= new char[length+1];
  strcpy(data,_data);;
}
String::String(const char c){
  length = 1;
  pos= 0;
  data = new char[length+1];
  data[0] = c;
  data[1] = '\0';
}
String::String(const String& rhs){
  length = rhs.length;
  data = new char[length+1];
  pos = rhs.pos;
  strcpy(data,rhs.data); 
}
String::String(const int a){
  length = int(log10(a)) + 1;
  pos = 0;
  data = new char[length+1];
  sprintf(data, "%d", a);
}
String::String(const uint32_t a){
  length = 8;
  pos = 0;
  data = new char[length+1];
  sprintf(data, "%08x", a);
}
String String::operator+(const String& rhs) const{
  String res;
  delete[] res.data;
  res.length = length + rhs.length; 
  res.data = new char[res.length+1];
  strcpy(res.data, data);
  strcat(res.data, rhs.data);
  return res;
}

String& String::operator=(const String& rhs){
  if(&rhs != this){
    delete[] data;
    length = rhs.length;
    data = new char[length+1];
    strcpy(data, rhs.data);
  }
  return *this;
}
bool String::operator&(const String& other) const{
  return strcmp(data, other.data) == 0;
}
bool String::operator==(char c) const{
  return strchr(data, c);
}
String String::substr(const char sep){
 String res;
 size_t i = pos;
 while(i < length && (*this)[i] != sep && (*this)[i] != '\n'){
  res+=(*this)[i++];
 }
 while( i < length && ((*this)[i] == sep || (*this)[i] == '\n')){
  i++;
 }
 pos = i;
 return res;
}
size_t String::getPos() const{
  return pos;
}
bool String::isalpha() const{
  for(size_t i = 0; i < length; ++i){
    if(!std::isalpha(data[i])) return false;
  }
  return true;
}
void String::toUpper(){
  for(size_t i = 0; i < length; ++i){
    if(std::isalpha(data[i])) data[i] = std::toupper(data[i]); 
  }
}
void String::toLower(){
  for(size_t i = 0; i < length; ++i){
    if(std::isalpha(data[i])) data[i] = std::tolower(data[i]); 
  }
}
String::~String(){
  delete[] data;
}
std::ostream& operator<<(std::ostream& os, const String& rhs){
  return (os << rhs.c_string());
}
std::istream& operator>>(std::istream& is, String& rhs){
  char c = '\0';
  String res;
  while(is.get(c) && isspace(c)){}
  res+=c;
  while(is.get(c) && !isspace(c) && c !='\n'){
    res+=c;
  }
  rhs = res;
  return is;
}


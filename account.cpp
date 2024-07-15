#include "account.h"

bool Account::verify(const String& username, const String& password){
  return (name_hash&(sha256(username+salt).hexdigest())) && (pass_hash&(sha256(password+salt).hexdigest())); 
}


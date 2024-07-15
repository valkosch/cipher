#include "string.h"
#include "sha256.h"
/**
 * @file account.h
 * Az Account osztály header fájlja.
 */
/**
 * A fiók adataihoz tartozó műveletekért felelős osztály.
 */
class Account{
  String name_hash,salt, pass_hash; /**< a fiók felhasználónevének + só, jelszavának + só hash értéke.*/
 public:
  /**
   * Konstruktor.
   */
  Account(const String& username = "",const String& salt="", const String& password=""): name_hash(username), salt(salt), pass_hash(password){};
  /**
   * Ellenőrzi, hogy a megadott felhasználónév + só, jelszó + só megegyezik-e a fiókban tároltakkal.
   * @param username felhasználónév.
   * @param password jelszó.
   * @return bool.
   */
  bool verify(const String& username, const String& password);
};


#ifndef CIPHER
#define CIPHER
#include "string.h"
#include "vector.hpp"

/** @file cipher.h
 *  A titkosító osztályok header fájlja.
*/

/**
 * Absztakt osztály amely összeköti a különböző tikosítási osztályokat.
 * A titkosítási módszereknek közös metódusait köti össze örökléssel, de
 * példányosítani nem lehet mert tisztán virtuális függvényeket tartalmaz.
 */
class Cipher{
  public:
 /**
  * Tisztán virtuális függvény amely az enkódolásért felelős.
  * Különböző titkosítási módoknál, különböző enkódolási algoritmusok érvényesülnek.
  * @param plaintext egy String típusú titkosítandó szöveg.
  */
 virtual Vector<uint8_t> encode(const String& plaintext) const = 0;

 /**
  * Tisztán virtuális függvény amely a dekódolásért felelős.
  * Különböző titkosítási módoknál, különböző dekódolási algoritmusok érvényesülnek.
  * @param ciphertext egy Vector<uint8_t> típusú tároló amelynek elemeit dekódolni szeretnénk. Azért Vector<uint8_t> mert a titkosított szöveg legtöbb esetben nem tárolható Stringként, mert nem felel meg a formátuma (pl. NULL érték információt képvisel nem pedig lezáró karaktert).
  */
 virtual String decode(const Vector<uint8_t>& ciphertext) const = 0;
 /**
  * Virtuális destruktor.
  */
 virtual ~Cipher(){};
};

/**
 * XOR tiktosítás.
 * Leszármazott osztály, amely a XOR titkosítást valósítja meg, ahol
 * a kulcs[i] elemét XOR-ozzuk a plaintext[i]/ciphertext[i]  elemével.
 * Ha a kulcs hossza < text hossza akkor a kulcsot ismételjük addig ameddig végig nem ér.
 */
class XOR: public Cipher{
 String key; /**< A titkosításhoz használt String típusú kulcs.*/
  public:
 /**
  * Konstruktor
  */
 XOR(const String&);

 /**
  * Enkódoló függvény, amely plaintextet titkosítja XOR-ral.
  * @return Vector<uint8_t> mivel a XOR szinte mindig elrontja a String formátumát, ezért minden tiktosítandó karaktert uint8_t-ként kezelünk és adunk is tovább titkosítva.
  * @param plaintext titkosítandó String típusú szöveg.
  */
 Vector<uint8_t> encode(const String& plaintext) const;

 /**
  * Dekódoló függvény, amely ciphertext titkosítását oldja föl.
  * @return  String
  * @param ciphertext feloldandó Vector<uint8_t> típusú tároló.
  */
 String decode(const Vector<uint8_t>& ciphertext) const;

 /**
  * Destruktor
  */
 ~XOR(){};
};
/**
 * Vigenere titkosítás.
 * Leszármazott osztály, amely a Vigenére féle titkosítást valosítja meg,
 * melyben több egymás utáni Caesar kódolás történik (ABC shiftelése).
 * Szöveg -> Szöveg típusú tehát a ciphertext és a plaintext is String.
 * Csak ASCII betűkkel működik, tehát az angol abc betűivel.
 */
class Vigenere: public Cipher{
 String key; /**< A titkosításhoz használt String típusú kulcs.*/
  public:
 /**
  * Konstruktor.
  */
 Vigenere(const String&);
 /**
  * Dekódoló függvény.
  * Ez végzi az egymásutáni ABC shiftelést visszafele a kulcs alapján, így feloldva a titkosítást.
  * @param ciphertext.
  * @return String.
  */
 String decode(const Vector<uint8_t>&)const; 
 /**
  * Enkódoló függvény.
  * Ez végzi az egymásutáni ABC shiftelést a kulcs alapján, így titkosítva a szöveget.
  * @param plaintext.
  * @return Vector<uint8_t> annak ellenére, hogy itt biztosan szövegként térne vissza, egyszerűbb generikusan így kezelni a titkosításokat. Ez nem fog különösebb problémát okozni mivel ha tudjuk, hogy a Vector<uint8_t> elemei ASCII karakterek, akkor a Vector<uint8_t> -> String konverzió egyszerű.
  */
 Vector<uint8_t> encode(const String&)const;
 /**
  * Destruktor.
  */
 ~Vigenere(){};
};
/**
 * Bifid titkosítás.
 * Leszármazott osztály, amely a Bifid féle titkosítást valosítja meg,
 * melyben a titkosítás folyamata a következő:
 * 1. egy 5x5 karakter mátrix kerül létrehozásra a kulcs alapján:
 *  B G W K Z
 *  Q P N D S
 *  I O A X E
 *  F C L U M
 *  T H Y V R
 * ez az angol abc szerint történik tehát itt is csak az angol abc-ből alkotott szavakat lehet titkosítani.
 * 2. a plaintextből koordinátákat alkotunk:
 * F L E E A T O N C E
 * 4 4 3 3 3 5 3 2 4 3
 * 1 3 5 5 3 1 2 3 2 5
 * 3. ez után egy sorba kiírjuk őket:
 * 4 4 3 3 3 5 3 2 4 3 1 3 5 5 3 1 2 3 2 5
 * 4. majd egymás mellett álló számpárokat koordinátáknak értelmezzük, ezket pedig karakterekké alakítva megkapjuk a titkosított szöveget:
 * 44 33 35 32 43 13 55 31 23 25
 * U  A  E  O  L  W  R  I  N  S
 *
 * Szöveg -> Szöveg típusú tehát a ciphertext és a plaintext is String.
 * Csak ASCII betűkkel működik, tehát az angol abc betűivel.
 */
class Bifid: public Cipher{
 char key[5][5]; /**< A titkosításhoz használt kulcs mátrix.*/
 /**
  * Egyszerű struktúra, ami segítségével indexeljük a key mátrixot.
  */
 struct Point{
  size_t x;
  size_t y;
  Point(size_t a=0, size_t b=0): x(a), y(b){}
 };
 /** Key mátrixban megkeresi az adott karaktert, majd visszaadja koordinátáit.
  * Ha nem találta meg a karaktert, akkor (5,5) ad vissza, ami a mátrix méretén kívül esik.
  * @param c a keresendő karakter.
  * @return Point.
  */
 Point find_it(char c) const;
  public:
 /**
  * Konstruktor.
  */
 Bifid(const String&);
 /**
  * Dekódoló függvény.
  * Ez végzi a titkosítást a taglalt módon.
  * @param ciphertext.
  * @return String.
  */
 String decode(const Vector<uint8_t>&)const; 
 /**
  * Enkódoló függvény.
  * Ez végzi a tikosítás visszafejtését.
  * @param plaintext.
  * @return Vector<uint8_t> annak ellenére, hogy itt biztosan szövegként térne vissza, egyszerűbb generikusan így kezelni a titkosításokat. Ez nem fog különösebb problémát okozni mivel ha tudjuk, hogy a Vector<uint8_t> elemei ASCII karakterek, akkor a Vector<uint8_t> -> String konverzió egyszerű.
  */
 Vector<uint8_t> encode(const String&)const;
 /**
  * Destruktor.
  */
 ~Bifid(){};
};
#endif

#ifndef SHA256
#define SHA256
#include "string.h"
#include <cstdint>
#include "vector.hpp"
/**
 * @file sha256.h
 * Az sha256 osztály header fájlja, ami tartalmaz az sha256 által használt egyéb globális függvények deklarációját is.
 */

/**
 * Ez egy enum ami olvashatóbbá teszi a programkódot.
 * Lehetséges értékei: little - ezzel jelezzük hogy little endian, big - ezzel pedig hogy big endian.
 */
enum endian{
  little,
  big,
};
/**
 * Ez egy enum ami olvashatóbbá teszi a programkódot.
 * A rotate függvényben hasznljuk, hogy jelezzük melyik irányba forgajtuk a bitsorozatot.
 */
enum dir{
  right,
  left,
};
/**
 * Az endiannes-t dönti el.
 * Végez egy ellenőrzést, hogy az architektúra, amin a program fut milyen endiannes-t használ.
 * @return endian az endian enumot használja visszatérésnek.
 */
inline endian endian_check(){
  int n = 1;
  if(*(char*)&n == 1) return little;
  else return big;
}
/**
 * Megfordítja a byte sorrendet.
 * @param buf a buffer amiben a megfordítandó byteok vannak.
 * @param buf_size a buffer mérete.
 */
template <typename T>
void reverse_byte_order(T *buf, size_t buf_size){
  uint8_t *buf0 = buf;
  for (size_t i = 0; i<buf_size/2; ++i) {
    uint8_t temp = buf0[i];
    buf0[i] = buf0[buf_size - i -1];
    buf0[buf_size - i -1] = temp;
  }
}
/**
 * Egy bitsorozaton végez forgatást.
 * Lényegében bit shift jobbra / balra, csak a kilépő bitek a legnagyobb / legkisebb helyiértékeken visszakerülnek. 
 * @param buf a forgatandó bitsorozat.
 * @param buf_size a bitsorozat mérete.
 * @param rotN a forgatás hányszorosa.
 * @param d a forgatás iránya.
 */
template <typename T>
T rotate(const T& buf, size_t buf_size, int rotN, dir d){
  switch(d){
    case right:
        return (buf >> rotN) | (buf << (buf_size*8-rotN));
        break;
    case left:
        return (buf << rotN) | (buf >> (buf_size*8-rotN));
        break;
    default:
        return buf;
  }
}
/**
 * SHA256 hash függvény.
 * Az SHA256-os hash függvényt megvalósító osztály, amellyel tetszőleges hosszú Stringeket tudunk hashelni.
 */
class sha256{
  String digest; /**< a 256 bites hexadecimális hash-t tároló String.*/
  String arg; /**< a hash-hez tartozó input String.*/
  static endian e; /**< endiannes-t tároló statikus változó.*/

  public:
  /**
   * Konstruktor.
   * Lényegében ez tartalmazza a hash-elő függvényt.
   * @param input String.
   */
  sha256(const String&);
  /**
   * Lecseréli a digest-et egy új hash-el.
   * Az eddigi arg-hoz hozzá fűz egy másik Stringet, majd ezt az új arg-on újra futtatja a hash függvényt, és frissíti a digestet ez által.
   * @param a hozzáfűzendő String.
   */
  void update(const String&);
  /**
   * Visszadja a digest Stringet.
   * @return String.
   */
  String hexdigest() const;
};
#endif

#ifndef STRING
#define STRING

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

/**
 * @file string.h.
 * A String osztály header fájlja.
 */

/**
 * String osztály, ami követi az std::string mintáját.
 * A szövegkezelésért felelős osztály, ami lehetővé teszi a szövegekkel való műveleteket.
 */
class String{
  char *data; /**< karakter pointer ami egy karakter tömbre mutat, aminek a végét '\0' jelzi.*/
  size_t length; /**< a String hossza, a lezáró karaktert nem beleértve.*/
  size_t pos; /**< a substr kihasználja mint mutató.*/
  public:
  /**
   * Konstruktor.
   * A Stringet az adott karaktertömb elemeivel inicializálja.
   * @param karakter tömb.
   */
  String(const char  *);
  /**
   * Paraméter nélüli konstruktor.
   * A Stringet 0 mérettel, egy '\0' karakterrel inicializálja. 
   */
  String();
  /**
   * Konstruktor.
   * A Stringet 1 mérettel, a paraméterként kapott char-ral inicializálja.
   * @param karakter.
   */
  String(const char);
  /**
   * Másoló konstruktor.
   * @param egy másik String objektum.
   */
  String(const String&);
  /**
   * Konstruktor.
   * A Stringet inicializálja az adott int szám alaki értékeinek számjegyeivel pl. a (int)100-ból "100"-at csinál.
   * @param integer szám.
   */
  String(const int);
  /**
   * Konstruktor.
   * A Stringet inicializálja az adott uint32_t szám hexadecimális alaki értékeinek számjegyeivel.
   * @param 32 bites unsigned int.
   */
  String(const uint32_t);
  /**
   * Visszadja read-only-ként a String karaktertömbjének pointerjét.
   * @return const char *.
   */
  const char * c_string() const{
    return data;
  }
  /**
   * Visszadja a String hosszát.
   * @return const size_t.
   */
  const size_t getLength() const{
    return length;
  }
  /**
   * Visszadja, hogy a tárol szöveg minden karaktere angol abc-beli e.
   * @return bool.
   */
  bool isalpha() const;
  /**
   * Összefűző + operátor.
   * Készít egy új Stringet amelyben a példányt összefűzi a paraméterként kapott másik Stringgel. Operator overload.
   * @param rhs a hozzáfűzendő String.
   * @return String.
   */
  String operator+(const String& rhs) const;
  /**
   * Összefűző + operátor.
   * Készít egy új Stringet amelyben a példányt összefűzi a paraméterként kapott másik T típusú változóval.
   * Csak azokra a T típusokra működik amikre implementálva van a T->String konverzió (const char*, char, int, uint32_t).
   * Operator overload.
   * @param rhs a hozzáfűzendő változó.
   * @return String.
   */
  template<typename T>
  String operator+(const T& rhs) const{
    return *this + String(rhs);
  }
  /**
   * Értékadó operátor.
   * A példány eddigi tartalmát tölri, majd újra inicializálja a paraméter String értékeivel. Operator overload.
   * @param rhs a példány új értéke.
   * @return String& String referencia, tehát használható a balértékként is.
   */
  String& operator=(const String&);
  /**
   * Értékadó operátor.
   * A példány eddigi tartalmát tölri, majd újra inicializálja a paraméter értékeivel. Operator overload.
   * Csak azokra a T típusokra működik amikre implementálva van a T->String konverzió (const char*, char, int, uint32_t).
   * Operator overload.
   * @param rhs a példány új értéke.
   * @return String& String referencia, tehát használható a balértékként is.
   */
  template<typename T>
  String& operator=(const T& rhs){
    return *this = String(rhs);
  }
   /**
   * Hozzáfűző + operátor.
   * A példányhoz hozzáfűzi a paraméterként kapott másik T típusú változót.
   * Csak azokra a T típusokra működik amikre implementálva van a T->String konverzió (const char*, char, int, uint32_t).
   * Operator overload.
   * @param rhs a hozzáfűzendő változó.
   * @return String.
   */
  template<typename T>
  String& operator+=(const T& rhs){
    return *this = *this + rhs;
  }
  /**
   * Indexelű operátor.
   * Visszadja az adott indexű karaktert, ha az index a karaktertömb méreténél kisebb. Ellenkező esetben exceptiont dob.
   * @param idx size_t index.
   * @return char& referencia tehát használható balértékként.
   */
  char& operator[](size_t idx){
    if(idx < 0 || idx >= length) throw std::out_of_range("Az index a sztring határain kívül esik!");
    else{
      return data[idx];
    }
  }
  /**
   * Konstans indexelű operátor.
   * Visszadja az adott indexű karaktert, ha az index a karaktertömb méreténél kisebb. Ellenkező esetben exceptiont dob.
   * @param idx size_t index.
   * @return const char& referencia.
   */
  const char& operator[](size_t idx) const{
    if(idx < 0 || idx >= length) throw std::out_of_range("Az index a sztring határain kívül esik!");
    else{
      return data[idx];
    }
  }
  /**
   * Összehasonlító operátor.
   * Összehasonlítja a példányt egy másik Stringgel, és ez szerint visszatér egy bool-lal.
   * @param rhs amivel összehasonlítunk.
   * @return bool az összehasonlítás logikai értéke.
   */
  bool operator&(const String&) const;
  /**
   * Needle in the haystack.
   * Megnézi hogy az példány tartalmazza-e az adott karaktert.
   * @param karakter.
   * @return bool a keresés eredménye.
   */
  bool operator==(char) const;
  /**
   * Rész stringet készít.
   * Az adott karakterig vagy az EOF-ig kimásol egy rész stringet.
   * Változtatja a pos változót, hogy a következő substr hívásnál már a következő rész stringet adja vissza.
   * @param sep szeparátor karakter.
   * @return String.
   */
  String substr(const char );
  /**
   * Visszadja pos értékét.
   */
  size_t getPos() const;
  /**
   * A szöveg összes abc-beli katarkterét nagy betűre cseréli.
   */
  void toUpper();
    /**
   * A szöveg összes abc-beli katarkterét kis betűre cseréli.
   */
  void toLower();
  /**
   * Destruktor.
   * Fölszabadítja a dinamikusan foglalt karaktertömböt.
   */
  ~String();
};
/**
 * Globális inserter operátor.
 */
std::ostream& operator<<(std::ostream& os, const String& rhs);
/**
 * Globális extractor operátor.
 * A whitespace-ket figyelmen kivűl hagyja.
 */
std::istream& operator>>(std::istream& is, String& rhs);
#endif // !STRING

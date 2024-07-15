#ifndef VECTOR
#define VECTOR

#include <cstddef>
#include <stdexcept>
/**
 * @file vector.hpp
 * A Vector generikus tároló osztály header fájlja.
 */

/**
 * Generikus tároló osztály.
 * Az std::vector szabványát követi, ezzel megvalósítva egy generikus nyújtató tömb osztályt.
 */
template<typename T>
class Vector{
  T* data; /**< az adatot tároló memóriára mutató pointer.*/
  size_t cap; /**< a tároló kapacitása.*/
  size_t realcap; /**< a tároló valódi kapacitása, ezt a memória foglalás optimalizálásánál használja.*/
public:
  /**
   * Iterátor osztály a generikus használat jegyében.
   */
  class iterator{
    T* cell;
  public:
    iterator(T* cell = NULL): cell(cell){}
    /**
     * Összehasonlító operátor overload.
     * Összehasoníltja az iterátor által mutatott elem adatát, egy másik iterátor által mutatott elem adatával.
     * @return bool érték, igaz ha egyenlőek, hamis ha nem.
     * @param other másik iterátor
     */
    bool operator==(const iterator other) const{
     return cell == other.cell; 
    }
    /**
     * Preinkremens operátor overload.
     * Az iterátort átállítja a következő cellára preinkremens módon.
     * @return iterator& iterátor refernciával tér vissza, így használható balértékként.
     */
    iterator& operator++(){
      if(cell != NULL) ++cell;
      return *this;
    }
    /**
     * Kivonás operátor.
     * Az iterátort átállítja rhs-sel előrébb lévő cellára.
     * Nem kezel hibát.
     * @param az eltolás értéke.
     */
    iterator& operator-(int rhs){
      cell -= rhs;
      return *this;
    }
    /**
     * Hozzáadás operátor.
     * Az iterátort átállítja rhs-sel később lévő cellára.
     * Nem kezel hibát.
     * @param az eltolás értéke.
     */
    iterator& operator+(int rhs){
      cell += rhs;
      return *this;
    }
    /**
     * Posztinkremens operátor overload.
     * Az iterátort átállítja a következő cellára posztinkremens módon.
     * @return iterator& iterátor refernciával tér vissza, így használható balértékként.
     */
    iterator operator++(int){
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * Összehasonlító-negált operátor overload.
     * Összehasoníltja az iterátor által mutatott elem adatát, egy másik iterátor által mutatott elem adatával.
     * @return bool érték, igaz ha nem egyenlőek, hamis ha igen.
     * @param other másik iterátor
     */
    bool operator!=(const iterator other)const{
      return cell != other.cell;
    }
    /**
     * Dereferáló operátor overload.
     * @return T& T referencia, visszadja az iterátor által mutatott elem adatának referenciáját.
     */
    T& operator*() const{
      return *cell;
    }
    /**
     * Nyíl operátor overload.
     */
    T* operator->() const{
      return cell;
    }
  };
  class const_iterator{
    T const * cell;
  public:
    const_iterator(T* cell = NULL): cell(cell){}
    bool operator==(const iterator other) const{
     return cell == other.cell; 
    }
    const_iterator& operator++(){
      if(cell != NULL) ++cell;
      return *this;
    }
    const_iterator operator++(int){
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator& operator-(int rhs){
      cell -= rhs;
      return *this;
    }
    const_iterator& operator+(int rhs){
      cell += rhs;
      return *this;
    }
    bool operator!=(const const_iterator other)const{
      return cell != other.cell;
    }
    const T& operator*() const{
      return *cell;
    }
    const T* operator->() const{
      return cell;
    }
  };
  /**
   * Visszadja a tároló első elemére mutató iterátort.
   * @return iterator
   */
  iterator begin() {
    return iterator(data);
  }
  /**
   * Visszadja a tároló utolsó eleme utánra mutató iterátort.
   * @return iterator
   */
  iterator end() {
    return iterator(data+cap);
  }
  const_iterator begin() const{
    return const_iterator(data);
  }
  const_iterator end() const {
    return const_iterator(data+cap);
  }
  /**
   * Konstruktor.
   * Lefoglal size méretű T típusú tömböt dinamikusan.
   * @param size a tömb mérete.
   */
  Vector(size_t size = 0): cap(size), realcap(size){
    data = new T[cap];
  }
  /**
   * Másoló konstruktor.
   * Inicializálja a tárolót a paraméterként kapott másik Vector tároló adataival és méretével.
   * @param other a másik Vector típusú tároló.
   */
  Vector(const Vector& other){
    cap = other.cap;
    realcap = other.realcap;
    data = new T[realcap];
    for (size_t i = 0; i < cap; i++) {
      data[i] = other.data[i]; 
    }
  }
  /**
   * Értékadó operátor.
   * Felszabadítja a tároló által foglalt memóriát, majd inicializálja a tárolót a paraméterként kapott másik Vector tároló adataival és méretével.
   * @param other a másik Vector típusó tároló.
   * @return Vector& referencia tehát használható balértékként.
   */
  Vector& operator=(const Vector& other){
    if(&other != this){
      delete[] data;
      cap = other.cap;
      realcap = other.realcap;
      data = new T[realcap];
      for (size_t i = 0; i < cap; i++) {
        data[i] = other.data[i]; 
      }
    }
    return *this;
  }
  /**
   * Méret lekérdezése.
   * Visszadja a tároló felhasználó által gondolt méretét.
   * @return size_t
   */
  size_t size() const{
    return cap;
  }
   /**
   * Valódi méret lekérdezése.
   * Visszadja a tároló valódi méretét.
   * @return size_t
   */
  size_t sizet() const{
    return realcap;
  }
  /**
   * Indexelő operátor.
   * Visszadja a tároló adott indexű elemét, ha az index a tároló tartományába esik.
   * Máskülönben hibát kezel és exceptiont dob.
   * @param idx az adott index.
   * @return T& referencia tehát használható balértékként.
   */
  T& operator[](size_t idx){
    if(idx < 0 || idx >= cap) throw std::out_of_range("Az index a vector határain kívül esik!");
    else{
      return data[idx];
    }
  }
  /**
   * Konstans indexelő operátor.
   * Visszadja a tároló adott indexű elemét, ha az index a tároló tartományába esik.
   * Máskülönben hibát kezel és exceptiont dob.
   * @param idx az adott index.
   * @return const T&.
   */
  const T& operator[](size_t idx) const{
    if(idx < 0 || idx >= cap) throw std::out_of_range("Az index a vector határain kívül esik!");
    else{
      return data[idx];
    }
  }
  /**
   * Dinamikus nyújtásos hozzáadás.
   * A tároló méretét megnyújta (ha szükséges), majd a végére rakja az adott új elemet.
   * Megnyújtás alatt azt értjük, hogy egy új nagyobb tömböt foglalunk majd oda másoljuk a régi adatot.
   * Itt használódik ki a realcap előnye, mivel minden nyújtásnál 10-zel előre megnyújtja a tároló méretét. így nem minden hozzáadásnál kell egyesével nyújtani, tehát a foglalás burstben történik.
   */
  void push_back(const T& _data){
    if(cap == realcap){
      realcap += 10;
      T* new_data = new T[realcap];
      for (size_t i = 0; i < cap; i++) {
      new_data[i] = data[i]; 
      }
      new_data[cap] = _data;
      delete[] data;
      data = new_data;
    }
    else {
      data[cap] = _data;
    }
    cap++;
  }
  /**
   * Kiírja az elemeket, az adott streamre.
   * Duck typing.
   * @param os az adott output stream,
   */
  void kiir(std::ostream& os) const{
    for(const_iterator it = begin(); it != end(); ++it){
      os << *it << " ";
    }
    os << '\n';
  }
  /**
   * Tartalom szerint összehasonlító operátor.
   * @param other egy másik Vector<T> tároló.
   * @return bool.
   */
  bool operator==(const Vector<T>& other){
    if(other.size() == cap){
      for(size_t i = 0; i < cap; ++i){
        if(other[i] != (*this)[i]) return false;
      }
     return true;
    }
    return false;
  }
  /**
   * Destruktor.
   * A dinamikusan foglalt memória területet felszabadítja.
   */
  ~Vector(){
    delete[] data;
  }
};
#endif // !VECTOR

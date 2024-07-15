#ifndef LIST
#define LIST

#include <cstddef>
#include <stdexcept>
#include <stdlib.h>

/**
 * @file list.hpp
 * Generikus List tároló header fájlja.
 */

/**
 * Template osztály amely az std::list szabványát követi.
 * Generikus tárolásra alkalmas osztály amely a láncolt lista adatstruktúrája szerint épül fel.
 * Elemei duplán lácoltak, belső iterátor osztályt is használ.
 */
template <typename T>
class List{
    /**
     * Belső struktúra amely a lista elemének felel meg.
     */
  struct Cell{
    Cell* next; /**< következő elemre mutató pointer*/
    Cell* prev; /**< előző elemre mutató pointer*/
    T data; /**< tárolt adat*/
    Cell(Cell *_next, Cell *_prev, const T& _data): next(_next), prev(_prev), data(_data){}
  };
  Cell *first, *last;
  size_t size_;
public:
  /**
   * Iterátor osztály a generikus használat jegyében.
   */
  class iterator{
    Cell* cell;
  public:
    iterator(Cell* cell = NULL): cell(cell){}
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
     * Az iterátort átállítja a következő lista elemre preinkremens módon.
     * @return iterator& iterátor refernciával tér vissza, így használható balértékként.
     */
    iterator& operator++(){
      if(cell != NULL) cell = cell->next;
      return *this;
    }
    /**
     * Posztinkremens operátor overload.
     * Az iterátort átállítja a következő lista elemre posztinkremens módon.
     * @return iterator iterátor tér vissza.
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
      return cell->data;
    }

    /**
     * Nyíl operátor overload.
     */
    T* operator->() const{
      return &(cell->data);
    }
  };
  class const_iterator{
    const Cell* cell;
  public:
    const_iterator(const Cell* cell = NULL): cell(cell){}
    bool operator==(const iterator other) const{
     return cell == other.cell; 
    }
    iterator& operator++(){
      if(cell != NULL) cell = cell->next;
      return *this;
    }
    iterator operator++(int){
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    bool operator!=(const iterator other)const{
      return cell != other.cell;
    }
    const T& operator*() const{
      return cell->data;
    }
    const T* operator->() const{
      return &(cell->data);
    }
  };
  iterator begin() {
    return iterator(first);
  }
  iterator end() {
    return iterator(NULL);
  }
  const_iterator begin() const{
    return const_iterator(first);
  }
  const_iterator end() const {
    return const_iterator(NULL);
  }
  /**
   * Konstruktor.
   * Inicialiálja a lista objektumot üresen.
   */
  List(): first(NULL), last(NULL), size_(0){}

  /**
   * Másoló konstruktor.
   * Inicialiálja a listát egy másik lista adatával.
   * @param other a másik lista.
   */
  List(const List& other): first(NULL), last(NULL), size_(0){
    Cell *tmp = other.first;
    while(tmp != NULL){
      (*this).push_back(tmp->data);
      tmp = tmp->next;
    }
  }

  /**
   * Értékadó operátor.
   * A lista eddigi memória területét fölszabadítja, majd inicializálja a masik lista elemeivel.
   * @param other a másik lista. 
   * @return List&, List referencia így használható balértékként.
   */
  List& operator=(const List& other){
    (*this).clear();
    Cell *tmp = other.first;
    while(tmp != NULL){
      (*this).push_back(tmp->data);
      tmp = tmp->next;
    }
    return *this;
  }
  /**
   * Kiolvassa a lista legelső elemének adatát.
   * Ha nem üres a lista visszadja a legelső elem adatát, máskülönben exceptiont dob.
   * @return const T& konstans referneciával tér vissza, így optimális marad nagyobb T struktúrákon is.
   */
  const T& read_front() const{
    if(first == NULL) throw std::runtime_error("Üres a lista, nincsen első elem!");
    else{
      return first->data;
    }
  }
  /**
   * Kiolvassa a lista utolsó elemének adatát.
   * Ha nem üres a lista visszadja az utolsó elem adatát, máskülönben exceptiont dob.
   * @return const T& konstans referneciával tér vissza, így optimális marad nagyobb T struktúrákon is.
   */
  const T& read_back() const{
    if(first == NULL) throw std::runtime_error("Üres a lista, nincsen utolsó elem!");
    else{
      return last->data;
    }
  }
  /**
   * Visszadja a lista aktuális méretét.
   * @return const size_t a lista mérete.
   */
  const size_t size(){return size_;}
  /**
   * A lista legvégéhez fűz új egy elemet.
   * @param _data az új elem tartalma.
   * @return void.
   */
  void push_back(const T& _data){
    size_++;
    Cell *tmp = new Cell(NULL, last, _data);
    if(last == NULL){
      first = tmp;
    }
    else{
      last->next = tmp;
    }
    last = tmp;
  }
  /**
   * A lista elejére szúr egy új elemet.
   * @param _data az új elem tartalma.
   * @return void.
   */
  void push_front(const T& _data){
    size_++;
    Cell *tmp = new Cell(first,NULL,_data);
    if(first == NULL){
      last = tmp;
    }
    else{
      first->prev = tmp;
    }
    first = tmp;
  }
  /**
   * Törli a lista adatát.
   * Kiüríti a listát, méretét 0-ra állítja.
   * @return void.
   */
  void clear(){
    while(first != NULL){
      Cell *mozgo = first->next;
      delete first;
      first = mozgo;
    }
    last = NULL;
    size_ = 0;
  }
  /**
   * Destruktor.
   */
  ~List(){
    while(first != NULL){
      Cell *mozgo = first->next;
      delete first;
      first = mozgo;
    }
  }
};
#endif // !LIST

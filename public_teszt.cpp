#include "cipher.h"
#include "vector.hpp"
#include "string.h"
#include "list.hpp"
#include "sha256.h"
#include "account.h"
#include <iostream>
#include "gtest_lite.h"
#include <stdexcept>

using std::cout;
using std::cin;
using std::endl;

int main(void){
/**
 *  1. A paraméter nélkül hívható konstruktora üres sztringet hozzon étre!
 */
    TEST(String1, construct) {
      String ures;
      if (ures.getLength() != 0)
        FAIL() << "Baj van a string hosszaval" << endl;
      else
        SUCCEED() << "Hurra! Van egy sztringem, de nem biztos, hogy ures!" << endl;
    } ENDM
/**
 *  2. Van olyan tagfüggvénye ( c_string() ), ami C-sztringgel, azaz nullával lezárt
 *     karaktersorozatra mutató pointerel (const char *) tér vissza.
 */
    TEST(String2, emptyStr) {
      String ures;
      EXPECT_EQ((size_t)0, ures.getLength()) << "Baj van a string hosszaval" << endl;

      /// Ellenőrizzük, hogy üres sztringet ("") kaptunk-e vissza?
      EXPECT_STREQ("", ures.c_string()) << "Nem ures sztring jott letre" << endl;
    } ENDM

/**
 * 3. Van olyan konstruktora, ami karakterből hoz létre sztringet.
 */
    TEST(String3, fromChr) {
      char ch = 'a';
      String a(ch);
      EXPECT_EQ((size_t)1, a.getLength()) << "Baj van a string hosszaval" << endl;
      EXPECT_STREQ("a", a.c_string()) << "Karakterbol sztring letrehozasa nem sikerult!" << endl;

      String b('a');    // konstansból is megy?
      EXPECT_STREQ("a", b.c_string()) << "Karakterbol sztring letrehozasa nem sikerult!" << endl;
    } ENDM

/**
 *  4. Van olyan konstruktora, ami C-sztringből (const char*) hoz létre sztringet.
 *  A const char* konstruktor lemásolja a sztringet, nem csak a pointert!
 */
    TEST(String4, FromCstr) {
      const char *hello = "Hello sztring";
      String a(hello);
      EXPECT_EQ(strlen(hello), a.getLength()) << "Baj van a string hosszaval" << endl;
      EXPECT_STREQ(hello, a.c_string()) << "C-sztringbol string letrehozasa nem sikerult!" << endl;

      /// Ellenőrizzük, hogy lemásolta-e a sztringet. Nem elég a pointert!
      char cpp[] = { 'C', '+', '+', '\0' };
      String b(cpp);
      cpp[0] = 'X';
      EXPECT_STRNE(cpp, b.c_string()) << "FromCstr: le kellett volna masolni a karaktertombot!" << endl;
    } ENDM

/**
 *  5. Az osztályból létrehozott objektum legyen átadható értékparaméterként!
 */
    TEST(String5, ctor) {
      const char *hello = "Hello sztring";
      String a(hello);
      String b = a;
      EXPECT_EQ(strlen(hello), b.getLength()) << "Baj van a string hosszaval!" << endl;
      EXPECT_STREQ(hello, b.c_string()) << "Baj van a masolo konstruktorral!" << endl;

      /// Ellenőrizzük, hogy lemásolta-e a sztringet. Nem elég a pointert!
      if (a.c_string() == b.c_string())     // Ha a két pointer egyezik, akkor nem másolta le az adatot
         FAIL()  << "!** ctor: nem elegendo a pointereket lemasolni!" << endl;

      EXPECT_STREQ(hello, a.c_string()) << "Masolo kontsr. elromlott a forras!" << endl;
      EXPECT_EQ(strlen(hello), a.getLength()) << "Masolo konstr. alatt elromlott a forras hossza!" << endl;

      const String c = a;
      String d = c;     // konstansból is megy?
      EXPECT_STREQ(hello, d.c_string()) << "Baj van a masolo konstruktorral" << endl;
    } ENDM

    TEST(String5, ctor_nullptr) {
      String d0;
      String ures = d0;     // üres sztringet is le tudja másolni?
      EXPECT_EQ((size_t)0, ures.getLength()) << "Baj van a string hosszaval" << endl;
      /// Ellenőrizzük, hogy üres sztringet ("") kaptunk-e vissza?
      EXPECT_STREQ("", ures.c_string()) << "Nem ures sztring jott letre" << endl;
    } ENDM

/**
 *  6. Támogassa a többszörös értékadást (b = a = a)!
 */
    TEST(String6, opAssign) {
      const char *hello = "Hello sztring";
      String a(hello);
      String b("Duma1"), c("Duma2");
      EXPECT_STRNE(a.c_string(), b.c_string());
      a = a;
      EXPECT_EQ(strlen(hello), a.getLength()) << "Baj van az ertekadassal: a = a hossz" << endl;
      EXPECT_STREQ(hello, a.c_string()) << "Baj van az ertekadassal: a = a" << endl;
      c = b = a;

      /// Ellenőrizzük, hogy lemásolta-e a sztringet. Nem elég a pointert!
      if (a.c_string() == b.c_string())     // Ha a két pointer egyezik, akkor nem másolta le az adatot
         FAIL()  << "!** op=: nem elegendo a pointereket lemasolni!" << endl;

      EXPECT_EQ(strlen(hello), a.getLength()) << "Ertekedasnal elromlott a forras hossza!" << endl;
      EXPECT_STREQ(hello, a.c_string()) << "Ertekadasnal elromlott a forras!" << endl;
      EXPECT_EQ(strlen(hello), b.getLength()) << "Ertekedas: nem jo a hossz!" << endl;
      EXPECT_STREQ(hello, b.c_string()) << "Ertekadas nem sikerult!" << endl;
      EXPECT_EQ(strlen(hello), c.getLength()) << "Ertekedas: nem jo a hossz!" << endl;
      EXPECT_STREQ(hello, c.c_string()) << "Ertekadas nem sikerult!" << endl;

      const String d("Konst.");
      c = d;        // konstansból is megy?
      EXPECT_EQ(c.getLength(), c.getLength()) << "Ertekedas konstansbol: nem jo a hossz!" << endl;
      EXPECT_STREQ(d.c_string(), c.c_string()) << "Ertekadas konstansbol nem sikerult!" << endl;
    } ENDM

    TEST(String6, opAssign_nullptr) {
      String d0;
      String c = d0;     // üres sztringet is le tudja másolni?
      EXPECT_EQ((size_t)0, c.getLength()) << "Baj van a string hosszaval" << endl;
      /// Ellenőrizzük, hogy üres sztringet ("") kaptunk-e vissza?
      EXPECT_STREQ("", c.c_string()) << "Nem ures sztring jott letre" << endl;

    } ENDM

/**
 *  7. Legyenek olyan operátorai (operator+), amivel a sztring végéhez sztringet
 *     és karaktert/intet lehet fűzni!
 */
    TEST(String7, strPlusStr) {
      String a("Hello ");
      String b("sztring");
      String c;
      c = a + b;
      EXPECT_STREQ("Hello sztring", c.c_string()) << "Nem sikerult a + String muvelet!" << endl;
      EXPECT_STREQ("Hello ", a.c_string()) << "A + muvelet elrontja a bal oldalt?" << endl;
      EXPECT_STREQ("sztring", b.c_string()) << "A + muvelet elrontja a jobb oldalt?" << endl;
      const String a1 = a, b1 = b;

      String c1;
      c1 = a1 + b1;          // konstansból is megy?
      EXPECT_STREQ("Hello sztring", c1.c_string()) << "Nem sikerult a + String muvelet!" << endl;
    } ENDM

    TEST(String7, strPlusChr) {
      String a("Hello ");
      String b;
      b = a + 'B';
      EXPECT_STREQ("Hello B", b.c_string()) << "Nem sikerult a + char muvelet!" << endl;
      EXPECT_STREQ("Hello ", a.c_string()) << "A + muvelet elrontja a bal oldalt?" << endl;

      const String a1 = a;
      String b1;
      b1 = a1 + 'B';          // konstansból is megy?
      EXPECT_STREQ("Hello B", b1.c_string()) << "Nem sikerult a + char muvelet!" << endl;
      EXPECT_STREQ("Hello ", a1.c_string()) << "A + muvelet elrontja a bal oldalt?" << endl;

    } ENDM

    TEST(String7, strPlusInt) {
      String a("Hello ");
      String b;
      b = a + 10;
      EXPECT_STREQ("Hello 10", b.c_string()) << "Nem sikerult a + char muvelet!" << endl;
      EXPECT_STREQ("Hello ", a.c_string()) << "A + muvelet elrontja a bal oldalt?" << endl;

      const String a1 = a;
      String b1;
      b1 = a1 + 10;          // konstansból is megy?
      EXPECT_STREQ("Hello 10", b1.c_string()) << "Nem sikerult a + char muvelet!" << endl;
      EXPECT_STREQ("Hello ", a1.c_string()) << "A + muvelet elrontja a bal oldalt?" << endl;

    } ENDM

/**
 * 8. Müködjön a substr(), isalpha(), toUpper() és toLower() tagfüggvények,
 */
    TEST(String8, subStr) {
      String a("Hello szia viszlát");
      String b, c, d;
      b = a.substr(' ');
      c = a.substr(' ');
      d = a.substr(' ');
      EXPECT_STREQ("Hello", b.c_string()) << "Nem sikerult a substr művelet" << endl;
      EXPECT_STREQ("szia", c.c_string()) << "Nem sikerult a substr művelet" << endl;
      EXPECT_STREQ("viszlát", d.c_string()) << "Nem sikerult a substr művelet" << endl;
    } ENDM

    TEST(String8, isalpha) {
      String a("Hello szia viszlát");
      EXPECT_EQ(false, a.isalpha());
      String b("abcde");
      EXPECT_EQ(true, b.isalpha());
      String c("123");
      EXPECT_EQ(false, c.isalpha());
    } ENDM

    TEST(String8, toUpper) {
      String a("Hello");
      a.toUpper();
      EXPECT_STREQ("HELLO", a.c_string()) << "Nem sikerult a toUpper művelet" << endl;
    } ENDM

    TEST(String8, toLower) {
      String a("HElOKa");
      a.toLower();
      EXPECT_STREQ("heloka", a.c_string()) << "Nem sikerult a toLower művelet" << endl;
    } ENDM

/**
 *  9. A tárolt a karakterek legyenek elérhetőek a szokásos módon indexeléssel!
 *     Az indexeléssel elért elem legyen használható balértékként is!
 *     A konstans objektumok is legyenek indexelhetők.
 *     Az indexelés operátor hiba esetén dobjon std::out_of_range kivételt!
 */
    TEST(String9, index) {
      String a("Hello 678");
      EXPECT_NO_THROW(a[0]);
      EXPECT_NO_THROW(a[7]);
      EXPECT_NO_THROW(a[8]);
      EXPECT_THROW(a[9], std::out_of_range const&);
      EXPECT_THROW(a[-1],std::out_of_range const&);
      EXPECT_EQ('7', a[7]);
      a[7] = '.';
      EXPECT_EQ('.', a[7]);
    } ENDM

/**
 * 10. Készítsen olyan << operátort, amivel egy sztring kiírható egy ostream típusú
 *     objektumra!
 */
    TEST(String10, insert) {
      String a("Hello sztring");
      std::stringstream ss;
      ss << a;
      EXPECT_STREQ("Hello sztring", ss.str().c_str());
    } ENDM
/**
 * 11. Készítsen olyan fűzhető >> operátort, amivel be tud olvasni egy szót egy sztring-be!
 *     A beolvasó működjön úgy, mint a scanf %s, azaz a szó végét white space határolja!
 *     A szó eleji white space karaktereket pedig el kell dobni.
 *    *Megj: az istream wshite space kezelése a flags() tagfüggvénnyel szabályozható
 */
    TEST(String11, extract) {
      String a("Hello   sztring \n Lajos12");
      std::stringstream ss;
      ss << a;
      String in1, in2, in3;
      ss >> in1 >> in2;         // fűzhetőnek kell lenni
      while (ss >> in3);        // csak kicsit gonosz a teszt !
      EXPECT_STREQ("Hello", in1.c_string());
      EXPECT_STREQ("sztring", in2.c_string());
      EXPECT_STREQ("Lajos12", in3.c_string());
    } ENDM
/**
 * 12. Teszteli hogy működik-e a String kompralálás és a needle in the haystack művelet.
 */
    TEST(String12, cmp) {
      String a("Micimackó");
      String b("tapsifüles");
      EXPECT_EQ(false, a&b);
      b = a;
      EXPECT_EQ(true, a&b);
    } ENDM

    TEST(String12, haystack) {
      String a("Micimackó");
      EXPECT_EQ(false, a=='q');
      EXPECT_EQ(true, a=='m');
    } ENDM
/**
 * 1. Teszteli a Vector konstruktorokat.
 */
    TEST(Vector1, ctor) {
       Vector<int> a;
       EXPECT_EQ((size_t)0, a.size());
       Vector<int> b(10);
       EXPECT_EQ((size_t)10, b.size());
    } ENDM

    TEST(Vector1, copyctor) {
       Vector<int> a(1);
       Vector<int> b = a;
       EXPECT_EQ((size_t)1, b.size());
    } ENDM
/**
 * 2. Teszteli a Vector értékadó operátort.
 */
    TEST(Vector2, equ) {
       Vector<int> a(5);
       Vector<int> b(10);
       b = a;
       EXPECT_EQ((size_t)5, b.size());
       Vector<int> c(9);
       b = a = c;
       EXPECT_EQ((size_t)9, b.size());
    } ENDM
/**
 * 3. Indexelő operátorok tesztelése.
 * Hibakezelés tesztelése.
 */
    TEST(Vector3, index) {
      Vector<int> a(9);
      EXPECT_NO_THROW(a[0]);
      EXPECT_NO_THROW(a[7]);
      EXPECT_NO_THROW(a[8]);
      EXPECT_THROW(a[9], std::out_of_range const&);
      EXPECT_THROW(a[-1],std::out_of_range const&);
    } ENDM
/**
 * 4. Push_back operátor tesztelése.
 * Nyújtást is leteszteljük itt.
 */
    TEST(Vector4, pushback) {
      Vector<int> a;
      a.push_back(3);
      EXPECT_EQ(3,a[0]);
      EXPECT_EQ((size_t)10,a.sizet());
    } ENDM
/**
 * 1. Titkosítások tesztelése
 */
    TEST(Cipher1,XOR ) {
     XOR mode0("almafa12");
     Vector<uint8_t> ciphertext0 = mode0.encode("Nagy titok");
     /* Manuális leteszteljük a titkosítás eredményét*/
     uint8_t tmp[] = {0x2f, 0x0d, 0x0a, 0x18, 0x46, 0x15, 0x58, 0x46, 0x0e, 0x07};
     Vector<uint8_t> elvart0;
     for(size_t i = 0; i < 10; ++i){
      elvart0.push_back(tmp[i]);
     }
     EXPECT_EQ(true, ciphertext0 == elvart0);
     String plaintext = mode0.decode(ciphertext0);
     EXPECT_STREQ("Nagy titok", plaintext.c_string());


     Cipher* test = new XOR("almafa12");
     Vector<uint8_t> ciphertext1 = test->encode("Nagy titok");
     /* Manuális leteszteljük a titkosítás eredményét*/
     EXPECT_EQ(true, ciphertext1 == elvart0);
     String plaintext1 = test->decode(ciphertext1);
     EXPECT_STREQ("Nagy titok", plaintext1.c_string());
     delete test;
    } ENDM

    TEST(Cipher1,Vigenere ) {
     Vigenere mode1("kulcs");
     Vector<uint8_t> elvart0;
     String tmp("XURAGLVEKLYE");
     for(size_t i = 0; i < 12; ++i){
      elvart0.push_back(tmp[i]);
     }
   Vector<uint8_t> ciphertext1 = mode1.encode("nagyobbtitok");
     EXPECT_EQ(true, ciphertext1 == elvart0);
     String plaintext1 = mode1.decode(ciphertext1);
     EXPECT_STREQ("NAGYOBBTITOK", plaintext1.c_string());
     EXPECT_THROW(Vigenere mode2("ilyétnemszabad34"), std::invalid_argument const&); //csak angol abc-beli karakterekből lehet jelszót csinálni
     EXPECT_THROW(mode1.encode("ilyátsemszabad98"), std::invalid_argument const&); //csak angol abc-beli karakterekből lehet titkoítandót csinálni

     Cipher* test = new Vigenere("kulcs");
     Vector<uint8_t> ciphertext0 = test->encode("nagyobbtitok");
     EXPECT_EQ(true, ciphertext0 == elvart0);
     String plaintext0 = test->decode(ciphertext0);
     EXPECT_STREQ("NAGYOBBTITOK", plaintext0.c_string());
     delete test;
    } ENDM

    TEST(Cipher1,Bifid ) {
     Bifid mode2("biztonsagos");
     Vector<uint8_t> ciphertext2 = mode2.encode("legnagyobbtitok");
     Vector<uint8_t> elvart0;
     String tmp("PSSUBBBDGZRUTGY");
     for(size_t i = 0; i < 15; ++i){
      elvart0.push_back(tmp[i]);
     }
     EXPECT_EQ(true, ciphertext2 == elvart0);
     String plaintext2 = mode2.decode(ciphertext2);
     EXPECT_STREQ("LEGNAGYOBBTITOK", plaintext2.c_string());
     EXPECT_THROW(Bifid mode3("ilyétnemszabad34"), std::invalid_argument const&); //csak angol abc-beli karakterekből lehet jelszót csinálni
     EXPECT_THROW(mode2.encode("ilyátsemszabad98"), std::invalid_argument const&); //csak angol abc-beli karakterekből lehet titkoítandót csinálni

     Cipher *test = new Bifid("biztonsagos");
     Vector<uint8_t> ciphertext1 = test->encode("legnagyobbtitok");
     EXPECT_EQ(true, ciphertext1 == elvart0);
     String plaintext0 = test->decode(ciphertext1);
     EXPECT_STREQ("LEGNAGYOBBTITOK", plaintext0.c_string());
     delete test;
    } ENDM
/**
 * 1. SHA256 tesztelése.
 * Bármely más értékekre is müködik.
 * Ugyanazt a hash értéket adja mint bármely más official sha256.
 * https://emn178.github.io/online-tools/sha256.html
 */
    TEST(Sha256, ctor ) {
     sha256 a("hasheljük ezt mert miért ne");
     EXPECT_STREQ("98bf09f5908c9781b72c37472c8d23368db6db9e1eabbb2ed1c36c94e69df3f1", a.hexdigest().c_string());
     sha256 b("jelszó");
     b.update("só");
     EXPECT_STREQ("c6906131b8e6fc1502eebab767b805cace4df0d56ff017bbd057c76bba2940f6", b.hexdigest().c_string());
    } ENDM
/**
 * 1. Fiók beléptetés tesztelése.
 */
    TEST(Account, login ) {
     Account fiok0("ca659234fe3bceeb51e1b4a0c01e43ae54180bf4a715858bebf6fe6277b3a939", "só", "0a90f84cd8fadb7b4a71c62db57b0d237a8fcf606ea1e61e3cc1980e04ad94db");
     EXPECT_EQ(true, fiok0.verify("Valentin", "almafa12"));

    } ENDM

return 0;
}

# FIT-PA1-PeopleDatabase

This is a project created as a homework for subject Programming and Algorithmic 1 (BI-PA1 - Programování a algoritmizace 1) at FIT CTU.

Full assignment in Czech language is below.

Summary of the assignment: Create database of people that will allow efficient search.

---

Basically, the point of this assignment is to implement your own linked list.

My solution is in file `peopledb.c`, function `main` of this file contains test data for basic scenarios.

My solution has received 5 points out of 5, which is maximum.

# Assignment

Úkolem je realizovat sadu funkcí, která umožní efektivní vyhledávání v databázi lidí.

Chceme realizovat databázi lidí. Pro každého člověka v databázi si budeme pamatovat jeho osobní číslo (`int`) a jméno. Dále známe informaci o rodičích (místo sexistického "otec" a "matka" budeme používat zcela korektní označení "rodič 1" a "rodič 2"). Do takové databáze budeme přidávat záznamy a budeme v nich vyhledávat. Vyhledávat chceme předky zadaného jedince a předky společné pro dva zadané jedince.

Vytvořená databáze bude realizovaná jako sada funkcí. Ty mají následující rozhraní:

```
typedef struct TResult
 {
   TResult   * m_Next;
   int         m_ID;
   char      * m_Name;
 } TRESULT;

void      Init             ( void );
void      Done             ( void );
int       Register         ( int               ID,
                             const char      * name,
                             int               ID1,
                             int               ID2 );
TRESULT * Ancestors        ( int               ID );
TRESULT * CommonAncestors  ( int               ID1,
                             int               ID2 );
void      FreeResult       ( TRESULT         * res );
```

- **`TRESULT`**
je struktura, která je používána pro navracení nalezených výsledků. Výsledky (nalezené osoby) jsou vracené v podobě jednosměrně zřetězeného spojového seznamu, ve kterém jsou prvky typu `TRESULT`. Složky mají následující význam:

- **`m_Next`** je ukazatel na další prvek ve spojovém seznamu. Poslední prvek spojového seznamu obsahuje v této složce hodnotu `NULL`,

- **`m_ID`** je ID nalezené osoby,

- **`m_Name`** je jméno nalezené osoby (ASCIIZ řetězec).

- **`Init`**
je funkce, která inicializuje databázi a připraví ji k použití. Na počátku bude databáze prázdná. Testovací prostředí zavolá tuto před voláním libovolných dalších funkcí.

- **`Done`**
je funkce, která uvolní prostředky alokované databází. Testovací prostředí zavolá tuto funkci v okamžiku, kdy končí testy a nechce s existující databází dále pracovat. Funkce Init/Done mohou být zavolány vícekrát, ale vždy párově, tedy Init Done Init Done ... Init Done.

- **`Register`**
je funkce, která přidá do databáze další osobu. Parametrem je ID přidávané osoby, jméno přidávané osoby a ID obou rodičů. Funkce provede potřebné změny v databázi a vrátí návratovou hodnotu 0 (neúspěch) nebo 1 (úspěch). Pro úspěšné zavedení osoby do databáze je potřeba:
  - zadané ID osoby je unikátní a nenulové,
  - ID rodiče je buď nulové (rodič není znám) nebo nenulové (a identifikuje existující záznam v databázi),
  - rodič 1 a rodič 2 nesmí být jedna a ta samá osoba.

- **`Ancestors`**
funkce vyhledá všechny známé předky zadané osoby. Návratovou hodnotou je spojový seznam nalezených osob (viz struktura `TRESULT`). Pořadí prvků v seznamu není dáno, ale seznam nesmí obsahovat duplicity. Pokud osoba se zadaným ID neexistuje nebo pokud existuje, ale předkové nejsou známí, vrací funkce návratovou hodnotu `NULL`.

- **`CommonAncestors`**
funkce vyhledá všechny známé předky společné pro dvě zadané osoby. Návratovou hodnotou je spojový seznam nalezených osob (viz struktura `TRESULT`). Pořadí prvků v seznamu není dáno, ale seznam nesmí obsahovat duplicity. Pokud osoba (osoby) se zadanými ID neexistují nebo pokud existují, ale předkové nejsou známí, nebo pokud zadané osoby nemají žádné známé společné předky, vrací funkce návratovou hodnotu `NULL`.

- **`FreeResult`**
funkce uvolní prostředky alokované pro zadaný výsledek. Funkce je volaná pro hodnoty vracené funkcemi `Ancestors` / `CommonAncestors` v okamžiku, kdy výsledek již není potřeba.


Odevzdávejte zdrojový kód s implementací zadaných funkcí. Je potřeba přesně dodržet rozhraní funkcí a struktury, pokud rozhraní změníte, program nepůjde zkompilovat. Za základ implementace použijte zdrojový kód z přílohy. V tomto zdrojovém kódu je jednak deklarace požadovaného rozhraní a dále i funkce main s testy, které jsou provedené v základním testu. Pro správnou funkci je potřeba ponechat ve zdrojovém kódu bloky podmíněného překladu.

Vnitřní realizace databáze není určena. Vzhledem k charakteru dat je ale rozumné pracovat se spojovou strukturou.

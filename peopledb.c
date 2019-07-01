#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TResult
 {
   struct TResult   * m_Next;
   int              m_ID;
   char             * m_Name;
 } TRESULT;
#endif /* __PROGTEST__ */

//definovanie databazy
typedef struct TEntry
 {
  struct TEntry   * next;
  int             id;
  char            * name;
  int             parent1;
  int             parent2;
 } TENTRY;

 //definovanie zaciatku zoznamu
 TENTRY *head, *tail;



void Init(void){
  
  //inicializovanie spojaku - nastavenie ukazatelov na NULL
  head = NULL;
  tail = NULL;

}

void Done(void){
  //premenna
  TENTRY *del = NULL;

  //zrusenie spojaku (postupne uvolnovanie pamate)
  if(head){
      while(head){
      del = head;
      head = head->next;
      free(del->name);
      free(del);
    }
    head = NULL;
    tail = NULL;
  }

}

void FreeResult(TRESULT * res){
  //premenne
  TRESULT *del = NULL;

  //odstranenie spojaku
  if(res){
    while(res){
      del = res;
      res = res->m_Next;
      free(del->m_Name);
      free(del);
    }
  }

}

int Register(int ID, const char * name, int ID1, int ID2){
  //premenne
  TENTRY *tmp;

  //overenie platnosti vstupnych udajov
    //rodic1 a rodic2 nesmu byt ta ista osoba
    if(ID1 == ID2 && ID1 != 0) return 0;
    //ak je rodic znamy (id != 0) musi identifikovat zaznam z databazy
    if(ID1 != 0){
      tmp = head;
      while(tmp){
        if(tmp->id == ID1) break;
        if(!tmp->next) return 0;
        tmp = tmp->next;
      }
    }
    if(ID2 != 0){
      tmp = head;
      while(tmp){
        if(tmp->id == ID2) break;
        if(!tmp->next) return 0;
        tmp = tmp->next;
      }
    }

    //zadane ID osoby je unikatne a nenulove
    if(ID == 0) return 0;
    tmp = head;
    while(tmp){
      if(tmp->id == ID) return 0;
      tmp = tmp->next;
    }


  //vlozenie zaznamu do databazy
    //alokacia 
    tmp = (TENTRY*) malloc(sizeof(TENTRY));
    //vlozenie dat
    tmp->id = ID;
    tmp->parent1 = ID1;
    tmp->parent2 = ID2;
    tmp->next = NULL;
    tmp->name = (char*) malloc((strlen(name)+1)*sizeof(char));
    strcpy(tmp->name, name);
    //uprava pointerov
    if(!head){
      head = tail = tmp;
    }
    else {
      tail->next = tmp;
      tail = tmp;
    }
    return 1;
}

TRESULT* appendToLinkedList(int ID, TRESULT* st){
  TENTRY* db;
  TRESULT* tmp = (TRESULT*) malloc(sizeof(TRESULT));

  //najdenie udajov o predkovi a jeho zapisanie do spojaku
  db = head;
  while(db){
    if(db->id == ID){
      tmp->m_ID = ID;
      tmp->m_Name = (char*) malloc((strlen(db->name)+1)*sizeof(char));
      strcpy(tmp->m_Name,db->name);
      tmp->m_Next = st;
      st = tmp;
      break;
    }
    db = db->next;
  }
  return st;
}

TRESULT* findAncestors(int ID, TRESULT* st){
  TENTRY *db;
  db = head;

  //rekurzivne vyhladavanie predkov
  while(db){
    if(db->id == ID){
      if(db->parent1 != 0){
        st = appendToLinkedList(db->parent1, st);
        st = findAncestors(db->parent1, st);
      }
      if(db->parent2 != 0){
        st = appendToLinkedList(db->parent2, st);
        st = findAncestors(db->parent2, st);
      }
      break;
    }
    db = db->next;
  }

  return st;
}

TRESULT * Ancestors(int ID){
  //premenne
  TRESULT *st = NULL, *tmp, *tmp2, *tmp2Prev;

  //hladanie predkov
  st = findAncestors(ID, st);

  //odstranenie duplicit
  tmp = st;
  while(tmp){
    tmp2 = tmp->m_Next;
    tmp2Prev = tmp;
    while(tmp2){
      if(tmp->m_ID == tmp2->m_ID){
        tmp2Prev->m_Next = tmp2->m_Next;
        free(tmp2->m_Name);
        free(tmp2);
        tmp2 = tmp2Prev->m_Next;
      } else {
        tmp2Prev = tmp2Prev->m_Next;
        tmp2 = tmp2->m_Next;
      }
    }
    tmp = tmp->m_Next;
  }

  //vratenie ukazatela na hlavu spojoveho zoznamu s vysledkami
  return st;
}

TRESULT * CommonAncestors(int ID1, int ID2){
  //premenne
  TRESULT *firstAncestors = NULL, *secondAncestors = NULL, *res = NULL, *tmp = NULL, *loop1, *loop2;

  //najdenie predkov hladanych 
  firstAncestors = Ancestors(ID1);
  secondAncestors = Ancestors(ID2);

  
  //kombinovanie spolocnych predkov do spojaka
  loop1 = firstAncestors;
  while(loop1){
    loop2 = secondAncestors;
    while(loop2){
      if(loop1->m_ID == loop2->m_ID){
        tmp = (TRESULT*) malloc(sizeof(TRESULT));
        tmp->m_ID = loop1->m_ID;
        tmp->m_Name = (char*) malloc((strlen(loop1->m_Name)+1)*sizeof(char));
        strcpy(tmp->m_Name,loop1->m_Name);
        tmp->m_Next = res;
        res = tmp;
      }
      loop2 = loop2->m_Next;
    }
    loop1 = loop1->m_Next;
  }

  FreeResult(firstAncestors);
  FreeResult(secondAncestors);


  //vratenie ukazatela na hlavu spojaku s vysledkami
  return res;
}

#ifndef __PROGTEST__
int main(void)
 {
   char name[100];
   TRESULT * l;

   Init ();
   assert ( Register ( 1, "John", 0, 0 ) == 1 );
   strncpy ( name, "Jane", sizeof ( name ) );
   assert ( Register ( 2, name, 0, 0 ) == 1 );
   assert ( Register ( 3, "Caroline", 0, 0 ) == 1 );
   assert ( Register ( 4, "Peter", 0, 0 ) == 1 );
   assert ( Register ( 5, "George", 1, 2 ) == 1 );
   assert ( Register ( 6, "Martin", 1, 2 ) == 1 );
   assert ( Register ( 7, "John", 3, 4 ) == 1 );
   assert ( Register ( 8, "Sandra", 3, 4 ) == 1 );
   assert ( Register ( 9, "Eve", 1, 2 ) == 1 );
   assert ( Register ( 10, "Douglas", 1, 4 ) == 1 );
   strncpy ( name, "Phillipe", sizeof ( name ) );
   assert ( Register ( 11, name, 6, 8 ) == 1 );
   strncpy ( name, "Maria", sizeof ( name ) );
   assert ( Register ( 12, name, 5, 8 ) == 1 );

   l = Ancestors ( 11 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=6, name="Martin"
    * ID=8, name="Sandra"
    */
   FreeResult ( l );
   assert ( Ancestors ( 3 ) == NULL );
   assert ( Ancestors ( 13 ) == NULL );
   
   l = CommonAncestors ( 11, 12 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=8, name="Sandra"
    */
   FreeResult ( l );
   l = CommonAncestors ( 10, 9 );
   /*
    * ID=1, name="John"
    */
   FreeResult ( l );
   assert ( CommonAncestors ( 7, 6 ) == NULL );
   l = CommonAncestors ( 7, 10 );
   /*
    * ID=4, name="Peter"
    */
   FreeResult ( l );
   assert ( Register ( 13, "Quido", 12, 11 ) == 1 );
   l = Ancestors ( 13 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=5, name="George"
    * ID=6, name="Martin"
    * ID=8, name="Sandra"
    * ID=11, name="Phillipe"
    * ID=12, name="Maria"
    */
   FreeResult ( l );
   l = CommonAncestors ( 9, 12 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    */
   FreeResult ( l );
   assert ( Register ( 1, "Francois", 0, 0 ) == 0 );
   Done ();
  
   Init ();
   assert ( Register ( 10000, "John", 0, 0 ) == 1 );
   assert ( Register ( 10000, "Peter", 0, 0 ) == 0 );
   assert ( Register ( 20000, "Jane", 10000, 0 ) == 1 );
   assert ( Register ( 30000, "Maria", 10000, 10000 ) == 0 );
   assert ( Register ( 40000, "Joe", 10000, 30000 ) == 0 );
   assert ( Register ( 50000, "Carol", 50000, 20000 ) == 0 );
   Done ();

   return 0;
 }
#endif /* __PROGTEST__ */

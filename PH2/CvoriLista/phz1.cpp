#include <iostream>
#include <stdexcept>


template <typename Tip>
class Lista {

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const=0;
    virtual const Tip &trenutni() const =0;
    virtual bool prethodni()=0;
    virtual bool sljedeci() =0;
    virtual void pocetak() =0;
    virtual void kraj() =0;
    virtual void obrisi() =0;
    virtual void dodajIspred(const Tip& el) =0;
    virtual void dodajIza(const Tip& el) =0;
    virtual const Tip &operator [](int i) const =0;

};



template< typename Tip >
class NizLista : public Lista<Tip> {
    
    int kapacitet;
    int broj_el = 0;
    int t_index = -1;
    Tip **niz;
    
public:

//KONSTRUKTOR
    NizLista(): kapacitet(1000) , broj_el(0), t_index(-1){
        niz = new Tip* [kapacitet];
    }

//KOPIRAJUCI KONSTRUKTOR
    NizLista(const NizLista &l) : niz(new Tip* [l.kapacitet]), kapacitet(l.kapacitet), broj_el(l.broj_el), t_index(l.t_index) {
         
        for(int i=0; i < l.broj_el; i++){
            niz[i] = new Tip( *l.niz[i] );
        }
    }

//POMIJERAJUCI KONSTRUKTOR
    NizLista(NizLista &&l) : niz(l.niz), kapacitet(l.kapacitet), broj_el(l.broj_el), t_index(l.t_index){
        
        l.niz = nullptr; l.broj_el = 0; l.t_index = -1;
    }

//KOPIRAJUCI OPERATOR DODJELE
    NizLista &operator =(const NizLista &l){
        if(kapacitet != l.kapacitet){
            if( kapacitet < l.kapacitet ){
                Tip** temp;
                temp = new Tip* [l.kapacitet];
                for(int i=0; i<broj_el; i++){
                    temp[i] = niz[i];
                }
                delete[] niz;
                niz = temp;
            }
        }
 
        if(l.broj_el > broj_el){
                
            for(int i=0; i<broj_el; i++){
                *niz[i] = *l.niz[i];
            }
            for(int i=broj_el; i<l.broj_el; i++){
                niz[i] = new Tip(*l.niz[i]);
            }
                
        }else{
            
            for(int i=0; i<l.broj_el; i++){
                *niz[i] = *l.niz[i];
            }
            for(int i = broj_el; i < l.broj_el ; i++) {
                delete niz[i];
            }
        }
        t_index = l.t_index;
        broj_el = l.broj_el;
        
        return *this;
    }
        
//POMIJERAJUCI OPERATOR DODJELE
    NizLista &operator =(NizLista &&l){
        if(&l != this){
            for(int i=0; i<broj_el; i++) delete niz[i];
            delete[] niz;
            broj_el = l.broj_el; niz = l.niz; t_index = l.t_index;
            l.niz = nullptr; l.broj_el = 0; t_index = -1;
        }
        return *this;
    }

//DESTRUKTOR
    ~NizLista() {
        for(int i=0; i<broj_el; i++){
            delete niz[i];
        }
        
        delete[] niz;
    }
    
//BEOJ ELEMENATA

    int brojElemenata() const override { return broj_el; }
   
//TRENUTNI 

    Tip &trenutni() {
        if(broj_el == 0 || t_index == -1) throw std::domain_error("Prazna lista!");
    
        return *(niz[t_index]);
    }
    
//CONST TRENUTNI 

    const Tip &trenutni() const override {
        if(broj_el == 0 || t_index == -1) throw std::domain_error("Prazna lista!");
    
        return *(niz[t_index]);
    }

//PRETHODNI   
    bool prethodni() override {
        if(broj_el == 0) throw std::domain_error("Prazna lista!");
        if(t_index == 0){
            return false;
        }
        t_index--;
        return true;
    }

//SLJEDECI   

    bool sljedeci() override {
        
        if(broj_el == 0) throw std::domain_error("Prazna lista!");
        if(t_index == broj_el-1){
            return false;
        }
        t_index++;
        return true;
    }

//POCETAK

    void pocetak() override {
        if(broj_el == 0) throw std::domain_error("Prazna lista!");
        t_index = 0;
    }
    
//KRAJ

   void kraj() override {
        if(broj_el == 0) throw std::domain_error("Prazna lista!");
        t_index = broj_el - 1;
    }
    
//OBRISI   

    void obrisi() override {
        if(broj_el == 0 || t_index == -1) throw std::domain_error("Prazna lista!");
        
        if( t_index == broj_el-1 ){
            t_index--;
            delete niz[broj_el-1];
        }else{
            delete niz[t_index];
            for(int i=t_index+1; i<broj_el; i++){
                niz[i-1]=niz[i];
            }
        }
        broj_el--;
    }
    
//FUNKCIJA ZA PROVJERU KAPACITETA

    void funkcijaZaProvjeruKapaciteta(){
            
        if(broj_el == kapacitet){
            kapacitet *= 100;
            Tip **temp;
            temp = new Tip*[kapacitet];
            
            for(int i=0; i<broj_el; i++){
                temp[i] = niz[i];
            }
            delete[] niz;
            niz = temp;
        }   
    }
    
    
//DODAJ ISPRED 

    void dodajIspred(const Tip& el) override {
        funkcijaZaProvjeruKapaciteta();
        
        if(broj_el == 0){
            niz[0] = new Tip(el);
        }else{
            for(int i=broj_el; i>t_index; i--){
                niz[i]=niz[i-1];
            }
            niz[t_index] = new Tip(el);
        }
        t_index++;
        broj_el++;
    }
    
//DODAJ IZA

    void dodajIza(const Tip& el) override {
        funkcijaZaProvjeruKapaciteta();
        
        if(broj_el == 0){
            niz[broj_el] = new Tip(el);
            t_index++;
            
        }else{
            for(int i = broj_el ; i>t_index+1; i--){
                niz[i] = niz[i-1];
            }
            niz[t_index+1] = new Tip(el);
        }
        broj_el++;
        
    }
    
//OPERATOR []
    Tip &operator [](int i) {
        if(i<0 || i>=broj_el ) throw std::domain_error("Pogresan argument!");
        return *niz[i];
    }
    
//CONST OPERATOR []
    const Tip &operator [](int i) const override {
        if(i<0 || i>=broj_el ) throw std::domain_error("Pogresan argument!");
        return *niz[i];
    }
   
};






template< typename Tip >
class JednostrukaLista : public Lista<Tip> {
 
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor (): sljedeci(nullptr){}
        Cvor (const Tip &el, Cvor *sljedeci) : element(el), sljedeci(sljedeci) {}
    };
    int broj_el=0;
    Cvor* poc;
    Cvor* pret;
    Cvor* tren;
    Cvor* sljed;
    
public:

//KONSTRUKTOR
    JednostrukaLista(): broj_el(0), poc(nullptr), pret(nullptr), tren(nullptr), sljed(nullptr){}
    
//KOPIRAJUCI KONSTRUKTOR
    JednostrukaLista(const JednostrukaLista &l): broj_el(l.broj_el) {
        
        Cvor* p = l.poc;
        Cvor* q;
        Cvor* prosli;
        bool nijePrvi(false);
        
        while(p != nullptr){
        
            q=new Cvor;
            if(nijePrvi){
                prosli->sljedeci = q;
            }
            nijePrvi = true;
            
            if(p == l.poc){
                poc = q;
            }
            if(p == l.pret){
                pret = q;
            }
            if(p == l.tren){
                tren = q;
            }
            if(p == l.sljed){
                sljed = q;
            }
            
            q->element = p->element;
            q->sljedeci = nullptr;
            prosli = q;
            
            p = p->sljedeci;
        }
    }
//POMJERAJUCI KONSTRUKTOR

//KOPIRAJUCI OPERATOR DODJELE
    JednostrukaLista &operator =(const JednostrukaLista &l) {
        
        Cvor* p = l.poc;
        Cvor* q;
        Cvor* prosli;
        bool nijePrvi(false);
        
        while(p != nullptr){
        
            q=new Cvor;
            if(nijePrvi){
                prosli->sljedeci = q;
            }
            nijePrvi = true;
            
            if(p == l.poc){
                poc = q;
            }
            if(p == l.pret){
                pret = q;
            }
            if(p == l.tren){
                tren = q;
            }
            if(p == l.sljed){
                sljed = q;
            }
            
            q->element = p->element;
            q->sljedeci = nullptr;
            prosli = q;
            broj_el++;
            
            p = p->sljedeci;
        }
        return *this;
    }

//POMJERAJUCI OPERATOR DODJELE

//DESTRUKTOR
    ~JednostrukaLista(){
        
        Cvor* p = poc;
        while(p != nullptr){
            Cvor* q = p;
            p = p -> sljedeci;
            delete q;
        }
        
    }

//BROJ ELEMENEATA  
    int brojElemenata() const override {
        return broj_el;
    }
    
    
//TRENUTNI    
    Tip &trenutni() {
        if(!poc) throw std::domain_error("Prazna lista!");
        
        return (tren->element);
    }
    
//CONST TRENUTNI
    const Tip &trenutni() const override {
        if(!poc) throw std::domain_error("Prazna lista!");
        
        return (tren->element);
    }
    
//PRETHODNI
    bool prethodni() override {
        if(!poc) throw std::domain_error("Prazna lista!");
        
        if(poc == tren){
            return false;
        }
        
        Cvor* p = poc;
        while(p != nullptr){
            if(p == pret) break;
                
            p = p->sljedeci;
        }
        sljed = tren;
        tren = pret;
        pret = p;
        
        return true;
    }
    
//SLJEDECI
    bool sljedeci() override {
        if(!poc) throw std::domain_error("Prazna lista!");
        
        if(tren->sljedeci == nullptr){
            return false;
        }
        pret = tren;
        tren = sljed;
        if(!sljed->sljedeci){
            sljed = tren;
        }else{
            sljed = sljed->sljedeci;
        }
        return true;
    }
   
//POCETAK
    void pocetak() override {
        if(!poc) throw std::domain_error("Prazna lista!");
        tren = poc;
        pret = poc;
        sljed = poc->sljedeci;
    }
//KRAJ
   void kraj() override {
       if(!poc) throw std::domain_error("Prazna lista!");
       
       if( poc != sljed ){
            Cvor* p = poc;
            while(p != nullptr){
                if(!(p->sljedeci)->sljedeci) break;
                p = p->sljedeci;
            }
            pret = p;
            sljed = p->sljedeci;
            tren = p->sljedeci;
       }
   }

//OBRISI
    void obrisi() override {
        if(!poc) throw std::domain_error("Prazna lista!");
        
        if( poc->sljedeci == nullptr ){
            pret = nullptr;
            tren = nullptr;
            sljed = nullptr;
            
            delete poc;
            poc = nullptr;
        }else if(poc == tren){
            
            tren = sljed;
            pret = sljed;
            delete poc;
            poc = tren;
            
            if(!sljed->sljedeci){
                
            }else{
                sljed = sljed->sljedeci;
            }
            
        }else if(!sljed->sljedeci){
            
            if(poc == pret){
                sljed = poc;
                delete tren;
                tren = sljed;
                tren->sljedeci = nullptr;
            }else{
                Cvor* p;
                for(p=poc; p!=nullptr; p = p->sljedeci){
                    if(p->sljedeci == pret) break;
                }
                pret = p;
                sljed = pret -> sljedeci;
                delete tren;
                tren = sljed;
                tren->sljedeci = nullptr;
            }
            
        }else{
            delete tren;
            tren = sljed;
            pret->sljedeci = tren;
            if(!sljed->sljedeci){
                
            }else{
                sljed = sljed->sljedeci;
            }
        }
        broj_el--;
    }
  
//DODAJ ISPRED
    void dodajIspred(const Tip& el) override {
        if( !poc ){
            poc = new Cvor;
            
            poc->element = el;
            poc->sljedeci = nullptr;
            
            tren = poc;
            pret = poc;
            sljed = poc;
            
        }else if( poc == tren ){
            pret = new Cvor;
            
            pret -> element = el;
            pret -> sljedeci = tren;
            
            poc = pret;
            
        }else{
             Cvor* novi = new Cvor;
             
             novi -> element = el;
             novi -> sljedeci = tren;
             pret -> sljedeci = novi;
             pret = novi;
             
        }
        broj_el++;
    }
    
//DODAJ IZA    
    void dodajIza(const Tip& el) override {
        
        if(!poc){
            poc = new Cvor;
            
            poc->element = el;
            poc->sljedeci = nullptr;
            
            tren = poc;
            pret = poc;
            sljed = poc;
            
        }else if( tren->sljedeci == nullptr ){
            sljed = new Cvor;
            
            sljed->element = el;
            sljed->sljedeci = nullptr;
            
            tren->sljedeci = sljed;
        }else{
            Cvor* novi = new Cvor;
            
            novi->element = el;
            novi->sljedeci = sljed;
            tren->sljedeci = novi;
            
            sljed = novi;
            
        }
    
        broj_el++;
    }
    
//OPERATOR []
    Tip &operator [](int i) {
        int j=0;
        Cvor* p = poc;
        while (p != nullptr) {
            if(i==j) break;
            p = p->sljedeci;
            j++;
        }
        return (p->element);
    }
    
// CONST OPERATOR []
    const Tip &operator [](int i) const override {
        int j=0;
        Cvor* p = poc;
        while (p != nullptr) {
            if(i==j) break;
            p = p->sljedeci;
            j++;
        }
        return (p->element);
    }
 
};





//Testna funkcija klase NizLista za konstruktor bez parametara, te metode: broj elemenata, dodaj iza, trenutni, prethodni, sljedeci, pocetak i kraj. 
bool testnaFunkcija1(){
    NizLista<int> lista;
    for(int i=0; i<4; i++){
        lista.dodajIza(i+2);
    }
    if( lista.brojElemenata() == 4 ){
        if( lista.trenutni() == 2 && ! lista.prethodni() ){
            if( lista.sljedeci() && lista.sljedeci() ){
                if( lista.trenutni() == 4){ 
                    lista.pocetak();
                    if( lista.trenutni() == 2 ){
                        lista.kraj();
                        if(lista.trenutni() == 3){
                            return true;
                            
                        }
                        return false;
                    }
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}

//Testna funkcija klase JednostrukaLista za konstruktor bez parametara, te metode: broj elemenata, dodaj iza, trenutni, prethodni, sljedeci, pocetak i kraj.
bool testnaFunkcija2(){
    
    JednostrukaLista<int> lista;
    for(int i=0; i<4; i++){
        lista.dodajIza(i+2);
    }
    
    if( lista.brojElemenata() == 4 ){
        if( lista.trenutni() == 2 && ! lista.prethodni() ){
            if( lista.sljedeci() && lista.sljedeci() ){
                if( lista.trenutni() == 4){ 
                    lista.pocetak();
                    if( lista.trenutni() == 2 ){
                        lista.kraj();
                        if(lista.trenutni() == 3){
                            return true;
                            
                        }
                        return false;
                    }
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}

//Testna f-ja za klasu NizLista. Testira: kopirajuci operator dodjele. Te metode: dodaj ispred, obrisi, operator[].
//Kao i dodatne provjere za metodu kraj i trenutni.
bool testnaFunkcija3(){
    NizLista<int> lista;
    for(int i=0; i<4; i++){
        lista.dodajIspred(i+3);
    }
    NizLista<int> nova_lista;
    nova_lista = lista;
    for(int i=0; i<4; i++){
        if( lista[i] != nova_lista[i] ) return false;
    }
    
    for(int i=0; i<4; i++){
        nova_lista.obrisi();
        if(i==0) nova_lista.pocetak();
    }
    
    if( nova_lista.brojElemenata() == 0 && lista.brojElemenata() != 4) return false;
    
    int broj_el_liste(lista.brojElemenata());
    lista[broj_el_liste-1] = 1000;
    lista.kraj();
    
    if( lista.trenutni() != 1000 ) return false;
    
    return true;
}


//Testna f-ja za klasu JednostrukaLista. Testira: kopirajuci operator dodjele. Te metode: dodaj ispred, obrisi, operator[].
//Kao i dodatne provjere za metode kraj i trenutni.
bool testnaFunkcija4(){
    JednostrukaLista<int> lista;
    for(int i=0; i<4; i++){
        lista.dodajIspred(i+3);
    }
    JednostrukaLista<int> nova_lista;
    nova_lista = lista;
    for(int i=0; i<4; i++){
        if( lista[i] != nova_lista[i] ) return false;
    }
    
    for(int i=0; i<4; i++){
        nova_lista.obrisi();
        if(i==0) nova_lista.pocetak();
    }
    
    if( nova_lista.brojElemenata() == 0 && lista.brojElemenata() != 4) return false;
    
    int broj_el_liste(lista.brojElemenata());
    lista[broj_el_liste-1] = 1000;
    lista.kraj();
    
    if( lista.trenutni() != 1000 ) return false;
    
    return true;
}

//Testna f-ja za klase NizLista i JednostrukaLista. Testira da li su klase genericke, te polimorfizam.
//Kao i dodatne provjere za neke od metoda.

bool testnaFunkcija5(){
    Lista<double>* lista;
    lista = new NizLista<double>;
    for(int i=0; i<4; i++){
        lista->dodajIspred(i+0.15);
    }
    
    if( abs(lista->trenutni() - 0.15) < 0.0001 && lista->brojElemenata() != 4) return false;
    
    delete lista;
    
    Lista<char>* l;
    l = new JednostrukaLista<char>;
    for(int i=0; i < 4; i++){
        l->dodajIza('a' + i);
    }
    
    if( l->trenutni() != 'a' && lista->brojElemenata() != 4) return false;
    
    delete l;
    
    return true;
}



int main() {  
    
   if(testnaFunkcija1()){
       std::cout << "Test 1 je ispravan!" << std::endl;
   }else{
       std::cout << "Test 1 Nije ispravan!" << std::endl;
   }
   
   if(testnaFunkcija2()){
       std::cout << "Test 2 je ispravan!" << std::endl;
   }else{
       std::cout << "Test 2 Nije ispravan!" << std::endl;
   }
   
   if(testnaFunkcija3()){
       std::cout << "Test 3 je ispravan!" << std::endl;
   }else{
       std::cout << "Test 3 Nije ispravan!" << std::endl;
   }
   
   if(testnaFunkcija4()){
       std::cout << "Test 4 je ispravan!" << std::endl;
   }else{
       std::cout << "Test 4 Nije ispravan!" << std::endl;
   }
   
   if(testnaFunkcija5()){
       std::cout << "Test 5 je ispravan!" << std::endl;
   }else{
       std::cout << "Test 5 Nije ispravan!" << std::endl;
   }
    
   return 0;
}

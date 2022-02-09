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
    int broj_el;
    int t_index;
    Tip **niz;
    
public:

//KONSTRUKTOR
    NizLista(): kapacitet(1000) , broj_el(0), t_index(-1){
        niz = new Tip* [kapacitet];
    }

//KOPIRAJUCI KONSTRUKTOR
    NizLista(const NizLista &l) {
        niz = new Tip* [l.kapacitet]; 
        kapacitet = l.kapacitet; 
        broj_el = l.broj_el;
        t_index = l.t_index;
         
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
class Stek{
    
    NizLista<Tip> stek;
    
    public:
    Stek(){}
    
    Stek(const Stek &s) : stek(s.stek) {}
    
    Stek &operator =(const Stek &s){
        if( this != &s)
            stek = s.stek;
        
        return *this;
    }
    
    void brisi(){
        int n(stek.brojElemenata());
    
        for(int i=0; i<n; i++){
            stek.obrisi();
        }
    }
    
    void stavi(const Tip &el){
        stek.dodajIza(el);
        stek.kraj();
    }
    
    Tip skini(){
        if( !stek.brojElemenata() ) throw std::domain_error("Prazna stek!");
        
        Tip obrisani = stek.trenutni();
        stek.obrisi();
        
        return obrisani;
    }
    
    Tip &vrh(){
        if( !stek.brojElemenata() ) throw std::domain_error("Prazna stek!");
        
        return stek.trenutni();    
    }
    
    int brojElemenata(){
        return (stek.brojElemenata());
    }
    
};

// Prva testna f-ja testira metode: stavi, brojElelmenata, vrh, skini; 

bool testnaFunkcija1(){
    Stek<int> stek;
    for(int i=0; i<3; i++){
        stek.stavi(i+2);
    }
    int n(stek.vrh());
    if( n != 4 && stek.brojElemenata() != 3) return false;
    
    stek.stavi(10);
    
    int obrisani(stek.skini());
    if( obrisani != 10 && stek.brojElemenata() != 3) return false;
    
    return true;
}


// Druga testna f-ja testira konstruktor, kopirajuci konstruktor, destruktivnu samododjelu i kopirajuci operator dodjele. Te metodu obrisi.

bool testnaFunkcija2(){
    Stek<int> stek;
    for(int i=0; i<5; i++){
        stek.stavi(i+5);
    }
    
    Stek<int> s1(stek);
    Stek<int> s2;
    
    if(s1.brojElemenata() != stek.brojElemenata()) return false;
    
    s1.brisi();
    
    if(s1.brojElemenata() == stek.brojElemenata() || s1.brojElemenata() != 0) return false;
    
    s2 = stek;
    
    if(s2.brojElemenata() != stek.brojElemenata()) return false;
    
    s2.brisi();
    
    if(s2.brojElemenata() == stek.brojElemenata() || s2.brojElemenata() != 0) return false;
    
    stek = stek; 
    
    if(stek.brojElemenata() != 5) return false;
    
    stek.brisi();
    
    if(stek.brojElemenata() != 0) return false;
   
    return true;
}

// Druga testna f-ja testira da li je klasa Stek generička; 

bool testnaFunkcija3(){
    Stek<float> stek;
    for(int i=0; i<3; i++){
        stek.stavi(i+2.15);
    }
    int n(stek.vrh());
    if( abs(n - 4.15) < 0.000001 && stek.brojElemenata() != 3) return false;
    
    stek.stavi(10.0675);
    
    int obrisani(stek.skini());
    
    if( abs(obrisani - 10.0675) < 0.000001 && stek.brojElemenata() != 3) return false;

    Stek<char> s1; 
    for(int i=0; i<3; i++){
        s1.stavi('A'+1);
    }
    
    char v( s1.vrh() );
    if( v != 'D' && s1.brojElemenata() != 3) return false;
   
    s1.stavi('Z');
    
    int bivsi(s1.skini());
    if( bivsi != 'Z' && s1.brojElemenata() != 3) return false;
   
    return true;
}



int main() {
    
    if( testnaFunkcija1() ){
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
    
    return 0;
}

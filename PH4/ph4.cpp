#include <iostream>
#include <stdexcept>


template< typename Tip >
class Red{
    struct Cvor{
        Tip element;
        Cvor* sljedeci;
        Cvor() : sljedeci(nullptr){}
        Cvor(const Tip &el, Cvor *sljedeci) : element(el), sljedeci(sljedeci) {} 
    };
    
    int br_elemenata;
    Cvor* pocetak;
    Cvor* kraj;
    
    public: 
    
    Red(): br_elemenata(0), pocetak(nullptr), kraj(nullptr) {}
    
    Red(const Red &r){
        br_elemenata = r.br_elemenata;
        Cvor* p = r.pocetak;
        Cvor* q;
        
        while (p != nullptr) {
            q = new Cvor();
            
            if(p == r.pocetak){
                pocetak = q;
                kraj = q;
            }else{
                
                kraj->sljedeci = q;
                kraj = kraj->sljedeci;
                
            }
            
            q->element = p->element;
            kraj->sljedeci = nullptr;
            p = p->sljedeci;
        }
    }
    
    Red &operator =(const Red &r){
        
        if(&r != this){
            
            if(br_elemenata == r.br_elemenata){
                Cvor* p = r.pocetak;
                Cvor* q = pocetak;
                while (p != nullptr) {
                    
                    q->element = p->element;
                    
                    q = q->sljedeci;
                    p = p->sljedeci;
                }
                
            }else if(br_elemenata < r.br_elemenata){
                
                Cvor *p = r.pocetak;
                Cvor *q = pocetak;
                
                while (q != nullptr) {
                    
                    if(p == r.pocetak){
                        pocetak = q;
                        kraj = q;
                    }else{
                        kraj->sljedeci = q;
                        kraj = kraj->sljedeci;
                    }
                    
                    q->element = p->element;
                    kraj->sljedeci = nullptr;
                    p = p->sljedeci;
                    q = q->sljedeci;
                }
                
                
                while (p != nullptr) {
                    
                    q = new Cvor();
                    
                    if(p == r.pocetak){
                        pocetak = q;
                        kraj = q;
                    }else{
                        kraj->sljedeci = q;
                        kraj = kraj->sljedeci;
                    }
                    
                    q->element = p->element;
                    kraj->sljedeci = nullptr;
                    p = p->sljedeci;
                }
            }else{
                
            }
            
        }
        br_elemenata = r.br_elemenata;
        return *this;
    }
    
    ~Red(){
        while(pocetak != nullptr){
            kraj = pocetak;
            pocetak = pocetak->sljedeci;
            delete kraj;
        }
    }
    
    void brisi(){
        while(pocetak != nullptr){
            kraj = pocetak;
            pocetak = pocetak->sljedeci;
            delete kraj;
        }
        pocetak = nullptr;
        kraj = nullptr;
        br_elemenata = 0;
    }
    
    void stavi(const Tip& el){
        if(!pocetak){
            pocetak = new Cvor();
            pocetak->element = el;
            kraj = pocetak;
            kraj->sljedeci = nullptr;
        }else{
            kraj->sljedeci = new Cvor();
            kraj = kraj->sljedeci;
            kraj->element = el;
            kraj->sljedeci = nullptr;
        }
        br_elemenata++;
    }
    
    Tip skini(){
        if( br_elemenata == 0 ) throw std::domain_error("Red prazan!");
        
        Tip el = pocetak->element;
        Cvor* brisi;
        brisi = pocetak;
        pocetak = pocetak->sljedeci;
        delete brisi;
        br_elemenata--;
        
        return el;
    }
    
    Tip &celo(){
        if( br_elemenata == 0 ) throw std::domain_error("Red prazan!");
        
        return pocetak->element;
    }
    
    int brojElemenata(){
        return br_elemenata;
    }
    
};



// Prva testna f-ja testira metode: stavi, brojElelmenata, celo, skini; 

bool testnaFunkcija1(){
    Red<int> red;
    for(int i=0; i<3; i++){
        red.stavi(i+2);
    }
    int n(red.celo());
    if( n != 2 && red.brojElemenata() != 3) return false;
    
    red.stavi(10);
    
    int obrisani(red.skini());
    if( obrisani != 2 && red.brojElemenata() != 3) return false;
    
    return true;
}


// Druga testna f-ja testira konstruktor, kopirajuci konstruktor, destruktivnu samododjelu i kopirajuci operator dodjele. Te metodu brisi.

bool testnaFunkcija2(){
    
    Red<int> red;
    
    for(int i=0; i<5; i++){
        red.stavi(i+5);
    }
    
    Red<int> r1(red);
    Red<int> r2;
    
    if(r1.brojElemenata() != red.brojElemenata()) return false;
    
    r1.brisi();
    
    if(r1.brojElemenata() == red.brojElemenata() || r1.brojElemenata() != 0) return false;
    
    r2 = red;
    
    if(r2.brojElemenata() != red.brojElemenata()) return false;
    
    r2.brisi();
    
    if(r2.brojElemenata() == red.brojElemenata() || r2.brojElemenata() != 0) return false;
    
    red = red; 
    
    if(red.brojElemenata() != 5) return false;
    
    red.brisi();
    
    if(red.brojElemenata() != 0) return false;
   
    return true;
}

// Druga testna f-ja testira da li je klasa Red generička; 

bool testnaFunkcija3(){
    Red<float> red;
    for(int i=0; i<3; i++){
        red.stavi(i+2.15);
    }
    int n(red.celo());
    if( abs(n - 2.15) < 0.000001 && red.brojElemenata() != 3) return false;
    
    red.stavi(10.0675);
    
    int obrisani(red.skini());
    
    if( abs(obrisani - 2.0675) < 0.000001 && red.brojElemenata() != 3) return false;

    Red<char> r1; 
    for(int i=0; i<3; i++){
        r1.stavi('A'+1);
    }
    
    char v( r1.celo() );
    if( v != 'D' && r1.brojElemenata() != 3) return false;
   
    r1.stavi('Z');
    
    int bivsi(r1.skini());
    if( bivsi != 'Z' && r1.brojElemenata() != 3) return false;
   
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

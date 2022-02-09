#include <iostream>
#include <utility>
#include <random>
#include <ctime>


using namespace std;


template < typename TipKljuc, typename TipVrijednost >
class Mapa{
    
public:

    Mapa(){}
    virtual ~Mapa() {}
    virtual int brojElemenata() const=0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuc& kljuc) =0;
    virtual TipVrijednost& operator [](const TipKljuc &k) =0;
    virtual const TipVrijednost operator [](const TipKljuc &k) const =0;
    
};



template < typename TipKljuc, typename TipVrijednost >
class NizMapa : public Mapa<TipKljuc, TipVrijednost> {
    
    int kapacitet;
    int broj_el = 0;
    pair<TipKljuc, TipVrijednost>** niz;
    void alocirajNovo();
    
public:
    
// Konstruktor bez parametara
    NizMapa(): kapacitet(1000), broj_el(0){
        niz = new pair<TipKljuc, TipVrijednost>* [kapacitet]; 
    }
    
    
//KOPIRAJUCI KONSTRUKTOR
    NizMapa(const NizMapa &m) : niz(new pair<TipKljuc, TipVrijednost>* [m.kapacitet]), kapacitet(m.kapacitet), broj_el(m.broj_el){
        
        for(int i=0; i < m.broj_el; i++){
            niz[i] = new pair<TipKljuc, TipVrijednost>( *m.niz[i] );
        }
    }


//POMIJERAJUCI KONSTRUKTOR
    NizMapa(NizMapa &&m) : niz(m.niz), kapacitet(m.kapacitet), broj_el(m.broj_el) {
        
        m.niz = nullptr; m.broj_el = 0; 
    }
    


//KOPIRAJUCI OPERATOR DODJELE
    NizMapa &operator =(const NizMapa &m){
        if( kapacitet < m.kapacitet ){
            kapacitet = m.kapacitet;
            alocirajNovo();
        }
 
        if(m.broj_el > broj_el){
                
            for(int i=0; i<broj_el; i++){
                *niz[i] = *m.niz[i];
            }
            for(int i=broj_el; i<m.broj_el; i++){
                niz[i] = new pair<TipKljuc, TipVrijednost>(*m.niz[i]);
            }
                
        }else{
            
            for(int i=0; i<m.broj_el; i++){
                *niz[i] = *m.niz[i];
            }
            for(int i = broj_el; i < m.broj_el ; i++) {
                delete niz[i];
            }
            
        }
        broj_el = m.broj_el;
        
        return *this;
    }
    
  
//POMIJERAJUCI OPERATOR DODJELE
    NizMapa &operator =(NizMapa &&m){
        if(&m != this){
            if(kapacitet < m.kapacitet){
                kapacitet = m.kapacitet;
                alocirajNovo();
            }
            for(int i=0; i<broj_el; i++) delete niz[i];
            delete[] niz;
            broj_el = m.broj_el; niz = m.niz;
            m.niz = nullptr; m.broj_el = 0;
        }
        return *this;
    }
    
        
    
//Destruktor
    ~NizMapa() {
        for(int i=0; i<broj_el; i++){
            delete niz[i];
        }
        delete[] niz;
    }
    
//OPERATOR []
    TipVrijednost &operator [](const TipKljuc &k) override {
        
        if( broj_el == kapacitet ){
            kapacitet *= 100;
            alocirajNovo();
        }
        
        int i;
        for(i=0; i<broj_el; i++){
            if( k == niz[i]->first ) return niz[i]->second;
        }
        
        if(i==broj_el){
            niz[broj_el] = new pair<TipKljuc, TipVrijednost>;
            niz[i]->first = k;
            broj_el++;
        }
        
        return niz[i]->second;
    }
    
//CONST OPERATOR []
    const TipVrijednost operator [](const TipKljuc &k) const  {
        
        
        for(int i=0; i<broj_el; i++){
            if( k == niz[i]->first ){
                return niz[i]->second;
            }
        }
        
        return TipVrijednost();
    }
    
    
//BEOJ ELEMENATA
    int brojElemenata() const override { return broj_el; }
    
    
//OBRISI SVE
    void obrisi() override {
        for(int i=0; i<broj_el; i++){
            delete niz[i];
        }
        broj_el = 0;
    }
    
    
//OBRISI
    void obrisi(const TipKljuc& kljuc) override{
        int i;
        for(i=0; i<broj_el; i++){
            if(kljuc == (*niz[i]).first){
               delete niz[i];
               break;
            }
        }
        while(i<broj_el){
            niz[i] = niz[i+1];
            i++;
        }
        broj_el--;
    }
   
};








template< typename TipKljuc, typename TipVrijednost >
void NizMapa<TipKljuc, TipVrijednost>::alocirajNovo(){
    pair<TipKljuc, TipVrijednost>** temp;
    temp = new pair<TipKljuc, TipVrijednost>* [kapacitet];
    for(int i=0; i<broj_el; i++){
        temp[i] = niz[i];
    }
    delete[] niz;
    niz = temp;
    
}







template< typename TipKljuc, typename TipVrijednost >
class BinStabloCvor {
    
public:


    BinStabloCvor *lijevo, *desno, *roditelj;
    TipKljuc kljuc;
    TipVrijednost value = TipVrijednost();

    BinStabloCvor() { lijevo=desno=roditelj = nullptr; }
    
    BinStabloCvor(const TipKljuc& key, const TipVrijednost&  v, BinStabloCvor* r){
        kljuc = key;
        value = v;
        roditelj = r;
        lijevo = nullptr;
        desno = nullptr;
    }
    
};





template <typename TipKljuc, typename TipVrijednost>
class BinStabloMapa : public Mapa<TipKljuc, TipVrijednost> {
    
   BinStabloCvor<TipKljuc, TipVrijednost>* korijen;
   int broj_el=0;
   
   void obrisiStablo(BinStabloCvor<TipKljuc, TipVrijednost>* korijen);
   void kopirajStablo(BinStabloCvor<TipKljuc, TipVrijednost>*& k, BinStabloCvor<TipKljuc, TipVrijednost>* sKorijen , BinStabloCvor<TipKljuc, TipVrijednost>* r );
   
public:


    BinStabloMapa() : korijen(nullptr), broj_el(0) {
        
    }   
    
    
//Kopirajuci konstruktor

    BinStabloMapa(const BinStabloMapa& s){
        kopirajStablo(korijen, s.korijen, nullptr);
    }
    
    
//Kopirajuci operator dodjele

    BinStabloMapa &operator =(const BinStabloMapa &s){
        if(this != &s){
            obrisiStablo(korijen);
            korijen = nullptr;
            broj_el = 0;
            kopirajStablo(korijen, s.korijen, nullptr);
        }
        return *this;
    }
    
    
    
//Destruktor    
    
    ~BinStabloMapa() {
        
        obrisiStablo(korijen);
        
    }
    

// Broj elemenata
    
    int brojElemenata() const override {
        return broj_el;
    }
    
    
//Obrisi sve

    
    void obrisi(){
        obrisiStablo(korijen);
        korijen = nullptr;
        broj_el = 0;
    }
    
    
//Obrisi cvor sa kljucem k   
    
    void obrisi(const TipKljuc& k){
        
        BinStabloCvor<TipKljuc, TipVrijednost>* p(korijen),* pret(nullptr), * temp(nullptr), * pm(nullptr), *tmp(nullptr);
        
        while( p != nullptr && k != p->kljuc ){
            
            pret = p;
            
            if( k < p->kljuc ){
                p = p->lijevo;
            }else{
                p = p->desno;
            }
        }
        
        if(p == nullptr) return;
        
        if(p->lijevo == nullptr){
            temp = p->desno;
        }else{
            
            if(p->desno == nullptr){
                temp = p->lijevo;
            }else{
                
                pm = p;
                temp = p->lijevo;
                tmp = temp->desno;
                
                while (tmp != nullptr) {
                    pm = temp;
                    temp=tmp;
                    tmp = temp->desno;
                }
                
                if(pm != p){
                    pm->desno = temp->lijevo;
                    temp -> lijevo =  p->lijevo;
                }
                
                temp -> desno = p -> desno;
                
            }
            
        }
        
        
        if(pret == nullptr){
            korijen = temp;
        }else{
            if(p == pret -> lijevo){
                pret->lijevo = temp;
            }else{
                pret->desno = temp;
            }
        }
        
        delete p;
        broj_el--;
        
    }
    

//Operator []    
    
    TipVrijednost& operator [](const TipKljuc &k) override {
        
        if(korijen == nullptr){
            korijen = new BinStabloCvor< TipKljuc, TipVrijednost >();
            korijen -> kljuc = k;
            korijen -> lijevo = nullptr;
            korijen -> desno = nullptr;
            korijen -> roditelj = nullptr;
            broj_el++;
            
            return korijen -> value;
        }
        
        BinStabloCvor<TipKljuc, TipVrijednost>* poc(korijen),* pret(korijen);
        bool right;
        
        while( poc != nullptr ){
            
            pret = poc;
            
            if( k == poc -> kljuc ){
                return poc -> value;
            }else if(k < poc -> kljuc){
                poc = poc -> lijevo;
                right = false;
            }else{
                poc = poc -> desno;
                right = true;
            }
            
        }
        
        poc = new BinStabloCvor< TipKljuc, TipVrijednost >();
        poc -> kljuc = k;
        poc -> lijevo = nullptr;
        poc -> desno = nullptr;
        poc -> roditelj = pret;
        broj_el++;
        
        if(right){
            pret->desno = poc;
        }else{
            pret->lijevo = poc;
        }
        return poc -> value;
    }
    
    
 
//CONST Operator []

    const TipVrijednost operator [](const TipKljuc &k) const {
        
        BinStabloCvor<TipKljuc, TipVrijednost>* poc = korijen;
        while( poc != nullptr ){
            
            if( k == poc->kljuc){
                return poc->value;
            }else if(k < poc->kljuc){
                poc = poc->lijevo;
            }else{
                poc = poc->desno;
            }
        }
        
        return TipVrijednost();
    }

};


template< typename TipKljuc, typename TipVrijednost >
void BinStabloMapa<TipKljuc, TipVrijednost>::obrisiStablo(BinStabloCvor<TipKljuc, TipVrijednost>* cvor){
    
    if(cvor == nullptr) return;
    
    obrisiStablo(cvor->lijevo);
    obrisiStablo(cvor->desno);
    
    delete cvor;
}



template< typename TipKljuc, typename TipVrijednost >
void BinStabloMapa<TipKljuc, TipVrijednost>::kopirajStablo(BinStabloCvor<TipKljuc, TipVrijednost>*& cvor, BinStabloCvor<TipKljuc, TipVrijednost>* sCvor, 
                                                           BinStabloCvor<TipKljuc, TipVrijednost>* r){
    
    if(sCvor == nullptr){
        return;
    }
    
    cvor = new BinStabloCvor<TipKljuc, TipVrijednost>( sCvor->kljuc, sCvor->value, r );
    kopirajStablo(cvor->lijevo, sCvor->lijevo, cvor);
    kopirajStablo(cvor->desno, sCvor->desno, cvor);
    
    broj_el++;
}





int main() {
//Punjene Niza Mapa   

    NizMapa<int, int> m;
    double ukvrijemeNM; 
    int vel(10000);

    clock_t  vrijeme_1 = clock();
    
    for(int i=0; i<vel; i++){
        m[i] = rand()%1000;
    }
    
    clock_t vrijeme_2 = clock();
    
    ukvrijemeNM = ( vrijeme_2 - vrijeme_1 ) / (CLOCKS_PER_SEC / 1000000);

//Punjenje Binarnog stabla mapa

    BinStabloMapa<int, int> m1;
    double ukvrijemeBSM;
    
    vrijeme_1 = clock();
    
    for(int i=0; i<vel; i++){
        m1[i] = rand()%1000;
    }
    
    vrijeme_2 = clock();
    
    ukvrijemeBSM = ( vrijeme_2 - vrijeme_1 ) / (CLOCKS_PER_SEC / 1000000);
    
    cout << "Prosjecno vrijeme dodavanja elemenata u NizMapa je: " << (ukvrijemeNM/vel) << " mikro sec.,"; // Prosječno vrijeme dodavanja elementa u Niz Mapa je 349.051 mikro sekundi
    cout << " a u Binarno Stablo Mapa je: " << (ukvrijemeBSM/vel) << " mikro sec." << endl; // Prosječno vrijeme dodavanja elementa u Binarno stablo Mapa je 427.937 mikro sekundi
    
    vrijeme_1 = clock();
    
    m[rand()%100];
    
    vrijeme_2 = clock();
    
    ukvrijemeNM = ( vrijeme_2 - vrijeme_1 ) / (CLOCKS_PER_SEC / 1000000);
    
    
    vrijeme_1 = clock();
    
    m1[rand()%100];
    
    vrijeme_2 = clock();
    
    ukvrijemeBSM = ( vrijeme_2 - vrijeme_1 ) / (CLOCKS_PER_SEC / 1000000);
    
    cout << "Prosjecno vrijeme pristupa elementu u NizMapa je: " << ukvrijemeNM << " mikro sec., "; // Prosjecno vrijeme pristupa elementu Niza mapa je 540 mikro sekundi
    cout << "a u Binarnom Stablu Mapa je: " << ukvrijemeBSM << " mikro sec."; // Prosjecno vrijeme pristupa elementu Niza mapa je 519 mikro sekundi

    return 0;
}

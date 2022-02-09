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

    
    cout << "Prosjecno vrijeme pristupa elementu u NizMapa je: " << ukvrijemeNM << " mikro secunds."; // Prosjecno vrijeme pristupa elementu Niza mapa je 540 mikro sekundi

    return 0;
}
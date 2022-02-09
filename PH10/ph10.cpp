#include <iostream>
#include <stdexcept>
#include <vector>
#include <iomanip>

using namespace std;


template< typename Tip >
class GranaIterator;

template< typename Tip >
class Grana;

template< typename Tip >
class Cvor;


template< typename Tip >
class UsmjereniGraf{
    
    int br_cvorova;
    
public:

    UsmjereniGraf(int br){}
    virtual ~UsmjereniGraf(){}
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int br)=0;
    virtual void dodajGranu(int p, int d, float tezina)=0;
    virtual void obrisiGranu(int p, int d)=0;
    virtual void postaviTezinuGrane(int p, int d, float tezina)=0;
    virtual float dajTezinuGrane(int p, int d) const = 0;
    virtual bool postojiGrana(int p, int d) const = 0;
    virtual void postaviOznakuCvora (int cvor, Tip oznaka) =0;
    virtual Tip dajOznakuCvora(int cvor) const=0;
    virtual void postaviOznakuGrane(int p, int d, Tip oznaka)=0;
    virtual Tip dajOznakuGrane(int p, int d)const=0;
    Grana<Tip> dajGranu(int p, int d);
    Cvor<Tip> dajCvor(int cvor);
    virtual GranaIterator<Tip> dajGranePocetak()=0;
    virtual GranaIterator<Tip> dajGraneKraj() = 0;
    virtual GranaIterator<Tip> dajSljedecuGranu(int p, int d)=0;
    virtual void ispisGraf() const=0;
    
};


template <typename Tip>
Grana<Tip> UsmjereniGraf<Tip>::dajGranu(int p, int d){ return Grana<Tip>(this, p, d); }


template <typename Tip>
Cvor<Tip> UsmjereniGraf<Tip>::dajCvor(int cvor){ return Cvor<Tip>(this,cvor); }



template< typename Tip >
class MatricaGraf : public UsmjereniGraf<Tip>{
    struct OpisGrane{
        Tip oznaka;
        float tezina;
        OpisGrane(float t) : oznaka(Tip()), tezina(t){}
        OpisGrane(Tip o, float t): oznaka(o), tezina(t){}
    };
    vector<vector<OpisGrane*>> matrica;
    vector<Tip> oznakeCv;
    
public:

    MatricaGraf(int br);
    ~MatricaGraf();
    MatricaGraf(const MatricaGraf<Tip>& mg);
    MatricaGraf<Tip>& operator=(const MatricaGraf<Tip>& mg); 
    int dajBrojCvorova() const;
    void postaviBrojCvorova(int br);
    void dodajGranu(int p, int d, float tezina);
    void obrisiGranu(int p, int d);
    void postaviTezinuGrane(int p, int d, float tezina);
    float dajTezinuGrane(int p, int d) const;
    bool postojiGrana(int p, int d) const;
    void postaviOznakuCvora (int cvor, Tip oznaka);
    Tip dajOznakuCvora(int cvor) const;
    void postaviOznakuGrane(int p, int d, Tip oznaka);
    Tip dajOznakuGrane(int p, int d)const;
    Grana<Tip> dajGranu(int p, int d);
    Cvor<Tip> dajCvor(int cvor);
    GranaIterator<Tip> dajGranePocetak();
    GranaIterator<Tip> dajGraneKraj();
    GranaIterator<Tip> dajSljedecuGranu(int p, int d);
    void ispisGraf() const{
        for(int i=0; i<matrica.size(); i++){
            for(int j=0; j<matrica.size(); j++){
                if(matrica[i][j] != nullptr){
                    cout << setw(6) << matrica[i][j]->tezina;
                }else{
                    cout << setw(6) << 0;
                }
            }
        }
    }
    
};


template<typename Tip>
MatricaGraf<Tip>::MatricaGraf(int br):UsmjereniGraf<Tip>(br){
    int vel = br;
    matrica.resize(vel);
    for(vector<OpisGrane*>& x : matrica){
        x.resize(br, nullptr);
    }
    oznakeCv.resize(vel);
}




template<typename Tip>
MatricaGraf<Tip>::~MatricaGraf(){
    for(vector<OpisGrane*> &x : matrica){
        for(OpisGrane*&r : x){
            if(r != nullptr) delete r;
        }
    }
}




template< typename Tip >
MatricaGraf<Tip>::MatricaGraf(const MatricaGraf<Tip>& mg){
    oznakeCv = mg.oznakeCv;
    matrica.resize(mg.matrica.size());
    for(vector<OpisGrane*>& x: matrica){
        x.resize(mg.matrica.size(), nullptr);
    }
    
    for(int i=0; i<mg.matrica.size(); i++){
        for(int j=0; j<mg.matrica.size(); j++){
            if(mg.matrica[i][j] != nullptr){
                matrica[i][j] = new OpisGrane(mg.matrica[i][j]->oznaka, mg.matrica[i][j]->tezina);
            }else{
                matrica[i][j] = nullptr;
            }
        }
    }
}



template< typename Tip >
MatricaGraf<Tip>& MatricaGraf<Tip>::operator =(const MatricaGraf<Tip>& mg){
   //zastita od samododjele
   if(this==&mg){
        return *this;
   }
   
   //destruktor
   for(vector<OpisGrane*> &x : matrica){
        for(OpisGrane*&r : x){
            if(r != nullptr) delete r;
        }
    }
    
    //konstruktor
    oznakeCv = mg.oznakeCv;
    matrica.resize(mg.matrica.size());
    for(vector<OpisGrane*>& x: matrica){
        x.resize(mg.matrica.size(), nullptr);
    }
    
    for(int i=0; i<mg.matrica.size(); i++){
        for(int j=0; j<mg.matrica.size(); j++){
            if(mg.matrica[i][j] != nullptr){
                matrica[i][j] = new OpisGrane(mg.matrica[i][j]->oznaka, mg.matrica[i][j]->tezina);
            }else{
                matrica[i][j] = nullptr;
            }
        }
    }
    
}




template< typename Tip >
int MatricaGraf<Tip>::dajBrojCvorova()const{
    int v = matrica.size();
    return v;
}


template< typename Tip >
void MatricaGraf<Tip>::postaviBrojCvorova(int br){
    if(br < matrica.size()) throw "Ne moze se smanjit";
    for(vector<OpisGrane*>& x : matrica){
        x.resize(br,nullptr);
    }
    
    vector<OpisGrane*> prazanRed(br, nullptr);
    
    matrica.resize(br, prazanRed);
    
}



template< typename Tip >
void MatricaGraf<Tip>::obrisiGranu(int p, int d){
    delete matrica[p][d];
    matrica[p][d] = nullptr;
}



template<typename Tip>
void MatricaGraf<Tip>::dodajGranu(int p, int d, float tezina){
    
    matrica[p][d] = new OpisGrane(tezina);

}


template< typename Tip >
void MatricaGraf<Tip>::postaviTezinuGrane(int p, int d, float tezina){
    matrica[p][d] -> tezina = tezina;
}


template<typename Tip>
float MatricaGraf<Tip>::dajTezinuGrane(int p, int d) const {
    return matrica[p][d] -> tezina ;
}




template< typename Tip >
bool MatricaGraf<Tip>::postojiGrana(int p, int d) const {
    return matrica[p][d] != nullptr;
}



template< typename Tip >
void MatricaGraf<Tip>::postaviOznakuCvora(int cvor, Tip oznaka){
    oznakeCv[cvor] = oznaka;
}


template< typename Tip >
Tip MatricaGraf<Tip>::dajOznakuCvora(int cvor) const {
    return oznakeCv[cvor];
}


template< typename Tip >
void MatricaGraf<Tip>::postaviOznakuGrane(int p, int d, Tip oznaka){
    matrica[p][d]->oznaka = oznaka;
}


template< typename Tip >
Tip MatricaGraf<Tip>::dajOznakuGrane(int p, int d) const {
    return matrica[p][d]->oznaka;
}



template< typename Tip >
GranaIterator<Tip> MatricaGraf<Tip>::dajSljedecuGranu(int p, int d){
    for(int i=p; i<matrica.size(); i++){
        for(int j=0; j<matrica.size(); j++){
            if(i==p && j<=d)
                continue;
            if(matrica[i][j] != nullptr){
                return GranaIterator<Tip>(this, i, j);
            }
        }
    }
    int k = -1;
    return GranaIterator<Tip>(this, k, k);
}



template< typename Tip >
GranaIterator<Tip> MatricaGraf<Tip>::dajGranePocetak(){
    GranaIterator<Tip> gIt(this, 0, -1);
    gIt++;
    return gIt;
}


template<typename Tip>
GranaIterator<Tip> MatricaGraf<Tip>::dajGraneKraj(){
    int k=-1;
    GranaIterator<Tip> gIt(this, k, k);
    return gIt;
}


template< typename Tip >
class Grana{
    
    UsmjereniGraf<Tip>* graf;
    int poo, doo;
    
public:

    Grana(UsmjereniGraf<Tip>* g, int p, int d): graf(g), poo(p), doo(d){}
    float dajTezinu() const { return graf->dajTezinuGrane(poo, doo);}
    void postaviTezinu(float tezina){ graf->postaviTezinuGrane(poo, doo, tezina); }
    Tip dajOznaku() const { return graf->dajOznakuGrane(poo, doo); }
    void postaviOznaku(Tip oznaka){ graf->postaviOznakuGrane(poo,doo, oznaka); }
    Cvor<Tip> dajPolazniCvor() const { return graf->dajCvor(poo); }
    Cvor<Tip> dajDolazniCvor() const { return graf->dajCvor(doo); }
    
};


template<typename Tip>
class Cvor{
    
    UsmjereniGraf<Tip>* graf;
    int rb;
    
    public:
    
    Cvor(UsmjereniGraf<Tip>* g, int redni ) : graf(g), rb(redni){}
    Tip dajOznaku() const { return graf->dajOznakuCvora(rb); }
    void postaviOznaku(Tip oznaka) { graf->postaviOznakuCvora(rb, oznaka); }
    int dajRedniBroj() const { return rb; }
};



template<typename Tip>
class GranaIterator{
    UsmjereniGraf<Tip>* graf;
    int polaz, dolaz;
    bool postoji;
    
public:
    
    GranaIterator(UsmjereniGraf<Tip>* g, int p, int d): graf(g), polaz(p), dolaz(d){}
    Grana<Tip> operator* ();
    bool operator ==(const GranaIterator& gIt) const;
    bool operator !=(const GranaIterator& gIt) const;
    GranaIterator& operator++();
    GranaIterator operator++(int);
    friend class Grana<Tip>;
    
};


template<typename Tip>
Grana<Tip> GranaIterator<Tip>::operator *(){
    return Grana<Tip>(graf, polaz, dolaz);
}

template<typename Tip>
bool GranaIterator<Tip>::operator ==(const GranaIterator<Tip>& gIt) const {
    return (graf==gIt.graf && polaz==gIt.polaz && dolaz==gIt.dolaz);
}

template<typename Tip>
bool GranaIterator<Tip>::operator !=(const GranaIterator<Tip>& gIt) const {
    return !(*this==gIt);
}


template <typename Tip >
GranaIterator<Tip>& GranaIterator<Tip>::operator ++(){
    
    if(polaz == -1 && dolaz == -1) throw "Losa pozicija";
    GranaIterator<Tip> g = graf->dajSljedecuGranu(polaz,dolaz);
    polaz = g.polaz;
    dolaz = g.dolaz;
    
    return *this;
}

template <typename Tip>
GranaIterator<Tip> GranaIterator<Tip>::operator ++(int){
    GranaIterator<Tip> pom(*this);
    ++(*this);
    return pom;
}



template<typename Tip>
void dfs(UsmjereniGraf<Tip>* graf, vector<Cvor<Tip>>& dfs_obilazak, Cvor<Tip> cvor){
    for(GranaIterator<Tip> iter=graf->dajGranePocetak(); iter != graf->dajGraneKraj(); iter++){
        graf->postaviOznakuCvora(iter->dajPolazniCvor(), false);
        graf->postaviOznakuCvora(iter->dajDolazniCvor(), false);
    }
    
    for(GranaIterator<Tip> iter=graf->dajGranePocetak(); iter != graf->dajGraneKraj(); iter++){
        if(graf->dajOznakuCvora(iter->dajPolazniCvor())==false)
            traziPoDubini(graf, dfs_obilazak, iter->dajPolazniCvor());
    }
    
}




int main() {
    
    cout << "Da"<< endl;
    
     UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
           
    delete g;

    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class UsmjereniGraf{
    int br_cvorova;
public:
    UsmjereniGraf(int br){}
    virtual ~UsmjereniGraf(){}
    virtual int dajBrojCvorova() const =0;
    virtual void postaviBrojCvorova(int br)=0;
    virtual void dodajGranu(int p, int d,float tezina)=0;
    virtual void obrisiGranu(int p,int d)=0;
    virtual void postaviTezinuGrane(int p, int d,float tezina)=0;
    virtual float dajTezinuGrane(int p,int d) const =0;
    virtual bool postojiGrana(int p, int d)const =0;
    virtual void postaviOznakuCvora (int cvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor) const =0;
    virtual void postaviOznakuGrane(int p,int d,TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int p, int d)const=0;
    Grana<TipOznake> dajGranu(int p, int d);
    Cvor<TipOznake> dajCvor(int cvor);
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj()=0;
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int p, int d)=0;
    virtual void ispisiGraf() const =0;
    
};
template <typename TipOznake>
Grana<TipOznake> UsmjereniGraf<TipOznake>::dajGranu(int p, int d){ return Grana<TipOznake>(this,p,d);}
template <typename TipOznake>
Cvor<TipOznake> UsmjereniGraf<TipOznake>::dajCvor(int cvor){return Cvor<TipOznake>(this,cvor);}


template <typename TipOznake>
class MatricaGraf:public UsmjereniGraf<TipOznake>{
    struct OpisGrane{
        TipOznake oznaka;
        float tezina;
        OpisGrane(float t):oznaka(TipOznake()),tezina(t){}
        OpisGrane(TipOznake o, float t): oznaka(o),tezina(t){}
        
    };
    std::vector<std::vector <OpisGrane*>>matrica;
    std::vector<TipOznake> oznakeCv;
public:
    MatricaGraf(int br);
    ~MatricaGraf();
    MatricaGraf(const MatricaGraf<TipOznake>&mg);
    MatricaGraf<TipOznake>& operator= (const MatricaGraf<TipOznake>& mg);
    int dajBrojCvorova() const;
    void postaviBrojCvorova(int br);
    void dodajGranu(int p, int d,float tezina);
    void obrisiGranu(int p,int d);
    void postaviTezinuGrane(int p, int d,float tezina);
    float dajTezinuGrane(int p,int d) const;
    bool postojiGrana(int p, int d)const;
    void postaviOznakuCvora (int cvor, TipOznake oznaka);
    TipOznake dajOznakuCvora(int cvor) const;
    void postaviOznakuGrane(int p,int d,TipOznake oznaka);
    TipOznake dajOznakuGrane(int p, int d)const;
    Grana<TipOznake> dajGranu(int p, int d);
    Cvor<TipOznake> dajCvor(int cvor);
    GranaIterator<TipOznake> dajGranePocetak();
    GranaIterator<TipOznake> dajGraneKraj();
    GranaIterator<TipOznake> dajSljedecuGranu(int p, int d);
    void ispisiGraf() const {
        for(int i =0;i<matrica.size();i++){
        for(int j=0;j<matrica.size();j++){
            if(matrica[i][j]!=nullptr)
                std::cout<<std::setw(6)<<matrica[i][j]->tezina;
                else std::cout<<std::setw(6)<<0;
        }
        std::cout<<std::endl;
        }
    }
    
};
template<typename TipOznake>
MatricaGraf<TipOznake>::~MatricaGraf(){
    for(std::vector<OpisGrane*> &x: matrica){
        for(OpisGrane*&r : x){
            if(r!=nullptr) delete r;
        }
    }
}
template<typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(int br):UsmjereniGraf<TipOznake>(br){
    int vel=br;
    matrica.resize(vel);
    for(std::vector<OpisGrane*>& x:matrica){
        x.resize(br,nullptr);
    }
    oznakeCv.resize(vel);
}

template<typename TipOznake>
MatricaGraf<TipOznake>:: MatricaGraf(const MatricaGraf<TipOznake>& mg){
    oznakeCv=mg.oznakeCv;
    matrica.resize(mg.matrica.size());
    for(std::vector<OpisGrane*>& x:matrica){
        x.resize(mg.matrica.size(),nullptr);
        
    }
    for(int i=0;i<mg.matrica.size();i++)
        for(int j=0;j<mg.matrica.size();j++){
            if(mg.matrica[i][j]!=nullptr)
                matrica[i][j]=new OpisGrane(mg.matrica[i][j]->oznaka,mg.matrica[i][j]->tezina);
                else
                matrica[i][j]=nullptr;
        }
}

template <typename TipOznake>
MatricaGraf<TipOznake>& MatricaGraf<TipOznake>::operator=(const MatricaGraf<TipOznake>&mg){
    //samododjela
    if(this==&mg)
        return *this;
    //destruktor
    for(std::vector<OpisGrane*> &x: matrica){
        for(OpisGrane*&r : x){
            if(r!=nullptr) delete r;}}
//konstr
 oznakeCv=mg.oznakeCv;
    matrica.resize(mg.matrica.size());
    for(std::vector<OpisGrane*>& x:matrica)
        x.resize(mg.matrica.size(),nullptr);
        
    for(int i=0;i<mg.matrica.size();i++)
        for(int j=0;j<mg.matrica.size();j++){
            if(mg.matrica[i][j]!=nullptr)
                matrica[i][j]=new OpisGrane(mg.matrica[i][j]->oznaka,mg.matrica[i][j]->tezina);
                else
                matrica[i][j]=nullptr;
        }
    return *this;
    }

template<typename TipOznake>
int MatricaGraf<TipOznake>::dajBrojCvorova()const{
    int v= matrica.size();
    return v;
}

template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviBrojCvorova(int br){
    if(br<matrica.size()) throw "ne moze se smanjit";
    for(std::vector<OpisGrane*>& x:matrica){
        x.resize(br,nullptr);
    
    }
    std::vector<OpisGrane*> prazanRed(br,nullptr);
    
    matrica.resize(br,prazanRed);
}

template<typename TipOznake>
void MatricaGraf<TipOznake>::obrisiGranu(int p,int d){
    delete matrica[p][d];
    matrica[p][d]=nullptr;
}

template<typename TipOznake>
void MatricaGraf<TipOznake>::dodajGranu(int p,int d,float tezina){
    
    matrica[p][d]=new OpisGrane(tezina);
}
template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviTezinuGrane(int p,int d,float tezina){
    matrica[p][d]->tezina=tezina;
}
template<typename TipOznake>
float MatricaGraf<TipOznake>::dajTezinuGrane(int p,int d)const{
    return matrica[p][d]->tezina;}
    
template<typename TipOznake>
bool MatricaGraf<TipOznake>::postojiGrana(int p,int d) const{
    return matrica[p][d]!=nullptr;}

template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuCvora(int cvor,TipOznake oznaka){
    oznakeCv[cvor]=oznaka;}

template<typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuCvora(int cvor) const {
    return oznakeCv[cvor];}
    
template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuGrane(int p,int d,TipOznake oznaka){
    matrica[p][d]->oznaka=oznaka;}
    
    template<typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuGrane(int p,int d)const{
    return matrica[p][d]->oznaka;}
    
template <typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajSljedecuGranu(int p, int d){
    for(int i=p;i<matrica.size();i++){
        for(int j=0;j<matrica.size();j++){
            if(i==p && j<=d)
                    continue;
            if(matrica[i][j]!=nullptr){
                return GranaIterator<TipOznake>(this,i,j);
            }
        }
    }
    int k=-1;
    return GranaIterator<TipOznake>(this,k,k);
}

template <typename TipOznake>
GranaIterator<TipOznake>MatricaGraf<TipOznake>::dajGranePocetak(){
    GranaIterator<TipOznake>gIt(this,0,-1);
    return ++gIt;
}

template <typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGraneKraj(){
    int k=-1;
    GranaIterator<TipOznake> gIt(this,k,k);
    return gIt;
}

template<typename TipOznake>
class Grana{
    UsmjereniGraf<TipOznake>*graf;
    //zbog polimorfizma
    int poo, doo;
public:
Grana(UsmjereniGraf<TipOznake>*g,int p,int d):graf(g),poo(p),doo(d){}
float dajTezinu() const {return graf-> dajTezinuGrane(poo,doo);}
void postaviTezinu(float tezina){graf->postaviTezinuGrane(poo,doo,tezina);}
TipOznake dajOznaku() const {return graf->dajOznakuGrane(poo,doo);}
void postaviOznaku(TipOznake oznaka){graf->postaviOznakuGrane(poo,doo,oznaka);}
Cvor<TipOznake> dajPolazniCvor() const {return graf->dajCvor(poo);}
Cvor<TipOznake>dajDolazniCvor()const {return graf->dajCvor(doo);}

};

template<typename TipOznake>
class Cvor{UsmjereniGraf<TipOznake>*graf;
int rb;
public:
Cvor(UsmjereniGraf<TipOznake>*g, int redni): graf(g),rb(redni){}
TipOznake dajOznaku() const {return graf-> dajOznakuCvora(rb);}
void postaviOznaku(TipOznake oznaka){graf->postaviOznakuCvora(rb,oznaka);}
int dajRedniBroj()const {return rb;}
};

template<typename TipOznake>
class GranaIterator{
    UsmjereniGraf<TipOznake>*graf;
    int polaz,dolaz;
    bool postoji;
public:
GranaIterator(UsmjereniGraf<TipOznake>*g,int p, int d):graf(g),polaz(p),dolaz(d){}
Grana<TipOznake> operator* ();
bool operator ==(const GranaIterator& gIt) const;
bool operator !=(const GranaIterator& gIt) const;
GranaIterator& operator ++();
GranaIterator operator++(int);
friend class Grana<TipOznake>;

};
template <typename TipOznake>
Grana<TipOznake> GranaIterator<TipOznake>::operator *(){
 return Grana<TipOznake>(graf,polaz,dolaz);   
}

template <typename TipOznake>
bool GranaIterator<TipOznake>::operator ==(const GranaIterator<TipOznake>& gIt)const{
return (graf==gIt.graf && polaz==gIt.polaz && dolaz==gIt.dolaz);}

template <typename TipOznake>
bool GranaIterator<TipOznake>::operator !=(const GranaIterator<TipOznake>&gIt) const{
    return !(*this==gIt);}

template <typename TipOznake>
GranaIterator<TipOznake>& GranaIterator<TipOznake>::operator ++(){
    if(polaz==-1 && dolaz==-1)   throw"losa pozicija";
    GranaIterator<TipOznake>g=graf-> dajSljedecuGranu(polaz,dolaz);
    polaz=g.polaz;
    dolaz=g.dolaz;
    return *this;
    
}
template <typename TipOznake>
GranaIterator<TipOznake> GranaIterator<TipOznake>::operator ++(int){
    GranaIterator<TipOznake> pom(*this);
    ++(*this);
    return pom;
}



template<typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf,std::vector<Cvor<TipOznake>>&dfs_obilazak, Cvor<TipOznake>cvor){
    for(GranaIterator<TipOznake> iter=graf->dajGranePocetak();iter!=graf-> dajGraneKraj();iter++){
        graf->postaviOznakuCvora(iter->dajPolazniCvor(),false);
        graf->postaviOznakuCvora(iter->dajDolazniCvor(),false);
    }
    for(GranaIterator<TipOznake> iter=graf->dajGranePocetak(); iter!=graf->dajGraneKraj();iter++)
    if(graf->dajOznakuCvora(iter->dajPolazniCvor())==false)
        traziPoDubini(graf,dfs_obilazak,iter->dajPolazniCvor());
}



int main() {
      
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

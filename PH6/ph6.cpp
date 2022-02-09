#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <random>
#include <stdexcept>






using namespace std;

void ispis(int* niz, int vel){
    for( int i=0; i<vel-1; i++){
        std::cout << niz[i] << ", ";
    }
    std::cout << niz[vel-1] << "." << std::endl;
}


//Bubble sort


template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    
    for(int i=vel-1; i>=1; i--){
        for(int j=1; j<=i; j++){
            if(niz[j-1] > niz[j]){
                Tip temp = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = temp;
            }
        }
    }
    
}


//Selection sort


template<typename Tip>
void selection_sort(Tip* niz, int vel){
    
    for(int i=0; i<vel-1; i++){
        int min_index=i;
        Tip min = niz[i];
        for(int j=i+1; j<=vel-1; j++){
            if(niz[j] < min){
                min = niz[j];
                min_index = j;
            }
        }
        niz[min_index] = niz[i];
        niz[i] = min;
    }
    
}



//Merge sort



template<typename Tip>
void merge_spajanje(Tip* niz, int l, int p, int q, int u){
    int i(0), j(q-l), k(l);
    
    Tip* priv = new Tip[u-l+1];
    
    for(int m=0; m <= u-l; m++){
        priv[m] = niz[l+m];
    }
    
    while (i <= p-l && j <= u-l) {
        if(priv[i] < priv[j]){
            niz[k] = priv[i];
            i++;
        }else{
            niz[k] = priv[j];
            j++;
        }
        k++;
    }
    
    while (i <= p-l) {
        niz[k] = priv[i];
        k++;
        i++;
    }
    
    while (j <= u-l) {
        niz[k] = priv[j];
        k++;
        j++;
    }
    
    delete[] priv;
    
}




template<typename Tip>
void merge_pomocna(Tip* niz, int l, int u){
   if(u > l){
       int p = (l+u-1)/2;
       int q = p+1;
       merge_pomocna(niz, l, p);
       merge_pomocna(niz, q, u);
       merge_spajanje(niz, l, p, q, u);
   }
}



template<typename Tip>
void merge_sort(Tip* niz, int vel){
    merge_pomocna(niz, 0, vel-1);
}




//Quick sort



template<typename Tip>
int quick_sort_particija(Tip* niz, int prvi, int zadnji){
    Tip pivot = niz[prvi];
    int p (prvi+1); 
    
    while(p <=zadnji && niz[p] < pivot){
        p++;
    }
    Tip temp;
    for(int i=p+1; i <= zadnji; i++){
        if(niz[i] < pivot){
            temp = niz[i];
            niz[i] = niz[p];
            niz[p] = temp;
            p++;
        }
    }
    temp = niz[prvi];
    niz[prvi] = niz[p-1];
    niz[p-1] = temp;
    return p-1;
      
}




template<typename Tip>
void quick(Tip* niz, int prvi, int zadnji){
    if(prvi < zadnji){
        int j = quick_sort_particija(niz, prvi, zadnji);
        quick( niz, prvi, j-1);
        quick( niz, j+1, zadnji);
    }
}




template<typename Tip>
void quick_sort(Tip* niz, int vel){
    quick( niz, 0, vel-1);
}



//Datoteka


void generisi(string filename, int vel){
    ofstream izlazni_tok;
    izlazni_tok.open(filename);
    for(int i=0; i<vel; i++){
        izlazni_tok << rand() % 100 << " ";
    }
    izlazni_tok.close();
}

void ucitaj(string filename, int*& niz, int &vel){
    ifstream ulaz;
    ulaz.open(filename);
    int i(0), b;
    
    while(ulaz >> b){
        i++;
    }
    
    vel = i;
    ulaz.close();
    
    niz = new int [i];
    ulaz.open(filename);
    
    for( i=0; i<vel; i++){
        ulaz >> niz[i];
    }
    
    ulaz.close();
    
}




//Ucitavanje u niz

void ucitajUNiz(string filename, int*& niz, int &vel){
    generisi(filename, vel);
    ucitaj(filename, niz, vel);
}


//Odabir sortiranja

void odaberiSortiranje(){
    
    
    cout << "\n---Menu-----------------\n"<< endl;
    cout << "1. Bubble sort\n";
    cout << "2. Selection sort\n";
    cout << "3. Quick sort\n";
    cout << "4. Merge sort\n";
    
    
}

// Sortiraj po izboru

void pozoviSort(int*&niz, int vel, int izbor){
    
    if(izbor == 1){
        bubble_sort(niz, vel);
    }else if(izbor == 2){
        selection_sort(niz, vel);
    }else if(izbor == 3){
        quick_sort(niz, vel);
    }else if(izbor == 4){
        merge_sort(niz, vel);
    }
    
}


//Vrijeme izvršavanje funkcije


void vrijemeIzvrsavanja(int*& niz, int &vel){
     int izbor;
     odaberiSortiranje();
     std::cin >> izbor;
     
     clock_t vrijeme_1 = clock();
     pozoviSort(niz, vel, izbor);
     clock_t vrijeme_2 = clock();
     
     int ukvrijeme = (vrijeme_2 - vrijeme_1) / (CLOCKS_PER_SEC / 1000);
     cout << "\nVrijeme izvrsavanja odabranog sortiranja je: " << ukvrijeme << "ms.\n";
 }

//Da li je niz sortiran


bool daLiJeNizSortiran(int* niz, int vel){
    for(int i=1; i<vel; i++){
        if(niz[i-1] > niz[i]) return false;
    }
    return true;
}



//Zapis sortiranog u novu izlaznu datoteku


void zapisSortiranogNiza(string filename, int* niz, int vel){
    ofstream izlazni_tok;
    izlazni_tok.open(filename);
    
    for(int i=0; i<vel; i++){
        izlazni_tok << niz[i] << " ";
    } 
    
    izlazni_tok.close();
    cout << "Sortirani niz je zapisan u datorteku : " << filename << " ." << endl;
}


int main() {           
    
    int* niz;
    int vel(15);
    
    ucitajUNiz("BROJEVI.TXT", niz, vel);
    
    vrijemeIzvrsavanja(niz, vel);
    
    if(daLiJeNizSortiran(niz, vel)){
        cout << "Niz je sortiran!" << endl;
    }else{
        cout << "Greska!!!\nNiz nije sortiran!" << std::endl;
    }
    
    ispis(niz, vel);
    
    zapisSortiranogNiza("SortiraniBrojevi.txt", niz, vel);
    
    return 0;
}

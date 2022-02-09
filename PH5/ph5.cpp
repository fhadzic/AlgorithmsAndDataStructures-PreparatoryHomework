#include <iostream>

int fakt(int n){
    
     int f(1);
     for(int i=2; i<=n; i++){
         f *= i;
     }
     
     return f;
}

int faktorijel(int n, int a=1){
    if( n == 0 ) return a;
    
    return faktorijel( n-1, a*n );
}


int fib(int n){
    if(n <= 1 ) return n;
    
    return fib(n-2) + fib(n-1);
}


int fib_petlja (int n) {
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}




int main() {
    std::cout << "Unesite n: ";
    int n;
    std::cin >> n;
    
    std::cout << "Fibonakijev broj za  " << n << ", je: "<< fib_petlja(n) << std::endl;
    
    
    
    return 0;
}

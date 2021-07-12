#include<iostream>
#include<bitset>

int main(){
    uint8_t x = 6;
    
    std::cout << std::bitset<8>(x & (1 << 3));
    return 0;
}

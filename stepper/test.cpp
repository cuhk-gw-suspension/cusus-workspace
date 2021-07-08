
#include <iostream>
#include <bitset>

int main() {
    std::bitset<8> x = 0b10001001;
    /* int x = 3; */
    std::cout << "\noriginal: " << x <<'\n';
    x |= 0 << 3;
    std::bitset<8> y(x);
    std::cout << "shifted:  "<< y << '\n';
    return 0;
}

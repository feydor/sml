/* pascal.cpp - pascal's triangle */
#include <iostream> // cout
#include <string> // stoi
#include <chrono>

size_t pascal_calc(size_t row, size_t i) {
    if (i == 0 || i == row-1) return 1;
    return pascal_calc(row-1, i-1) + pascal_calc(row-1, i);
}

void pascal(size_t row) {
    for (size_t i = 0; i < row; ++i)
        std::cout << pascal_calc(row, i) << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: pascal [row]" << std::endl;
        return -1;
    }

    std::cout << "Pascal's triangle, row #" << std::stoi(argv[1]) << ":" << std::endl;
    std::chrono::system_clock::time_point t1, t2;
    t1 = std::chrono::high_resolution_clock::now();
    pascal(std::stoi(argv[1]));
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "Duration: " << ms_double.count() << "ms\n";
    return 0;
}

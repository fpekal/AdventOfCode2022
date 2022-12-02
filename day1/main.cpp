#include <iostream>
#include <fstream>
#include <string>


void sort3(long long& first, long long& second, long long& third) {
    if (first < second) std::swap(first, second);
    if (second < third) std::swap(second, third);
    if (first < second) std::swap(first, second);
}


int main() {
    std::ifstream file {"input.txt"};

    int part;
    std::cin >> part;

    if (part == 1) {
        long long max_cal = 0;

        long long one_elf_sum = 0;

        while(file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            if (line == "") {
                if (max_cal < one_elf_sum) max_cal = one_elf_sum;
                one_elf_sum = 0;
                continue;
            }

            long long cal = std::atoi(line.c_str());

            one_elf_sum += cal;
        }

        std::cout << max_cal << '\n';
    }
    else if (part == 2) {
        long long max_cal_1 = 0;
        long long max_cal_2 = 0;
        long long max_cal_3 = 0;

        long long one_elf_sum = 0;

        while(file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            if (line == "") {
                if (max_cal_3 < one_elf_sum) {
                    max_cal_3 = one_elf_sum;
                    sort3(max_cal_1, max_cal_2, max_cal_3);
                }
                one_elf_sum = 0;
                continue;
            }

            long long cal = std::atoi(line.c_str());

            one_elf_sum += cal;
        }

        std::cout << max_cal_1 + max_cal_2 + max_cal_3 << '\n';
    }
}
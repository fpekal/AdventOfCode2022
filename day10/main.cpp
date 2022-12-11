#include <iostream>
#include <fstream>

void CRT(int x, int cycle) {
    int x_pos = (cycle - 1)%40;

    if (std::abs(x - x_pos) <= 1) std::cout << '#';
    else std::cout << ' ';

    if ((cycle)%40 == 0) std::cout << '\n';
}

int main() {
    std::fstream file {"input.txt"};

    int part;
    std::cin >> part;

    if (part == 1) {

        int x = 1;
        unsigned long long sum = 0;

        int i = 0;

        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            ++i;
            if ((i-20)%40==0)
                sum+=i*x;

            if (line != "noop") {
                ++i;
                if ((i-20)%40==0)
                    sum+=i*x;
                int x_delta = std::stoi(line.substr(5));
                x += x_delta;
            }
        }

        std::cout << sum << ' ';
    }
    else if (part == 2) {
        int x = 1;

        unsigned int wait = 0; // addx works for 2 cycles, so input must wait

        int cycle = 1;

        int delta_x = 0;

        while (file.peek() != EOF) {
            if (wait == 0) {
                std::string line;
                std::getline(file, line);

                if (line != "noop") {
                    wait = 1;
                    delta_x = std::stoi(line.substr(5));
                }
            }
            else --wait;
            CRT(x, cycle);

            if (wait == 0) {
                x += delta_x;
                delta_x = 0;
            }

            ++cycle;
        }
    }
}
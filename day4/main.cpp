#include <iostream>
#include <fstream>
#include <utility>

typedef std::pair<long long, long long> range;

long long range_size(const range& r) {
    return r.second - r.first + 1;
}

bool range_contains(const range& bigger, const range& smaller) {
    return bigger.first <= smaller.first && bigger.second >= smaller.second;
}

bool range_overlap(const range& bigger, const range& smaller) {
    return (bigger.first <= smaller.first && bigger.second >= smaller.first) || (bigger.first <= smaller.second && bigger.second >= smaller.second);
}

int main() {
    std::ifstream file {"input.txt"};

    long long sum = 0;

    int part;
    std::cin >> part;

    if (part == 1) {
        while (file.peek() != EOF) {
            range range1;
            range range2;

            file >> range1.first;
            file.ignore(1,'-');
            file >> range1.second;
            file.ignore(1,',');
            file >> range2.first;
            file.ignore(1,'-');
            file >> range2.second;

            if (range_size(range1) < range_size(range2)) std::swap(range1, range2);

            if (range_contains(range1, range2)) ++sum;
        }
    }
    else if (part == 2) {
        while (file.peek() != EOF) {
            range range1;
            range range2;

            file >> range1.first;
            file.ignore(1,'-');
            file >> range1.second;
            file.ignore(1,',');
            file >> range2.first;
            file.ignore(1,'-');
            file >> range2.second;

            if (range_size(range1) < range_size(range2)) std::swap(range1, range2);

            if (range_overlap(range1, range2)) ++sum;
        }
    }

    std::cout << sum << '\n';
}
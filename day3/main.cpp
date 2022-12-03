#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <array>

constexpr int ITEM_TYPES = 26*2;

typedef char priority;
typedef std::array<priority, ITEM_TYPES> items;

priority char_to_priority(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a' + 1;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 1 + 26;

    return 0;
}

items string_to_items(std::string_view str) {
    items ite {0};

    for (char c : str) {
        priority item_priority = char_to_priority(c);

        ++ite[item_priority - 1];
    }

    return ite;
}

priority find_duplicate(const items& first_compartment, const items& second_compartment) {
    for (int i = 0; i < ITEM_TYPES; ++i) {
        if (first_compartment[i] > 0 && second_compartment[i] > 0) return i + 1;
    }

    return 0;
}

priority find_duplicate_3(const items& first, const items& second, const items& third) {
    items duplicates {0};

    for (int i = 0; i < ITEM_TYPES; ++i) {
        if (first[i] > 0 && second[i] > 0) {
            ++duplicates[i];
        }
    }

    for (int i = 0; i < ITEM_TYPES; ++i) {
        if (duplicates[i] > 0 && third[i] > 0) {
            return i + 1;
        }
    }

    return 0;
}

int main() {
    std::ifstream file {"input.txt"};

    int part;
    std::cin >> part;

    long long sum = 0;

    if (part == 1) {
        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            std::string_view first_half {line.cbegin(), line.cend() - line.length() / 2};
            std::string_view second_half {line.cend() - line.length() / 2, line.cend()};

            items first_compartment = string_to_items(first_half);
            items second_compartment = string_to_items(second_half);

            priority duplicate = find_duplicate(first_compartment, second_compartment);
            sum += duplicate;
        }
    }
    else if (part == 2) {
        while (file.peek() != EOF) {
            std::string sack1, sack2, sack3;
            std::getline (file, sack1);
            std::getline (file, sack2);
            std::getline (file, sack3);

            items sack1_items = string_to_items(sack1);
            items sack2_items = string_to_items(sack2);
            items sack3_items = string_to_items(sack3);

            priority badge = find_duplicate_3(sack1_items, sack2_items, sack3_items);

            sum += badge;
        }
    }

    std::cout << sum << '\n';
}
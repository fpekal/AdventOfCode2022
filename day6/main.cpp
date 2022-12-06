#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <algorithm>

class entry_t : public std::pair<char, unsigned short> {
    public:
    constexpr bool operator==(char c) const {
        return c == first;
    }
    
    entry_t(char c, unsigned short i) : std::pair<char, unsigned short> {c, i} {}
};

class letters_t : public std::list<entry_t> {
    public:
    void add(char c) {
        auto iter = std::find(begin(), end(), c);
        if (iter == end()) {
            emplace_back(c , 1);
            return;
        }
        
        ++iter->second;
    }
    
    void remove(char c) {
        auto iter = std::find(begin(),end(),c);
        if (iter != end()) {
            --iter->second;
            if (iter->second == 0)
                erase(iter);
        }
    }
};

int main() {
    std::ifstream file {"input.txt"};

    std::string s;
    std::getline(file, s);

    int part;
    std::cin >> part;
    
    if (part == 1) {

        auto begin = s.cbegin();
        auto end = s.cbegin();
        std::advance(end, 4);
        
        letters_t letters;
        letters.add(begin[0]);
        letters.add(begin[1]);
        letters.add(begin[2]);
        letters.add(begin[3]);

        while (true) {
            if (letters.size() == 4) break;
            
            letters.remove(*begin);
            letters.add(*end);
            ++begin;
            ++end;
        }

        std::cout << std::distance(s.cbegin(), end) << '\n';
    }
    else if (part == 2) {
        auto begin = s.cbegin();
        auto end = s.cbegin();
        std::advance(end, 14);
        
        letters_t letters;
        for (int i = 0; i < 14; i++) {
            letters.add(begin[i]);
        }
        
        while (true) {
            if (letters.size() == 14) break;
            
            letters.remove(*begin);
            letters.add(*end);
            ++begin;
            ++end;
        }

        std::cout << std::distance(s.cbegin(), end) << '\n';
    }
    
    
}
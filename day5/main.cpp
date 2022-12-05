#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <list>

typedef std::list<char> stack;
typedef std::array<stack, 9> stacks;

void move_stacks_9000(stacks& stacks, int how_much, int what, int where) {
    stack& in = stacks[what - 1];
    stack& out = stacks[where - 1];

    stack::reverse_iterator rbegin = in.rbegin();
    stack::iterator begin = in.end();
    std::advance(begin, -how_much);

    stack::reverse_iterator rend = in.rbegin();
    std::advance(rend, how_much);
    stack::iterator end = in.end();

    std::move(rbegin, rend, std::inserter(out, out.end()));
    in.erase(begin, end);
}

void move_stacks_9001(stacks& stacks, int how_much, int what, int where) {
    stack& in = stacks[what - 1];
    stack& out = stacks[where - 1];

    stack::iterator begin = in.end();
    std::advance(begin, -how_much);

    stack::iterator end = in.end();

    std::move(begin, end, std::inserter(out, out.end()));
    in.erase(begin, end);
}

void print_stacks(stacks& stacks) {
    for (auto& s : stacks) {
        for (auto& c : s) {
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\n\n";
}

int main() {
    std::ifstream file {"input.txt"};

    int part;
    std::cin >> part;

    std::string result;

    stacks stacks;

    std::regex reg {"[\\[\\s]([\\w\\s\\d])[\\]\\s]\\s"};
    while (file.peek() != EOF) {
        std::string line;
        std::getline(file, line);
        if (line == "") break;
        line += ' ';
        auto iter = line.cbegin();

        std::smatch match;

        int i = 0;

        while(std::regex_search(iter, line.cend(), match, reg)) {
            iter = match[0].second;
            if (match[1].str()[0] != ' ')
                stacks.at(i).push_front(match[1].str()[0]);
            ++i;
        }
    }

    if (part == 1) {
        std::regex reg2 {"move (\\d+) from (\\d+) to (\\d+)"};
        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            std::smatch match;

            std::regex_match(line, match, reg2);

            int how_much = std::atoi(match[1].str().c_str());
            int what = std::atoi(match[2].str().c_str());
            int where = std::atoi(match[3].str().c_str());

            move_stacks_9000(stacks, how_much, what, where);
        }

        for (auto& s : stacks) {
            result += s.back();
        }
    }
    else if (part == 2) {
        std::regex reg2 {"move (\\d+) from (\\d+) to (\\d+)"};
        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            std::smatch match;

            std::regex_match(line, match, reg2);

            int how_much = std::atoi(match[1].str().c_str());
            int what = std::atoi(match[2].str().c_str());
            int where = std::atoi(match[3].str().c_str());

            move_stacks_9001(stacks, how_much, what, where);
        }

        for (auto& s : stacks) {
            result += s.back();
        }
    }

    std::cout << result << '\n';
}
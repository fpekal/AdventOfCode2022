#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <regex>

class Operation {
public:
    virtual unsigned long long operator()(unsigned long long item) = 0;
};

class Add : public Operation {
public:
    unsigned int _a;

    Add(unsigned int a) : _a{a} {}

    unsigned long long operator()(unsigned long long item) {
        return item + _a;
    }
};

class Mul : public Operation {
public:
    unsigned int _a;

    Mul(unsigned int a) : _a{a} {}

    unsigned long long operator()(unsigned long long item) {
        return item * _a;
    }
};

class Pow : public Operation {
public:
    unsigned long long operator()(unsigned long long item) {
        return item * item;
    }
};

struct Monkey {
    std::list<unsigned long long> items;
    std::shared_ptr<Operation> operation;
    unsigned int divisible;
    unsigned int if_true;
    unsigned int if_false;

    unsigned long long inspected;
};

int main() {
    std::vector<Monkey> monkeys;

    std::ifstream file {"input.txt"};

    unsigned long long dumb_NWW = 1;

    while (file.peek() != EOF) {
        Monkey new_monkey;

        std::string tmp;
        std::getline(file, tmp); // Monkey 0:

        std::getline(file, tmp); // Starting items: 79, 98
        std::regex reg1 {"(\\d+)"};

        std::smatch match;

        auto begin = tmp.cbegin();

        while (std::regex_search(begin, tmp.cend(), match, reg1)) {
            begin = match[0].second;
            std::string number = match[1].str();
            unsigned long long item = std::stoi(number);

            new_monkey.items.push_back(item);
        }



        std::getline(file, tmp); // Operation: new = old * 19
        tmp = tmp.substr(23);

        switch (tmp[0]) {
            case '+':
                {
                    int number = std::atoi(tmp.c_str() + 2);
                    new_monkey.operation = std::make_unique<Add>(number);
                }
                break;

            case '*':
                {
                    if (tmp == "* old") {
                        new_monkey.operation = std::make_unique<Pow>();
                    }
                    else {
                        int number = std::atoi(tmp.c_str() + 2);
                        new_monkey.operation = std::make_unique<Mul>(number);
                    }
                }
                break;
        }

        std::getline(file, tmp); // Test: divisible by 23
        new_monkey.divisible = std::atoi(tmp.c_str() + 21);
        dumb_NWW *= std::atoi(tmp.c_str() + 21);

        std::getline(file, tmp); // If true: throw to monkey 2
        new_monkey.if_true = std::atoi(tmp.c_str() + 29);

        std::getline(file, tmp); // If false: throw to monkey 3
        new_monkey.if_false = std::atoi(tmp.c_str() + 30);

        std::getline(file, tmp); // Blank line

        monkeys.emplace_back(new_monkey);
    }

    int part;
    std::cin >> part;

    if (part == 1) {
        for (int i = 0; i < 20; ++i) {
            for (Monkey& monkey : monkeys) {
                for (unsigned long long item : monkey.items) {
                    item = monkey.operation->operator()(item);
                    item /= 3;
                    if (item % monkey.divisible == 0) monkeys[monkey.if_true].items.push_back(item);
                    else monkeys[monkey.if_false].items.push_back(item);

                    ++monkey.inspected;
                }

                monkey.items.clear();
            }
        }
    }
    else if (part == 2) {
        for (int i = 0; i < 10000; ++i) {
            for (Monkey& monkey : monkeys) {
                for (unsigned long long item : monkey.items) {
                    item = monkey.operation->operator()(item);
                    item %= dumb_NWW;
                    if (item % monkey.divisible == 0) monkeys[monkey.if_true].items.push_back(item);
                    else monkeys[monkey.if_false].items.push_back(item);

                    ++monkey.inspected;
                }

                monkey.items.clear();
            }
        }
    }

    for (const auto& monkey : monkeys) {
        std::cout << monkey.inspected << '\n';
    }
}
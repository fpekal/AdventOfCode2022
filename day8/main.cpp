#include <iostream>
#include <fstream>
#include <vector>
#include <string>

typedef std::vector<std::vector<int>> forest_t;
typedef std::pair<int, int> coords_t;

coords_t& operator+=(coords_t& a, const coords_t& b) {
    a.first += b.first;
    a.second += b.second;

    return a;
}

bool is_inside(const forest_t& forest, const coords_t& position) {
    return position.first >= 0 && position.first < forest.size() && position.second >= 0 && position.second < forest[0].size();
}

bool _is_visible_dir(const forest_t& forest, coords_t position, const coords_t& direction) {
    int tree = forest.at(position.first).at(position.second);

    while(true) {
        position += direction;
        if (!is_inside(forest, position)) break;
        int actual = forest.at(position.first).at(position.second);

        if (tree <= actual) return false;
    }

    return true;
}

unsigned int _distance_view_dir(const forest_t& forest, coords_t position, const coords_t& direction) {
    int tree = forest.at(position.first).at(position.second);

    unsigned int distance = 0;

    while(true) {
        position += direction;
        if (!is_inside(forest, position)) break;
        int actual = forest.at(position.first).at(position.second);

        ++distance;
        if (tree <= actual) break;
    }

    return distance;
}

unsigned int scenic_score(const forest_t& forest, coords_t position) {
    unsigned int score = 1;

    score *= _distance_view_dir(forest, position, {1, 0});
    score *= _distance_view_dir(forest, position, {-1, 0});
    score *= _distance_view_dir(forest, position, {0, 1});
    score *= _distance_view_dir(forest, position, {0, -1});

    return score;
} 

bool is_visible(const forest_t& forest, const coords_t& position) {
    return _is_visible_dir(forest, position, {1, 0}) ||
        _is_visible_dir(forest, position, {-1, 0}) ||
        _is_visible_dir(forest, position, {0, 1}) ||
        _is_visible_dir(forest, position, {0, -1});
}

int main() {
    std::ifstream file {"input.txt"};

    forest_t forest;

    for (unsigned int i = 0; file.peek() != EOF; ++i) {
        std::string line;
        std::getline(file, line);

        std::vector<int> tmp;

        for (unsigned int j = 0; j < line.length(); ++j) {
            tmp.push_back(line[j]);
        }

        forest.push_back(tmp);
    }

    int part;
    std::cin >> part;

    if (part == 1) {
        unsigned long long sum = 0;

        for (unsigned int i = 0; i < forest.size(); ++i) {
            for (unsigned int j = 0; j < forest.at(0).size(); ++j) {
                if (is_visible(forest, {i, j})) ++sum;
            }
        }

        std::cout << sum << '\n';
    }
    else if (part == 2) {
        unsigned long long max_score = 0;

        for (unsigned int i = 0; i < forest.size(); ++i) {
            for (unsigned int j = 0; j < forest.at(0).size(); ++j) {
                unsigned int score = scenic_score(forest, {i, j});

                if (max_score < score) max_score = score;
            }
        }

        std::cout << max_score << '\n';
    }
}
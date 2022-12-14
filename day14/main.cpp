#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <charconv>

struct coords_t {
    int x = -1;
    int y = -1;

    bool operator==(const coords_t& other) {
        return x == other.x && y == other.y;
    }
};

constexpr coords_t map_size {400, 200};
typedef std::array<std::array<bool, map_size.x>, map_size.y>  map_t;

coords_t convert_input_to_world(const coords_t& coords) {
    return {coords.x - 500 + map_size.x/2, coords.y};
}

coords_t _parse(std::string_view str) {
    coords_t ret;

    auto begin = str.begin();
    auto end = std::find(begin, str.end(), ',');

    std::from_chars(begin, end, ret.x);

    begin = end + 1;

    std::from_chars(begin, str.end(), ret.y);

    return convert_input_to_world(ret);
}

std::vector<coords_t> parse(std::string_view str) {
    std::vector<coords_t> ret;

    auto begin = str.begin();

    while (true) {
        auto end = std::find(begin, str.end(), ' ');

        auto parsed = _parse({begin, end});
        ret.push_back(parsed);

        if (end == str.end()) break;

        begin = end + 4;
    }

    return ret;
}

bool simulate_sand(map_t& map, coords_t pos) {
    while (true) {
        if (!map[pos.y+1][pos.x]) ++pos.y;
        else {
            if (!map[pos.y+1][pos.x-1]) {++pos.y; --pos.x;}
            else if (!map[pos.y+1][pos.x+1]) {++pos.y; ++pos.x;}
            else {
                map[pos.y][pos.x] = true;
                return false;
            }
        }

        if (pos.y >= map_size.y) return true;
    }
}

void SetCursorPos(int XPos, int YPos)
{
 printf("\033[%d;%dH", YPos+1, XPos+1);
}

int main() {
    std::ifstream file {"input.txt"};

    map_t map {};

    while (file.peek() != EOF) {
        std::string line;
        std::getline(file, line);
        auto parsed = parse(line);

        auto previous = parsed.begin();

        map[previous->y][previous->x] = true;

        while (true) {
            auto now = previous + 1;
            if (now == parsed.end()) break;

            if (now->x - previous->x != 0) { // horizontal
                if (now->x - previous->x > 0) { // right
                    coords_t tmp = *previous;

                    while (tmp != *now) {
                        ++tmp.x;
                        map[tmp.y][tmp.x] = true;
                    }
                }
                else {  // left
                    coords_t tmp = *previous;

                    while (tmp != *now) {
                        --tmp.x;
                        map[tmp.y][tmp.x] = true;
                    }
                }
            }
            else {  // vertical
                if (now->y - previous->y > 0) { // down
                    coords_t tmp = *previous;

                    while (tmp != *now) {
                        ++tmp.y;
                        map[tmp.y][tmp.x] = true;
                    }
                }
                else {  // up
                    coords_t tmp = *previous;

                    while (tmp != *now) {
                        --tmp.y;
                        map[tmp.y][tmp.x] = true;
                    }
                }
            }

            previous = now;
        }
    }

    int part;
    std::cin >> part;

    if (part == 1) {
        int i = 0;

        while (true) {
            if (simulate_sand(map, convert_input_to_world({500, 0}))) {
                std::cout << i << '\n';
                break;
            }

            ++i;
        }
    }
    else if (part == 2) {
        int max_depth = 0;

        for (int i = map_size.y - 1; i > 0; --i) {
            for (int j = 0; j < map_size.x; ++j) {
                if (map[i][j]) {
                    max_depth = i;
                    goto out;
                }
            }
        }

        out:

        for (int i = 0; i < map_size.x; ++i) {
            map[max_depth + 2][i] = true;
        }

        int i = 0;

        coords_t spawn = convert_input_to_world({500, 0});

        while (true) {
            simulate_sand(map, spawn);

            ++i;

            if (map[spawn.y][spawn.x]) {
                std::cout << i << '\n';
                break;
            }
        }
    }
}
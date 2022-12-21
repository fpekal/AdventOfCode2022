#include <iostream>
#include <fstream>
#include <algorithm>
#include <charconv>
#include <array>

struct coords_t {
    int x, y, z;
};

coords_t operator+(coords_t first, const coords_t& second) {
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;

    return first;
}

typedef std::array<std::array<std::array<bool, 22>, 22>, 22> space_t;

coords_t parse(std::string_view str) { // 17,13,15
    coords_t o;

    auto begin = str.begin();
    auto end = std::find(str.begin(), str.end(), ',');

    std::from_chars(begin, end, o.x);

    begin = end + 1;
    end = std::find(begin, str.end(), ',');

    std::from_chars(begin, end, o.y);

    begin = end + 1;
    end = str.end();

    std::from_chars(begin, end, o.z);

    return o;
}

bool is_inside(const coords_t& pos) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0) return false;
    if (pos.x > 21 || pos.y > 21 || pos.z > 21) return false;

    return true;
}

bool has_droplet(const space_t& space, const coords_t& pos) {
    if (!is_inside(pos)) return false;

    return space[pos.x][pos.y][pos.z];
}

int uncovered_sides(const space_t& space, const coords_t& pos) {
    int ret = 0;

    if (!has_droplet(space, pos + coords_t{1, 0, 0}))  ++ret;
    if (!has_droplet(space, pos + coords_t{-1, 0, 0})) ++ret;
    if (!has_droplet(space, pos + coords_t{0, 1, 0}))  ++ret;
    if (!has_droplet(space, pos + coords_t{0, -1, 0})) ++ret;
    if (!has_droplet(space, pos + coords_t{0, 0, 1}))  ++ret;
    if (!has_droplet(space, pos + coords_t{0, 0, -1})) ++ret;

    return ret;
}

void _fill_hollow_one(space_t& out, const space_t& space, const coords_t& pos) {
    out[pos.x][pos.y][pos.z] = false;

    {
        coords_t tmp_pos = pos + coords_t{1, 0, 0};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
    {
        coords_t tmp_pos = pos + coords_t{-1, 0, 0};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
    {
        coords_t tmp_pos = pos + coords_t{0, 1, 0};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
    {
        coords_t tmp_pos = pos + coords_t{0, -1, 0};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
    {
        coords_t tmp_pos = pos + coords_t{0, 0, 1};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
    {
        coords_t tmp_pos = pos + coords_t{0, 0, -1};
        if (is_inside(tmp_pos) && !space[tmp_pos.x][tmp_pos.y][tmp_pos.z] && out[tmp_pos.x][tmp_pos.y][tmp_pos.z]) _fill_hollow_one(out, space, tmp_pos);
    }
}

void fill_hollow(space_t& out, const space_t& space) {
    for (int x = 0; x <= 21; ++x) {
        for (int y = 0; y <= 21; ++y) {
            for (int z = 0; z <= 21; ++z) {
                out[x][y][z] = true;
            }
        }
    }

    _fill_hollow_one(out, space, coords_t{1, 1, 1});
}

int main() {
    std::ifstream file {"input.txt"};

    space_t space {};

    while (file.peek() != EOF) {
        std::string line;
        std::getline(file, line);

        auto parsed = parse(line);

        space.at(parsed.x).at(parsed.y).at(parsed.z) = true;
    }

    int part;
    std::cin >> part;

    if (part == 1) {
        int sum = 0;

        for (int x = 0; x <= 21; ++x) {
            for (int y = 0; y <= 21; ++y) {
                for (int z = 0; z <= 21; ++z) {
                    if (space[x][y][z]) {
                        sum += uncovered_sides(space, coords_t{x, y, z});
                    }
                }
            }
        }

        std::cout << sum << '\n';
    }
    else if (part == 2) {
        space_t no_hollow_space {};

        fill_hollow(no_hollow_space, space);

        int sum = 0;

        for (int x = 0; x <= 21; ++x) {
            for (int y = 0; y <= 21; ++y) {
                for (int z = 0; z <= 21; ++z) {
                    if (no_hollow_space[x][y][z]) {
                        sum += uncovered_sides(no_hollow_space, coords_t{x, y, z});
                    }
                }
            }
        }

        std::cout << sum << '\n';
    }
}
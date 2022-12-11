#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

typedef std::pair<int, int> coords_t;

coords_t operator+(coords_t a, const coords_t& b) {
	a.first += b.first;
	a.second += b.second;
	
	return a;
}

coords_t operator-(coords_t a, const coords_t& b) {
	a.first -= b.first;
	a.second -= b.second;
	
	return a;
}

template<class T, class U>
class std::hash<std::pair<T, U>> {
public:
	size_t operator()(const std::pair<T, U>& val) const {
		std::hash<std::string> hash;
		
		return (hash(std::to_string(val.first)) << 1) ^ hash(std::to_string(val.second));
	}
};

typedef std::unordered_set<coords_t> visited_t;

const std::unordered_map<coords_t, coords_t> behavior {
	{{0,2},{0,1}},
	{{0,-2},{0,-1}},
	{{2,0},{1,0}},
	{{-2,0},{-1,0}},
	{{1,2},{1,1}},
	{{2,1},{1,1}},
	{{1,-2},{1,-1}},
	{{2,-1},{1,-1}},
	{{-1,2},{-1,1}},
	{{-2,1},{-1,1}},
	{{-1,-2},{-1,-1}},
	{{-2,-1},{-1,-1}},
	{{1,0},{0,0}},
	{{1,1},{0,0}},
	{{0,1},{0,0}},
	{{-1,1},{0,0}},
	{{-1,0},{0,0}},
	{{-1,-1},{0,0}},
	{{0,-1},{0,0}},
	{{1,-1},{0,0}},
    {{0,0},{0,0}},
    {{2,2},{1,1}},
    {{2,-2},{1,-1}},
    {{-2,2},{-1,1}},
    {{-2,-2},{-1,-1}}
};

const std::unordered_map<char, coords_t> directions {
	{'U', {0,1}},
	{'D', {0,-1}},
	{'R', {1,0}},
	{'L', {-1,0}}
};

coords_t new_tail(const coords_t& head, const coords_t& tail) {
	coords_t diff = head - tail;
	coords_t move = behavior.at(diff);
    return tail + move;
}

int main() {
    std::ifstream file {"input.txt"};
    
    visited_t visited;

    int part;
    std::cin >> part;


    if (part == 1) {
        coords_t head {0,0}, tail {0,0};
        
        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            char dir_c = line[0];
            int rep = atoi(line.c_str() + 2);
            
            
            coords_t direction = directions.at(dir_c);
            
            for (int i = 0; i < rep; ++i) {
                head = head + direction;
                tail = new_tail(head, tail);
                visited.insert(tail);
            }
        }
    }
    else if (part == 2) {
        std::array<coords_t, 10> rope {{{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}}};

        while (file.peek() != EOF) {
            std::string line;
            std::getline(file, line);

            char dir_c = line[0];
            int rep = atoi(line.c_str() + 2);
            
            
            coords_t direction = directions.at(dir_c);
            
            for (int i = 0; i < rep; ++i) {
                rope[0] = rope[0] + direction;

                for (int i = 1; i < 10; ++i) {
                    rope[i] = new_tail(rope[i-1], rope[i]);
                }

                visited.insert(rope[9]);
            }
        }
    }

    
    std::cout << visited.size() << '\n';
}
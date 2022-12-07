#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

class dir_t;
class file_t;

class node_t {
	public:
	enum type_t {
		dir, file
	};
	
	type_t type;
	std::string name;
	std::weak_ptr<dir_t> parent;
	bool is_root;
	
	static std::list<std::shared_ptr<node_t>> all_nodes;
	
		node_t() {
		}
		
		node_t(type_t type, std::string name, std::weak_ptr<dir_t> parent) : type{type}, name{name}, parent{parent}, is_root{false} {
		}
		
		node_t(type_t type, std::string name) : type{type}, name{name}, is_root{true} {}
};

std::list<std::shared_ptr<node_t>> node_t::all_nodes;

class file_t : public node_t {
	public:
	static constexpr type_t type = file;
	
	unsigned long long size;
	
	file_t (std::string name, std::weak_ptr<dir_t> parent, unsigned long long size) : node_t{type, name, parent}, size{size} {}
};

class dir_t : public node_t {
	public:
	static constexpr type_t type = dir;
	
	std::list<std::shared_ptr<node_t>> list;
	
	dir_t(std::string name, std::weak_ptr<dir_t> parent) : node_t {type, name, parent} {
	}
	
	dir_t(std::string name) : node_t{type, name} {}
	
	void add_node(std::shared_ptr<node_t> node) {
		list.push_back(node);
	}
};

std::string get_path(std::shared_ptr<node_t> node) {
	if (!node->is_root)
		return get_path(node->parent.lock()) + '/' + node->name;
	else
		return std::string {""};
}


template<class T, class... Args>
std::shared_ptr<T> construct_node(std::string name, std::shared_ptr<dir_t> parent, Args... args) {
	auto ptr = std::make_shared<T>(name, parent,  args...);
	parent->add_node(ptr);
		
	node_t::all_nodes.push_back(ptr);
		
	return ptr;
}

template<class T, class... Args>
std::shared_ptr<T> construct_node(std::string name, Args... args) {
	auto ptr = std::make_shared<T>(name,  args...);
		
	node_t::all_nodes.push_back(ptr);
		
	return ptr;
}

unsigned long long get_size(std::shared_ptr<node_t> node) {
	if (node->type == node_t::file) {
		return ((file_t*)node.get())->size;
	}
	else if (node->type == node_t::dir) {
		unsigned long long sum = 0;
		
		for (auto p : ((dir_t*)node.get())->list) {
			sum += get_size(p);
		}
		
		return sum;
	}

    return 0;
}


int main() {
    std::ifstream file {"input.txt"};

    int part;
    std::cin >> part;

    std::shared_ptr<dir_t> root = construct_node<dir_t>("");

    std::shared_ptr<dir_t> pwd = root;

    while (file.peek() != EOF) {
        std::string line;
        std::getline(file, line);

        if (line[0] == '$') {
            if (line[2] == 'c') {   // $ cd [directory]
                std::string new_dir = line.substr(5);

                if (new_dir == "..") pwd = pwd->parent.lock();
                else {
                    for (auto dir : pwd->list) {
                        if (dir->name == new_dir) {
                            pwd = std::static_pointer_cast<dir_t>(dir);
                            break;
                        }
                    }
                }
            }
        }
        else if (line[0] == 'd') {  // dir [directory]
            std::string dir_name = line.substr(4);
            construct_node<dir_t>(dir_name, pwd);
        }
        else {                      // [size] [file]
            std::stringstream sstr;
            sstr << line;

            unsigned long long size;
            std::string file_name;
            sstr >> size >> file_name;

            construct_node<file_t>(file_name, pwd, size);
        }
    }


    if (part == 1) {
        unsigned long long sum = 0;

        for (std::shared_ptr<node_t> ptr : node_t::all_nodes) {
            if (ptr->type == node_t::dir) {
                unsigned long long size = get_size(ptr);

                if (size <= 100000)
                    sum += size;
            }
        }

        std::cout << sum << '\n';
    }
    else if (part == 2) {
        unsigned long long root_size = get_size(root);
        unsigned long long needed_space = 30000000 + root_size - 70000000;

        std::vector<unsigned long long> size_of_dirs;

        for (std::shared_ptr<node_t> ptr : node_t::all_nodes) {
            if (ptr->type == node_t::dir) {
                size_of_dirs.push_back(get_size(ptr));
            }
        }

        std::sort(size_of_dirs.begin(), size_of_dirs.end());

        for (auto s : size_of_dirs) {
            if (s > needed_space) {
                std::cout << s << '\n';
                return 0;
            }
        }
    }
}
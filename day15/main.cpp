#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <charconv>
#include <array>

// Store position
struct coords_t {
    long long x, y;

    constexpr bool operator==(const coords_t& other) const {
        return x == other.x && y == other.y;
    }

    constexpr coords_t operator+(const coords_t& other) const {
        coords_t ret = *this;

        ret.x += other.x;
        ret.y += other.y;

        return ret;
    }
};

// Manhattan distance
long long calc_distance(const coords_t& a, const coords_t& b) {
    return std::abs(b.x - a.x) + std::abs(b.y - a.y);
}

// Input entry
struct sensor_t {
    coords_t sensor_pos;
    coords_t seen_beacon_pos;
    long long distance_to_beacon;

    sensor_t(const coords_t& sensor_pos, const coords_t& seen_beacon_pos) :
        sensor_pos{sensor_pos}, seen_beacon_pos{seen_beacon_pos} {
        distance_to_beacon = calc_distance(sensor_pos, seen_beacon_pos);
    }
};

// Sum distances from edges of sensors
long long get_all_distance(const coords_t& pos, const std::vector<sensor_t>& vec) {
    long long dist = 0;

    for (const auto& sens : vec) {
        dist += std::abs(calc_distance(pos, sens.sensor_pos) - sens.distance_to_beacon);
    }

    return dist;
}

int main() {
    std::ifstream file {"input.txt"};

    std::vector<sensor_t> sensors;

    // Start of my awful parser
    while (file.peek() != EOF) {
        coords_t sensor_pos;
        coords_t seen_beacon_pos;

        std::string line;
        std::getline(file, line);

        auto begin = std::find(line.begin(), line.end(), '=');
        ++begin;
        auto end = std::find(begin, line.end(), ',');

        std::from_chars(begin.base(), end.base(), sensor_pos.x);

        begin = end + 4;
        end = std::find(begin, line.end(), ':');

        std::from_chars(begin.base(), end.base(), sensor_pos.y);

        begin = std::find(end, line.end(), '=');
        ++begin;
        end = std::find(begin, line.end(), ',');

        std::from_chars(begin.base(), end.base(), seen_beacon_pos.x);

        begin = end + 4;
        std::from_chars(begin.base(), line.end().base(), seen_beacon_pos.y);

        sensors.emplace_back(sensor_pos, seen_beacon_pos);
    }
    // End of parser

    int part;
    std::cin >> part;

    if (part == 1) {
        int sum = 0;

        long long max_left = 0;
        long long max_right = 0;

        for (const auto& sensor : sensors) {
            if (sensor.sensor_pos.x - sensor.distance_to_beacon < max_left) max_left = sensor.sensor_pos.x - sensor.distance_to_beacon;
            if (sensor.sensor_pos.x + sensor.distance_to_beacon > max_right) max_right = sensor.sensor_pos.x + sensor.distance_to_beacon;
        }

        for (long long i = max_left - 10; i < max_right + 10; ++i) {
            coords_t testing {i, 2000000};

            bool found = false;

            for (const auto& sensor : sensors) {    // Finding if testing pos is in range of any sensor
                if (calc_distance(testing, sensor.sensor_pos) <= sensor.distance_to_beacon) {
                    bool is_beacon = false;
                    
                    for (const auto& sensor : sensors) {    // Testing if there is a beacon already
                        if (sensor.seen_beacon_pos == testing) {
                            is_beacon = true;
                            break;
                        }
                    }

                    found = !is_beacon;
                    break;
                }
            }

            if (found) ++sum;
        }

        std::cout << sum << '\n';
    }
    // START OF THE SECOND PART
    else if (part == 2) {
        std::vector<sensor_t> found_sensors;    // Sensors around distress beacon

        for (int i = 0; i < sensors.size() - 1; ++i) {
            for (int j = i + 1; j < sensors.size(); ++j) {
                const auto& sensor = sensors[i];
                const auto& sensor2 = sensors[j];
                if (sensor.sensor_pos == sensor2.sensor_pos) continue;
                long long distance = calc_distance(sensor.sensor_pos, sensor2.sensor_pos);

                if (distance - sensor.distance_to_beacon - sensor2.distance_to_beacon == 2) {   // Find two beacons that make 1-unit-wide corridor between them
                    found_sensors.push_back(sensor);
                    found_sensors.push_back(sensor2);
                }
            }
        }

        // At this point `found_sensors` has 4 beacons

        // Position somewhere in between of these 4 beacons
        coords_t position = {(found_sensors[0].sensor_pos.x + found_sensors[1].sensor_pos.x)/2, (found_sensors[0].sensor_pos.y + found_sensors[1].sensor_pos.y)/2};

        while (true) {
            if (get_all_distance(position, found_sensors) == 4) break;  // If `position` is one unit apart from distress beacon, break

            // If going to the right makes you closer to the disress beacon, go right
            if (get_all_distance(position + coords_t{1, 0}, found_sensors) < get_all_distance(position, found_sensors)) {
                position = position + coords_t{1, 0};
                continue;
            }
            if (get_all_distance(position + coords_t{-1, 0}, found_sensors) < get_all_distance(position, found_sensors)) {
                position = position + coords_t{-1, 0};
                continue;
            }
            if (get_all_distance(position + coords_t{0, 1}, found_sensors) < get_all_distance(position, found_sensors)) {
                position = position + coords_t{0, 1};
                continue;
            }
            if (get_all_distance(position + coords_t{0, -1}, found_sensors) < get_all_distance(position, found_sensors)) {
                position = position + coords_t{0, -1};
                continue;
            }
        }

        // Because you are 1 unit apart from distress beacon, look around for another position where distance == 4
        if (get_all_distance(position + coords_t{1, 0}, found_sensors) == 4) position = position + coords_t{1, 0};
        else if (get_all_distance(position + coords_t{-1, 0}, found_sensors) == 4) position = position + coords_t{-1, 0};
        else if (get_all_distance(position + coords_t{0, 1}, found_sensors) == 4) position = position + coords_t{0, 1};
        else if (get_all_distance(position + coords_t{0, -1}, found_sensors) == 4) position = position + coords_t{0, -1};

        // Profit
        std::cout << position.x * 4000000 + position.y << '\n';
    }
}
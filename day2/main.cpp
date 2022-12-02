#include <iostream>
#include <fstream>
#include <unordered_map>

enum rps {
    rock, paper, scissors
};

enum state {
    win, draw, lose
};

const std::unordered_map<char, rps> opponent_moves {{'A', rock}, {'B', paper}, {'C', scissors}};
const std::unordered_map<char, rps> player_moves {{'X', rock}, {'Y', paper}, {'Z', scissors}};
const std::unordered_map<char, state> states {{'X', lose}, {'Y', draw}, {'Z', win}};

const std::unordered_map<state, int> state_score {{win, 6}, {draw, 3}, {lose, 0}};
const std::unordered_map<rps, int> shape_score {{rock, 1}, {paper, 2}, {scissors, 3}};

state first_state(rps first, rps second) {
    if (first == rock) {
        if (second == rock) return draw;
        else if (second == paper) return lose;
        else return win;
    }
    else if (first == paper) {
        if (second == rock) return win;
        else if (second == paper) return draw;
        else return lose;
    }
    else if (first == scissors) {
        if (second == rock) return lose;
        else if (second == paper) return win;
        else return draw;
    }

    return draw;
}

rps examine_shape(rps opponent, state desired_state) {
    if (opponent == rock) {
        if (desired_state == win) return paper;
        else if (desired_state == draw) return rock;
        else if (desired_state == lose) return scissors;
    }
    else if (opponent == paper) {
        if (desired_state == win) return scissors;
        else if (desired_state == draw) return paper;
        else if (desired_state == lose) return rock;
    }
    else if (opponent == scissors) {
        if (desired_state == win) return rock;
        else if (desired_state == draw) return scissors;
        else if (desired_state == lose) return paper;
    }

    return rock;
}

int main() {
    std::ifstream file {"input.txt"};

    long long score = 0;

    int part;
    std::cin >> part; // 1, 2

    if (part == 1) {
        while (file.peek() != EOF) {
            char opponent_char, player_char;
            file >> opponent_char >> player_char;

            rps player_shape = player_moves.at(player_char);
            rps opponent_shape = opponent_moves.at(opponent_char);
            state ending_state = first_state(player_shape, opponent_shape);

            score += shape_score.at(player_shape);
            score += state_score.at(ending_state);
        }
    }
    else if (part == 2) {
        while (file.peek() != EOF) {
            char opponent_char, desired_state_char;
            file >> opponent_char >> desired_state_char;

            state desired_state = states.at(desired_state_char);
            rps opponent_shape = opponent_moves.at(opponent_char);
            rps player_shape = examine_shape(opponent_shape, desired_state);

            score += shape_score.at(player_shape);
            score += state_score.at(desired_state);
        }
    }

    std::cout << score << '\n';
}

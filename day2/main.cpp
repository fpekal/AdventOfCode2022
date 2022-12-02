#include <iostream>
#include <fstream>
#include <unordered_map>

enum rps {
    rock, paper, scissors
};

enum state {
    win, draw, lose
};

std::unordered_map<char, rps> opponent_moves {{'A', rock}, {'B', paper}, {'C', scissors}};
std::unordered_map<char, state> player_moves {{'X', lose}, {'Y', draw}, {'Z', win}};

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

int shape_score(rps shape) {
    if (shape == rock) return 1;
    else if (shape == paper) return 2;
    else return 3;
}

int state_score(state _state) {
    if (_state == win) return 6;
    else if (_state == draw) return 3;
    else return 0;
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

    while (file.peek() != EOF) {
        char opponent, player;
        file >> opponent >> player;

        //if (player == '\0' || opponent == '\0') break;

        state player_state = player_moves[player];
        rps opponent_shape = opponent_moves[opponent];
        rps player_shape = examine_shape(opponent_shape, player_state);

        score += shape_score(player_shape);
        score += state_score(player_state);
    }

    std::cout << score;
}
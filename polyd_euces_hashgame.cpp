#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Puzzle {
    std::string clue;
    std::string answer;
    int points;
};

std::string normalize(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return text;
}

std::vector<std::string> make_choices(const std::string& answer) {
    std::vector<std::string> choices = {answer, "block", "nonce", "miner", "mempool"};
    std::sort(choices.begin(), choices.end());
    choices.erase(std::unique(choices.begin(), choices.end()), choices.end());

    while (choices.size() < 5) {
        choices.push_back("option" + std::to_string(choices.size()));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(choices.begin(), choices.end(), gen);
    return choices;
}

int main() {
    std::vector<Puzzle> puzzles = {
        {"A batch of confirmed transactions.", "block", 10},
        {"A changing number used in mining puzzles.", "nonce", 15},
        {"A participant that helps secure a proof-of-work network.", "miner", 20},
        {"Where pending transactions wait before confirmation.", "mempool", 25},
        {"A digital fingerprint of input data.", "hash", 30}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    int score = 0;
    int round = 1;

    std::cout << "POLYD_EUCES HASHGAME\n";
    std::cout << "Terminal puzzle game prototype. Type q to quit.\n";

    while (true) {
        std::uniform_int_distribution<size_t> dist(0, puzzles.size() - 1);
        Puzzle puzzle = puzzles[dist(gen)];
        auto choices = make_choices(puzzle.answer);

        std::cout << "\nRound " << round << " | Points: " << puzzle.points << "\n";
        std::cout << "Clue: " << puzzle.clue << "\n";

        for (size_t i = 0; i < choices.size(); ++i) {
            std::cout << "  " << i + 1 << ") " << choices[i] << "\n";
        }

        std::cout << "Choose 1-5, type the answer, or q: ";
        std::string guess;
        std::getline(std::cin, guess);

        if (normalize(guess) == "q" || normalize(guess) == "quit") {
            break;
        }

        std::string chosen = guess;
        try {
            int selected = std::stoi(guess);
            if (selected >= 1 && selected <= static_cast<int>(choices.size())) {
                chosen = choices[static_cast<size_t>(selected - 1)];
            }
        } catch (...) {
            // The player typed a word instead of a number.
        }

        if (normalize(chosen) == normalize(puzzle.answer)) {
            score += puzzle.points;
            std::cout << "Correct. Score: " << score << "\n";
        } else {
            std::cout << "Incorrect. Correct answer: " << puzzle.answer << "\n";
            std::cout << "Score: " << score << "\n";
        }

        ++round;
    }

    std::cout << "\nFinal score: " << score << "\n";
    return 0;
}

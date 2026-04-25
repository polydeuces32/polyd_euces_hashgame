#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Puzzle {
    std::string clue;
    std::string answer;
    std::vector<std::string> choices;
    int reward;
    int difficulty;
};

struct RoundRecord {
    int round;
    std::string clue;
    std::string expected;
    std::string chosen;
    bool correct;
    int reward;
    int total_score;
};

std::string normalize(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return text;
}

std::string make_fingerprint(const std::string& text) {
    const std::string normalized = normalize(text);
    const std::string project_salt = "polyd-euces-hashgame-v1";
    const std::size_t value = std::hash<std::string>{}(project_salt + ":" + normalized);

    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(sizeof(std::size_t) * 2) << value;
    return out.str();
}

bool verify_answer(const std::string& guess, const std::string& expected_fingerprint) {
    return make_fingerprint(guess) == expected_fingerprint;
}

std::vector<std::string> make_choices(const Puzzle& puzzle) {
    std::vector<std::string> choices = puzzle.choices;

    if (std::find(choices.begin(), choices.end(), puzzle.answer) == choices.end()) {
        choices.push_back(puzzle.answer);
    }

    std::vector<std::string> fallback = {"block", "nonce", "miner", "mempool", "hash", "signet", "taproot"};
    for (const auto& item : fallback) {
        if (choices.size() >= 5) {
            break;
        }
        if (std::find(choices.begin(), choices.end(), item) == choices.end()) {
            choices.push_back(item);
        }
    }

    std::sort(choices.begin(), choices.end());
    choices.erase(std::unique(choices.begin(), choices.end()), choices.end());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(choices.begin(), choices.end(), gen);

    if (choices.size() > 5) {
        choices.resize(5);
        if (std::find(choices.begin(), choices.end(), puzzle.answer) == choices.end()) {
            choices.back() = puzzle.answer;
            std::shuffle(choices.begin(), choices.end(), gen);
        }
    }

    return choices;
}

void print_intro() {
    std::cout << "POLYD_EUCES HASHGAME\n";
    std::cout << "Phase 1-4 C++ prototype\n";
    std::cout << "- Phase 1: verifier function\n";
    std::cout << "- Phase 2: verifier connected to puzzle answers\n";
    std::cout << "- Phase 3: simulated reward score\n";
    std::cout << "- Phase 4: round history summary\n";
    std::cout << "Type q or quit to exit.\n";
}

void print_history(const std::vector<RoundRecord>& history) {
    std::cout << "\n========== ROUND HISTORY ==========" << "\n";

    if (history.empty()) {
        std::cout << "No rounds played.\n";
        return;
    }

    for (const auto& record : history) {
        std::cout << "Round " << record.round
                  << " | " << (record.correct ? "WIN" : "MISS")
                  << " | expected=" << record.expected
                  << " | chosen=" << record.chosen
                  << " | reward=" << (record.correct ? record.reward : 0)
                  << " | total=" << record.total_score
                  << "\n";
    }
}

int main() {
    std::vector<Puzzle> puzzles = {
        {
            "A batch of confirmed transactions.",
            "block",
            {"block", "wallet", "oracle", "thread", "packet"},
            10,
            1
        },
        {
            "A changing number used in mining-style puzzles.",
            "nonce",
            {"nonce", "router", "canvas", "ledger", "signal"},
            15,
            2
        },
        {
            "A participant that helps secure a proof-of-work network.",
            "miner",
            {"miner", "browser", "folder", "bridge", "token"},
            20,
            2
        },
        {
            "Where pending transactions wait before confirmation.",
            "mempool",
            {"mempool", "terminal", "kernel", "cache", "script"},
            25,
            3
        },
        {
            "A digital fingerprint of input data.",
            "hash",
            {"hash", "stack", "array", "frame", "index"},
            30,
            3
        },
        {
            "A testing network used for safe experiments.",
            "signet",
            {"signet", "mainnet", "frontend", "backend", "archive"},
            35,
            3
        }
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<RoundRecord> history;

    int score = 0;
    int round = 1;

    print_intro();

    while (true) {
        std::uniform_int_distribution<size_t> dist(0, puzzles.size() - 1);
        Puzzle puzzle = puzzles[dist(gen)];
        const std::string target_fingerprint = make_fingerprint(puzzle.answer);
        auto choices = make_choices(puzzle);

        std::cout << "\n========== ROUND " << round << " ==========" << "\n";
        std::cout << "Difficulty: " << puzzle.difficulty << "/3\n";
        std::cout << "Simulated reward: " << puzzle.reward << " credits\n";
        std::cout << "Clue: " << puzzle.clue << "\n";
        std::cout << "Verifier fingerprint: " << target_fingerprint << "\n\n";

        for (size_t i = 0; i < choices.size(); ++i) {
            std::cout << "  " << (i + 1) << ") " << choices[i] << "\n";
        }

        std::cout << "\nChoose 1-5, type the answer, or q: ";
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

        const bool correct = verify_answer(chosen, target_fingerprint);

        if (correct) {
            score += puzzle.reward;
            std::cout << "Correct. Verified answer matched the fingerprint.\n";
        } else {
            std::cout << "Incorrect. Expected answer: " << puzzle.answer << "\n";
        }

        history.push_back({
            round,
            puzzle.clue,
            puzzle.answer,
            chosen,
            correct,
            puzzle.reward,
            score
        });

        std::cout << "Current score: " << score << " credits\n";
        ++round;
    }

    print_history(history);
    std::cout << "\nFinal score: " << score << " credits\n";
    return 0;
}

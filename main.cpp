// main.cpp
// Field Hockey Scoreboard Simulator
// Beginner C++ project – December 2025

#include <iostream> // for input and output
#include <string> // for strings
#include <vector> // for vectors
#include <random> // for random numbers
#include <format> // C++20 formatted output
#include <chrono> // for time
#include <thread> // for threading

// structures to log events in field hockey matches

enum class Card { Green, Yellow, Red };
// CardType::Green or CardType::Yellow or CardType::Red. Typesafe

class Team {
    private: // underscores distinguish private member variables from local variables
        std::string name_;
        int goals_ = 0;
        int greenCards_ = 0;
        int yellowCards_ = 0;
        int redCards_ = 0;
        int penaltyCorners_ = 0;

    public:
        explicit Team(std::string name) : name_(std::move(name)) {}
        // "Create a Team from a string. Do not allow implicit conversion. Efficiently move the string data into the private member name_, and do nothing else."

        
        // const getters:
        // const member functions dont modify the object.
        // best practice: always make getters be const.
        const std::string& name() const { return name_; } // returns reference to the internal string
        int goals() const { return goals_; }
        int greenCards() const { return greenCards_; }
        int yellowCards() const { return yellowCards_; }
        int redCards() const { return redCards_; }
        int penaltyCorners() const { return penaltyCorners_; }
    
        // actions:
        void scoreGoal() { ++goals_; }
        void awardCard(CardType type) {
            switch (type) {
                case CardType::Green: ++greenCards_; break;
                case CardType::Yellow: ++yellowCards_; break;
                case CardType::Red: ++redCards_; break;
                default:
                    // This should never happen - it's a programming error.
                    std::cout << "Invalid card type" << std::endl;
                    throw std::invalid_argument("Invalid card type");
            }
        }
        void awardPenaltyCorner() { ++penaltyCorners_; }

        // formatted summary:
        std::string statsLine() const {
            return std::format("{}G {}Y {}R {}PC",
                               greenCards_, yellowCards_, redCards_, penaltyCorners_);
        }
    
};

class MatchEvent {
    private:
        int quarter_;
        std::string description_;

    public:
        // constructor:
        MatchEvent(int quarter, std::string description) :
            // initializer list:
            quarter_(quarter),
            description_(std::move(description)) {}

        std::string toString() const {
            return std::format("Q{} - {}", quarter_, description_);
        }
}


class HockeyMatch {
    private:
        Team home_;
        Team away_;
        int currentQuarter_ = 1;
        std::vector<MatchEvent> event_log_;

        void log() {
            event_log
        }

    public:
}


int main() {


    Match match;
    std::string welcome_message = "Welcome to the field hockey match simulator!";
    std::cout << welcome_message << std::endl;
    return 0;
}
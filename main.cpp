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
#include "magic_enum.hpp" // automatic enum-to-string

// structures to log events in field hockey matches

enum class CardType { Green, Yellow, Red };
// CardType::Green or CardType::Yellow or CardType::Red. Typesafe



class Team {
    private: // underscores distinguish private member variables from local variables
        std::string name_;
        int goals_ = 0;
        int green_cards_ = 0;
        int yellow_cards_ = 0;
        int red_cards_ = 0;
        int penalty_corners_ = 0;

    public:
        explicit Team(std::string name) : name_(std::move(name)) {}
        // "Create a Team from a string. Do not allow implicit conversion.
        // Efficiently move the string data into the private member name_, and do nothing else."
        
        const std::string& name() const     { return name_; }
        int goals() const                   { return goals_; }
        int greenCards() const              { return green_cards_; }
        int yellowCards() const             { return yellow_cards_; }
        int redCards() const                { return red_cards_; }
        int penaltyCorners() const          { return penalty_corners_; }
    
        void scoreGoal()                    { ++goals_; }
        
        void awardCard(CardType type) {
            switch (type) {
                case CardType::Green: ++green_cards_; break;
                case CardType::Yellow: ++yellow_cards_; break;
                case CardType::Red: ++red_cards_; break;
                default:
                // This should never happen -ureachable.
                    std::cerr << "Invalid CardType passed to awardCard" << std::endl;
                    throw std::invalid_argument("Invalid CardType passed to awardCard");
            }
        }
        void awardPenaltyCorner()           { ++penalty_corners_; }

        // formatted summary:
        std::string statsLine() const {
            return std::format("{}G {}Y {}R {}PC",
                               green_cards_, yellow_cards_, red_cards_, penalty_corners_);
        }    
};

// -----------------------------------------------------------------------------
// Small value class representing a single event in the match timeline
// -----------------------------------------------------------------------------
class MatchEvent {
    private:
        int quarter_;
        std::string description_;

    public:
        // constructor:
        MatchEvent(int quarter, std::string description) :
            quarter_(quarter), description_(std::move(description)) {}

        std::string toString() const {
            return std::format("Q{} - {}", quarter_, description_);
        }
};


// -----------------------------------------------------------------------------
// HockeyMatch class – core match orchestration
// -----------------------------------------------------------------------------
class HockeyMatch {
    private:
        Team home_team_;
        Team away_team_;
        int current_quarter_ = 1;
        std::vector<MatchEvent> event_log_; // Chronological list of all events

        void log(const std::string& event_description) {
            event_log_.emplace_back(current_quarter_, event_description);
            // emplace_back constructs MatchEvent in-place → efficient
        }

        void awardCard(Team& team, CardType type) {
            team.awardCard(type);

            // Automatic display name from enum using magic_enum
            std::string card_name = std::string(magic_enum::enum_name(type));

            log(card_name + " card - " + team.name());
        }

        void awardPenaltyCorner(Team& team) {
            team.awardPenaltyCorner();
            log("Penalty corner - " + team.name());
        }

        void scoreGoalFor(Team& team) {
            team.scoreGoal();
            log(team.name() + " goal!");
        }

    public:
        // Constructor initializes the two teams
        HockeyMatch(std::string home_name, std::string away_name) : 
            home_team_(std::move(home_name)), away_team_(std::move(away_name)) {
                std::cout << "Created home team and away team" << std::endl;
            };

        // --------------------- Const accessors ---------------------
        const Team& home() const                            { return home_team_; }
        const Team& away() const                            { return away_team_; }
        int quarter() const                                 { return current_quarter_; }
        const std::vector<MatchEvent>& events() const       { return event_log_; }

        // --------------------- Game actions ---------------------

        void goalForHome() { scoreGoalFor(home_team_); }
        void goalForAway() { scoreGoalFor(away_team_); }


        void cardForHome(CardType type)  { awardCard(home_team_, type); }
        void cardForAway(CardType type)  { awardCard(away_team_, type); }

        void penaltyCornerForHome()      { awardPenaltyCorner(home_team_); }
        void penaltyCornerForAway()      { awardPenaltyCorner(away_team_); }


        // Returns false when match is over (after quarter 4)
        bool nextQuarter() {
            if (current_quarter_ < 4) {
                ++current_quarter_;
                log("=== End of Q" + std::to_string(current_quarter_ - 1) + " ===");
                return true;
            }
            return false;
        }

        // --------------------- Display functions ---------------------
        void printScoreboard() const {}
        void printEventLog() const {}
        void clearScreen() { std::cout << "\x1B[2J\x1B[H"; }
};


int main() {
    std::string welcome_message = "Welcome to Field Hockey Scoreboard Simulator (Professional OOP Edition)!\n";
    std::cout << welcome_message << std::endl;

    std::string home_name, away_name;
    std::cout << "Enter home team: ";
    std::getline(std::cin, home_name);
    std::cout << "Enter away team: ";
    std::getline(std::cin, away_name);

    HockeyMatch match(std::move(home_name), std::move(away_name));
    std::cout << "Match ended. Thank you for using the simulator!\n";

    return 0;
}
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
#include <array>
#include "magic_enum.hpp"  // Header-only library for automatic enum-to-string

enum class CardType { Green, Yellow, Red }; // CardType::Green etc. strongly-typed enum.

// -----------------------------------------------------------------------------
// Team class – encapsulates team state and behavior
// -----------------------------------------------------------------------------
class Team {
    private: // underscores distinguish private member variables from local variables
        std::string name_;
        int goals_ = 0;
       
        // one counter per card type. CardType values are contiguous starting from 0.
        std::array<int, magic_enum::enum_count<CardType>()> card_counts_{};  // all zero-initialized        
        
        int penalty_corners_ = 0;

    public:
        explicit Team(std::string name) : name_(std::move(name)) {}
        // "Create a Team from a string. Do not allow implicit conversion.
        
        const std::string& name() const             { return name_; }
        int goals() const                           { return goals_; }
        int penaltyCorners() const                  { return penalty_corners_; }

        // per-card getters:
        int cardCount(CardType type) const {
            return card_counts_[magic_enum::enum_index(type).value()];  // .value() safe due to exhaustive enum
        }

        int greenCards() const                      { return cardCount(CardType::Green); }
        int yellowCards() const                     { return cardCount(CardType::Yellow); }
        int redCards() const                        { return cardCount(CardType::Red); }
    

        // actions - state changes
        void scoreGoal()            { ++goals_; }
        void awardPenaltyCorner()   { ++penalty_corners_; }

        void receiveCard(CardType type) {
            auto index = magic_enum::enum_index(type);

            if (index.has_value()) {
                ++card_counts_[index.value()];
            } else {
                throw std::invalid_argument("Invalid CardType");
            }
        }

        // formatted summary:
        std::string statsLine() const {
            return std::format("{}G {}Y {}R {}PC",
                greenCards(), yellowCards(), redCards(), penalty_corners_);
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
            event_log_.emplace_back(current_quarter_, event_description); // emplace_back constructs MatchEvent in-place
        }

        void scoreGoalFor(Team& team, const std::string& scorer = "") {
            team.scoreGoal();
            if (scorer.empty()) {
                log(team.name() + " goal!");
            } else {
                log(team.name() + " goal! (" + scorer + ")");
            }
        }

        void showCardFor(Team& team, CardType type) {
            team.receiveCard(type);
            std::string card_name = std::string(magic_enum::enum_name(type)); // Automatic display name from enum using magic_enum
            log(card_name + " card - " + team.name());
        }

        void awardPenaltyCornerFor(Team& team) {
            team.awardPenaltyCorner();
            log("Penalty corner - " + team.name());
        }


    public:
        HockeyMatch(std::string home_name, std::string away_name) : 
            home_team_(std::move(home_name)), away_team_(std::move(away_name)) {};

        // --------------------- Const accessors ---------------------
        const Team& home() const                            { return home_team_; }
        const Team& away() const                            { return away_team_; }
        int quarter() const                                 { return current_quarter_; }
        const std::vector<MatchEvent>& events() const       { return event_log_; }


        // --------------------- Game actions ---------------------
        void goalForHome()                                  { scoreGoalFor(home_team_); }
        void goalForAway()                                  { scoreGoalFor(away_team_); }

        void cardForHome(CardType type)                     { showCardFor(home_team_, type); }
        void cardForAway(CardType type)                     { showCardFor(away_team_, type); }

        void penaltyCornerForHome()                         { awardPenaltyCornerFor(home_team_); }
        void penaltyCornerForAway()                         { awardPenaltyCornerFor(away_team_); }

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
        void printScoreboard() const {
            std::cout << "\n=== FIELD HOCKEY SCOREBOARD ===\n";

            std::cout << std::format("{:<20} {} - {} {:<20}\n",
                home_team_.name(), home_team_.goals(),
                away_team_.goals(), away_team_.name());

            std::cout << std::format("Quarter: {}/4\n\n", current_quarter_);

            std::cout << "Cards & PCs:\n";
            std::cout << std::format("{:<20} {}\n", home_team_.name(), home_team_.statsLine());
            std::cout << std::format("{:<20} {}\n", away_team_.name(), away_team_.statsLine());
            std::cout << "================================\n\n";
        }


        void printEventLog() const {
            std::cout << "\n--- Event Log ---\n";
            if (event_log_.empty()) { std::cout << "No events yet.\n"; }
            else {
                for (const auto& event : event_log_) {
                    std::cout << event.toString() << "\n";
                }
            }
            std::cout << "-----------------\n\n";
        }
};

// display things
void clearScreen() {
    std::cout << "\x1B[2J\x1B[H";  // ANSI escape code – works on macOS, Linux, most terminals
}

int main() {
    std::cout << "🏑 Welcome to Field Hockey Scoreboard Simulator 🏑\n\n";

    std::string home_name, away_name;
    std::cout << "Enter home team: ";
    std::getline(std::cin, home_name);
    std::cout << "Enter away team: ";
    std::getline(std::cin, away_name);

    HockeyMatch match(std::move(home_name), std::move(away_name));

    while (match.quarter() <= 4) {
        clearScreen();
        match.printScoreboard();

        std::cout << "Actions:\n"
                  << "1. Goal " << match.home().name() << "\n"
                  << "2. Goal " << match.away().name() << "\n"
                  << "3. Green card\n"
                  << "4. Yellow card\n"
                  << "5. Red card\n"
                  << "6. Penalty corner\n"
                  << "7. Next quarter\n"
                  << "8. Show event log\n"
                  << "9. Quit match early\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        std::cin.ignore();  // consume newline after number

        switch (choice) {
            case 1:
                match.goalForHome();
                break;
            case 2:
                match.goalForAway();
                break;
            case 3: case 4: case 5: {
                char side;
                std::cout << "For which team? (h = " << match.home().name()
                          << ", a = " << match.away().name() << "): ";
                std::cin >> side;
                std::cin.ignore();

                CardType type;
                if (choice == 3) type = CardType::Green;
                else if (choice == 4) type = CardType::Yellow;
                else type = CardType::Red;

                if (side == 'h' || side == 'H')
                    match.cardForHome(type);
                else if (side == 'a' || side == 'A')
                    match.cardForAway(type);
                else
                    std::cout << "Invalid team choice.\n";

                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                break;
            }
            case 6: {
                char side;
                std::cout << "For which team? (h/a): ";
                std::cin >> side;
                std::cin.ignore();

                if (side == 'h' || side == 'H')
                    match.penaltyCornerForHome();
                else if (side == 'a' || side == 'A')
                    match.penaltyCornerForAway();
                else
                    std::cout << "Invalid team choice.\n";

                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                break;
            }
            case 7:
                if (!match.nextQuarter()) {
                    goto end_match;
                }
                break;
            case 8:
                clearScreen();
                match.printEventLog();
                std::cout << "Press Enter to return to scoreboard...";
                std::cin.get();
                break;
            case 9:
                std::cout << "Ending match early...\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                goto end_match;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

end_match:
    clearScreen();
    std::cout << "\n=== FINAL RESULT ===\n";
    match.printScoreboard();
    match.printEventLog();
    std::cout << "Match ended. Thank you for using the Field Hockey Scoreboard Simulator!\n\n";

    return 0;
}
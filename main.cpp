// main.cpp
// Field Hockey Scoreboard Simulator
// Beginner C++ project – December 2025

#include <iostream> // for input and output
#include <string> // for strings
#include <vector> // for vectors
#include <format> // C++20 formatted output
#include <chrono> // for time
#include <thread> // for threading
#include <array>
#include <limits> // bulletproof against input garbage
#include <cstdlib>  // for std::system
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <utility>


constexpr int TOTAL_QUARTERS = 4;

enum class CardType : unsigned char { Green = 0, Yellow = 1, Red = 2, Count };

constexpr std::string_view cardName(CardType type) noexcept {
    switch (type) {
        case CardType::Green:  return "Green";
        case CardType::Yellow: return "Yellow";
        case CardType::Red:    return "Red";
        case CardType::Count:  break;
    }
    return "Unknown";
}


// Helpers
void ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


// -----------------------------------------------------------------------------
// Team class – encapsulates team state and behavior
// -----------------------------------------------------------------------------
class Team {
    private: // underscores distinguish private member variables from local variables
        std::string name_;
        int goals_ = 0, green_ = 0, yellow_ = 0, red_ = 0, penalty_corners_ = 0;

    public:
        explicit Team(std::string name) : name_(std::move(name)) {}
        // "Create a Team from a string. Do not allow implicit conversion.
        
        const std::string& name() const noexcept    { return name_; }
        int goals() const noexcept                  { return goals_; }
        int penaltyCorners() const noexcept         { return penalty_corners_; }

        int greenCards() const noexcept             { return green_; }
        int yellowCards() const noexcept            { return yellow_; }
        int redCards() const noexcept               { return red_; }
    

        // actions - state changes
        void scoreGoal() noexcept { ++goals_; }
        void awardPenaltyCorner() noexcept { ++penalty_corners_; }

        void receiveCard(CardType type) noexcept {
            switch (type) {
                case CardType::Green:  ++green_; break;
                case CardType::Yellow: ++yellow_; break;
                case CardType::Red:    ++red_; break;
                case CardType::Count:  break;
            }
        }

        // formatted summary:
        std::string statsLine() const {
            std::ostringstream oss;
            oss << green_ << "G "
                << yellow_ << "Y "
                << red_ << "R "
                << penalty_corners_ << "PC";
            return oss.str();
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
            std::ostringstream oss;
            oss << "Q" << quarter_ << " - " << description_;
            return oss.str();
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

        void addEvent(const std::string& event_description) {
            event_log_.emplace_back(current_quarter_, event_description); // emplace_back constructs MatchEvent in-place
        }

        void scoreGoalFor(Team& team, const std::string& scorer = {}) {
            team.scoreGoal();
            if (scorer.empty()) {
                addEvent(team.name() + " goal!");
            } else {
                addEvent(team.name() + " goal! (" + scorer + ")");
            }
        }

        void showCardFor(Team& team, CardType type) {
            team.receiveCard(type);
            addEvent(std::string(cardName(type)) + " card - " + team.name());

        }

        void awardPenaltyCornerFor(Team& team) {
            team.awardPenaltyCorner();
            addEvent("Penalty corner - " + team.name());
        }


    public:
    // constructor:
    HockeyMatch(std::string home_name, std::string away_name)
        :   home_team_(std::move(home_name)),
            away_team_(std::move(away_name)) {
            addEvent("=== Start of Q1 ===");
        }


        // --------------------- Const accessors ---------------------
        const Team& home() const noexcept                            { return home_team_; }
        const Team& away() const noexcept                           { return away_team_; }
        int quarter() const noexcept                                 { return current_quarter_; }
        const std::vector<MatchEvent>& events() const       { return event_log_; }


        // --------------------- Game actions ---------------------
        void goalForHome()  { scoreGoalFor(home_team_); }
        void goalForAway()  { scoreGoalFor(away_team_); }

        void cardForHome(CardType type) { showCardFor(home_team_, type); }
        void cardForAway(CardType type) { showCardFor(away_team_, type); }

        void penaltyCornerForHome() { awardPenaltyCornerFor(home_team_); }
        void penaltyCornerForAway() { awardPenaltyCornerFor(away_team_); }

        // Returns false when match is over (after quarter 4)
        bool nextQuarter() {
            if (current_quarter_ > TOTAL_QUARTERS) {
                return false;
            }
        
            // Always log the end of the current quarter
            addEvent("=== End of Q" + std::to_string(current_quarter_) + " ===");
        
            if (current_quarter_ < TOTAL_QUARTERS) {
                ++current_quarter_;
                addEvent("=== Start of Q" + std::to_string(current_quarter_) + " ===");
                return true;
            }
        
            // After Q4 ends, match is over — no start of Q5
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
            if (event_log_.empty()) {
                std::cout << "No events yet.\n";
            } else {
                for (const auto& event : event_log_) {
                    std::cout << event.toString() << "\n";
                }
            }
            std::cout << "-----------------\n\n";
        }
};

// display things
static void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::cout << "\x1B[2J\x1B[H" << std::flush;
    #endif
    }

int main() {
    std::cout << "🏑 Welcome to Field Hockey Scoreboard Simulator 🏑\n\n";

    std::string home_name;
    std::string away_name;

    std::cout << "Enter home team: ";
    std::getline(std::cin, home_name);
    std::cout << "Enter away team: ";
    std::getline(std::cin, away_name);

    if (home_name.empty()) { home_name = "Home"; }
    if (away_name.empty()) { away_name = "Away"; }

    HockeyMatch match(std::move(home_name), std::move(away_name));

    bool match_in_progress = true;

    while (match_in_progress && match.quarter() <= TOTAL_QUARTERS) {
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

        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            ignoreLine();
            std::cout << "Invalid input. Please enter a number.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        ignoreLine();

        switch (choice) {
            case 1:
                match.goalForHome(); break;
            case 2:
                match.goalForAway(); break;
            case 3: case 4: case 5: {
                char side;
                std::cout << "For which team? (h = " << match.home().name()
                          << ", a = " << match.away().name() << "): ";
                std::cin >> side;
                ignoreLine();

                const CardType type = (choice == 3) ? CardType::Green
                                    : (choice == 4) ? CardType::Yellow
                                                    : CardType::Red;

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
                char side = '\0';
                std::cout << "For which team? (h/a): ";
                std::cin >> side;
                ignoreLine();

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
                    match_in_progress = false;
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
                match_in_progress = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
        }
    }

clearScreen();
std::cout << "\n=== FINAL RESULT ===\n";
match.printScoreboard();
match.printEventLog();
std::cout << "Match ended. Thank you for using the Field Hockey Scoreboard Simulator!\n\n";

return 0;
}
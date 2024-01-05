#include <iostream>
#include "json.hpp"
using namespace std;

class Game {
    public:

        string playerInput() {
            string input;
            cout << "enter an action: " ;
            getline(cin, input);
            return input;
        }

        vector<string>choppedInput() {
            string input = playerInput();
            vector<string>choppedInput_;

            string word;
            istringstream iss(input);
            while (iss >> word) {
                choppedInput_.push_back(word);
            }
            return choppedInput_;
        }

        vector<string> playerEquip() {
            return equipment;
        }

        void updatePlayerEquip(vector<string> v) {
            equipment = v;
        }

        string helpCommands() {
            string help;
            cout << "List of possible inputs: " << endl;
            cout << "go []: travel between rooms" << endl;
            cout << "take []: take an object in a room" << endl;
            cout << "look []: look around for objects" << endl;
            cout << "kill []: kill an enemy in a room" << endl;
            cout << "check: check your inventory" << endl;
            return help;
        }

        int roomId = 0;
        string word;

    private:
        vector<string> equipment;
};
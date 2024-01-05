#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "game.h"
using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        //read from file
        ifstream fin(argv[1]);
        json j;
        fin >> j;

        Game game;
        
        //set to objective
        bool winCondition = false;

        string objective = "";
        if (j["objective"]["type"] = "kill") {
            string objective = j["objective"]["what"].get<string>();
        }
        else if (j["objective"]["type"] = "collect") {
            string objective = j["objective"]["what"].get<string>();
        }
        else if (j["objective"]["type"] = "room") {
            string objeactive = j["objective"]["what"].get<string>();
        }

        //init starter room desc
        cout << "the game requires you to input commands twice for one action! i couldn't find a solution in time so i had no choice but to leave it..." << endl;
        cout << "type 'help' for list of possible inputs" << endl;
        cout << j["rooms"][game.roomId]["desc"] << endl;
        string currentRoom = j["player"]["initialroom"];

        while (winCondition == false) {
            game.playerInput();
            vector<string>words = game.choppedInput();//chops input into individual words

            auto it = find(words.begin(), words.end(), "go");//travel
            if (it != words.end()) {
                string direction = j["rooms"][game.roomId]["exits"][words[1]].get<string>();//finds exit direction input from json

                //enemy check
                string initialRoom = j["enemies"][0]["initialroom"].get<string>();//get enemy initial room (zombie = room2)
                if (currentRoom == initialRoom) {//evaluate if we are in same room
                    int randNum = rand() % 100+1;
                    int agg = j["enemies"][0]["aggressiveness"].get<int>();
                    if (agg <= randNum) {
                        cout << "attacked as you were leaving!" << endl;
                        cout << "game over..." << endl;
                        exit(0);
                    }
                }

                //room change
                for (auto e : j["rooms"][game.roomId]["exits"].items()) {
                    string s = e.key();
                    if (words[1] == s) {
                        currentRoom = direction;//sets currentRoom from 'room1' to 'room2'
                        for (auto e : j["rooms"].items()) {//search rooms for id
                            string s = e.key();
                            int s2 = stoi(s);//convert to int
                            if (j["rooms"][s2]["id"] == currentRoom) {
                                game.roomId = s2;//save new roomId
                            }
                        }
                        if (currentRoom == objective) {
                            winCondition = true;
                        }
                        cout << j["rooms"][game.roomId]["desc"] << endl;
                    }
                    else {
                        cout << "invalid move. try another direction?" << endl;
                    }
                }
            }

            auto it2 = find(words.begin(), words.end(), "take");//take
            if (it2 != words.end()) {
                vector<string> v;
                for (auto e : j["objects"].items()) {//search objects
                    string s  = e.key();
                    int s2 = stoi(s);//need int for next step
                    if (j["objects"][s2]["initialroom"] == currentRoom) {//checks if item is in the room
                        string item = j["objects"][s2]["id"].get<string>();
                        v.push_back(item);
                        cout << "picked up " + item << endl;
                        if (item == objective) {
                            winCondition = true;
                        }
                    }
                    else cout << "nothing to pick up" << endl;
                }
                game.updatePlayerEquip(v);
            }

            auto it3 = find(words.begin(), words.end(), "look");//look around
            if (it3 != words.end()) {
                cout << "There are: " << endl;
                for (auto e : j["objects"].items()) {
                    string s = e.key();
                    int s2 = stoi(s);
                    if (j["objects"][s2]["initialroom"] == currentRoom) {
                        string object = j["objects"][s2]["id"].get<string>();
                        cout << object << endl;
                    }
                }
                cout << "Objects in this room." << endl;
            }

            auto it4 = find(words.begin(), words.end(), "kill");//attack
            if (it4 != words.end()) {
                for (auto e : j["enemies"].items()) {
                    string s = e.key();
                    int s2 = stoi(s);
                    if (j["enemies"][s2]["initialroom"] == currentRoom) {//check if same room
                        vector<string> v = j["enemies"][s2]["killedby"].get<vector<string>>();
                        for (string s : v) {//for each enemy
                            for (string t : game.playerEquip()) {
                                if (s == t) {
                                    cout << "killed zombie!" << endl;
                                    if (s == objective) {
                                        winCondition = true;
                                    }
                                }
                                else {
                                    cout << "could not kill zombie!" << endl;
                                    cout << "game over..." << endl;
                                    exit(0);
                                }
                            }
                        }
                    }
                    else cout << "no enemies to attack..." << endl;
                }
            }

            auto it5 = find(words.begin(), words.end(), "check");//check stats
            if (it5 != words.end()) {
                if (game.playerEquip().empty()) {
                    cout << "you are carrying no items" << endl;
                }
                else {
                    cout << "current inventory: " << endl;
                    for (string word : game.playerEquip()) {
                        cout << word << " " << endl;
                    }
                }
            }

            auto it6 = find(words.begin(), words.end(), "help");
            if (it6 != words.end()) {
                game.helpCommands();
            }
        }
        cout << "you win! congratulations!" << endl;
        exit(0);
    }
    else cout << "Please enter a map file (Invalid Input)" << endl;
}
// made by cocos
// use it like you want to use it
// just give credit and dont sell this

// pls dont break

#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include "sdl_utils.h"
#include "SheetCreator.h"

using namespace std;


// here are the config.txt values stored
struct {
    string version;
    int wSize;
    bool aOnTop;
} fConfig;

// g++ compile
// g++ -o fruity fruity.cpp sdl_utils.cpp SheetCreator.cpp -ISTB -ISDL2/include -LSDL2/lib -lSDL2 -lSDL2_image

int extractValue(const string& line, const string& prefix) {
    if (line.find(prefix) == 0) {  // Überprüfe, ob die Zeile mit dem Präfix beginnt
        string numberPart = line.substr(prefix.length());  // Entferne das Präfix
        try {
            return stoi(numberPart);  // Konvertiere den verbleibenden Teil in eine Ganzzahl
        } catch (const invalid_argument& e) {
            cerr << "Invalid number: " << numberPart << endl;
        } catch (const out_of_range& e) {
            cerr << "Number out of range: " << numberPart << endl;
        }
    }
    return -1;  // Gib einen Fehlerwert zurück, wenn das Präfix nicht gefunden wird
}

int print(const string& message) {
    cout << "[Fruity Animation Player] " << message << endl;
    return 0;
}

bool isNumber(const string& str) {
    try {
        size_t pos;
        int num = stoi(str, &pos);
        return pos == str.size() && num <= 128;
    } catch (invalid_argument&) {
        return false; 
    } catch (out_of_range&) {
        return false; 
    }
}

// creating animation directory and prop file
int createAnim(const string& name, int px, int delay) {
    if (filesystem::exists("./" + name)) {
        print("ERROR: Duplication found!");
        return 0;
    }
    print("Animation Name: " + name);
    string pix = to_string(px);
    print("Sprite Size: " + pix + " (" + pix + "x" + pix + ")");
    print("Creating new Animation...");
    try {
        filesystem::create_directory("./" + name); // yay create dir
    } catch(exception&) {
        print("There was an error while creating your Animation.");
        return 0;
    }
    try {
        ofstream MyFile("./" + name + "/PROP.txt"); // yay create prop file :D
        MyFile << "name=" << name;
        MyFile << endl;
        MyFile << "pbDelay=" << delay;
        MyFile.close();
    } catch(exception&) {
        print("There was an error creating the properties file for your animation! Try running Command Prompt with Administrator");
        return 0;
    }
    print("Success! Please order your Animation Sprites like this: 1.png, 2.png, etc.");
    print("You can live edit your animation at runtime! \nTo do this simply type 'fruity play " + name + "' inside your command prompt.");
    print("Have fun!");
    return 1;
}

int playAnim(const string& name, int msDelay = 250) {
    print("Searching for animation '" + name + "'...");
    if (!filesystem::exists("./" + name)) {
        print("Couldn't find animation '" + name + "' in this directory!");
        return 0;
    }
    print("NOTE: You can edit your Sprites while Runtime and it will be changed live!");
    print("Started playback.");
    return displayAnimation(name, msDelay, fConfig.wSize, fConfig.aOnTop); // sdl_utils.cpp will display the animation
}


// shit
void OpenFileWithEditor(const std::string& path) {
    std::string command = "start \"\" \"" + path + "\"";
    system(command.c_str());
}

// gets the fruity.exe path
std::string getExecutablePath() {
    char buffer[1024];
    GetModuleFileNameA(NULL, buffer, sizeof(buffer));
    return std::string(buffer);
}

// ^same
std::string getExecutableDirectory() {
    std::string execPath = getExecutablePath();
    return execPath.substr(0, execPath.find_last_of("\\/"));
}


// heres all the command handling and stuff
int main(int argc, char* argv[]) {
    // change cmd title with windows api
    SetConsoleTitleA("fruity");

    // get the path to the fruity.exe
    std::string execDir = getExecutableDirectory();
    std::string configPath = execDir + "/config.txt";

    fConfig.version = "public-1.0";

    // get Values from Config File
    if (filesystem::exists(configPath)) {
        ifstream confi(configPath);
        string line;
        if(getline(confi, line)) {
            fConfig.wSize = extractValue(line, "windowSize=");
            if(getline(confi, line)) {
                fConfig.aOnTop = extractValue(line, "windowFocus=");
            }
        }
        confi.close();
    } else {
        print("fruity didnt found a config file. Creating config...");
        ofstream conf(configPath);
        conf << "windowSize=300" << endl;
        conf << "windowFocus=1";
        conf << endl << endl;
        conf << "// windowSize in Pixel" << endl;
        conf << "// windowFocus = 1 - Window will always be rendered on Top of everything else even when its not in focus";
        conf.close();
        print("Created Config file!");
        return 0;
    }

    if (argc < 2) {
        string ver = "v1.4";
        cout << endl;
        cout << "fruity-animation-player-" + fConfig.version + " by cocos";
        cout << "\nfruity is a easy to use Sprite Animation Player designed for creating sprites for games";
        cout << "\nYou can also just do normal sprites with it by just ignoring this 'px' value inside prop files.";
        cout << "\nIf you need help simply type 'fruity help' for a command list";
        cout << endl;
        return 0;
    }
    
    string command = string(argv[1]);

    if (command == "create") {
        if (argc == 4) {
            string animName = string(argv[2]);
            int pixels = stoi(argv[3]);
            int delay = stoi(argv[4]);
            command = "";
            createAnim(animName, pixels, delay);
            return 1;
        } else {
            print("Invalid or incomplete syntax");
            return 0;
        }
    } else if (command == "play") {
        if (argc == 3) {
            string animName = string(argv[2]);
            if (animName.length() > 28) {
                print("Illegal Animation name. (Maybe too long?)");
                return 0;
            }
            ifstream file("./" + animName + "/PROP.txt");
            int spee;
            string line;
            if (file.is_open()) {
                for (int i = 0; i < 3 && getline(file, line); ++i) {
                    // Skip to the third line
                }
                if (file) {
                    command = "";
                    spee = extractValue(line, "pbDelay=");
                    while (file.is_open()) {
                        playAnim(animName, spee);
                        file.close();
                    }
                } else {
                    print("Invalid PROP file! Try refreshing the Animation with 'fruity refresh " + animName + "'");
                }
            } else {
                print("Could not open PROP file!");
            }
        } else {
            print("Invalid or incomplete syntax");
        }
    } else if (command == "refresh") {
        command = "";
        if (argc == 3) {
            string animName = string(argv[2]);
            print("Refreshing PROP file...");
            ifstream file("./" + animName + "/PROP.txt");
            if (file.is_open()) {
                string line;
                bool updated = false;
                if (getline(file, line)) {
                    // Read second line
                    if (getline(file, line)) {
                            print("PROP file is up to date.");
                        } else {
                            updated = true;
                            int del;
                            print("Your Prop file is missing a Playback delay. Please type in a Delay in Milliseconds.");
                            cout << "> ";
                            cin >> del;
                            file.close();
                            ofstream wfile("./" + animName + "/PROP.txt", ios_base::app);
                            wfile << endl <<"pbDelay=" << del;
                            wfile.close();
                            file.open("./" + animName + "/PROP.txt");
                            print("PROP file updated.");
                        }
                } else {
                    updated = true;
                    string nam;
                    print("Your Prop file is missing a Name. Please type in an Animation name.");
                    cout << "> ";
                    cin >> nam;
                    file.close();
                    ofstream wfile("./" + animName + "/PROP.txt", ios_base::app);
                    wfile << "name=" << nam;
                    wfile.close();
                    file.open("./" + animName + "/PROP.txt");
                    print("PROP file updated.");
                }
                file.close();
                if (!updated) {
                    print("PROP file was already up to date.");
                }
            } else {
                print("Could not open PROP file!");
            }
        } else {
            print("Invalid or Incomplete Syntax.");
            return 0;
        }
    } else if (command == "help") {
        // will print the help list
        command = "";
        print("Fruity Help list");
        print(" ");
        print("fruity create <name> <playbackDelay>(In MS)");
        print("Creates a new Animation");
        print(" ");
        print("fruity play <name>");
        print("Plays an Animation when the specified Animation is in the Directory");
        print(" ");
        print("fruity sheet <name>");
        print("Creates a Sprite Sheet with a plist file for you!");
        print(" ");
        print("fruity refresh <name>");
        print("Updates a fruity Animation-Prop file to a newer Version so it's compatible.");
        print("Also can be used to fix a project if it wasn't messed up too much");
        print(" ");
        print("fruity config");
        print("Opens the Config File where you can edit the Window Size and Focus");
        print(" ");
        print("fruity help");
        print("sorry guys I tripped");
        return 0;
    } else if(command == "config") {
        OpenFileWithEditor(configPath);
        print("Opening config");
        return 0;
    } else if(command == "sheet") {
        string name = string(argv[2]);
        print("Searching for animation '" + name + "'...");
        if(filesystem::exists("./" + name)) {
            print("Found animation folder!");
            if(filesystem::exists("./" + name + "/PROP.txt")) {
                print("Found prop file!");
                ifstream file("./" + name + "/PROP.txt");
                string line;
                bool validProp = false;

                while (getline(file, line)) {
                    if (line.find("name=") == 0) {
                        string propName = line.substr(5); // get name value from "name="
                        if (propName == name) {
                            validProp = true;
                            break;
                        }
                    } else {
                        print("Invalid prop file! Try 'fruity refresh " + name + "'!");
                        return 0;
                    }
                }
                if(validProp) {
                    print("Checked Prop file: Succes!");
                    print("Creating sheet and plist...");
                    exportSheet(string(name));
                    print("Finished!");
                    return 1;
                } else {
                    print("Invalid prop file!");
                    return -1;
                }
            } else {
                print("No animation prop file found.");
                return 0;
            }
        } else {
            print("No animation folder found.");
            return 0;
        }
    }

    return 0;
}

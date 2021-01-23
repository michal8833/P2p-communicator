#include <config/Config.h>

string Config::configFile = CONFIG_DEFAULT_FILE_PATH;
unordered_map<string, string> Config::values;

string& Config::get(string& key) {
    return values.at(key);
}

string& Config::get(const char* key) {
    return values.at(string(key));
}

void Config::init() {
    configFile = CONFIG_DEFAULT_FILE_PATH;
    if (shouldCreateDefaultConfig()) {
        writeDefaultConfig();
    }

    loadConfiguration();
}

void Config::init(string& filename) {
    configFile = filename;
    if (shouldCreateDefaultConfig()) {
        writeDefaultConfig();
    }

    loadConfiguration();
}

void Config::loadConfiguration() {
    ifstream file(configFile);
    if (file.is_open()) {
        for (string line; getline(file, line);) {
            int index = line.find('=');
            string key = line.substr(0, index);
            string value = line.substr(index + 1, line.size());
            values[key] = value;
        }
    } else {
        throw new IOException("Failed opening the configuration file");
    }
    file.close();
}

void Config::writeDefaultConfig() {
    ofstream file(configFile);
    if (!file.is_open()) {
        throw new IOException("Failed creating default configuration file");
    }
    file << CONFIG_DEFAULT_FILE_CONTENTS;
    file.close();
}

bool Config::shouldCreateDefaultConfig() {
    ifstream file(Config::configFile);
    if (!file.is_open()) {
        return true;
    } else {
        file.close();
        return false;
    }
}

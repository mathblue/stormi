#ifndef CONFIG
#define CONFIG
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

class Config {
public:
    static const Config& getInstance() {
        static Config instance;
        return instance;
    }

    void loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open config file");
        }

        std::unordered_map<std::string, std::string> configValues;
        std::string line;

        //add a try except

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key, value;
            if (std::getline(ss, key, '=') && std::getline(ss, value)) {
                configValues[key] = value;
            }
        }

        N_BOIDS = std::stoi(configValues["N_BOIDS"]);
        dt = std::stod(configValues["dt"]);
        T_TOT = std::stod(configValues["T_TOT"]);

        VMAX = std::stod(configValues["VMAX"]);
        TorusSpace = configValues["TorusSpace"] == "true";
        repulsion = std::stod(configValues["repulsion"]);

        XSpace[0] = std::stod(configValues["XSpace0"]);
        XSpace[1] = std::stod(configValues["XSpace1"]);
        YSpace[0] = std::stod(configValues["YSpace0"]);
        YSpace[1] = std::stod(configValues["YSpace1"]);

        d = std::stod(configValues["d"]);
        d_s = std::stod(configValues["d_s"]);
        s = std::stod(configValues["s"]);
        a = std::stod(configValues["a"]);
        c = std::stod(configValues["c"]);
    }

    // Accessors (read-only)
    int getNBoids() const { return N_BOIDS; }
    double getDt() const { return dt; }
    double getTTot() const { return T_TOT; }
    double getVMax() const { return VMAX; }
    bool isTorusSpace() const { return TorusSpace; }
    double getRepulsion() const { return repulsion; }
    const double* getXSpace() const { return XSpace; }
    const double* getYSpace() const { return YSpace; }
    double getD() const { return d; }
    double getDS() const { return d_s; }
    double getS() const { return s; }
    double getA() const { return a; }
    double getC() const { return c; }

private:
    Config() {}  // Private constructor for Singleton
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Parameters
    int N_BOIDS;
    double dt;
    double T_TOT;
    double VMAX;
    bool TorusSpace;
    double repulsion;
    double XSpace[2];
    double YSpace[2];
    double d;
    double d_s;
    double s;
    double a;
    double c;
};

#endif
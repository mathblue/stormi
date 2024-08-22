#ifndef CONFIG
#define CONFIG
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept> 
#include <string>
#include <unordered_map>

class Config
{
 public:
  static const Config& getInstance()
  {
    static Config instance;
    return instance;
  }

  void loadConfig(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open config file");
    }

    std::unordered_map<std::string, std::string> configValues;
    std::string line;

    try {
      while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key, value;
        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
          configValues[key] = value;
        }
      }

      N_BOIDS = std::stoi(configValues.at("N_BOIDS"));
      dt      = std::stod(configValues.at("dt"));
      T_TOT   = std::stod(configValues.at("T_TOT"));

      VMAX       = std::stod(configValues.at("VMAX"));
      TorusSpace = configValues.at("TorusSpace") == "true";
      repulsion  = std::stod(configValues.at("repulsion"));

      XSpace[0] = std::stod(configValues.at("XSpace0"));
      XSpace[1] = std::stod(configValues.at("XSpace1"));
      YSpace[0] = std::stod(configValues.at("YSpace0"));
      YSpace[1] = std::stod(configValues.at("YSpace1"));

      d   = std::stod(configValues.at("d"));
      d_s = std::stod(configValues.at("d_s"));
      s   = std::stod(configValues.at("s"));
      a   = std::stod(configValues.at("a"));
      c   = std::stod(configValues.at("c"));
    } catch (const std::invalid_argument& e) {
      throw std::runtime_error("Invalid argument in config file: "
                               + std::string(e.what()));
    } catch (const std::out_of_range& e) {
      throw std::runtime_error("Missing key in config file: "
                               + std::string(e.what()));
    } catch (const std::exception& e) {
      throw std::runtime_error("Error processing config file: "
                               + std::string(e.what()));
    }
  }

  int getNBoids() const
  {
    return N_BOIDS;
  }
  double getDt() const
  {
    return dt;
  }
  double getTTot() const
  {
    return T_TOT;
  }
  double getVMax() const
  {
    return VMAX;
  }
  bool isTorusSpace() const
  {
    return TorusSpace;
  }
  double getRepulsion() const
  {
    return repulsion;
  }
  const double* getXSpace() const
  {
    return XSpace;
  }
  const double* getYSpace() const
  {
    return YSpace;
  }
  double getD() const
  {
    return d;
  }
  double getDS() const
  {
    return d_s;
  }
  double getS() const
  {
    return s;
  }
  double getA() const
  {
    return a;
  }
  double getC() const
  {
    return c;
  }

 private:
  Config()
  {} // Private constructor for Singleton
  // Delete copy constructor and assignment operator
  Config(const Config&)            = delete;
  Config& operator=(const Config&) = delete;

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

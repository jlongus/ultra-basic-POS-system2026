#include <string>
#include <array>
#include <map>

// exercising use of singleton pattern, but i think
// using a namespace is fine.
class Config{
public: 
  static Config& instance(){
    static Config* instance = new Config();
    return *instance;
  }

  std::map<string, string>& getInventoryMap(){
    return INVENTORY_MAP;
  }
  std::string& getInventoryPath(){
    return INVENTORY_FILE_PATH;
  }
  std::uint8_t& getMaxLoginAttempts(){
    return MAX_LOGIN_ATTEMPTS;
  }
  std::array<string, 3>& getProductFields(){
    return PRODUCT_FIELDS;
  }

private:
  std::map<string, string> INVENTORY_MAP;
  std::array<string, 3> PRODUCT_FIELDS;
  std::uint8_t MAX_LOGIN_ATTEMPTS = 3;
  std::string INVENTORY_FILE_PATH;
};
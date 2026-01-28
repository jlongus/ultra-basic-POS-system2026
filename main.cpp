#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
#endif

#include "./include/screen_delete_items.hpp"
#include "./include/screen_edit_items.hpp"
#include "./include/screen_list_items.hpp"
#include "./include/screen_add_items.hpp"
#include "./include/global_vars.hpp"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <fstream>
#include <string>
#include <map>

using std::string, std::cout, std::cin;

Config GlobalConfig();

void censorOutput(char[], const uint8_t, const char);
void displayLoginScreen();

int loginUSER(const string, const char, uint8_t &);
int loginPASS(const string, const char, uint8_t &);
int8_t invMap_create(bool);

const string CORRECT_USERNAME = "Manager";
const string CORRECT_PASSWORD = "password";

unsigned char loginAttempts = 0;

int main(int argc, char *argv[])
{
  std::cout << std::fixed << std::setprecision(2);
  displayLoginScreen();

  if (invMap_create(true))
  {
    system("cls");
    std::cout << global_variables::INVENTORY_FILE_PATH << " could not be opened. The system will close without changes.";
    system("pause");

    return 1;
  }

  string RESULT = "login successful!";

  // da program
  while (1)
  {
    switch (SCREEN_MAIN(USERNAME, RESULT))
    {
    case -1:
      RESULT = "err -1 @ SCREEN_MAIN";
      break;
    case 0:
      RESULT = "unrecognized input";
      break;
    case 1:
      RESULT = "adding inventory";
      SCREEN_ADD(USERNAME, RESULT);
      break;
    case 2:
      RESULT = "editing inventory";
      SCREEN_EDIT(USERNAME, RESULT);
      break;
    case 3:
      RESULT = "deleting inventory";
      SCREEN_DEL(USERNAME, RESULT);
      break;
    case 4:
      RESULT = "listing inventory";
      SCREEN_LIST(USERNAME, RESULT);
      break;
    }
  }
}

// Censors text into a character of choice.
// @param
// @param
// @param
void censorOutput(char input[], const uint8_t readLength, const char censorChar)
{
  uint8_t i;
  for (i = 0; i < readLength;)
  {
    input[i] = getch();

    if (input[i] == '\r' || input[i] == '\n')
      break;
    if (input[i] == '\b')
    {
      if (i > 0)
      {
        std::cout << "\b \b"; // remove char from output
        i--;
      }
      continue;
    }

    std::cout << '*';
    i++;
  }
}

// Login logic for username. Returns 0 upon successful login
// @param USER is the correct, expected username
// @param MAX is the maximum allowed attempts
// @param curAttempt is the correct, expected username
int loginUSER(const string USER, const char MAX, uint8_t &curAttempt)
{
  string inputAttempt;
  while (1)
  {
    cout << "USERNAME: ";
    cin >> inputAttempt;
    if (USER.compare(inputAttempt) == 0)
      return 0; // successful login

    system("cls");

    cout << "LOGIN\n";
    cout << "> Incorrect USERNAME.\n";
    cout << "> You have " << MAX - curAttempt << " attempt(s) left. \n\n";

    if (curAttempt >= MAX)
      return 1;
    curAttempt++;
  }
}

// Login logic for password.
int loginPASS(const string PASS, const char MAX, uint8_t &curAttempt)
{
  string inputAttempt;
  while (1)
  {
    const uint8_t str_len = PASS.length();
    char input[str_len];

    cout << "PASSWORD: ";
    censorOutput(input, str_len, '*');

    if (PASS.compare(input) == 0)
      return 0; // successful login

    system("cls");

    cout << "LOGIN\n";
    cout << "> Incorrect PASSWORD.\n";
    cout << "> You have " << MAX - curAttempt << " attempt(s) left. \n\n";

    if (curAttempt >= MAX)
      return 1;
    curAttempt++;
  }
}

// Sorts inventory file into an unordered map, for easy access instead of constantly reading from the file itself. Returns 1 on failure.
int8_t invMap_create(bool view)
{
  using global_variables::INVENTORY_FILE_PATH,
      global_variables::INVENTORY_MAP;

  system("cls");

  std::ifstream f_inv(INVENTORY_FILE_PATH);

  if (!f_inv.is_open())
    return 1;

  // represents the amount of fields per item
  // item code, description, price
  const uint8_t ITEM_FIELDS = global_variables::productFields.size();

  uint16_t currentElement = 0;
  string current, previous;

  int logID;
  INVENTORY_MAP.clear();
  do
  {
    previous = current;
    getline(f_inv, current, '|');
    if (current.empty())
      break;

    if (current[0] == '\n')
    {
      if (current.size() == 1)
        break;
      string temp = current;
      current.erase();
      for (const char &c : temp)
      {
        if (c == '\n')
          continue;
        current += c;
      }
    }

    if (!current.compare(previous))
      break;
    uint8_t position = currentElement % fileElements;

    if (position == 0)
    {
      logID;
      try
      {
        logID = stoi(current);
      }
      catch (const std::invalid_argument &ex)
      {
        std::cerr << current << " was unable to be turned to an int";
        system("pause");
        exit(1);
      }
      INVENTORY_MAP[logID];
      INVENTORY_MAP[logID].SIX_DIGIT = current;
    }
    else if (position == 1)
      INVENTORY_MAP[logID].DESC = current;
    else if (position == 2)
    {
      try
      {
        INVENTORY_MAP[logID].PRICE = stof(current);
      }
      catch (const out_of_range &ex)
      {
        cerr << logID << " price was out of range float(" + sizeof(float) + ')';
        system("pause");
        exit(1);

        INVENTORY_MAP.erase(logID);
        continue;
      }
      catch (const invalid_argument &ex)
      {
        cerr << logID << " price was invalid (" + current + ")";
        system("pause");
        exit(1);

        INVENTORY_MAP.erase(logID);
        continue;
      }
    }
    currentElement++;
  } while (current.compare(previous));
  f_inv.close();

  if (view)
  {
    system("cls");
    cout << "invMap_create(view=true): \n\n";
    for (pair kv : INVENTORY_MAP)
    {
      cout << kv.first << '_' << kv.second.SIX_DIGIT << '|' << kv.second.DESC << '|' << kv.second.PRICE << endl;
    }
    system("pause");
  }

  return 0;
}

void displayLoginScreen()
{
  cout << "LOGIN\n";

  if (loginUSER(USERNAME, MAX_ATTEMPTS, loginAttempts))
  {
    cout << "[!] USER LOGIN FAILED: Max attempts reached." << endl;
    system("pause");
    return 0;
  }
  system("cls"); // clear console
  cout << "LOGIN\n";

  loginAttempts = 0;
  if (loginPASS(PASSWORD, MAX_ATTEMPTS, loginAttempts))
  {
    cout << "[!] PASS LOGIN FAILED: Max attempts reached." << endl;
    system("pause");
    return 0;
  }
}

void clearConsole()
{
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
}
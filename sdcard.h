#include <SD.h> 
#include <FS.h>

#include <sqlite3.h>

#include <sstream>

// ----------------------------
// MicroSD Card Reader pins
// ----------------------------

#define CS 5

// ----------------------------

struct MicroSDCardReader
{
public:
  static int initializeMicroSDReader();

  static void setLastError(char* errorMessage);
  static char* getLastError();

private:
  static char* lastError;
};

class DatabaseReader
{
public:
  DatabaseReader(char* path)
  {
    sqlite3_open_v2((const char*)path, &connection, SQLITE_OPEN_READWRITE, NULL);
  };

  ~DatabaseReader()
  {
    sqlite3_close(connection);
  };

  int queryDatabase(char* query, int (*callback)(void *, int, char **, char **));

  void setLastError(char* errorMessage)
  {
    lastError = errorMessage;
  };

  char* getLastError()
  {
    return lastError;
  };

private:
  char* lastError;

  static sqlite3* connection;
};
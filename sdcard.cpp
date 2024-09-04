#include "sdcard.hpp" 

sqlite3* DatabaseReader::connection;

char* MicroSDCardReader::lastError;

int MicroSDCardReader::initializeMicroSDReader()
{
  if (!SD.begin(CS))
  {
    setLastError("Failed to initalize MicroSD Card Reader. Is the MicroSD Card inserted correctly?");
    return 0;
  }

  if(SD.cardType() == CARD_NONE)
  {
    setLastError("No MicroSD Card Attached...");
    return 0;
  }
  return 1;
}

int DatabaseReader::queryDatabase(char* query, int (*callback)(void *, int, char **, char **))
{
  int status = sqlite3_exec(DatabaseReader::connection, query, callback, NULL, NULL);
  if (status != SQLITE_OK)
  {
    std::stringstream ss;
    ss << "SQLite Error: " << sqlite3_errmsg(DatabaseReader::connection);
    setLastError((char*)ss.str().c_str());
    return 0;
  }
  return 1;
}

void MicroSDCardReader::setLastError(char* errorMessage)
{
  MicroSDCardReader::lastError = errorMessage;
}

char* MicroSDCardReader::getLastError()
{
  return MicroSDCardReader::lastError;
}
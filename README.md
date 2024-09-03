# Don't Panic
Offline wikipedia reader for the CYD inspired by [The Hitchhiker's Guide to The Galaxy](https://en.wikipedia.org/wiki/The_Hitchhiker%27s_Guide_to_the_Galaxy)

## How to compile
- Open `DontPanic.ino` with the Arduino Ide. Make sure you have lvgl, TFT_eSPI, XPT2046_Bitbang, SD and Sqlite3Esp32 installed.
- Plug in your CYD into any usb port and choose it in the arduino ide. Set the type as `ESP32 Dev Module`.
- Click `Tools` on the top, then click `Partition Scheme` and select `Huge APP`.
- Click upload.

## How to Use
- Download and run [MediaWiki Parser](https://github.com/GlitchedPanda/MediaWikiParser) for the data.
- It will generate a .db file. Run the SQL Statement `CREATE VIRTUAL TABLE IF NOT EXISTS pages_fts USING fts4(title, content); INSERT INTO pages_fts (title, content) SELECT title, content FROM pages;` with the SQLite3 cli.
- Rename it to `data.db` if it isn't called that already.
- Put `data.db` onto a MicroSD card and plug it in to your CYD (Cheap Yellow Display)
- After you ahev started it click anywhere on the screen and you will be able to search.

## Limitations
The CYD does not have a lot of memory. It only has 256MB of it. This means that with large databaes (Like a wikipedia dump), it will often run out of memory. Therefore you should try to search in a way that doesn't make the CYD search too much. Any help to selve this issue would be greatly appreciated :).

## Resources
https://github.com/TheNitek/XPT2046_Bitbang_Arduino_Library

https://github.com/Bodmer/TFT_eSPI

https://github.com/siara-cc/esp32_arduino_sqlite3_lib

https://github.com/arduino-libraries/SD


https://www.sqlite.org/fts5.html

https://www.sqlite.org/cintro.html

https://www.sqlite.org/amalgamation.html


https://docs.lvgl.io/master/

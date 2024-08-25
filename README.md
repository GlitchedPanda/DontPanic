# Don't Panic
Offline wikipedia reader inspired by [The Hitchhiker's Guide to The Galaxy](https://en.wikipedia.org/wiki/The_Hitchhiker%27s_Guide_to_the_Galaxy)

## How to compile
- Get a copy of the sqlite3 amalgamation and place `sqlite3.c` and `sqlite3.h` in the libs directory.
- Open `DontPanic.ino` with the Arduino Ide. Make sure you have lvgl, TFT_eSPI and XPT2046_Touchscreen installed.
- Plug in your CYD into any usb port and choose it in the arduino ide. Set the type as `ESP32 Dev Module`.
- Click upload.

## Resources
https://github.com/PaulStoffregen/XPT2046_Touchscreen

https://github.com/Bodmer/TFT_eSPI


https://www.sqlite.org/fts5.html

https://www.sqlite.org/cintro.html

https://www.sqlite.org/amalgamation.html


https://docs.lvgl.io/master/

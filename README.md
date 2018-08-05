# OsuDbParser
A simple c++ program which parse binary data in osu!.db into an object

#### Please note that this program only works for osu version >= 20140609 (2014/06/09)!

# Usage
Examples are in main.cpp

# Caution
The keys used to access beatmaps dictionary in the object are the nameOfOsuFile (etc. Omoi - Snow Drive(01.23) (Kroytz) [Rumi's Expert].osu) which might not be unique(?), so there is a chance of beatmaps being overlapped by the beatmaps with same nameOfOsuFile. Method of checking if this happened is already in main.cpp.

If this problem exists, consider changing the key used to a unique attribute.

# Support
If you find this useful, please support me to help me get a better laptop!

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://paypal.me/CookieHoodie/0USD)

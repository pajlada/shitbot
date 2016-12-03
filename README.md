# shitbot

# installation instructions
<h3> Linux </h3>
1. clone this repo `git clone git@github.com:hemirt/shitbot.git`
2. cd into the repo directory `cd shitbot`
3. Create the build directory `mkdir build`
4. cd into the build directory `cd build`
5. Run cmake `cmake ..`
6. Compile the bot `make`
7. Start the bot with `./WNMAbot oauth username`
   
<h3> Windows </h3>
1. clone
2. mkdir build
3. cmake .. -G "Visual Studio 14 2015 Win64" -T v140_clang_c2
4. open WNMAbot.sln
5. open WNMAbot properties (in solution explorer)
6. choose C/C++ / Preprocessor / Preprocessor Definitions and add <b>ASIO_STANDALONE</b>
(you can optionally add
                _CRT_SECURE_NO_WARNINGS
                _WIN32_WINNT = 0x0501
   )
7. optionally you can C/C++ / Command Line add "-Wno-reorder -Wno-c99-extensions -Wno-macro-redefined -Wno-microsoft-enum-value -Wno-unused-local-typedef" (without quotes)
8. build with visual studio
9. find WNMAbot.exe (build\Release\ or build\Debug ...)
10. start from command line 
          with "WNMAbot.exe oauth username"
   for example "WNMAbot.exe oauth:adojsiodhsa myname"

# shitbot

# installation instructions
<h3> linux </h3>
1. clone
2. mkdir build
3. cmake ..
4. make
5. start with  "./WNMAbot oauth username"
   for example "./WNMAbot oauth:adojsiodhsa myname"
   
<h3> windows </h3>
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

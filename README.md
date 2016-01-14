# LCP_BWT
Computing the longest common prefix based on the Burrows Wheeler Transform

http://www.fer.unizg.hr/predmet/bio/


# Installation

1) Install the SDSL library. 
   This library contains succint data structures that are by this program.
   To download and install SDSL library use the following instructions.
   ```sh
  git clone https://github.com/simongog/sdsl-lite.git
  cd sdsl-lite
  ./install.sh
  ```
  
2) Clone this project to your local repository.
   This can be done using the following instruction.
  ```sh
  git clone https://github.com/wissil/LCP_BWT.git
  ```
  
3) Compilation.
   Position yourself inside the cloned repository and compile the code.
   ```sh
  cd LCP_BWT
  g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib main.cpp OrderedAlphabet.cpp WTRank.cpp Algorithm1A.cpp Algorithm2A.cpp -o program     -lsdsl -ldivsufsort -ldivsufsort64
  ```
  
4) Run the program. You can choose between various runtime options. First parameter is the input file. Second is the string defining what    implementation of rank function will be used. If the string "b_wt" is passed as an argument to main function, then    the Binary         Wavelet Tree is used to implement rank function. If the string is "m_wt", Multiary Wavelet Tree will be used, and it is       expected    to pass it addiotional n-arity parameter. If the string is "non_wt", the implementation that does not use any Wavelet Tree      will     be ran.

  a) run with Binary Wavelet Tree
  ```sh
  ./program input.txt b_wt
  ```
  b) run without the Wavelet Tree implementation
   ```sh
  ./program input.txt non_wt
  ```
  
  c) run with Multiary Wavelet Tree, n = 4
   ```sh
   ./program input.txt m_wt 4
   ```

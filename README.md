# 🧨 Mr Minesweeper

![GitHub repo size](https://img.shields.io/github/repo-size/mr-mrf-dev/Mr-Minesweeper)
[![CMake on multiple platforms](https://github.com/Mr-MRF-Dev/Mr-Minesweeper/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Mr-MRF-Dev/Mr-Minesweeper/actions/workflows/cmake-multi-platform.yml)
[![GitHub License](https://img.shields.io/github/license/mr-mrf-dev/mr-minesweeper)](/LICENSE)

Play the classic Minesweeper game in the terminal and have a great time! This is a simple terminal game developed with the C programming language.

![Screenshot](/images/screenshot.png)

> I recommend using [Windows Terminal](https://apps.microsoft.com/store/detail/windows-terminal/9N0DX20HK701)
> for the best experience.

## 📥 Getting Started

To get started with the game, follow these steps:

1. Clone the Repository

    If You Have [Git](https://git-scm.com/) installed:

    ```bash
    git clone https://github.com/Mr-MRF-Dev/Mr-Minesweeper.git
    ```

    If You Have [Github CLI](https://cli.github.com/) installed:

    ```bash
    gh repo clone Mr-MRF-Dev/Mr-Minesweeper
    ```

2. Navigate to the Project Directory

    ```bash
    cd ./Mr-Minesweeper/src
    ```

3. Compile

    Compile the game using a C compiler. For example, using GCC:

    ```bash
    gcc .\Minesweeper.c -o Minesweeper
    ```

4. Run

    ```bash
    ./Minesweeper.exe
    ```

### Compile with CMake

after cloning the repository, you can compile the game using [CMake](https://cmake.org/download/):

1. Navigate to the Project Directory

    ```bash
    cd ./Mr-Minesweeper
    ```

2. Create a build directory

   ```bash
   mkdir build
   cd build
   ```

3. Build the project using the following commands:

    ```bash
    cmake ..
    make
    ```

    Or use [MinGW MakeFiles](https://cmake.org/cmake/help/latest/generator/MinGW%20Makefiles.html) by running: (Windows)

    ```bash
    cmake .. -G "MinGW Makefiles"
    mingw32-make
    ```

4. Run Project!

    ```bash
    ./Minesweeper.exe
    ```

## ✨️ Features

- Admin Panel: Gain access to the admin panel to perform administrative tasks.
- Prevent Wrong Inputs: The game prevents invalid inputs to ensure smooth game play.
- Custom Board: Customize the board size and mine count to adjust the difficulty level.
- Etc: Additional features provide an enhanced gaming experience.

## 🤝 Contributing

we welcome any contributions you may have. If you're interested in helping out, fork the repository
and create an [Issue](https://github.com/Mr-MRF-Dev/Mr-Minesweeper/issues) and
[PR](https://github.com/Mr-MRF-Dev/Mr-Minesweeper/pulls).

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](/LICENSE) file for details.

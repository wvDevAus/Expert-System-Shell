# Expert-System-Shell
A graphical system for developing and operating an Expert System AI.

## Dependencies
### Requirements
* Git
* CMake
* Perl
* Python
* C++ Compiler
  * Clang
  * GCC
  * MSVC
* Build System
  * Make
  * NMAKE
  * MSBuild
  * Ninja

### 3rd Party Libraries Used
This project depends upon the following 3rd party libraries:
* [JSON](https://github.com/nlohmann/json) - The framework for JSON file management
* [Qt5](https://code.qt.io/qt/qt5.git) - The toolkit used for GUI production.

## Building
**Make sure you have all of the dependencies already installed!**  
First, get a copy of this repository and it's submodules:

1. `git clone https://github.com/wvDevAus/Expert-System-Shell`
2. `cd Expert-System-Shell`
3. `git submodule update --init`

 Next, set-up your installation of Qt5:  
 **This step is OPTIONAL, and should only be done once!**
 
4. `cd lib/qt5`
5. `perl init-repository --module-subset=essential`
6. `mkdir ../qt5-build`
7. `cd ../qt5-build`
8. `../qt5/configure -developer-build -opensource -nomake examples -nomake tests`
    * Windows users should use `../qt5/configure.bat` instead.
    * You may need to elevate permissions (i.e. `sudo`) on this step.
    * Make sure to agree with the license, or repeat this step if you didn't!
9. `make -j$(nproc)`
    * Windows users should use one of the following instead:
        * `nmake` - If in doubt, use this
        * `jom`
        * `mingw32-make`

## License
This project is licensed under the [MIT License](LICENSE).

# URL Checker


### About

C++ Qt5 multi-threaded application for checking HTTP response status of urls (**Work in progress**).

### Screenshot

![Screenshot](_/screenshot.png)


### Requirements

- C++ compiler (>=C++17)
- CMake (>=3.15)
- Qt (>=5.11 tested with this but probably should work with older minor releases)


### Build instructions

Clone repository:
```
git clone https://github.com/fuzzy69/urlchecker.git
```
Navigate to project directory:
```
cd /urlchecker
```
Download dependencies:
```
git submodule update --init --recursive 
```
Create build directory somewhere (preferably out of project directory) and navigate into it:
```
mkdir build
cd build
```
Build project by passing the full path to a cloned urlchecker project:
```
cmake <YOUR PATH>/urlchecker
make
```
Run executable with:
```
src/urlchecker
```
Run tests with:
```
tests/tests
```

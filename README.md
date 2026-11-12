[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

[![GitHub forks](https://img.shields.io/github/forks/2210223t-lang/2-linked-list)](https://github.com/2210223t-lang/2-linked-list/network/members)
[![GitHub stars](https://img.shields.io/github/stars/2210223t-lang/2-linked-list)](https://github.com/2210223t-lang/2-linked-list/stargazers)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/2210223t-lang/2-linked-list)](https://github.com/2210223t-lang/2-linked-list/pulls)
[![GitHub last commit](https://img.shields.io/github/last-commit/2210223t-lang/2-linked-list)](https://github.com/2210223t-lang/2-linked-list/commits/main)


![Example of program](https://github.com/2210223t-lang/utility/raw/main/list.png)

# Overview
The repository contain 2-linked list realisation, equiped by a powerful debugging html - output system.

## Main functions
```
1. list_pushb <--- Pushes into the end of the sequence
2. list_init  <--- Initializes list struct
3. list_destr <--- Destroys list struct
4. list_add   <--- Adds after chosen element
```

## Debugging meaning
```
 RED - symbolizes error

 BLUE - head or tail

 GREEN - list active elem

 PURPLE - free elem
```
### Repo structure

```
.
|
├── example.cpp <-- Example of programm
|
|
├── list <--- Contains list functions
|   |
|   ├── list_config <--- List settings
|   |
|   ├── list.h <--- Main list func
|   └── list.cpp
|
|
├── debug <--- Debugging functions
|   |
|   ├── colours.h <--- Colours-putput defines
|   |
|   ├── debug.h <--- Verifier
|   ├── debug.cpp
|   |
|   ├── dump.h <--- Dumping functions
|   ├── dump.cpp
|   |
|   ├── log.h <--- Logger-output func
|   └── log.cpp
|
|
└── list_log <--- Images and graphs src
    |
    ├── img <--- Dot-src
    |
    └── svg <--- Images
```

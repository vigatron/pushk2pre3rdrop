#pragma once

#include <stdint.h>
#include <stdio.h>

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#define u8 uint8_t
#define u16 uint16_t
#define verr uint8_t
#define vok 0
#define verror(x) (x)

#define PACKED_STRUCT __attribute__((packed))

verr verrmsg(int x, std::string strerr);

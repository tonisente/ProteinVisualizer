#pragma once

#include <vector>

struct Atom;
struct Helix;
struct Sheet;
struct ProteinData;
using Chain = std::vector<Atom>;
using Model = std::vector<Chain>;

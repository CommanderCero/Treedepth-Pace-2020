#pragma once

#include "BaseTypes.h"
#include "Tree.h"

UndirectedGraph read_graph(std::istream& stream);
void write_file(std::string input_path, const Tree& solution);
void write_stdout(const Tree& solution);
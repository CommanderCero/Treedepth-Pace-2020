#pragma once

#include "BaseTypes.h"

struct Tree
{
    int Depth;
    std::vector<int> ParentLookup;

    Tree(size_t size)
        : ParentLookup(size), Depth(0)
    {
    }

    Tree()
    : ParentLookup(0), Depth(-1)
    {
    }
};
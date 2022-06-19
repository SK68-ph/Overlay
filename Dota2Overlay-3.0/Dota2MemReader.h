#pragma once
#include "include.h"
#include "proc.h"
#include "patternscan.h"

std::vector<unsigned int> getOffsetFromText();
void initHack();
int getVbe();
void close();

enum Status {
	Visible,
	Waiting
};
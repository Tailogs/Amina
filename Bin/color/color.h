// color.h

#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

// Function to set the text color
void setColor(const char* color);

// Function to set the background color
void setBackground(const char* color);

// Function to reset the color to default
void resetColor();

void enableAnsiSupport();

void printError1(const char *text);

void printError2(const char *text1, const char *text2);

#endif
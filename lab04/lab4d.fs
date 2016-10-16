/*
 *  Simple fragment sharder for Lab 2
 */

#version 430 core

in vec4 ocolour;
out vec4 fragcolor;

void main() {
	fragcolor = ocolour;
}

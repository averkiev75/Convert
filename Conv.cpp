// ConsoleApplication1.cpp : Defines the entry point for the console application. Version 2 1/9/2017
//

#include "stdafx.h"
#include <iostream>  
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
float temp;
fstream infile;
fstream outfile;
string line, coordinates;
cmatch match;
float lambda, cella, cellb, cellc, cellA, cellB, cellC, al, bt, gm, Al, Bt, Gm;
float x, y, z, X, Y, Z, det;
float matr[3][3];
float invt[3][3];
float scalar(int m, int n);

int main(int argc, char **argv)
{
	if (argc == 1) {
		cout << "Conv program (version 2)\n";
		cout << "Usage: Conv Filename\n";
		system("pause");
		return 1;
	}
	infile.open(argv[1]);
	if (infile.is_open()) {
		outfile.open("convert.res", fstream::out);
		cout << "Input matr\n";
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cin >> matr[i][j];
		det = matr[0][0] * matr[1][1] * matr[2][2] + matr[0][1] * matr[1][2] * matr[2][0] + matr[0][2] * matr[1][0] * matr[2][1] -
			matr[0][0] * matr[1][2] * matr[2][1] - matr[0][1] * matr[1][0] * matr[2][2] - matr[0][2] * matr[1][1] * matr[2][0];
		cout << "Determinant is " << det << endl;
		invt[0][0] = (matr[1][1] * matr[2][2] - matr[1][2] * matr[2][1]) / det;
		invt[0][1] = -(matr[1][0] * matr[2][2] - matr[1][2] * matr[2][0]) / det;
		invt[0][2] = (matr[1][0] * matr[2][1] - matr[1][1] * matr[2][0]) / det;
		invt[1][0] = -(matr[0][1] * matr[2][2] - matr[0][2] * matr[2][1]) / det;
		invt[1][1] = (matr[0][0] * matr[2][2] - matr[0][2] * matr[2][0]) / det;
		invt[1][2] = -(matr[0][0] * matr[2][1] - matr[0][1] * matr[2][0]) / det;
		invt[2][0] = (matr[0][1] * matr[1][2] - matr[0][2] * matr[1][1]) / det;
		invt[2][1] = -(matr[0][0] * matr[1][2] - matr[0][2] * matr[1][0]) / det;
		invt[2][2] = (matr[0][0] * matr[1][1] - matr[0][1] * matr[1][0]) / det;
		while (getline(infile, line))
		{
			char* next;
			if (int(line.find("CELL")) > -1) {
				lambda = strtof(line.c_str() + 4, &next);
				cella = strtof(next, &next);
				cellb = strtof(next, &next);
				cellc = strtof(next, &next);
				al = strtof(next, &next) * M_PI / 180.0;
				bt = strtof(next, &next) * M_PI / 180.0;
				gm = strtof(next, NULL) * M_PI / 180.0;

				cellA = sqrt(scalar(0, 0));
				cellB = sqrt(scalar(1, 1));
				cellC = sqrt(scalar(2, 2));
				Al = acos(scalar(1, 2) / (cellB*cellC))*180.0 / M_PI;
				Bt = acos(scalar(2, 0) / (cellC*cellA))*180.0 / M_PI;
				Gm = acos(scalar(0, 1) / (cellA*cellB))*180.0 / M_PI;

				outfile << "CELL " << fixed << lambda << "  " << setprecision(4) << cellA << "  " << cellB << "  " << cellC << "  " << setprecision(3) << Al << "  " << Bt << "  " << Gm << "  " << endl;
			}
			else if (regex_search(line.c_str(), match, regex("^([[:print:]]+[[:s:]]+[[:d:]]+)(([[:s:]]+-?[[:d:]]*\\.[[:d:]]*){3})(.*)")))
			{
				coordinates = match[2];
				x = strtof(coordinates.c_str(), &next);
				y = strtof(next, &next);
				z = strtof(next, NULL);
				X = invt[0][0] * x + invt[0][1] * y + invt[0][2] * z;
				Y = invt[1][0] * x + invt[1][1] * y + invt[1][2] * z;
				Z = invt[2][0] * x + invt[2][1] * y + invt[2][2] * z;
				outfile << match[1] << fixed << setprecision(5) << setw(11) << X << setw(11) << Y << setw(11) << Z << "  " << match[4] << endl;
			}
			else
				outfile << line << endl;
		}
		outfile.close();
		infile.close();
	}
	else
		cout << "Cannot open file: " << argv[1] << endl;
	system("pause");

	return 0;
}
float scalar(int m, int n)
{
	float cell = 0;
	cell = cella*cella*matr[m][0]*matr[n][0] + cellb*cellb*matr[m][1]*matr[n][1] + cellc*cellc*matr[m][2]*matr[n][2] +
    cella*cellb*matr[m][0]*matr[n][1]*cos(gm) + cellb*cellc*matr[m][1]*matr[n][2]*cos(al) + cellc*cella*matr[m][2]*matr[n][0]*cos(bt) +
	cella*cellb*matr[n][0]*matr[m][1]*cos(gm) + cellb*cellc*matr[n][1]*matr[m][2]*cos(al) + cellc*cella*matr[n][2]*matr[m][0]*cos(bt);
	return cell;
}
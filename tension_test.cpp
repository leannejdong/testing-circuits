#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "tension.h"
#include "makesquarematrix.h"

using std::vector;

static void
readMatrixFrom(std::istream &stream, vector<vector<float>> &matrix, int r)
{
    for (int i=0; i!=r; ++i) {
        for (int j=0; j!=r; ++j) {
            stream >> matrix[i][j];
        }
    }
}


static const char *tension_input_text =
        "r:\n"
        "11\n"
        "res:\n"
        " 0 0 0 0 0 0 0 100 0 0 0\n"
        " 0 0 10 0 0 0 0 0 0 0 0\n"
        " 0 10 0 20 0 0 0 200 0 0 0\n"
        " 0 0 20 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 100 0 0\n"
        " 0 0 0 0 0 0 20 0 0 0 0\n"
        " 0 0 0 0 0 20 0 0 0 0 0\n"
        " 100 0 0 0 0 0 0 0 20 0 0\n"
        " 0 0 0 0 100 0 0 0 0 10 0\n"
        " 0 0 0 0 0 0 0 0 10 0 200\n"
        " 0 0 0 0 0 0 0 0 0 200 0\n"
        "current:\n"
        " 0 56454216 0 0 0 0 0 0 0 0 0\n"
        " 0 0 56454216 0 0 0 0 0 0 0 0\n"
        " 0 0 0 55462356 0 0 0 991860 0 0 0\n"
        " 0 0 0 0 56262076 0 0 0 0 0 0\n"
        " 0 0 0 0 0 56262076 0 0 0 0 0\n"
        " 0 0 0 0 0 0 56262076 0 0 0 0\n"
        " 56106288 0 0 0 0 0 0 0 0 0 155789.44\n"
        " 347929.91 0 0 799719.56 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 155789.44 0 0 0\n"
        " 0 0 0 0 0 0 0 0 155789.44 0 0\n"
        " 0 0 0 0 0 0 0 0 0 155789.44 0\n"

        "volt:\n"
        " 0 0 0 0 0 0 -10 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 10 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n"
        " 0 0 0 0 0 0 0 0 0 0 0\n";


//static vector<float*> makePointers(vector<vector<float>> &matrix)
//{
//    int r2 = matrix.size();
//    vector<float *> res(r2);
//
//    for (int i=0; i!=r2; ++i) {
//        res[i] = matrix[i].data();
//    }
//
//    return res;
//}


static vector<vector<float>> makeMatrix(int r2)
{
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}


static void
saveTensionTo(
        std::ostream &fs,
        int r,
        const vector<vector<float>> &tensi,
        const vector<vector<float>> &tensf,
        const vector<vector<float>> &fall
)
{
    fs.setf(std::ios::fixed);
    int i,j;

    /*Scrivo su file*/
    fs << "The values ​​obtained for the voltages respect the physical laws\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << tensi[i][j] << "\t";
        }
        fs << tensi[i][r-1] << "\n";
    }
    fs << "The values ​​of the voltages at the end of the branch are saved in the final voltage matrix.\n";

    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << tensf[i][j] << "\t";
        }
        fs << tensf[i][r-1] << "\n";
    }
    fs << "The potential drops on each resistance are saved in the potential drop matrix..\n";
    for(i=0; i<=r-1; i++){
        for(j=0; j<r-1; j++) {
            fs << fall[i][j] << "\t";
        }
        fs << fall[i][r-1] << "\n";
    }

    if (!fs)
        std::cout << "\nC'è stato un errore nella scrittura del file 'tension.dat'!\n";
    else
        printf("\nThe tension.dat file was written correctly!\n");
}


static void tension(int r, vector<vector<float>> &res, vector<vector<float>> &current, vector<vector<float>> &volt, std::ostream &fs)
{
    auto fall = makeSquareMatrix(r);
    auto tensi = makeSquareMatrix(r);
    auto tensf = makeSquareMatrix(r);
    tension2(r, res, current, volt, fall, tensi, tensf);
    saveTensionTo(fs, r, tensi, tensf, fall);
}


static const char *expected_output_text =
        "The values ​​obtained for the voltages respect the physical laws\n"
        "0.000000	10.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	10.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	-564542144.000000	0.000000	0.000000	0.000000	233164992.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000\n"
        "-2799030784.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-2799030784.000000\n"
        "34793000.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1672231296.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1641073408.000000	0.000000\n"
        "The values ​​of the voltages at the end of the branch are saved in the final voltage matrix.\n"
        "0.000000	10.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	-564542144.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	34793000.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-2799030784.000000	0.000000	0.000000	0.000000	0.000000\n"
        "10.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-2799030784.000000\n"
        "10.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1673789184.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	-1672231296.000000	0.000000\n"
        "The potential drops on each resistance are saved in the potential drop matrix..\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	564542144.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	1109247104.000000	0.000000	0.000000	0.000000	198372000.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	1125241472.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "34792992.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	1557894.375000	0.000000	0.000000\n"
        "0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	31157888.000000	0.000000\n"
;

int main()
{
    //assert(false);
    std::istringstream fs(tension_input_text);
    std::string line;
    std::getline(fs, line);
    assert(line == "r:");
    int r2 = 0;
    fs >> r2;
    std::getline(fs, line);
    std::getline(fs, line);
    assert(line == "res:");
    auto res2 = makeMatrix(r2);
    readMatrixFrom(fs, res2, r2);
    std::getline(fs, line);
    std::getline(fs, line);
    assert(line == "current:");
    auto current2 = makeMatrix(r2);
    readMatrixFrom(fs, current2, r2);
    std::getline(fs, line);
    std::getline(fs, line);
    assert(line ==  "volt:");
    auto volt2 = makeMatrix(r2);
    readMatrixFrom(fs, volt2, r2);
    std::getline(fs, line);
//    vector<float *> res = makePointers(res2);
//    vector<float *> current = makePointers(current2);
//    vector<float *> volt = makePointers(volt2);
    std::ostringstream output_stream;
    tension(r2, res2, current2, volt2, output_stream);
    std::string output = output_stream.str();
    std::cerr << output << "\n";
    assert(output == expected_output_text);
}

/* The purpose of the test is to check results havn't changed
 * If we change the input data the results will change as well
 * The test here is to make sure tension2 is correct
 * the point of having the test is so that if you make a mistake in changing tension2 it can be caught quickly
 * Make sure to test the same version of tension2 that you are using for your main project */
// #include <iostream>
// #include <fstream>
// #include <bits/stdc++.h>
// #include <sstream>
// #include <cmath>
// #include <iomanip>
// using namespace std;

// vector<float>dist;

// /*void fn(float x1,float y1,float z1,float x2,float y2,float z2)
// {
//     float x = (x1 - x2)*(x1 - x2);
//     float y = (y1 - y2)*(y1 - y2);
//     float z = (z1 - z2)*(z1 - z2);

//     float a = sqrtf(x + y + z);
//     cout << a<<endl;
//     dist.push_back(a);

// }*/
// int main() {
//   string myText;

// // Read from the text file
// ifstream MyReadFile("5fr9.txt");
// vector<string>ans;
// // Use a while loop together with the getline() function to read the file line by line
// while (getline (MyReadFile, myText)) {
//   // Output the text from the file
//   string que = myText;
//   ans.push_back(que);
// }
// int count = 0;
// vector<string>final;
// for(auto x:ans)
// {
//     if(x[13] == 'N' && x[12] ==' ' && x[14] == ' ')
//     {final.push_back(x);}
//    // cout<<x<<endl;
// }
// vector<vector<string>>vc;
// for(auto x:final)
// {
//     stringstream p(x);
//     vector<string>abc;
//     string word;
//     while(p >> word)
//     {

//         abc.push_back(word);
//     }
//     vc.push_back(abc);
//     //cout<<p<<endl;

// }
// vector<vector<string>>nuc;
// for(auto it:vc)
// {
//     vector<string>xyz;
//     xyz.push_back(it[2]);
//     xyz.push_back(it[3]);
//      xyz.push_back(it[5]);
//     xyz.push_back(it[6]);
//     xyz.push_back(it[7]);
//     xyz.push_back(it[8]);
//     nuc.push_back(xyz);
//     for(auto m:xyz)
//     {
//         //cout << m << " ";
//     }
//     //cout << endl;
// }
// float x1,y1,z1,x2,y2,z2;

// for(int i =0;i<nuc.size();i++)
// {
//     x1 = stof(nuc[i][3]);
//     y1 = stof(nuc[i][4]);
//     z1 = stof(nuc[i][5]);
//     for(int j =i+1;j<nuc.size();j++)
//     {
//     x2 = stof(nuc[j][3]);
//     y2 = stof(nuc[j][4]);
//     z2 = stof(nuc[j][5]);
//     //fn(x1,y1,z1,x2,y2,z2);
// float x = 1LL*(x1 - x2)*(x1 - x2);
//     float y = 1LL*(y1 - y2)*(y1 - y2);
//     float z = 1LL*(z1 - z2)*(z1 - z2);

//     float def =x + y + z;

//     float a = sqrtf(def);
//     cout << a <<"  first id and base-"<<nuc[i][1]<<" "<<nuc[i][2]<<"  second id and base- "<<" "<<nuc[j][1]<<" "<<nuc[j][2] <<endl;
//     dist.push_back(a);
//     }


// }

// //cout << count;

// // Close the file
// MyReadFile.close();
// return 0;
// }
//Residue-ligand distances:
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Python.h"
#include "numpy/arrayobject.h"
#include "pybiopython.h"
#include "pyneighborsearch.h"
#include "pyatom.h"
#include "pyresidue.h"
#include "pymolecule.h"

int main() {
    // Initialize Python
    Py_Initialize();
    import_array();

    // Load the Biopython modules
    PyObject* PyBio = PyImport_ImportModule("Bio");
    PyObject* PyPDB = PyImport_ImportModule("Bio.PDB");
    PyObject* PyParser = PyObject_GetAttrString(PyPDB, "PDBParser");
    PyObject* PyNeighborSearch = PyObject_GetAttrString(PyPDB, "NeighborSearch");
    PyObject* PyLevel = PyUnicode_FromString("R");

    // Create a PDB parser object and parse the structure
    PyObject* pParser = PyObject_CallObject(PyParser, NULL);
    PyObject* pStructure = PyObject_CallMethod(pParser, "get_structure", "ss", "PDB", "5fr9.pdb");

    // Create a neighbor search object and set the cutoff distance
    PyObject* pAtoms = PyObject_CallMethod(pStructure, "get_atoms", NULL);
    PyObject* pNS = PyObject_CallFunctionObjArgs(PyNeighborSearch, pAtoms, NULL);
    double cutoff_distance = 5.0;

    // Loop through all the residues in the structure and find their neighbors within the cutoff distance
    PyObject* pResidues = PyObject_CallMethod(pStructure, "get_residues", NULL);
    Py_ssize_t n_residues = PySequence_Size(pResidues);
    for (Py_ssize_t i = 0; i < n_residues; i++) {
        PyObject* pResidue1 = PySequence_GetItem(pResidues, i);
        PyObject* pUnpacked1 = PyObject_CallMethod(pResidue1, "get_unpacked_list", NULL);
        PyObject* pNeighbors = PyObject_CallMethod(pNS, "search", "OdO", pUnpacked1, cutoff_distance, PyLevel);
        Py_ssize_t n_neighbors = PySequence_Size(pNeighbors);
        for (Py_ssize_t j = 0; j < n_neighbors; j++) {
            PyObject* pResidue2 = PySequence_GetItem(pNeighbors, j);
            if (pResidue1 != pResidue2) {
                PyObject* pCA1 = PyObject_GetAttrString(pResidue1, "CA");
                PyObject* pCA2 = PyObject_GetAttrString(pResidue2, "CA");
                PyObject* pDistance = PyObject_CallMethod(pCA1, "__sub__", "O", pCA2);
                double distance = PyFloat_AsDouble(pDistance);
                if (distance <= cutoff_distance) {
                    PyObject* pFullID1 = PyObject_CallMethod(pResidue1, "get_full_id", NULL);
                    PyObject* pFullID2 = PyObject_CallMethod(pResidue2, "get_full_id", NULL);
                    std::cout << "Residue " << PyUnicode_AsUTF8(PyObject_Repr(pFullID1)) << "  " << PyUnicode_AsUTF8(PyObject_Repr(pFullID2));
                }
            }
        }
    }

}


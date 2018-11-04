/*
Copyright 2012-2018 Tamas Bolner

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <cinttypes>
#include <iostream>
#include "Eigen/Dense"
#include "SimplexSolver.h"
#include "exception.h"
#include <cmath>
#include <vector>
#include <fstream>
#include "csv.h"
#include <map>
#include <unordered_set>
#include <tuple>

using namespace std;
typedef vector< tuple<string, double> > tuple_list;
using namespace Eigen;

int main()
{
    //dummy hashmap
    
    string line;
    ifstream inputs;
    
    inputs.open("test.txt");
    
    cout<<line<<endl;
    map<string, string> gender;
    map<string, string> age;
    map<string, string> weight;
    map<string, string> height;
    
    string b;
    int i =0;
    while(!inputs.eof())
    {
        getline(inputs, line);
        for (int i=0; i<line.length(); i++)
        {
            
            
        }


    }
    
    //cout<<gender<<endl;
    ofstream outputs;
    tuple_list Nut_min;
    tuple<string,double> inside ("Sodium, Na",40);
    Nut_min.push_back(inside);
    tuple<string,double> inside2 ("Carbohydrate, by difference",20);
    Nut_min.push_back(inside2);
    tuple<string,double> inside3 ("Vitamin A, IU",80);
    Nut_min.push_back(inside3);
    tuple<string,double> inside4 ("Calcium, Ca",500);
    Nut_min.push_back(inside3);
    tuple<string,double> inside5 ("Vitamin C",200);
    Nut_min.push_back(inside3);
    tuple<string,double> inside6 ("Ash",110);
    Nut_min.push_back(inside3);
    tuple<string,double> inside7 ("Cholesterol",120);
    Nut_min.push_back(inside3);
    vector<double> o;
    
    io::CSVReader<3, io::trim_chars<' '>, io::no_quote_escape<'\"'>> in("Nutrients.csv");
    in.read_header(io::ignore_extra_column, "NDB_No", "Nutrient_name", "Output_value");
    
    int NDB_No;
    std::string Nutrient_name;
    double Output_value;
    
    std::map<int, tuple_list> NDB_Nutrient_Amount;
    std::map<std::string, std::unordered_set<int>> nutrient_NDBlist;
    std::map<int,int> NDBtoi;
    
    while(in.read_row(NDB_No, Nutrient_name, Output_value)){
        //populate NDB_Nutrient_Amount
        tuple<string,double> inside (Nutrient_name,Output_value);
        if(NDB_Nutrient_Amount[NDB_No].empty()){
            tuple_list v;
            NDB_Nutrient_Amount[NDB_No] =  v;
        }
        NDB_Nutrient_Amount[NDB_No].push_back(inside);
        //populate nutrient_NDBlist
        if(nutrient_NDBlist[Nutrient_name].empty()){
            unordered_set<int> a;
            nutrient_NDBlist[Nutrient_name] = a;
        }
        nutrient_NDBlist[Nutrient_name].insert(NDB_No);
    }
    
    io::CSVReader<2, io::trim_chars<' '>, io::no_quote_escape<','>> in2("Products.csv");
    in2.read_header(io::ignore_extra_column, "NDB_Number", "long_name");
    
    //int NDB_No;
    std::string Product_name;
    
    std::map<int, string> NDB_Name;
    
    in2.next_line();
    //int e = 6;
    try {
            while(in2.read_row(NDB_No, Product_name)){
                //tuple<string, double> inside (Product_name, long_name);
                NDB_Name[NDB_No] = Product_name;
        }
    }
    catch (int e)
        {
            in2.next_line();
        }
    
        
    
    
    
    
    int unique = 1;
    for (int counter=0;counter<7;counter++) {
        std::string nutr = get<0>(Nut_min[counter]);
        unordered_set<int> a = nutrient_NDBlist[nutr];
        for (auto elem : a) {
            if (NDBtoi.empty() or NDBtoi[elem]==0) {
                NDBtoi[elem] = unique;
                unique += 1;
            }
        }
    }
    
    
    //    cout<<"NDB print: "<<NDBtoi.size()<<endl;
    int rowSizeA=NDBtoi.size();  //should initialise columns size in A
//    cout<<"The value of rowSizeA: "<<rowSizeA<<endl;
    int colSizeA = 7;  //should initialise columns row in A[][] vector
    
    
    double C[NDBtoi.size()];  //should initialis the c arry here
    for (int counter=0;counter<NDBtoi.size();counter++) {
        C[counter] = 1;
    }
    

   double a[colSizeA][rowSizeA+1];

    for (int counter=0;counter<NDBtoi.size();counter++) {

        std::unordered_set<int> q = nutrient_NDBlist[get<0>(Nut_min[counter])];
        for (int id : q) {

            tuple_list m = NDB_Nutrient_Amount[id];
       
            for (tuple<string, double> u : m) {

                if (get<0>(u)== get<0>(Nut_min[counter])) {

                    a[counter][NDBtoi[id]] = get<1>(u);
                    break;
                    }

                }
            }

        a[counter][NDBtoi.size()+1] = get<1>(Nut_min[counter]);

    }

    try {
        
        /*
            Minimization problem
        */
        Eigen::Map <MatrixXd> constraints1((double*) a, colSizeA, NDBtoi.size()+1);

        Eigen::Map <VectorXd> objectiveFunction((double*) C, NDBtoi.size());
        
        SimplexSolver solver2(SIMPLEX_MINIMIZE, objectiveFunction, constraints1);

        
        if (solver2.hasSolution()) {
            double *vc = solver2.getSolution().data();

            
            cout<<vc[12]<<endl;
            cout<<vc[13]<<endl;
            cout<<vc[14]<<endl;
            cout<<vc[15]<<endl;
            cout<<vc[16]<<endl;
            cout<<vc[17]<<endl;
            cout<<vc[18]<<endl;
            cout<<vc[19]<<endl;
            
            
            for (int i = 0; i < solver2.getSolution().array().rows(); i++) {
                cout << "i is: " << i << endl;
                o.push_back(i);
            }
            outputs.open ("outputs.txt");
            for(std::map<int,int>::iterator iter = NDBtoi.begin(); iter != NDBtoi.end(); ++iter)
            {
                int n =  iter->first;
                if (NDBtoi[n] <= o.size() && o[NDBtoi[n]] != 0) {
                    string name = NDB_Name[n];
                    double amount = o[NDBtoi[n]];
                    outputs << " '"<< name<<"'    '"<<amount<<"'";
                }
            }
            outputs.close();
        
        } else {
        }
    } catch (const FException &ex) {
        ex.Print();
        return 1;
    }
    
    return 0;
}


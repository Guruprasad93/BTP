#include <iostream>
#include <vector>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <fstream>
#include <sstream>

using namespace std;
using namespace boost::algorithm;

const int l_matrix = 5;

void read_ConcMatrix(int conc_grad[10][10])
{
    string line;
    ifstream infile;
    infile.open("thefile.txt");
    vector<string> tokens;
    int row = 0;
    
    if (infile.is_open())
    {
        while(getline(infile, line))
        {
            split(tokens, line, is_any_of(","));
            
            for (int i=0; i<l_matrix; i++)
            {
                stringstream(tokens[i]) >> conc_grad[row][i];
            }
            row++;
        }
    }
    infile.close();
    
}            



int main()
{
    
    int conc_grad[10][10];
    
    read_ConcMatrix(conc_grad);
    //Display Conc_gradient
    for (int row = 0; row<2; row++)
    {
        for (int col = 0; col<5; col++)
        {
            cout<<conc_grad[row][col]<<" ";
        }
        cout<<endl;
    }    
}
    
    
   
      
        

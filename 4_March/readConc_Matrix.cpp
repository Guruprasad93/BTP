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

void read_ConcMatrix(double conc_grad[10][10], char filename[100])
{
    string line;
    ifstream infile;
    infile.open(filename);
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
    
    int time, time_of_sim=2;
    ifstream infile;
    char filename[100];
    
    for (time = 0; time<time_of_sim; time++)
    {
        
        cout<<endl<<"Time = "<<time<<endl;
        snprintf(filename, sizeof(char) * 32, "Conc_Matrix_%i.txt", time+1);
        //filename = sprintf("Conc_Matrix_%d",time+1);
        double conc_grad[10][10];
        
        //infile.open(filename);
        
        read_ConcMatrix(conc_grad, filename);
        
        //Display Conc_gradient
        for (int row = 0; row<5; row++)
        {
            for (int col = 0; col<5; col++)
            {
                cout<<conc_grad[row][col]<<" ";
            }
            cout<<endl;
        }        
        //infile.close();
    }
}
    
    
   
      
        

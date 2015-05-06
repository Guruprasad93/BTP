//2D Simulation Matrix

#include<iostream>
#include<stdio.h>
#include<omp.h>
#include<random>
#include<stdlib.h>
#include<map>
#include<math.h>
#include<fstream>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>


using namespace std;
using namespace Eigen;
using namespace boost::algorithm;

typedef SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Triplet<double> T;

//GLOBAL Variables
const int l_matrix = 50;
const int b_matrix = 50;
//const int h_matrix = 3;
const int time_interval = 12;
const int T_Div = ceil(1200/time_interval);
int Sim_Matrix[l_matrix][b_matrix];
int mig_index[l_matrix][b_matrix];
int persctr[l_matrix][b_matrix];
int divctr[l_matrix][b_matrix];
char gridsvisited[l_matrix][b_matrix];
int glc_ctr[l_matrix][b_matrix];
int starve_counter = ceil(180/time_interval); //3hrs without consumption of glucose
int collide_persctr = ceil(10000/time_interval);
int stationary_persctr = ceil(24/time_interval);
int collisioncellsNum;
int stationarycellsNum;
int cellCount[l_matrix*b_matrix];
//int index_cellCount = 0;
int collisionCount[100000];
int stationaryCount[100000];
int Cbulk = 5;
double r = 0.14;

//int index_


//MAPPING Mig_Index to Persistence counter
//map<int, int> m = {{1, 24}, {2, 24}, {3, 24}, {4, 24}, {5, 24}, {6, 24}, {7, 30}, {0, 5}};

int checkpersctr(int mig_dir)
{
    int persctr;
    switch(mig_dir)
    {
        case 0: persctr = ceil(24/time_interval);
                break;
        case 1: 
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:persctr = ceil(24/time_interval);
               break;
        case 7:persctr = ceil(10000/time_interval);
               break;
    }
    return persctr;
}


int random_time_division()
{
    int Division_time;
    double randc;
    randc = ((double) rand()/(RAND_MAX));
    
    boost::mt19937 igen(randc*1000);
    boost::variate_generator<boost::mt19937, boost::normal_distribution<> > gen(igen, boost::normal_distribution<>(20,1.5));
    
    Division_time = ceil(gen()*60/time_interval);
    //cout<<Division_time<<" ";
    //Division_time = Division_time*60;
    return Division_time;
}
    


//Populate Matrices
void uniform_populateMatrix(int x, int y)
{
    double randc;
    randc = ((double) rand()/(RAND_MAX));
    //int arbit = 1000;
    
      
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dis(0, 1);
    //double randc = dis(gen);
    //srand (time(NULL));
    int migration;
    if (randc>0.99)
    {
        
        migration = rand() % 4 + 1;
        Sim_Matrix[x][y] = 1;
        mig_index[x][y] = migration;
        persctr[x][y] = checkpersctr(migration);
        //divctr[x][y] = T_Div;
        divctr[x][y] = random_time_division();
        gridsvisited[x][y] = 'N';
        glc_ctr[x][y] = starve_counter;
    }
    else
    {
        Sim_Matrix[x][y] = 0;
        mig_index[x][y] = 0;
        persctr[x][y] = 0;
        divctr[x][y] = 0;
        gridsvisited[x][y] = 'N';
        glc_ctr[x][y] = 0;
    }
}


//Doesn't make sense in a 2D Matrix
/*void initseedontop()
{
    int i, j, k;
    int migration;
    
    
    
    
    for (i=h_matrix-1;i>h_matrix-2;i--)
    {
        for (j=0;j<b_matrix;j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                migration = rand()%6+1;
                Sim_Matrix[k][j][i] = 1;
                mig_index[k][j][i] = migration;
                persctr[k][j][i] = checkpersctr(migration);
                //divctr[k][j][i] = T_Div;
                divctr[k][j][i] = random_time_division();
        
            }
        }
    }
}*/
                

void wound_healing(int radius)
{
    int i,j,k;
    int migration;
    //int arbit = 10000;
    
    
    for (j=0; j<b_matrix;j++)
    {  
        for (k=0;k<l_matrix;k++)
        {
            if (((j-b_matrix/2)*(j-b_matrix/2) + (k-l_matrix/2)*(k-l_matrix/2) - radius*radius)>0)
            {
                migration = rand()%4+1;
                Sim_Matrix[k][j] = 1;
                mig_index[k][j] = migration;
                persctr[k][j] = checkpersctr(migration);
                //divctr[k][j] = T_Div;
                divctr[k][j] = random_time_division();
                glc_ctr[k][j] = starve_counter;
            }
        }
    }
  
                
    
    
    /*for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for(k=0; k<l_matrix; k++)
            {
                if (((j-b_matrix/2)*(j-b_matrix/2) + (k-l_matrix/2)*(k-l_matrix/2) - radius*radius)>0)
                {
                    migration = rand()%6+1;
                    Sim_Matrix[k][j][i] = 1;
                    mig_index[k][j][i] = migration;
                    persctr[k][j][i] = checkpersctr(migration);
                    //divctr[k][j][i] = T_Div;
                    divctr[k][j][i] = random_time_division();
        
                }
             }
         }
     }*/
}
                    
void Seed_Matrix()
{
    
    int i, j, k;
    double t1, t2, el_time;
    
    
    cout<<endl<<"Populating Scaffold";
    t1 = clock();   
    //#pragma omp parallel for collapse(3)
    
    for (j=0; j<b_matrix; j++)
    {
        for (k=0;k<l_matrix;k++)
        {
            uniform_populateMatrix(k,j);
        }
    }
    
    
    /*for (i = 0; i<h_matrix; i++)
    {
        for (j=0;j<b_matrix; j++)
        {
            for (k=0;k<l_matrix; k++)
            {
                 uniform_populateMatrix(k,j,i);
            }
        }
    }*/
    t2 = clock();
    el_time = (t2-t1)/CLOCKS_PER_SEC;
    cout<<"Time Elapsed_series"<<el_time;
}
  

void view_Matrix()
{
    int i, j, k;
    //Viewing Contents of the Matrix
    cout<<endl<<"Contents of the Matrix are"<<endl;
    
    for (j=0; j<b_matrix; j++)
    {
        for (k=0;k<l_matrix;k++)
        {
            cout<<"SIM:"<<Sim_Matrix[k][j]<<" "<<"MIG_index:"<<mig_index[k][j]<<" "<<"Pers_ctr:"<<persctr[k][j]<<" ";
            cout<<"DIV_ctr:"<<divctr[k][j]<<"Position:"<<k<<" "<<j;
            cout<<endl;
        }
    }
 
    
    /*for (i = 0; i<h_matrix; i++)
    {
        for (j=0;j<b_matrix; j++)
        {
            for (k=0;k<l_matrix; k++)
            {
                cout<<"SIM:"<<Sim_Matrix[k][j][i]<<" "<<"MIG_index:"<<mig_index[k][j][i]<<" "<<"Pers_ctr:"<<persctr[k][j][i]<<" ";
                cout<<"DIV_ctr:"<<divctr[k][j][i]<<"Position:"<<k<<" "<<j<<" "<<i;
                cout<<endl;
            }
        }
    }*/
}
    
void change_grid_status()
{
    int i, j, k;
    
    for (j=0; j<b_matrix; j++)
    {
        for (k=0;k<l_matrix; k++)
        {
            gridsvisited[k][j] = 'N';
        }
    }       
    
    
    /*for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                gridsvisited[k][j][i] = 'N';
            }
        }
    }*/
}

int checkcell(int k, int j)
{
    if (Sim_Matrix[k][j] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkdivctr(int k, int j)
{
    if (divctr[k][j] == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int cell_neighbours(int k, int j, int* xneighbour, int* yneighbour)
{
    //int xneighbour[6], yneighbour[6], zneighbour[6];
    int index = 0;
    //Check if k-1 && k+1 is within matrix
    if ((k-1)>=0 && (k-1)<l_matrix)
    {
        xneighbour[index] = k-1;
        yneighbour[index] = j;
        //zneighbour[index] = i;
        index ++;
    }
    if ((k+1)>=0 && (k+1)<l_matrix) 
    {
        xneighbour[index] = k+1;
        yneighbour[index] = j;
        //zneighbour[index] = i;
        index ++;
    }
    if ((j-1)>=0 && (j-1)<b_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j-1;
        //zneighbour[index] = i;
        index ++;
    }
    if ((j+1)>=0 && (j+1)<b_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j+1;
        //zneighbour[index] = i;
        index ++;
    }
    /*if ((i-1)>=0 && (i-1)<h_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j;
        zneighbour[index] = i-1;
        index ++;
    }
    if ((i+1)>=0 && (i+1)<h_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j;
        zneighbour[index] = i+1;
        index ++;
    }*/
    return (index);

}
    
int cell_neighbours_empty(int* xneighbours, int* yneighbours, int num_neighbours, int* empty_x, int* empty_y)
{
    int i, timediv;
    int mt_i = 0;
    for (i = 0; i<num_neighbours; i++)
    {
        if (Sim_Matrix[xneighbours[i]][yneighbours[i]] == 0)
        {
            empty_x[mt_i]= xneighbours[i];
            empty_y[mt_i]= yneighbours[i];
            //empty_z[mt_i]= zneighbours[i];
            mt_i ++;
        }
    }
    return (mt_i);
}
            
void gridcell_stationaryphase(int k, int j)
{
     int timediv;
     timediv = divctr[k][j];
     Sim_Matrix[k][j] = 1;
     mig_index[k][j] = 0;
     persctr[k][j] = stationary_persctr;
     stationarycellsNum ++;
}


void dividecells(int k, int j)
{
    
    int xneighbours[6], yneighbours[6];
    int empty_x[6], empty_y[6];
    int num_neighbours, empty_num_neighbours;
    int mig_old, randomposition, mig_new;
    
    num_neighbours = cell_neighbours(k, j, xneighbours, yneighbours);   
    empty_num_neighbours = cell_neighbours_empty(xneighbours, yneighbours, num_neighbours, empty_x, empty_y);
    
    //cout<<"Number of empty neighbours"<<empty_num_neighbours;
    
    //Check if there are no empty grids
    if (empty_num_neighbours == 0)
    {
        gridcell_stationaryphase(k, j);
    }
    else
    {
        randomposition = rand()%empty_num_neighbours;
        
        mig_old = rand()%4 + 1;
        Sim_Matrix[k][j] = 1;
        mig_index[k][j] = mig_old;
        persctr[k][j] = checkpersctr(mig_old);
        divctr[k][j] = T_Div;
        glc_ctr[k][j] = starve_counter;
        
        
        mig_new = rand()%4 + 1;
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]] = 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]]= mig_new;
        persctr[empty_x[randomposition]][empty_y[randomposition]] = checkpersctr(mig_new);
        divctr[empty_x[randomposition]][empty_y[randomposition]]= T_Div;        
        gridsvisited[empty_x[randomposition]][empty_y[randomposition]] = 'Y';
        glc_ctr[empty_x[randomposition]][empty_y[randomposition]] = starve_counter;
        
    }
}


int checkpersistencectr(int k, int j)
{
	if (persctr[k][j] == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


int checkmigdir(int k, int j, int x_newpos, int y_newpos)
{
	int migdir = -1;
	if (j+1 == y_newpos)
	{
	    migdir = 1;
	}
	else if (k+1 == x_newpos)
	{
	    migdir = 2;
	}
	else if (j-1 == y_newpos)
	{
	    migdir = 3;
	}
	else if (k-1 == x_newpos)
	{
	    migdir = 4;
	}
	/*else if (i-1 == z_newpos)
	{
	    migdir = 5;
	}
	else if (i+1 == z_newpos)
	{
	    migdir = 6;
	}*/
	return migdir;
}
	     


void changedirofmigration(int k, int j)
{
	        
    int xneighbours[6], yneighbours[6];
    int empty_x[6], empty_y[6];
    int num_neighbours;
    int migdir, tdiv;
    int empty_num_neighbours, randomposition;
    int food_ctr;
    
    num_neighbours = cell_neighbours(k, j, xneighbours, yneighbours);   
    empty_num_neighbours = cell_neighbours_empty(xneighbours, yneighbours, num_neighbours, empty_x, empty_y);
    
    //cout<<"empty grids"<<empty_num_neighbours;
    
    //Check if there are no empty grids
    if (empty_num_neighbours == 0)
    {
        gridcell_stationaryphase(k, j);
    }
    else
    {
        randomposition = rand()%empty_num_neighbours;
        migdir = checkmigdir(k,j,empty_x[randomposition], empty_y[randomposition]);
        tdiv = divctr[k][j];
        food_ctr = glc_ctr[k][j];
        //NEW GRID CELL
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]]= 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]] = migdir;
        persctr[empty_x[randomposition]][empty_y[randomposition]] = checkpersctr(migdir);
        divctr[empty_x[randomposition]][empty_y[randomposition]] = tdiv;
        glc_ctr[empty_x[randomposition]][empty_y[randomposition]] = food_ctr;
        
        //OLD GRID CELL
        Sim_Matrix[k][j] = 0;
        mig_index[k][j]  = 0;
        persctr[k][j] = 0;
        divctr[k][j]  = 0;
        glc_ctr[k][j] = 0;
        
        //updating gridsvisited
        gridsvisited[empty_x[randomposition]][empty_y[randomposition]] = 'Y';
        
    }
}

void next_step(int k, int j, int mig_dir, int *newpos)
{
    //int *newpos;
        
    if (mig_dir == 1)
    {
        newpos[0] = k;
        newpos[1] = j+1;
        //newpos[2] = i;
    }
    else if (mig_dir == 2)
    {
        newpos[0] = k+1;
        newpos[1] = j;
        //newpos[2] = i;
    }
    else if (mig_dir == 3)
    {
        newpos[0] = k;
        newpos[1] = j-1;
        //newpos[2] = i;
    }    
    else if (mig_dir == 4)
    {
        newpos[0] = k-1;
        newpos[1] = j;
        //newpos[2] = i;
    }        
    /*else if (mig_dir == 5)
    {
        newpos[0] = k;
        newpos[1] = j;
        newpos[2] = i-1;
    }    
    else if (mig_dir == 6)
    {
        newpos[0] = k;
        newpos[1] = j;
        newpos[2] = i+1;
    }    
    else
    {
        newpos[0] = k;
        newpos[1] = j;
        newpos[2] = i;
    }*/        
    
    if ((newpos[0]>=0 && newpos[0]<l_matrix) && (newpos[1]>=0 && newpos[1]<b_matrix))
    {
        //cout<<"Valid"<<endl;
        //cout<<newpos[0]<<" "<<newpos[1]<<" "<<newpos[2]<<endl;
        //return &newpos;
        return;
    }
    else
    {
        //int *v;
        newpos[0] = -1;
        newpos[1] = -1;
        //newpos[2] = -1;
        //return v;
    }
}


void gridcell_collisionstate(int k, int j)
{
    int tdiv;
    tdiv = divctr[k][j];
    
    //cout<<"Inside grid-cell wa collisionstate desu"<<endl;
    //cout<<m[7];
    //Re-assign values
    Sim_Matrix[k][j] = 1;
    mig_index[k][j] = 7;
    persctr[k][j] = collide_persctr;
    collisioncellsNum ++;
    //divctr[k][j][i] = tdiv;
}

void cell_migrate(int k, int j)
{
    int mig_dir;
    int newpos[2];
    int ctr_pers, tdiv;
    int food_ctr;
    
    mig_dir = mig_index[k][j];
    
    //Checking if stationary/collision state
    if (mig_dir == 0 || mig_dir == 7)
    {
        return;
    }
    next_step(k,j,mig_dir,newpos);
    
    //Check if movement is valid
    
    if (newpos[0] == -1)
    {
        gridcell_collisionstate(k,j);
        return;
    }
    
    //Check if site is vacant
    //cout<<"Oldsite"<<k<<" "<<j<<" "<<i<<"New site is"<<newpos[0]<<" "<<newpos[1]<<" "<<newpos[2]<<endl;
    
    if (Sim_Matrix[newpos[0]][newpos[1]] == 0)
    {
        //cout<<"Site is empty";
        ctr_pers = persctr[k][j];
        tdiv = divctr[k][j];
        food_ctr = glc_ctr[k][j];
        
        Sim_Matrix[newpos[0]][newpos[1]] = 1;
        mig_index[newpos[0]][newpos[1]] = mig_dir;
        persctr[newpos[0]][newpos[1]] = ctr_pers;
        divctr[newpos[0]][newpos[1]] = tdiv;
        glc_ctr[newpos[0]][newpos[1]] = food_ctr;
        
        Sim_Matrix[k][j] = 0;
        mig_index[k][j] = 0;
        persctr[k][j] = 0;
        divctr[k][j] = 0;
        glc_ctr[k][j] = 0;
    }
    else
    {
        //Go to collision state
        //cout<<"Going to collision state!!!";
        gridcell_collisionstate(k,j);
        gridcell_collisionstate(newpos[0],newpos[1]);
        divctr[newpos[0]][newpos[1]] --;
    }
    
    gridsvisited[newpos[0]][newpos[1]] = 'Y';
}

void tissue_state(FILE *fid, int counterForCells)
{
    int i,j,k;
    const long int vectorsize = l_matrix*b_matrix;
    //int xcoord[l_matrix*b_matrix*h_matrix], ycoord[l_matrix*b_matrix*h_matrix], zcoord[l_matrix*b_matrix*h_matrix];
    short int xcoord[vectorsize];
    short int ycoord[vectorsize];
    //short int zcoord[vectorsize];
    
    long int index = 0;

    for (j=0; j<b_matrix; j++)
    {
        for (k=0; k<l_matrix; k++)
        {
            if (Sim_Matrix[k][j] == 1)
            {
                xcoord[index] = k;
                ycoord[index] = j;
                index ++;
            }
        }
    }
    cellCount[counterForCells] = index;
    
    /*for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                if (Sim_Matrix[k][j][i] == 1)
                {
                    //cout<<index<<k<<j<<i<<Sim_Matrix[k][j][i]<<" "<<divctr[k][j][i];
                    xcoord[index] = k;
                    ycoord[index] = j;
                    zcoord[index] = i;
                    index ++;
                    
                }
            }
        }
    }*/
    
    
    
    //Printing firstValue
    //fprintf(fid, "%d", xcoord[0]);
    
    //fprintf(fidy, "%d", ycoord[0]);
    //fprintf(fidz, "%d", zcoord[0]);
    
    //Print allvalues
    for (int ctr = 0; ctr<index; ctr++)
    {
        //fprintf(fidx, ",%d", xcoord[ctr]);
        //fprintf(fidy, ",%d", ycoord[ctr]);
        //fprintf(fidz, ",%d", zcoord[ctr]);
        
        fprintf(fid, "%d", xcoord[ctr]);
        fprintf(fid, " ");
        fprintf(fid, "%d", ycoord[ctr]);
        fprintf(fid, " ");
        //fprintf(fid, "%d", zcoord[ctr]);
        //fprintf(fid, " ");
        fprintf(fid,"\n");
        
        
    }

    
    //fprintf(fid, "\n");
    //fprintf(fidy, "\n");
    //fprintf(fidz, "\n");    


}


/*void tissue_state1(FILE *fidx, FILE *fidy)
{
    int j,k;
    const long int vectorsize = l_matrix*b_matrix;
    //int xcoord[l_matrix*b_matrix*h_matrix], ycoord[l_matrix*b_matrix*h_matrix], zcoord[l_matrix*b_matrix*h_matrix];
    short int xcoord[vectorsize];
    short int ycoord[vectorsize];
    //short int zcoord[vectorsize];
    
    long int index = 0;

    for (j=0; j<b_matrix; j++)
    {
        for (k=0; k<l_matrix; k++)
        {
            if (Sim_Matrix[k][j] == 1)
            {
                xcoord[index] = k;
                ycoord[index] = j;
                index ++;
            }
        }
    }*/



    
    /*for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                if (Sim_Matrix[k][j][i] == 1)
                {
                    //cout<<index<<k<<j<<i<<Sim_Matrix[k][j][i]<<" "<<divctr[k][j][i];
                    xcoord[index] = k;
                    ycoord[index] = j;
                    zcoord[index] = i;
                    index ++;
                    
                }
            }
        }
    }*/
    
    //Writing to a file

    /*FILE *fidx, *fidy, *fidz;
    fidx = fopen("/home/guru/Desktop/x_coordinates.txt", "a");
    fidy = fopen("/home/guru/Desktop/y_coordinates.txt", "a");
    fidz = fopen("/home/guru/Desktop/z_coordinates.txt", "a");
    */ 
    
    
    //Printing firstValue
    /*fprintf(fidx, "%d", xcoord[0]);
    fprintf(fidy, "%d", ycoord[0]);
    //fprintf(fidz, "%d", zcoord[0]);
    
    //Print allvalues
    for (int ctr = 1; ctr<index; ctr++)
    {
        fprintf(fidx, ",%d", xcoord[ctr]);
        fprintf(fidy, ",%d", ycoord[ctr]);
        //fprintf(fidz, ",%d", zcoord[ctr]);
    }
    
    fprintf(fidx, "\n");
    fprintf(fidy, "\n");
    fprintf(fidz, "\n");    
    
    //delete [] xcoord;
    //delete [] ycoord;
    //delete [] zcoord;
    
   
    
}*/
        


void initializeConcglc(double conc_profile[l_matrix][b_matrix])
{
    int i,j;
    for (i = 1; i<l_matrix-1; i++)
    {
        for (j = 1; j<b_matrix-1; j++)
        {
            conc_profile[i][j] = 0;
        }
    }
    for (i = 0; i<l_matrix; i++)
    {
        conc_profile[i][0] = Cbulk;
        conc_profile[0][i] = Cbulk;
        conc_profile[l_matrix-1][i] = Cbulk;
        conc_profile[i][l_matrix-1] = Cbulk;
    }
    
}

void find_neighbors(int valid_neighbors[10], int v_bdry[2], int x, int y, int l_matrix)
{
    int set_of_neighbors[5][2];
    //int v_bdry[2];
    
    //Initializing v_bdry ==> num_valid & num_bdry
    v_bdry[0] = 0;
    v_bdry[1] = 0;
    
    
    //Initializing neighbors
    set_of_neighbors[0][0] = x-1;
    set_of_neighbors[0][1] = y;
    set_of_neighbors[1][0] = x+1;
    set_of_neighbors[1][1] = y;
    set_of_neighbors[2][0] = x;
    set_of_neighbors[2][1] = y-1;
    set_of_neighbors[3][0] = x;
    set_of_neighbors[3][1] = y+1;

    for (int index = 0; index<4; index++)
    {   
        if ((set_of_neighbors[index][0] == 0 || set_of_neighbors[index][0] == l_matrix+1) || (set_of_neighbors[index][1] == 0 || set_of_neighbors[index][1] == l_matrix+1))
        {
            v_bdry[1]++;
        }
        else
        {
            valid_neighbors[v_bdry[0]] = (set_of_neighbors[index][1]-1)*l_matrix+set_of_neighbors[index][0]-1;
            v_bdry[0]++;
        }
    }
}
   
double neighbors_Conc_sum(int x, int y, double conc_profile[l_matrix][b_matrix])
{
                
    int set_of_neighbors[5][2];
    double conc_val;
   
    
    //Initializing neighbors
    set_of_neighbors[0][0] = x-1;
    set_of_neighbors[0][1] = y;
    set_of_neighbors[1][0] = x+1;
    set_of_neighbors[1][1] = y;
    set_of_neighbors[2][0] = x;
    set_of_neighbors[2][1] = y-1;
    set_of_neighbors[3][0] = x;
    set_of_neighbors[3][1] = y+1;   
    
    conc_val = 0;
    
    for (int index = 0; index<4; index++)
    {
        //cout<<conc_val;
        conc_val += conc_profile[set_of_neighbors[index][0]][set_of_neighbors[index][1]];
    }
    
    return conc_val;
}
    
int find_rowNumber(int x, int y, int interior_length)
{
    int row_num;
    //cout<<y<<x<<l_matrix;
    row_num = (y-1)*interior_length + x;
    //cout<<row_num;
    return row_num;
}


    
void crank_nicolson(double conc_profile[l_matrix][b_matrix], int time)
{
	double val;
	int i, j, row_num, index, k;
	int nz;
	const int n = (l_matrix-2)*(b_matrix-2);
	int x, y, ctr_bdry, v_bdry[2];
	double Conc_val;
	int valid_neighbors[10];
	char filen[100];
	double min_glc = 0.4125;
	
	vector<T> tripletList;
	
	for (j=0; j<l_matrix; j++)
	{
        for (k=0; k<l_matrix; k++)
        {
            if (Sim_Matrix[k][j] == 1)
            {
                if (conc_profile[k][j]>min_glc)
                {
                    conc_profile[k][j] = conc_profile[k][j]-min_glc;
                    glc_ctr[k][j] = starve_counter;
                }
                else
                {
                    conc_profile[k][j] = 0;
                    
                    if (glc_ctr[k][j] == 0)
                    {
                        Sim_Matrix[k][j] = 0;
                        mig_index[k][j] = 0;
                        persctr[k][j] = 0;
                        divctr[k][j] = 0;
                        glc_ctr[k][j] = 0;
                    }
                    else
                    {
                        glc_ctr[k][j]--;
                    }
                }
            }
        }
    }
	
	ofstream ofp;
    //snprintf(filen, sizeof filen, "glucose/Glucose_conc%d",time);
	sprintf(filen, "glucose/Glucose_conc%d", time);
	
	ofp.open(filen);	
	
	//Sparse matrix (1:No. of elements)
	//n = (l_matrix-2)*(b_matrix-2);
	
	VectorXd LHS_col, RHS_col, Intermediate;
	LHS_col.resize(n);
	RHS_col.resize(n);
	Intermediate.resize(n);
	
	//No. of non-zero elements
	nz = n*6;
	//cout<<nz;
	
	SpMat LHS_A;
	LHS_A.resize(n,n);
	//VectorXf RHS_col(n);
	//VectorXf LHS_col(n);
	
	//cout<<"\nIs this the problem? Rfdaffd";
	//cout<<"\n Space Reserved!";
	//Reserve space for non-zero elements in LHS_A
	//tripletList.reserve(nz);
	
	
	for (y = 1; y<l_matrix-1; y++)
	{
	    for (x = 1; x<l_matrix-1; x++)
	    {
	        find_neighbors(valid_neighbors, v_bdry, x, y, l_matrix-2);
	        Conc_val = neighbors_Conc_sum(x,y,conc_profile);
	        row_num = find_rowNumber(x, y, l_matrix-2);
	        tripletList.push_back(T(row_num-1,row_num-1,1+4*r));
	        for (index = 0; index<v_bdry[0]; index++)
	        {
	            tripletList.push_back(T(row_num-1, valid_neighbors[index], -r));
	        }
	        //LHS_col(row_num-1) = -r*v_bdry[1]*Cbulk;
	        //RHS_col(row_num-1) = conc_profile[x][y]*(1-4*r) + r*(Conc_val);
	        //cout<<endl<<"gridValue"<<x<<y<<"   "<<"Row number"<<row_num<<endl;
	        //cout<<endl<<LHS_col;
	    }
	}    
	//cout<<"Exiting Loop";
	LHS_A.setFromTriplets(tripletList.begin(), tripletList.end());
	
	//SparseQR<SparseMatrix<double>, COLAMDOrdering<int> > solverA;
	BiCGSTAB<SparseMatrix<double>, IncompleteLUT<double> > solverA;
	solverA.preconditioner().setDroptol(0.001);
	solverA.compute(LHS_A);
		
	//cout<<"Sparse Matrix"<<endl<<LHS_A<<endl;
	
	for (int t = 0; t<720; t++)
	{
	
		for (y = 1; y<l_matrix-1; y++)
		{
			for (x=1; x<l_matrix-1; x++)
			{
				find_neighbors(valid_neighbors, v_bdry, x, y, l_matrix-2);
				Conc_val = neighbors_Conc_sum(x, y, conc_profile);
				row_num = find_rowNumber(x, y, l_matrix-2);
				LHS_col(row_num-1) = -r*v_bdry[1]*Cbulk;
				RHS_col(row_num-1) = conc_profile[x][y]*(1-4*r) + r*(Conc_val);
			}
		}
	
	
		for (index = 0; index<n; index++)
		{
			Intermediate(index) = RHS_col(index) - LHS_col(index);
		}
	
		VectorXd Solution = solverA.solve(Intermediate);
	
		index = 0;
	
		for (y = 1; y<l_matrix-1; y++)
		{
			for (x = 1; x<l_matrix-1; x++)
			{
			    conc_profile[x][y] = Solution(index);
			    index++;
			    
			}
			
		}
	
	        //cout<<"New conc Profile"<<Solution;
	    /*if (time%60 == 0)
	    {
	        snprintf(filen, sizeof filen, "glucose/Glucose_conc%d",time);
	        ofp.open(filen);
	        for (y=0; y<l_matrix; y++)
	        {
	            for (x=0; x<l_matrix; x++)
	            {
	                ofp<<conc_profile[x][y]<<",";
	            }
	            ofp<<"\n";
	        }
	        ofp.close();
	    }*/
	}
	
	for (y=0; y<l_matrix; y++)
	{
	    for (x=0; x<l_matrix; x++)
	    {
	        ofp<<conc_profile[x][y]<<",";
	    }
	    ofp<<"\n";
	}
	   
	
	ofp.close();
	
}    

void start_automaton(double conc_profile[l_matrix][b_matrix])
{
    
    cout<<"\n I'm in!";
    int time_of_sim = ceil(10000/time_interval);
    int op1, op2, op3;
    int i, j, k;
    char filename[20];
    FILE *fid, *fid2,*fid3, *fid4;
    int counterForCells = 1;
    //char cell_count[100];
    //char stat_count[100];
    //char collision_count[100];
    //double conc_profile[l_matrix][b_matrix];
    
    stationaryCount[0] = 0;
    collisionCount[0] = 0;
    //Initializing Glucose concentration in 2D scaffold
    //initializeConcglc(conc_profile);
    
    
    for (int time =0; time<time_of_sim; time++)
    {
        change_grid_status();
        collisioncellsNum = 0;
        stationarycellsNum = 0;
        crank_nicolson(conc_profile, time);
        //cout<<"Changed grid status"<<endl;
        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                if (gridsvisited[k][j] == 'Y')
                {
                    
                }
                else
                {
                    op1 = checkcell(k,j);
                    if (op1 == 1)
                    {
                        op2 = checkdivctr(k,j);
                        if (op2 == 0)
                        {
                            dividecells(k,j);
                        }
                        else
                        {
                            divctr[k][j] --;
                            op3 = checkpersistencectr(k,j);
                            if (op3 == 0)
                            {
                                changedirofmigration(k,j);
                            }
                            else
                            {
                                persctr[k][j] = persctr[k][j] - 1;
                                cell_migrate(k,j);
                            }
                        }
                    }
                }
            }
        }
        //Writing to a file
        sprintf (filename, "/home/guru/BTP/5_May/2D_Plots/cell_position%d.txt", time);      
        fid = fopen(filename,"w");        
        cout<<endl<<"Viewing tissue state after t="<<time<<endl;
        tissue_state(fid, counterForCells);
        //cout<<collisioncellsNum<<"  "<<stationarycellsNum;
        collisionCount[counterForCells] = collisioncellsNum;
        stationaryCount[counterForCells] = stationarycellsNum;
        
        counterForCells ++;
        //view_Matrix();
        fclose(fid);
        
    }
         
    //sprintf (cell_count, "/home/guru/BTP/5_May/cell_count.txt");
    //sprintf (stat_count, "/home/guru/BTP/5_May/stationary_count.txt");
    //sprintf (collision_count, "/home/guru/BTP/5_May/collision_count.txt");
    
    //fid2 = fopen(cell_count, "w");
    fid2 = fopen("/home/guru/BTP/5_May/cell_count.txt", "w");
    fid3 = fopen("/home/guru/BTP/5_May/stationary_count.txt", "w");
    fid4 = fopen("/home/guru/BTP/5_May/collision_count.txt", "w");
    
    //fid3 = fopen(stat_count, "w");
    //fid4 = fopen(collision_count, "w");
        
    for (i = 0; i<counterForCells; i++)
    {
        fprintf(fid2, "%d", cellCount[i]);
        fprintf(fid2, "\n");
        fprintf(fid3, "%d", stationaryCount[i]);
        fprintf(fid3, "\n");
        fprintf(fid4, "%d", collisionCount[i]);
        fprintf(fid4, "\n");
    }
    fclose(fid2);
    fclose(fid3);
    fclose(fid4);
    
    /*for (int time = 0; time<time_of_sim; time++)
    {   
        //cout<<endl<<"Time ="<<time<<endl;
        change_grid_status();
        //cout<<"Changed grid status"<<endl;
        for (i = 0; i<h_matrix; i++)
        {
            for (j=0; j<b_matrix; j++)
            {
                for (k=0; k<l_matrix; k++)
                {
                    //cout<<endl<<k<<" "<<j<<" "<<i;
                    //cout<<endl<<"Current_Position"<<k<<j<<i<<endl;
                    if (gridsvisited[k][j][i] == 'Y')
                    {
                        //cout<<"Visited da! Pack off"<<" "<<k<<" "<<j<<" "<<i;
                        
                    }
                    else
                    {
                        op1 = checkcell(k,j,i);
                        if (op1 == 1)
                        {
                            op2 = checkdivctr(k,j,i);
                            if (op2 == 0)
                            {
                                //cout<<endl<<"Cell is dividing"<<k<<" "<<j<<" "<<i<<endl;
                                dividecells(k,j,i);
                            }
                            else
                            {
                                divctr[k][j][i] = divctr[k][j][i] - 1;
                                op3 = checkpersistencectr(k,j,i);
                                if (op3 == 0)
                                {
                                    //cout<<endl<<"changing dir of migration"<<endl;
                                    changedirofmigration(k,j,i);
                                }
                                else
                                {
                                    persctr[k][j][i] = persctr[k][j][i] - 1;
                                    //cout<<endl<<"Cell is migrating"<<k<<" "<<j<<" "<<i<<endl;
                                    cell_migrate(k,j,i);
                                    //view_Matrix();
                                    
                                }
                            }
                        }
                    }
                }
            }
        }
        cout<<endl<<"Viewing tissue state after t="<<time<<endl;
        tissue_state1(fidx, fidy, fidz);
        //view_Matrix();
        
        june ponal july katre
    }*/
}
                    
int main()
{
    int radius;
    double conc_profile[l_matrix][b_matrix];
    initializeConcglc(conc_profile);
    
    /*Writing to a file
    FILE *fidx, *fidy, *fidz;
    fidx = fopen("/home/guru/Desktop/x_coordinates.txt", "w");
    fidy = fopen("/home/guru/Desktop/y_coordinates.txt", "w");
    fidz = fopen("/home/guru/Desktop/z_coordinates.txt", "w");*/

    FILE *fid;
    fid = fopen("/home/guru/BTP/5_May/2D_Plots/Initial.txt", "w");

    //Initial Seeding Matrix
    cout<<endl<<"Seeding Matrix"<<endl;    
    //initseedontop();
    
    //cout<<"Enter radius of wound =";
    //cin>>radius;
    
    //wound_healing(radius);
    
    Seed_Matrix();
    cout<<endl<<"Viewing Tissue State"<<endl;
    
    tissue_state(fid,0);
    
    fclose(fid);
    //view_Matrix();
    
    //Running Automaton
    cout<<endl<<"Starting Automaton"<<endl;
    start_automaton(conc_profile);
    
    /*fclose(fidx);
    fclose(fidy);
    fclose(fidz);*/
    
    //Viewing Contents
    //view_Matrix();
    
    return (0);
}
                   


        

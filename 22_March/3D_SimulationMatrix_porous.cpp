//2D Simulation Matrix


#include<iostream>
#include<stdio.h>
#include<omp.h>
#include<random>
#include<stdlib.h>
#include<map>
#include<fstream>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
using namespace boost::algorithm;


//GLOBAL Variables
const int l_matrix = 5;
const int b_matrix = 5;
const int h_matrix = 5;
//const int h_matrix = 3;
const int T_Div = 1200;
int Sim_Matrix[l_matrix][b_matrix][h_matrix];
int mig_index[l_matrix][b_matrix][h_matrix];
int persctr[l_matrix][b_matrix][h_matrix];
int divctr[l_matrix][b_matrix][h_matrix];
char gridsvisited[l_matrix][b_matrix][h_matrix];
char porous_matrix[l_matrix][b_matrix][h_matrix];
int collide_persctr = 7;
int collide_persctr_porous = 30;
int stationary_persctr = 5;


void Matrix_Porosity()
{
    int l_pos, b_pos, h_pos;
    int num_pores, i, l, b, h;
    
    //Initializing Porous Matrix
    
    for (h = 0; h<h_matrix; h++)
    {   
        for (b=0; b<b_matrix; b++)
        {
            for (l=0; l<l_matrix; l++)
            {
                porous_matrix[l][b][h] = 'N';
            }
        }
    }
                
    
    /*for (col=0; col<b_matrix; col++)
    {
        for (row=0; row<l_matrix; row++)
        {
            porous_matrix[row][col] = 'N';
        }
    }*/
    
    //Number of pores = 50% of Matrix ;
    num_pores = 0.5*(l_matrix*b_matrix*h_matrix);
    
    for (i=0; i<num_pores; i++)
    {   
        l_pos = rand()%(l_matrix-1)+0;
        b_pos = rand()%(b_matrix-1)+0;
        h_pos = rand()%(h_matrix-1)+0;
        
        porous_matrix[l_pos][b_pos][h_pos] = 'Y';

    }
}
    
void view_porous_matrix()
{
    int k,j;
    
    for (k=0; k<l_matrix; k++)
    {
        for (j=0; j<b_matrix; j++)
        {
            //cout<<k<<","<<j<<" ";
            cout<<porous_matrix[k][j]<<" ";
        }
        cout<<endl;
    }
}
            

//MAPPING Mig_Index to Persistence counter
//map<int, int> m = {{1, 24}, {2, 24}, {3, 24}, {4, 24}, {5, 24}, {6, 24}, {7, 30}, {0, 5}};

int checkpersctr(int mig_dir)
{
    int persctr;
    switch(mig_dir)
    {
        case 0: persctr = 5;
                break;
        case 1: 
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:persctr = 24;
               break;
        case 7:persctr = 30;
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
    boost::variate_generator<boost::mt19937, boost::normal_distribution<> > gen(igen, boost::normal_distribution<>(20,2.5));
    
    Division_time = gen()*60;
    //Division_time = Division_time*60;
    return Division_time;
}
    


//Populate Matrices
void uniform_populateMatrix(int x, int y, int z)
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
    if (randc>0.9)
    {
        
        migration = rand() % 6 + 1;
        Sim_Matrix[x][y][z] = 1;
        mig_index[x][y][z] = migration;
        persctr[x][y][z] = checkpersctr(migration);
        //divctr[x][y] = T_Div;
        divctr[x][y][z] = random_time_division();
        gridsvisited[x][y][z] = 'N';
    }
    else
    {
        Sim_Matrix[x][y][z] = 0;
        mig_index[x][y][z] = 0;
        persctr[x][y][z] = 0;
        divctr[x][y][z] = 0;
        gridsvisited[x][y][z] = 'N';
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
                
/*
void wound_healing(int radius)
{
    int i,j,k;
    int migration;
    int arbit = 1000;
    
    
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
            }
        }
    }*/
  
                
    
    
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
     }
}*/
                    
void Seed_Matrix()
{
    
    int i, j, k;
    double t1, t2, el_time;
    
    
    cout<<endl<<"Populating Scaffold";
    t1 = clock();   
    //#pragma omp parallel for collapse(3)
    
    for (i = 0; i<h_matrix; i++)
    {
         for (j=0; j<b_matrix; j++)
         {
            for (k=0;k<l_matrix;k++)
            {
                if (porous_matrix[k][j][i] == 'N')
                {
                    //cout<<porous_matrix[k][j]<<" "<<k<<j;
                    uniform_populateMatrix(k,j,i);
                }
            }
         }
    }
    t2 = clock();
    el_time = (t2-t1)/CLOCKS_PER_SEC;
    //cout<<"Time Elapsed_series"<<el_time;
}
  

void view_Matrix(double conc_grad[l_matrix][b_matrix][h_matrix])
{
    int i, j, k;
    //Viewing Contents of the Matrix
    cout<<endl<<"Contents of the Matrix are"<<endl;
    
    for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0;k<l_matrix;k++)
            {
                cout<<"SIM:"<<Sim_Matrix[k][j][i]<<" "<<"MIG_index:"<<mig_index[k][j][i]<<" "<<"Pers_ctr:"<<persctr[k][j][i]<<" ";
                cout<<"DIV_ctr:"<<divctr[k][j][i]<<"Position:"<<k<<" "<<j<<" "<<i<<" Conc: "<<conc_grad[k][j][i]<<" Pores: "<<porous_matrix[k][j][i];
                cout<<endl;
            }
        }   
    } 
    

}
    
void change_grid_status()
{
    int i, j, k;
    
    for (i = 0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0;k<l_matrix; k++)
            {
                if (porous_matrix[k][j][i] == 'N')
                {
                    gridsvisited[k][j][i] = 'N';
                }
                else
                {
                    gridsvisited[k][j][i] = 'Y';
                }
            }
        }
    } 
}

int checkcell(int k, int j, int i)
{
    if (Sim_Matrix[k][j][i] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkdivctr(int k, int j, int i)
{
    if (divctr[k][j][i] == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int cell_neighbours(int k, int j,int i, int* xneighbour, int* yneighbour, int* zneighbour)
{
    //int xneighbour[6], yneighbour[6], zneighbour[6];
    int index = 0;
    //Check if k-1 && k+1 is within matrix
    if ((k-1)>=0 && (k-1)<l_matrix)
    {
        xneighbour[index] = k-1;
        yneighbour[index] = j;
        zneighbour[index] = i;
        index ++;
    }
    if ((k+1)>=0 && (k+1)<l_matrix) 
    {
        xneighbour[index] = k+1;
        yneighbour[index] = j;
        zneighbour[index] = i;
        index ++;
    }
    if ((j-1)>=0 && (j-1)<b_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j-1;
        zneighbour[index] = i;
        index ++;
    }
    if ((j+1)>=0 && (j+1)<b_matrix) 
    {
        xneighbour[index] = k;
        yneighbour[index] = j+1;
        zneighbour[index] = i;
        index ++;
    }
    if ((i-1)>=0 && (i-1)<h_matrix) 
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
    }
    return (index);

}
    
int cell_neighbours_empty(int* xneighbours, int* yneighbours, int* zneighbours, int num_neighbours, int* empty_x, int* empty_y, int* empty_z)
{
    int i, timediv;
    int mt_i = 0;
    for (i = 0; i<num_neighbours; i++)
    {
        if ((Sim_Matrix[xneighbours[i]][yneighbours[i]][zneighbours[i]] == 0) && (porous_matrix[xneighbours[i]][yneighbours[i]][zneighbours[i]] == 'N'))
        {
            empty_x[mt_i]= xneighbours[i];
            empty_y[mt_i]= yneighbours[i];
            //cout<<"EMPTY Neighbours"<<"-->"<<yneighbours[i]<<","<<xneighbours[i];
            empty_z[mt_i]= zneighbours[i];
            mt_i ++;
        }
    }
    return (mt_i);
}
            
void gridcell_stationaryphase(int k, int j, int i)
{
     int timediv;
     timediv = divctr[k][j][i];
     Sim_Matrix[k][j][i] = 1;
     mig_index[k][j][i] = 0;
     persctr[k][j][i] = stationary_persctr;
}


void dividecells(int k, int j, int i)
{
    
    int xneighbours[6], yneighbours[6], zneighbours[6];
    int empty_x[6], empty_y[6], empty_z[6];
    int num_neighbours, empty_num_neighbours;
    int mig_old, randomposition, mig_new;
    
    num_neighbours = cell_neighbours(k, j, i, xneighbours, yneighbours, zneighbours);   
    empty_num_neighbours = cell_neighbours_empty(xneighbours, yneighbours, zneighbours, num_neighbours, empty_x, empty_y, empty_z);
    
    //cout<<"Number of empty neighbours"<<empty_num_neighbours;
    
    //Check if there are no empty grids
    if (empty_num_neighbours == 0)
    {
        gridcell_stationaryphase(k, j, i);
    }
    else
    {
        randomposition = rand()%empty_num_neighbours;
        
        mig_old = rand()%6 + 1;
        Sim_Matrix[k][j][i] = 1;
        mig_index[k][j][i] = mig_old;
        persctr[k][j][i] = checkpersctr(mig_old);
        divctr[k][j][i] = T_Div;
        
        
        mig_new = rand()%6 + 1;
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]]= mig_new;
        persctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = checkpersctr(mig_new);
        divctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]]= T_Div;
        
        gridsvisited[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = 'Y';
        
    }
}


int checkpersistencectr(int k, int j, int i)
{
	if (persctr[k][j][i] == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


int checkmigdir(int k, int j, int i, int x_newpos, int y_newpos, int z_newpos)
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
	else if (i-1 == z_newpos)
	{
	    migdir = 5;
	}
	else if (i+1 == z_newpos)
	{
	    migdir = 6;
	}
	return migdir;
}
	     
int check_chemicalgradient(int* empty_x, int* empty_y, int* empty_z, int empty_num_neighbours, double conc_grad[l_matrix][b_matrix][h_matrix])
{
    int i, max, index;
    max = -10;
    int pos_x, pos_y, pos_z;
    for (i=0; i<empty_num_neighbours; i++)
    {
        pos_x = empty_x[i];
        pos_y = empty_y[i];
        pos_z = empty_z[i];
        if (conc_grad[pos_x][pos_y][pos_z]>max)
        {
            max = conc_grad[pos_x][pos_y][pos_z];
            index = i;
        }
    }
    return index;
}
       
    

void changedirofmigration(int k, int j, int i, double conc_grad[l_matrix][b_matrix][h_matrix])
{
	        
    int xneighbours[6], yneighbours[6], zneighbours[6];
    int empty_x[6], empty_y[6], empty_z[6];
    int num_neighbours;
    int migdir, tdiv;
    int empty_num_neighbours, randomposition;
    
    num_neighbours = cell_neighbours(k, j, i, xneighbours, yneighbours, zneighbours);   
    empty_num_neighbours = cell_neighbours_empty(xneighbours, yneighbours, zneighbours, num_neighbours, empty_x, empty_y, empty_z);
    
    //cout<<"empty grids"<<empty_num_neighbours;
    
    //Check if there are no empty grids
    if (empty_num_neighbours == 0)
    {
        gridcell_stationaryphase(k, j, i);
    }
    else
    {
        //randomposition = rand()%empty_num_neighbours; //Undirected Migration
        randomposition = check_chemicalgradient(empty_x, empty_y, empty_z, empty_num_neighbours, conc_grad); //directed Migration
        
        cout<<"New position ="<<"("<<empty_x[randomposition]<<","<<empty_y[randomposition]<<")"<<conc_grad[empty_x[randomposition]][empty_y[randomposition]]<<endl;
        
        
        migdir = checkmigdir(k,j,i,empty_x[randomposition], empty_y[randomposition],empty_z[randomposition]);
        tdiv = divctr[k][j][i];
        
        //NEW GRID CELL
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]]= 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = migdir;
        persctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = checkpersctr(migdir);
        divctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = tdiv;
        
        //OLD GRID CELL
        Sim_Matrix[k][j][i] = 0;
        mig_index[k][j][i]  = 0;
        persctr[k][j][i] = 0;
        divctr[k][j][i]  = 0;
        
        //updating gridsvisited
        gridsvisited[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = 'Y';
        
    }
}


void next_step(int k, int j, int i, int mig_dir, int *newpos)
{
    //int *newpos;
        
    if (mig_dir == 1)
    {
        newpos[0] = k;
        newpos[1] = j+1;
        newpos[2] = i;
    }
    else if (mig_dir == 2)
    {
        newpos[0] = k+1;
        newpos[1] = j;
        newpos[2] = i;
    }
    else if (mig_dir == 3)
    {
        newpos[0] = k;
        newpos[1] = j-1;
        newpos[2] = i;
    }    
    else if (mig_dir == 4)
    {
        newpos[0] = k-1;
        newpos[1] = j;
        newpos[2] = i;
    }        
    else if (mig_dir == 5)
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
    }        
    
    if (((newpos[0]>=0 && newpos[0]<l_matrix) && (newpos[1]>=0 && newpos[1]<b_matrix)) && ((porous_matrix[newpos[0]][newpos[1]][newpos[2]]=='N') && (newpos[2]>=0 && newpos[2]<=h_matrix)))
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
        newpos[2] = -1;
        //return v;
    }
}


void gridcell_collisionstate(int k, int j, int i)
{
    int tdiv;
    tdiv = divctr[k][j][i];
    
    //cout<<"Inside grid-cell wa collisionstate desu"<<endl;
    //cout<<m[7];
    //Re-assign values
    Sim_Matrix[k][j][i] = 1;
    mig_index[k][j][i] = 7;
    persctr[k][j][i] = collide_persctr;
    //divctr[k][j][i] = tdiv;
}

void gridcell_collisionstate_porous(int k, int j, int i)
{
    int tdiv;
    tdiv = divctr[k][j][i];
    cout<<endl<<"Collision due to porous scaffold"<<endl;
    //cout<<"Inside grid-cell wa collisionstate desu"<<endl;
    //cout<<m[7];
    //Re-assign values
    Sim_Matrix[k][j][i] = 1;
    mig_index[k][j][i] = 7;
    persctr[k][j][i] = collide_persctr_porous;
    //divctr[k][j][i] = tdiv;
}

void cell_migrate(int k, int j, int i)
{
    int mig_dir;
    int newpos[3];
    int ctr_pers, tdiv;
    
    mig_dir = mig_index[k][j][i];
    
    //Checking if stationary/collision state
    if (mig_dir == 0 || mig_dir == 7)
    {
        return;
    }
    next_step(k,j,i, mig_dir,newpos);
    
    //Check if movement is valid
    
    if (newpos[0] == -1)
    {
        gridcell_collisionstate_porous(k,j,i);
        return;
    }
    
    //Check if site is vacant
    //cout<<"Oldsite"<<k<<" "<<j<<" "<<i<<"New site is"<<newpos[0]<<" "<<newpos[1]<<" "<<newpos[2]<<endl;
    
    if (Sim_Matrix[newpos[0]][newpos[1]][newpos[2]] == 0)
    {
        //cout<<"Site is empty";
        ctr_pers = persctr[k][j][i];
        tdiv = divctr[k][j][i];
        
        Sim_Matrix[newpos[0]][newpos[1]][newpos[2]] = 1;
        mig_index[newpos[0]][newpos[1]][newpos[2]] = mig_dir;
        persctr[newpos[0]][newpos[1]][newpos[2]] = ctr_pers;
        divctr[newpos[0]][newpos[1]][newpos[2]] = tdiv;
        
        Sim_Matrix[k][j][i] = 0;
        mig_index[k][j][i] = 0;
        persctr[k][j][i] = 0;
        divctr[k][j][i] = 0;
    }
    else
    {
        //Go to collision state
        //cout<<"Going to collision state!!!";
        gridcell_collisionstate(k,j,i);
        gridcell_collisionstate(newpos[0],newpos[1],newpos[2]);
        divctr[newpos[0]][newpos[1]][newpos[2]] --;
    }
    
    gridsvisited[newpos[0]][newpos[1]][newpos[2]] = 'Y';
}

void tissue_state(FILE *fid)
{
    int i,j,k;
    cout<<"entering Tissue_state";
    int xcoord[125];
    int ycoord[125];
    int zcoord[125];
    
    /*const long int vectorsize = l_matrix*b_matrix*h_matrix;
    //int xcoord[l_matrix*b_matrix*h_matrix], ycoord[l_matrix*b_matrix*h_matrix], zcoord[l_matrix*b_matrix*h_matrix];
    short int xcoord[vectorsize];
    short int ycoord[vectorsize];
    short int zcoord[vectorsize];*/
    //short int zcoord[vectorsize];
    
    long int index = 0;

    for (i=0;i<h_matrix; i++)
    {

        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                if (Sim_Matrix[k][j][i] == 1)
                {
                    xcoord[index] = j;
                    ycoord[index] = k;
                    zcoord[index] = i;
                    index ++;
                }
            }
        }
    }
    
    

    
    

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
        fprintf(fid, "%d", zcoord[ctr]);
        fprintf(fid, " ");
        fprintf(fid,"\n");
        
    }
    
}


void read_ConcMatrix(double conc_grad[l_matrix][b_matrix][h_matrix], char filename[100])
{
    string line;
    ifstream myfile;
    myfile.open(filename);
    //vector<string> tokens;
    int row = 0;
    double Conc_profile[l_matrix*b_matrix*h_matrix];
    //int a =0;
    int i, j, k;
    
    if(!myfile) 
    {
        cout<<"Error opening output file"<<endl;
        system("pause");
        //return -1;
    }
    while(!myfile.eof())
    {
        myfile>>Conc_profile[row];
        //getline(myfile, conc_grad[row],'\n');
        row ++;
    }
    int row_number;
    
    for (i=0; i<h_matrix; i++)
    {
		for (j=0; j<b_matrix; j++)
		{
		    for (k=0;k<l_matrix; k++)
		    {
				row_number = i*l_matrix*b_matrix+j*b_matrix+k;
				conc_grad[k][j][i] = Conc_profile[row_number];
    	    }
        }
    }

   
    myfile.close();
    //return 1;
}

   
    

void start_automaton()
{
    int time_of_sim = 10;
    int op1, op2, op3;
    int i, j, k;
    char filename[20];
    char Conc_filename[100];
    //int arbit_num = 9;
    FILE *fid;
    
    
    
    
    for (int time =0; time<time_of_sim; time++)
    {
        change_grid_status();
        //cout<<"Changed grid status"<<endl;
        
        sprintf(Conc_filename, "/home/guru/BTP/octave_diffusion/Concfiles/Conc_Matrix_%d.txt", time+1);
        double conc_grad[l_matrix][b_matrix][h_matrix];
        read_ConcMatrix(conc_grad, Conc_filename);
        
        //cout<<endl<<"Concentration in 2D Matrix"<<endl;
        
        //Display concentration gradient
        /*for (int row = 0; row<b_matrix; row++)
        {
            for (int col = 0; col<l_matrix; col++)
            {
                cout<<conc_grad[row][col]<<" ";
            }
            cout<<endl;
        }*/

        
        //view_Matrix(conc_grad);
        for (i=0; i<h_matrix; i++)
        {
            for (j=0; j<b_matrix; j++)
            {
                for (k=0; k<l_matrix; k++)
                {
                    if (gridsvisited[k][j][i] == 'Y')
                    {
                    
                    }
                    else
                    {
                        op1 = checkcell(k,j,i);
                        if (op1 == 1)
                        {
                            op2 = checkdivctr(k,j,i);
                            if (op2 == 0)
                            {
                                dividecells(k,j,i);
                            }
                            else
                            {
                                divctr[k][j][i] --;
                                op3 = checkpersistencectr(k,j,i);
                                if (op3 == 0)
                                {
                                    changedirofmigration(k,j,i,conc_grad);
                                }
                                else
                                {
                                    persctr[k][j][i] --;
                                    cell_migrate(k,j,i);
                                }
                            }
                        }
                    }
                }
            }
        }
         //Writing to a file
        sprintf (filename, "/home/guru/BTP/22_March/2D_Plots/file%d.txt", time);
        fid = fopen(filename,"w");
        cout<<endl<<"Viewing tissue state after t="<<time<<endl;
        tissue_state(fid);
        
        fclose(fid);
    }
 
}
                    
int main()
{
    int radius;
    
   
    FILE *fid;
    fid = fopen("/home/guru/BTP/22_March/2D_Plots/Initial.txt", "w");
        
    //Scaffold Porosity
    Matrix_Porosity();
    
    cout<<endl<<"Viewing porous matrix!"<<endl;
    //view_porous_matrix();
    
    
    //Initial Seeding Matrix
    cout<<endl<<"Seeding Matrix"<<endl;    
    //initseedontop();
    
    //cout<<"Enter radius of wound =";
    //cin>>radius;
    
    //wound_healing(radius);
    
    Seed_Matrix();
    cout<<endl<<"Viewing Tissue State"<<endl;
    //tissue_state(fid);
    
    //fclose(fid);
    //view_Matrix();
    
    //Running Automaton
    cout<<endl<<"Starting Automaton"<<endl;
    start_automaton();
    
    
    //Viewing Contents
    //view_Matrix();
    
    return (0);
}


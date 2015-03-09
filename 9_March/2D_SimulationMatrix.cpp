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

using namespace std;



//GLOBAL Variables
const int l_matrix = 50;
const int b_matrix = 50;
//const int h_matrix = 3;
const int T_Div = 1200;
int Sim_Matrix[l_matrix][b_matrix];
int mig_index[l_matrix][b_matrix];
int persctr[l_matrix][b_matrix];
int divctr[l_matrix][b_matrix];
char gridsvisited[l_matrix][b_matrix];
char porous_matrix[l_matrix][b_matrix];
int collide_persctr = 30;
int collide_persctr_porous = 10;
int stationary_persctr = 5;


void Matrix_Porosity()
{
    int row_pos, col_pos;
    int num_pores, i, row, col;
    
    //Initializing Porous Matrix
    
    for (col=0; col<b_matrix; col++)
    {
        for (row=0; row<l_matrix; row++)
        {
            porous_matrix[row][col] = 'N';
        }
    }
    
    //Number of pores = 50% of Matrix ;
    num_pores = 0.5*(l_matrix*b_matrix);
    
    for (i=0; i<num_pores; i++)
    {   
        row_pos = rand()%(l_matrix-1)+0;
        col_pos = rand()%(b_matrix-1)+0;
        porous_matrix[row_pos][col_pos] = 'Y';

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
    if (randc>0.9)
    {
        
        migration = rand() % 4 + 1;
        Sim_Matrix[x][y] = 1;
        mig_index[x][y] = migration;
        persctr[x][y] = checkpersctr(migration);
        //divctr[x][y] = T_Div;
        divctr[x][y] = random_time_division();
        gridsvisited[x][y] = 'N';
    }
    else
    {
        Sim_Matrix[x][y] = 0;
        mig_index[x][y] = 0;
        persctr[x][y] = 0;
        divctr[x][y] = 0;
        gridsvisited[x][y] = 'N';
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
            if (porous_matrix[k][j] == 'N')
            {
                //cout<<porous_matrix[k][j]<<" "<<k<<j;
                uniform_populateMatrix(k,j);
            }
        }
    }
    
    t2 = clock();
    el_time = (t2-t1)/CLOCKS_PER_SEC;
    //cout<<"Time Elapsed_series"<<el_time;
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
            if (porous_matrix[k][j] == 'N')
            {
                gridsvisited[k][j] = 'N';
            }
            else
            {
                gridsvisited[k][j] = 'Y';
            }
        }
    } 
    
             
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
        if ((Sim_Matrix[xneighbours[i]][yneighbours[i]] == 0) && (porous_matrix[xneighbours[i]][yneighbours[i]] == 'N'))
        {
            empty_x[mt_i]= xneighbours[i];
            empty_y[mt_i]= yneighbours[i];
            //cout<<"EMPTY Neighbours"<<"-->"<<yneighbours[i]<<","<<xneighbours[i];
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
        
        
        mig_new = rand()%4 + 1;
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]] = 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]]= mig_new;
        persctr[empty_x[randomposition]][empty_y[randomposition]] = checkpersctr(mig_new);
        divctr[empty_x[randomposition]][empty_y[randomposition]]= T_Div;
        
        gridsvisited[empty_x[randomposition]][empty_y[randomposition]] = 'Y';
        
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
        //NEW GRID CELL
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]]= 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]] = migdir;
        persctr[empty_x[randomposition]][empty_y[randomposition]] = checkpersctr(migdir);
        divctr[empty_x[randomposition]][empty_y[randomposition]] = tdiv;
        
        //OLD GRID CELL
        Sim_Matrix[k][j] = 0;
        mig_index[k][j]  = 0;
        persctr[k][j] = 0;
        divctr[k][j]  = 0;
        
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
    
    if (((newpos[0]>=0 && newpos[0]<l_matrix) && (newpos[1]>=0 && newpos[1]<b_matrix)) && (porous_matrix[newpos[0]][newpos[1]]=='N'))
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
    //divctr[k][j][i] = tdiv;
}

void gridcell_collisionstate_porous(int k, int j)
{
    int tdiv;
    tdiv = divctr[k][j];
    
    //cout<<"Inside grid-cell wa collisionstate desu"<<endl;
    //cout<<m[7];
    //Re-assign values
    Sim_Matrix[k][j] = 1;
    mig_index[k][j] = 7;
    persctr[k][j] = collide_persctr_porous;
    //divctr[k][j][i] = tdiv;
}

void cell_migrate(int k, int j)
{
    int mig_dir;
    int newpos[2];
    int ctr_pers, tdiv;
    
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
        gridcell_collisionstate_porous(k,j);
        return;
    }
    
    //Check if site is vacant
    //cout<<"Oldsite"<<k<<" "<<j<<" "<<i<<"New site is"<<newpos[0]<<" "<<newpos[1]<<" "<<newpos[2]<<endl;
    
    if (Sim_Matrix[newpos[0]][newpos[1]] == 0)
    {
        //cout<<"Site is empty";
        ctr_pers = persctr[k][j];
        tdiv = divctr[k][j];
        
        Sim_Matrix[newpos[0]][newpos[1]] = 1;
        mig_index[newpos[0]][newpos[1]] = mig_dir;
        persctr[newpos[0]][newpos[1]] = ctr_pers;
        divctr[newpos[0]][newpos[1]] = tdiv;
        
        Sim_Matrix[k][j] = 0;
        mig_index[k][j] = 0;
        persctr[k][j] = 0;
        divctr[k][j] = 0;
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

void tissue_state(FILE *fid)
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
                xcoord[index] = j;
                ycoord[index] = k;
                index ++;
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
        //fprintf(fid, "%d", zcoord[ctr]);
        //fprintf(fid, " ");
        fprintf(fid,"\n");
        
    }
    
}


   
    

void start_automaton()
{
    int time_of_sim = 8000;
    int op1, op2, op3;
    int i, j, k;
    char filename[20];
    FILE *fid;
    
    
    
    
    for (int time =0; time<time_of_sim; time++)
    {
        change_grid_status();
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
        sprintf (filename, "/home/guru/BTP/9_March/2D_Plots_porous/file%d.txt", time);
        fid = fopen(filename,"w");
        cout<<endl<<"Viewing tissue state after t="<<time<<endl;
        tissue_state(fid);
        //view_Matrix();
        fclose(fid);
    }
 
}
                    
int main()
{
    int radius;
    
   
    FILE *fid;
    fid = fopen("/home/guru/BTP/9_March/2D_Plots_porous/Initial.txt", "w");
        
    //Scaffold Porosity
    Matrix_Porosity();
    
    cout<<endl<<"Viewing porous matrix!"<<endl;
    view_porous_matrix();
    
    
    //Initial Seeding Matrix
    cout<<endl<<"Seeding Matrix"<<endl;    
    //initseedontop();
    
    //cout<<"Enter radius of wound =";
    //cin>>radius;
    
    //wound_healing(radius);
    
    Seed_Matrix();
    cout<<endl<<"Viewing Tissue State"<<endl;
    
    tissue_state(fid);
    
    fclose(fid);
    //view_Matrix();
    
    //Running Automaton
    cout<<endl<<"Starting Automaton"<<endl;
    start_automaton();
    
    
    //Viewing Contents
    //view_Matrix();
    
    return (0);
}
                   


        

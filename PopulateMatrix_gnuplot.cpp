#include<iostream>
#include<stdio.h>
#include<omp.h>
#include<random>
#include<stdlib.h>
#include<map>
#include<fstream>
#include<string.h>


using namespace std;



//GLOBAL Variables
const int l_matrix = 100;
const int b_matrix = 100;
const int h_matrix = 100;
const int T_Div = 1200;
int Sim_Matrix[l_matrix][b_matrix][h_matrix];
int mig_index[l_matrix][b_matrix][h_matrix];
int persctr[l_matrix][b_matrix][h_matrix];
int divctr[l_matrix][b_matrix][h_matrix];
char gridsvisited[l_matrix][b_matrix][h_matrix];
int collide_persctr = 30;
int stationary_persctr = 5;




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



//Populate Matrices
void uniform_populateMatrix(int x, int y, int z)
{
    double randc;
    randc = ((double) rand()/(RAND_MAX));
    
      
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dis(0, 1);
    //double randc = dis(gen);
    //srand (time(NULL));
    int migration;
    if (randc>0.95)
    {
        
        migration = rand() % 6 + 1;
        Sim_Matrix[x][y][z] = 1;
        mig_index[x][y][z] = migration;
        persctr[x][y][z] = checkpersctr(migration);
        divctr[x][y][z] = T_Div;
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

void initseedontop()
{
    int i, j, k;
    int migration;
    
    for (i=h_matrix-1;i>h_matrix-5;i--)
    {
        for (j=0;j<b_matrix;j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                migration = rand()%6+1;
                Sim_Matrix[k][j][i] = 1;
                mig_index[k][j][i] = migration;
                persctr[k][j][i] = checkpersctr(migration);
                divctr[k][j][i] = T_Div;
            }
        }
    }
}
                

void wound_healing(int radius)
{
    int i,j,k;
    int migration;
    
    for (i=0; i<h_matrix; i++)
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
                    divctr[k][j][i] = T_Div;
                }
             }
         }
     }
}
                    
              
    
    


void Seed_Matrix()
{
    
    int i, j, k;
    double t1, t2, el_time;
    
    
    cout<<endl<<"Populating Scaffold";
    t1 = clock();   
    //#pragma omp parallel for collapse(3)
    for (i = 0; i<h_matrix; i++)
    {
        for (j=0;j<b_matrix; j++)
        {
            for (k=0;k<l_matrix; k++)
            {
                 uniform_populateMatrix(k,j,i);
            }
        }
    }
    t2 = clock();
    el_time = (t2-t1)/CLOCKS_PER_SEC;
    cout<<"Time Elapsed_series"<<el_time;
}
  

void view_Matrix()
{
    int i, j, k;
    //Viewing Contents of the Matrix
    cout<<endl<<"Contents of the Matrix are"<<endl;
    for (i = 0; i<h_matrix; i++)
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
    }
}
    
void change_grid_status()
{
    int i, j, k;
    for (i=0; i<h_matrix; i++)
    {
        for (j=0; j<b_matrix; j++)
        {
            for (k=0; k<l_matrix; k++)
            {
                gridsvisited[k][j][i] = 'N';
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


int cell_neighbours(int k, int j, int i, int* xneighbour, int* yneighbour, int* zneighbour)
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
        if (Sim_Matrix[xneighbours[i]][yneighbours[i]][zneighbours[i]] == 0)
        {
            empty_x[mt_i]= xneighbours[i];
            empty_y[mt_i]= yneighbours[i];
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
        mig_index[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = mig_new;
        persctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = checkpersctr(mig_new);
        divctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = T_Div;
        
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
	     


void changedirofmigration(int k, int j, int i)
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
        randomposition = rand()%empty_num_neighbours;
        migdir = checkmigdir(k,j,i,empty_x[randomposition], empty_y[randomposition], empty_z[randomposition]);
        tdiv = divctr[k][j][i];
        //NEW GRID CELL
        Sim_Matrix[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = 1;
        mig_index[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = migdir;
        persctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = checkpersctr(migdir);
        divctr[empty_x[randomposition]][empty_y[randomposition]][empty_z[randomposition]] = tdiv;
        
        //OLD GRID CELL
        Sim_Matrix[k][j][i] = 0;
        mig_index[k][j][i]  = 0;
        persctr[k][j][i]  = 0;
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
    
    if ((newpos[0]>=0 && newpos[0]<l_matrix) && (newpos[1]>=0 && newpos[1]<b_matrix) && (newpos[2]>=0 && newpos[2]<h_matrix))
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
    next_step(k,j,i,mig_dir,newpos);
    
    //Check if movement is valid
    
    if (newpos[0] == -1)
    {
        gridcell_collisionstate(k,j,i);
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
    const long int vectorsize = l_matrix*b_matrix*h_matrix;
    //int xcoord[l_matrix*b_matrix*h_matrix], ycoord[l_matrix*b_matrix*h_matrix], zcoord[l_matrix*b_matrix*h_matrix];
    short int xcoord[vectorsize];
    short int ycoord[vectorsize];
    short int zcoord[vectorsize];
    
    long int index = 0;

    
    
    for (i=0; i<h_matrix; i++)
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
    }
    
    
    
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
        fprintf(fid, "%d", zcoord[ctr]);
        fprintf(fid, " ");
        fprintf(fid,"\n");
        
    }
    
    //fprintf(fid, "\n");
    //fprintf(fidy, "\n");
    //fprintf(fidz, "\n");    


}

void tissue_state1(FILE *fidx, FILE *fidy, FILE *fidz)
{
    int i,j,k;
    const long int vectorsize = l_matrix*b_matrix*h_matrix;
    //int xcoord[l_matrix*b_matrix*h_matrix], ycoord[l_matrix*b_matrix*h_matrix], zcoord[l_matrix*b_matrix*h_matrix];
    short int xcoord[vectorsize];
    short int ycoord[vectorsize];
    short int zcoord[vectorsize];
    
    long int index = 0;

    
    
    for (i=0; i<h_matrix; i++)
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
    }
    
    //Writing to a file

/*    FILE *fidx, *fidy, *fidz;
    fidx = fopen("/home/guru/Desktop/x_coordinates.txt", "a");
    fidy = fopen("/home/guru/Desktop/y_coordinates.txt", "a");
    fidz = fopen("/home/guru/Desktop/z_coordinates.txt", "a");
*/
    
    
    //Printing firstValue
    fprintf(fidx, "%d", xcoord[0]);
    fprintf(fidy, "%d", ycoord[0]);
    fprintf(fidz, "%d", zcoord[0]);
    
    //Print allvalues
    for (int ctr = 1; ctr<index; ctr++)
    {
        fprintf(fidx, ",%d", xcoord[ctr]);
        fprintf(fidy, ",%d", ycoord[ctr]);
        fprintf(fidz, ",%d", zcoord[ctr]);
    }
    
    fprintf(fidx, "\n");
    fprintf(fidy, "\n");
    fprintf(fidz, "\n");    
    
    //delete [] xcoord;
    //delete [] ycoord;
    //delete [] zcoord;
    
   
    
}
        
    
    
    

void start_automaton()
{
    int time_of_sim = 2000;
    int op1, op2, op3;
    int i, j, k;
    char filename[20];
    FILE *fid;
        
    
    
    for (int time = 0; time<time_of_sim; time++)
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
        //Writing to a file
        sprintf (filename, "/home/guru/BTP/Plots/file%d.txt", time);
        fid = fopen(filename,"w");
        cout<<endl<<"Viewing tissue state after t="<<time<<endl;
        tissue_state(fid);
        
        fclose(fid);
        
        
        //view_Matrix();
        
        
    }
}
                    
                    


int main()
{
    int radius;
    
    /*Writing to a file
    FILE *fidx, *fidy, *fidz;
    fidx = fopen("/home/guru/Desktop/x_coordinates.txt", "w");
    fidy = fopen("/home/guru/Desktop/y_coordinates.txt", "w");
    fidz = fopen("/home/guru/Desktop/z_coordinates.txt", "w");*/

    FILE *fid;
    fid = fopen("/home/guru/BTP/Plots/Initial.txt", "w");

    //Initial Seeding Matrix
    cout<<endl<<"Seeding Matrix"<<endl;    
    //Seed_Matrix();
    initseedontop();
    
    //cout<<"Enter radius of wound =";
    //cin>>radius;
    
    //wound_healing(radius);
    
    
    cout<<endl<<"Viewing Tissue State"<<endl;
    
    tissue_state(fid);
    
    fclose(fid);
    //view_Matrix();
    
    //Running Automaton
    cout<<endl<<"Starting Automaton"<<endl;
    start_automaton();
    
    /*fclose(fidx);
    fclose(fidy);
    fclose(fidz);*/
    
    //Viewing Contents
    //view_Matrix();
    
    return (0);
}

    
    
    
    
        

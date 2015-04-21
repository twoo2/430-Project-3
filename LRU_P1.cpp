//CSE 430 - Operating Systems
//By: Tyler Woo - 1203788009
//Project 3 - Milestone
//Description: LRU implementation

/**************************************************************************************
Design and Analysis

The data structure used for my implementation of the LRU replacement policy was to 
utilize three functions: search, LRU, and read. Read was initially called by main which 
read in the input file and inputted each line into a respective location in the 4 arrays 
set up to hold values. The Search function used a simple double ‘for’ loop to seek 
through the array and check if there are any duplicates. Lastly, the LRU function did 
all the block movements and computations. The four argument values were initialized as 
long and the pointer array is set up as an int. There was no rounding or truncation 
involved.

The hits were recorded by increasing the value every time a similar value was seen in 
the program. By utilizing the search function these values can be tracked and checked 
throughout the program at key points.
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//INITIALIZE GLOBAL VARIABLES
int j = 0;
int k = 0;
int y = 0;
int blockred = 0;
int pagehit = 0;
int pagefault = 0;
int inc = 0;
int numblockpres = 0;
int startblockpres = 0;
int check;
int lru;
bool done = false;

int search(int *fill, long startblock_1, long numblocks_2, long csize) //Search function returns -1 if found and i if not
{
	for(int i = 0; i < csize; i++)
	{
		for(int j = 0; j < numblocks_2; j++)
		{
			if(fill[i + j] == startblock_1)
			{
				return(i);
			}
		}
	}
	return(-1);
}

int LRU(int *fill, long startblock_1, long numblocks_2, long requestnum_4, long csize) //function that handles all number variables entered from file
{
	int dec = numblocks_2; //set decrementer to number of blocks
	j = 0; //reset j to 0
	if(j < csize) //checks if j is less than the set cache size
	{ 
        if((check = search(fill, startblock_1, numblocks_2, csize)) != -1) //checks if value exists in array
        {
        	cout << startblock_1 << " already exists in frame\n";
        	pagehit++; //increase page hit
        	j++; //increase j
        }
        else
        {
        	cout << startblock_1 << " does not exist in frame\n"; 
        	for(y; y < csize; y++) 
    	    {
    	    	if(fill[y] == 0) //checks if location is empty
            	{
    	            for(int x = 0; x < numblocks_2; x++) 
    	            {
    	            	if((y + x) == csize) //checks if x hits the max size
    	            	{
    	            		cout << "Limit reached\n";
    	            		numblockpres = numblocks_2 - x; //set preserve value to overflow
    	            		startblockpres = startblock_1 + x; //set preserve block to max block
    	            		j = y + x; //set j to max reached value
    	            		lru = fill[inc]; //set the least recently used variable
    	            		cout << "dec: " << dec << "\n";
		    	        	break; 
    	            	}
	            		cout << startblock_1 + x << " has been allocated in location: " << y + x << "\n";
		            	fill[x] = startblock_1 + x; //set block to location
		            	blockred = x; //set preserve value
		            	j++; //increment j
		            	dec--; //decrement dec
		            	pagefault++; //increase page fault
    	            }
    	            lru = fill[inc]; //set the least recently used variable
    	            y = y + blockred; //add preserve value to build y
    	            break;
    	        }
    	        else
    	        {
    	        	break;
    	        } 
    	    }
        }
    }
	while(dec > 0)
	{
    	j = 0;
    	inc = 0;
    	if((numblockpres != 0) && (numblockpres != numblocks_2)) //check for difference in number of blocks
		{
			numblockpres = numblocks_2;
		}
		if((startblockpres != 0) && (startblockpres != startblock_1)) //check for difference in starting block
		{
			startblockpres = startblock_1;
		}
        else
	    {
	    	for(int z = inc; z < csize; z++) 
		    {
		    	if(fill[z] != 0) //checks if location is empty
		        {
		        	for(int u = 0; u < numblockpres; u++)
		        	{
		        		if(numblockpres == u) //checks if the location has reached its max
			        	{
			        		done = true; //set boolean value to true
			        		dec = 0; //reset decrementer
			        		break;
			        	}
			        	if(((u + z) < csize) && !done)
			        	{
			        		cout << "No more slots left, block: " << startblockpres + u << " replacing block: " << fill[u] << " in LRU location: " << inc + u << "\n";
			        		fill[u] = startblockpres + u; //set location value
			        		dec--; //decrease dec
			        		pagefault++; //increase page fault
			        	}
			        	if(((u + z) == csize) && !done)
			        	{
			        		numblockpres = numblockpres - u; //set preserve value
			        		startblockpres = startblockpres + u; //set preserve value
			        		dec--; //decrement dec
			        		inc++; //increase inc
			        		z = 0; //reset z
			        		u = 0; //reset u
			        	}
		        	}
		        	if(done)
		        	{
		        		break;
		        	}
		        }
		    }
    	}
	}
}

int read(long csize)
{
	long startblock_1[csize], numblocks_2[csize], ignore_3[csize], requestnum_4[csize]; //initialize variables
	int *fill = new int[10000000]; //initialize huge array
	for(int i = 0; i < csize; i++)
	{
		fill[i] = 0; //zero out fill
	}
	ifstream infile; //set up file for reading
	infile.open("P1.lis"); //open file

	for(int i = 0; i < csize; i++)
	{
		infile >> startblock_1[i];
		infile >> numblocks_2[i];
		infile >> ignore_3[i];
		infile >> requestnum_4[i];
		cout << "Starting Block: " << startblock_1[i] << " ";
		cout << "Number of Blocks: " << numblocks_2[i] << " ";
		cout << "Ignore: " << ignore_3[i] << " ";
		cout << "Request Number: " << requestnum_4[i] << "\n";
		LRU(fill, startblock_1[i], numblocks_2[i], requestnum_4[i], csize);
	}
	cout << "Page hits: " << pagehit << "\n"; //print page hits
	cout << "Page faults: " << pagefault << "\n"; //print page faults
	delete[] fill; //delete fill
}

int main()
{
	int csize;

	cout << "Please enter a cache size:\n";
	cin >> csize; //read in cache size
	cout << "Reading file:\n";
	read(csize); //begin read
	return(0); //end program
}

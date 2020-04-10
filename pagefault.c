/*...............code by Ramandeep Kaur...................*/

#include <stdio.h>    
#include <stdlib.h>  
#include <string.h> 
#include <time.h> 
typedef long long ll; // ll shorhand for long long will be used further

int randomFlag = 0;   // false
void showInputs(ll *VirtualMemoryPages, ll NoOfPages, ll NoOfFrames);
void createPattern();
int findingPageHit(ll *FrameList, ll NoOfFrames, ll target)
{
	    for(int k = 0; k < NoOfFrames; k++)
		        {
				        if(FrameList[k] == target)
				        return 1;
			 }
	        return 0;
}
void populatePages(ll *VirtualMemoryPages, ll NoOfPages)
{
	    
	    for (int k = 0; k < NoOfPages; k++)
	    VirtualMemoryPages[k] = (rand() % 10);
}
int main(int argc, const char **argv)
{
    srand(time(NULL));
    ll NoOfPages;
    ll *VirtualMemoryPages;
    if(argc == 1)
	    {   printf("******************ASSIGNMENT BY RAMANDEEP KAUR****************\n");
	        printf("section-K18SJ------ROLL NO.57------REG.NO. 11803348\n");
	        printf("This is the program for implementing FIFO Algorithm\n");
	        printf("Provide Pages:For Virtual Memory ");
		 scanf("%lld", &NoOfPages);
	         VirtualMemoryPages = (ll *)malloc(sizeof(ll) * NoOfPages);
            populatePages(VirtualMemoryPages, NoOfPages);
            randomFlag = 1; //true
    }
    else
	    {
		        NoOfPages = argc - 1;
		        VirtualMemoryPages = (ll *)malloc(sizeof(ll) * NoOfPages);
		        int index = 0;
		        for(int k = 1; k < argc; k++)
		        VirtualMemoryPages[index++] = atoll(argv[k]);
	    }
ll NoOfFrames;
printf("No. of Frames: ");
scanf("%lld", &NoOfFrames);
showInputs(VirtualMemoryPages, NoOfPages, NoOfFrames);
ll *FrameList = (ll *)malloc(NoOfFrames * sizeof(ll));
memset(FrameList, -1, NoOfFrames * sizeof(ll));
ll count_page_hit, countPageFault;
count_page_hit = countPageFault = 0;
			        
ll last = -1;
for(int k = 0; k < NoOfPages; k++)
	    {
		            if(findingPageHit(FrameList, NoOfFrames, VirtualMemoryPages[k]))
			    count_page_hit++;
			    else
			      {
				 FrameList[++last % NoOfFrames] = VirtualMemoryPages[k];
                                 countPageFault++;					            }
		               }
 printf("This is the number of PAGE HITS %lld\n", count_page_hit);
 printf("These are the number of PAGE FAULTS %lld\n", countPageFault);
 return 0;
}
void showInputs(ll *VirtualMemoryPages, ll NoOfPages, ll NoOfFrames)
{
	printf("Details of the Data you entered");
	        
        printf("Number of pages\t\t\t: %lld\n", NoOfPages);
  if(randomFlag)
	  {
		   printf("Random ");
	  }
  else            
	  {
		   printf("Custom ");
          }
 printf("virtual memory pages are \t: ");
  for(int k = 0; k < NoOfPages; k++)
  printf("%lld ", VirtualMemoryPages[k]);
  printf("\nNo. of Frames:\t\t: %lld", NoOfFrames);
	         
}

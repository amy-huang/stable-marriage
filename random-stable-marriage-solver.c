#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

/* 
    This program creates a random instance of the stable marriage problem, and then 
    solves it using the stable marriage algorithm.

    The problem was originally articulated in terms of men and women being paired
    off, but I use the analogy of sellers and buyers here. Say we have n sellers
    and n buyers. Each seller has a preference list ranking each buyer, with the 
    first on the list being the most desirable and the last being the least.
    Each buyer has an analogous list for the sellers.

    Our goal is to make "stable pairs" of buyers and sellers, so that after we
    have paired off everyone, there doesn't exist a buyer and seller who aren't
    together but like each other more than the buyer/seller they 
    were matched to.

    The algorithm to accomplish this is as follows: For each seller, make a 
    proposal to the buyers on their list starting from the most desired one.
    If the buyer is available, match the two and move on to the next seller. 
    If not, compare with the incumbent seller - is our seller more desired by 
    this particular buyer than the existing one? If so, switch the two, adding
    the formerly matched seller back into the pool of sellers making proposals.
    Repeat until all sellers are matched.

    Lloyd S. Shapley and Alvin E. Roth won the 2012 Nobel Prize in Economics 
    for developing the theory of this problem and algorithm, with applications
    ranging from college admissions matching, medical residency student 
    matching, and matching users to internet servers.
*/

/* For making an array of integers 1 through n in a random order. Copied from
   https://benpfaff.org/writings/clc/shuffle.html */
void shuffle_array(int *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

int main(int argc, char **argv) {
    /* Record time we started execution, to measure performance */
    time_t start_time;
    srand(time(&start_time));

    /* Check for input value for n; use default if none */
    int n = 0;  
    if (argc == 1) { // no command line arguments
        printf("Usage: ./sm <value for n>\n"); 
        exit(1);
    } else if (argc == 2) {
        n = atoi(argv[1]);
    } 
    /* Initialize arrays to store preferences, intermediate matches, and final results */
	int seller_prefs[n][n];  // ith row is ith seller's preference list
	int buyer_prefs[n][n]; 
	int seller_next_choices[n];  // ith entry is ith seller's next choice of buyer 
	int buyer_final_prefs[n];   // for verification - ith entry is the ith buyer's ranking
	int seller_matches[n];  // ith entry is ith seller's matched buyer 
	int buyer_matches[n];   

    // Create array of numbers 1 to n to randomly shuffle to make each preference list 
    int randlist[n];
    for (int i = 0; i < n; i++) {
        randlist[i] = i;
    }
    // Create random seller's and buyer's preference lists
    for (int i = 0; i < n; i++) {
        shuffle_array(randlist, n);
        for (int j = 0; j < n; j++) {
            seller_prefs[i][j] = randlist[j];
        }
        shuffle_array(randlist, n);
        for (int j = 0; j < n; j++) {
            buyer_prefs[i][j] = randlist[j];
        }
    }
    // All sellers' next proposals are to their favorite buyers, since we haven't started
    // yet. No matches have been made, so initialize buyer_final_prefs and the match 
    // array entries to -1
    for (int i = 0; i < n; i++) {
        seller_next_choices[i] = 0;
        buyer_final_prefs[i] = -1;
        seller_matches[i] = -1; 
        buyer_matches[i] = -1;
    }

    /* Initialize variables for execution */
    int bachelor_count = n; // Number of unmatched sellers. We terminate when we get this to 0
    int curr_buyer = -1;    // ID # of buyer being proposed to 
    // Variables below used when comparing two competing sellers  
    int curr_seller_rank = -1;  // Rank of current seller on current buyer's pref list
    int other_seller = -1;  // ID # of seller already matched to buyer being proposed to
    int other_seller_rank = -1;  
    
    while(bachelor_count > 0) {
        for (int curr_seller = 0; curr_seller < n; curr_seller++) {   
            if (seller_matches[curr_seller] == -1) {    // For each unmatched seller, ... 
                curr_buyer = seller_prefs[curr_seller][seller_next_choices[curr_seller]];  // Get buyer to propose to next
                seller_next_choices[curr_seller]++;  // Update next buyer to propose to, if seller not matched this round 
                other_seller = buyer_matches[curr_buyer];   // if no competing seller, is -1
                // Get current seller's rank on this buyer's pref list
	            for (int j = 0; j < n; j++) {
	                if (buyer_prefs[curr_buyer][j] == curr_seller) {
	                    curr_seller_rank = j;
	                }
	            }
	
                /* No competing seller matched to this buyer already, so match this buyer
                   and seller. First, find current buyer's ranking of current seller and 
                   record; then, record the matching and decrease bachelor count.  */
                if (other_seller == -1) {  
	                buyer_final_prefs[curr_buyer] = curr_seller_rank;
                    buyer_matches[curr_buyer] = curr_seller;
                    seller_matches[curr_seller] = curr_buyer;
                    bachelor_count--;
                } else {    
                /* There is a competing seller. If current seller is ranked higher than
                   competitor, assign as new match to this buyer, and make former matched
                   seller open again  */
                    other_seller_rank = buyer_final_prefs[curr_buyer];
                    if (curr_seller_rank < other_seller_rank) {
                        buyer_final_prefs[curr_buyer] = curr_seller_rank;
                        buyer_matches[curr_buyer] = curr_seller;
                        seller_matches[curr_seller] = curr_buyer;
                        seller_matches[other_seller] = -1;
                    }
                }
            } // if bachelor is unmatched make a proposal
        } // for each bachelor, 
    } // while there are still bachelors

    // Print pref lists and results
    printf("Pref lists - sellers\n");
    for (int i = 0; i < n; i++) {
        printf("seller %d: ", i);
        for (int j = 0; j < n; j++) {
           printf("%d ", seller_prefs[i][j]); 
        }
        printf("\n");
    }
    printf("Pref lists - buyers\n");
    for (int i = 0; i < n; i++) {
        printf("buyer %d: ", i);
        for (int j = 0; j < n; j++) {
           printf("%d ", buyer_prefs[i][j]); 
        }
        printf("\n");
    }
    printf("Matches, ordered by both proposers and receivers.\n");
    for (int i = 0; i < n; i++) {
        printf("seller %d with buyer %d;    ", i, seller_matches[i]);
    }
    printf("\n");
        
    printf("Time taken: %d seconds \n", time(NULL) - start_time);
    return 0;
}

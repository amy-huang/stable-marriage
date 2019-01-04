# stable-marriage
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

  Compile with gcc -o sm random-stable-marriage-solver.c

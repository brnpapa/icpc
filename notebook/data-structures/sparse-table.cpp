/*
   For efficient range queries on a static array A of size N.
*/
#include <bits/stdc++.h>
using namespace std;
#define pow2(i) (1 << (i)) // 2^i

template <typename T = int>
class sparse_table {
 private:
   vector<T> A; int N;
   vector<vector<T>> table; // table[p][n]: answer for the range [n, n+2^p) (size 2^p) in A

   /* O(1) - change here to an "overlap friendly" function */
   T range_combination(T a, T b) {
      return min(a, b);
   }

   /* O(N * log(N)) - fill table with the answers for all ranges in A of size 2^p, for each non-negative integer p such that 2^p <= N */
   void build() {
      int P = (int)floor(log2(N)); // maior P, tal que 2^P <= N
      table.assign(P+1, vector<T>(N));

      for (int n = 0; n < N; n++) table[0][n] = A[n];
      
      for (int p = 1; p <= P; p++)
         for (int n = 0; n+pow2(p) <= N; n++) {
            // the answer for a range is the union of the answers for 2 sub-ranges (already computed)
            table[p][n] = range_combination(
               table[p-1][n],          // left range of size 2^(p-1)
               table[p-1][n+pow2(p-1)] // right range of size 2^(p-1)
            );
         }
   }

 public:
   sparse_table() {}
   sparse_table(vector<T> const &A) {
      this->A = A;
      N = (int)A.size();
      build();
   }

   /* O(1) - answer for range query [l, r] in A */
   int range_query(int l, int r) {
      int size = r-l+1;
      int p = (int)floor(log2(size)); // maior p, tal que 2^p <= size
      return range_combination(table[p][l], table[p][r-pow2(p)+1]);
   }
};

/* e.g */
int main() {
   sparse_table<long long> st({4,2,3,7,1,5,3,3,9,6,7,-1,4});
   cout << st.range_query(2,7) << endl;
   return 0;
}

// f is associative function, if: f(a, f(b,c)) = f(f(a,b), c) for all a,b,c
// f is "overlap friendly" function, if: f(f(a,b), f(c,d)) = f(a, f(b,c)) for all a,b,c,d

// se range_combination não for "overlap friendly", mas associativa (como sum), range_query deve ser reimplementado para uma versão O(log2(N)), continuar vendo em https://youtu.be/uUatD9AudXo?t=877
#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <random>
#include <algorithm>

using namespace std;

vector<double> M;
vector<int> position;

template <typename S>
ostream& operator<<(ostream& os,
                    const vector<S>& vector)
{
    // Printing all the elements
    // using <<
    for (auto element : vector) {
        os << element << " ";
    }
    return os;
}

void makeSketch(vector <pair<int,int>>vector1, int m1, int m2){

    for(int i=1; i<=m1; i++){
        M.push_back(numeric_limits<double>::infinity());
        position.push_back(0);
    }
    for(int i=1; i<=m2; i++) { // arrival of element (i, alfa_I)
        for (int k = 0; k < m1; k++) {
            hash<string> hash_obj;
            string binary = bitset<32>(vector1[i-1].first ).to_string()+bitset<32>(k+1).to_string();
            double U = hash_obj(binary );
            double E = - log(U / vector1[i-1].second);
            if ( E < M[k] ){
                M[k] = E;
                position[k] = i;
            }
           // M[k] = min((double) M[k], E);
        }
    }
    double MG;
    for(auto element: M){
        MG += element;
    }
    cout<<M<<endl;
    cout<<MG<<endl;
    cout<<position<<endl;
}


void fastExpSketch(vector <pair<int,int>>vector1, int m1,int m2){
    int permInit[m2];
    for(int i=0;i<m2;i++){
        permInit[i]=i+1; // permInit <- (1,2,3,...,m)
    }
    vector<double> M;
    vector<int> position;
    for(int i=1; i<=m1; i++){
        M.push_back(numeric_limits<double>::infinity());
    }
    double MAX = numeric_limits<double>::infinity();

    for(int i=1; i<=m2; i++) { // arrival of element (i, alfa_I)
        double S = 0;
        bool updateMax = false;
        int P[m2]; // p <- permInit do poporawki
        for(int i=0;i<m2;i++){
            P[i]=i; // permInit <- (1,2,3,...,m)
        }
        for (int k = 0; k < m1; k++) {
            hash<string> hash_obj;
            string binary = bitset<32>(vector1[i-1].first ).to_string()+bitset<32>(k+1).to_string();
            double U = hash_obj(binary );
            double E = - log(U / vector1[i-1].second);
            S = S + E / (m1 - k + 2);
            if ( S > MAX ){
                break;
            }
            std::mt19937 gen(vector1[i-1].first); // seed the generator
            std::uniform_int_distribution<> distr(k, m1);
            int r = distr(gen);
            swap(P[k], P[r]);
            int j = P[k];
            if ( M[j] == MAX) {
                updateMax = true;
            }
            if ( S < M[j] ){
                M[k] = S;
                position[k] = i;
            }
            //M[j] = min((double) M[j], S);
        }
        if(updateMax){
            MAX = *max_element(M.begin(), M.end());
        }
    }
    double MG;
    for(auto element: M){
        MG += element;
    }
    cout<<M<<endl;
    cout<<MG<<endl;
}

void printSketch(int m1, int m2){
    vector<double> W;
    for(int i=0;i<m1;i++){
        hash<string> hash_obj;
        string binary = bitset<32>(position[i]).to_string()+bitset<32>(i).to_string();
        double U = hash_obj(binary );
        W[i] = -log(U)/M[i];
    }
}

int main() {
    vector <pair<int,int>> vec;
    int m2=100;
    for(int i=1;i<=m2;i++){
        vec.push_back(make_pair(i,1));
    }
    makeSketch(vec,20,m2);

    //fastExpSketch(vec,20, m2);
    return 0;
}

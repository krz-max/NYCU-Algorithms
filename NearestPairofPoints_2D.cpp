#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// Divide and Conquer to solve Minimum distance between two points in 2D space
struct Restaurant{
    double x, y;
    int id;
    int rankX = -1;
    int rankY = -1;
    Restaurant(double x, double y) : x(x), y(y){}
    double distance(Restaurant& r){
        return sqrt((x - r.x) * (x - r.x) + (y - r.y) * (y - r.y));
    }
};
class Solution{
private:
    /* data */
    int numOfRestaurant;
    vector<Restaurant> restaurants;
public:
    Solution(int n) : numOfRestaurant(n){
        double x, y;
        for(int i = 0; i < n; i++){
            cin >> x >> y;
            restaurants.push_back(Restaurant(x, y));
            restaurants[i].id = i;
        }
    }
    // Divide and Conquer
    void solve(){
        // Sort by y-coordinate
        vector<Restaurant> temp = restaurants;
        sort(temp.begin(), temp.end(), [](Restaurant &a, Restaurant &b){
            return a.y < b.y;
        });
        vector<int> rankByYAxis(numOfRestaurant);
        for(int i = 0; i < numOfRestaurant; i++){
            restaurants[temp[i].id].rankY = i;
            rankByYAxis[i] = temp[i].id;
        }
        // Sort by x-coordinate
        sort(temp.begin(), temp.end(), [](Restaurant &a, Restaurant &b){
            return a.x < b.x;
        });
        vector<int> rankByXAxis(numOfRestaurant);
        for(int i = 0; i < numOfRestaurant; i++){
            restaurants[temp[i].id].rankX = i;
            rankByXAxis[i] = temp[i].id;
        }
        cout << fixed << setprecision(6) << minDistance(rankByXAxis, rankByYAxis, 0, numOfRestaurant - 1) << endl;
    }
    // [left, right] (inclusive)
    double minDistance(vector<int>& rankByXAxis, vector<int>& rankByYAxis, int left, int right){
        if(left >= right) return 1e9;
        // Only 2 restaurants
        if(right - left == 1){
            return restaurants[rankByXAxis[left]].distance(restaurants[rankByXAxis[right]]);
        }
        // Only 3 restaurants
        if(right - left == 2){
            double result = restaurants[rankByXAxis[left]].distance(restaurants[rankByXAxis[left + 1]]);
            result = min(result, restaurants[rankByXAxis[left]].distance(restaurants[rankByXAxis[right]]));
            result = min(result, restaurants[rankByXAxis[left + 1]].distance(restaurants[rankByXAxis[right]]));
            return result;
        }
        // More than 3 restaurants
        int mid = (left + right) / 2;
        // Divide
        double minDistanceLeft = minDistance(rankByXAxis, rankByYAxis, left, mid);
        double minDistanceRight = minDistance(rankByXAxis, rankByYAxis, mid, right);
        double result = min(minDistanceLeft, minDistanceRight);
        // Conquer
        double midX = restaurants[rankByXAxis[mid]].x;
        // Find the restaurants that are near the midX (X distance <= min(dL, dR))
        int nearIndexLeft = mid - 1, nearIndexRight = mid + 1;
        while(nearIndexLeft >= left && midX - restaurants[rankByXAxis[nearIndexLeft]].x <= result){
            nearIndexLeft--;
        }
        while(nearIndexRight <= right && restaurants[rankByXAxis[nearIndexRight]].x - midX <= result){
            nearIndexRight++;
        }
        // For all restaurants in [nearIndexLeft, nearIndexRight], check those restaurants that are near them (index in [rankY - 4, rankY + 4])
        // First find the restaurant's rankY in rankByYAxis
        for(int i = max(left, nearIndexLeft); i <= min(right, nearIndexRight); i++){
            int pos = restaurants[rankByXAxis[i]].rankY;
            // Check the restaurants that are near the restaurant i
            for(int j = max(0, pos - 4); j <= min(numOfRestaurant - 1, pos + 4); j++){
                if(j == pos) continue;
                result = min(result, restaurants[rankByXAxis[i]].distance(restaurants[rankByYAxis[j]]));
            }
        }
        return result;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    short T;
    cin >> T;
    while(T--){
        int n;
        cin >> n;
        Solution solution(n);
        solution.solve();
    }
    return 0;
}

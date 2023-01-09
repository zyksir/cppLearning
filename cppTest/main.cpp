#include<iostream>
#include<vector>
#include<deque>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    deque<int> q;
    for(int i = 0; i < k; ++i) {
        while(!q.empty() && nums[i] >= nums[q.back()]) {
            q.pop_back();
        }
        q.push_back(i);
    }
    vector<int> ans;
    ans.reserve(n - k + 1);
    ans.emplace_back(nums[q.front()]);
    for(int i = k; i < n; ++i) {
        while(!q.empty() && nums[i] >= nums[q.back()]) {
            q.pop_back();
        }
        while(!q.empty() && q.front() <= i - k) {
            q.pop_front();
        }
        q.push_back(i);
        ans.emplace_back(nums[q.front()]);
    }
    return ans;
}

int main() {
    return 0;
}
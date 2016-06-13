#include<iostream>
using namespace std;
#include<vector>

bool Find(vector<int> &v,int num)
{
    int len = v.size();
    for(int i = 0;i < len ;++i)
    {
        if(num == v[i])
        {
            return true;
        }
    }
        return false;
}
int main()
{
    int n,m;//m is the max page size,n is length of cin
    while(cin>>n>>m)
    {
        vector<int> Cache;
        vector<int> Page;
        int count = 0;
        for(int i = 0;i < m;++i)
        {
            cin>>Page[i];
        }
        for(int i =  0;i < m;++i)
        {
            if(find(Cache,Page[i]))
            {
                continue;
            }
            else
            {
             if(Cache.size() < n)
                {
                    Cache.push_back(Page[i]);
                }
                else{
                    Cache.erase(Cache.begin());
                    Cache.push_back(Page[i]);
                }
                ++count;
            }
        }
    cout<<count<<endl;
    }
    return 0; 
}

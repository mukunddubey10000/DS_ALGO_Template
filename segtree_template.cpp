#include <bits/stdc++.h>
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
#define ordered_set1 tree<int, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
// #define ordered_set1 tree<pair<int,int>, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
#define ordered_set2 tree<int, null_type,greater<int>, rb_tree_tag,tree_order_statistics_node_update>
#define int long long int
#define ld long double
using namespace std;
const int mod = 1e9 + 7;
const int mod2 = 8128812800000059;
struct custom_hash
{
    static uint64_t splitmix64(uint64_t x)
    {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const
    {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};
template<typename T>
ostream& operator<<(ostream& x, vector<T>& y)
{
    for (auto i : y)
        x << i << " ";
    return x;
}
template<typename T>
istream& operator>>(istream& x, vector<T>& y)
{
    for (T& i : y)
        cin >> i;
    return x;
}
//S1 is for without Lazy updates
class S1
{
    vector<int>Tree;
    //do operations only on indices [0,n-1]
public:
    S1(int n)
    {
        Tree.resize(4 * n + 1);
    }
    void build(int node, int l, int r, vector<int>& A)
    {
        if (l == r)
            Tree[node] = A[l];
        else
        {
            int mid = (l + r) / 2;
            build(2 * node, l, mid, A);
            build(2 * node + 1, mid + 1, r, A);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    void update(int node, int l, int r, int L, int R, int val)
    {
        if (l > R || r < L)
            return;
        if (L == l && r == R)
        {
            Tree[node] += val * (r - l + 1);
        }
        else
        {
            int mid = (l + r) / 2;
            update(2 * node, l, mid, L, min(mid, R), val);
            update(2 * node + 1, mid + 1, r, max(mid + 1, L), R, val);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    void update(int node, int l, int r, int pos, int val)
    {
        if (l == r)
            Tree[node] = val;
        else
        {
            int mid = (l + r) / 2;
            if (pos <= mid)
                update(2 * node, l, mid, pos, val);
            else
                update(2 * node + 1, mid + 1, r, pos, val);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    int query(int node, int l, int r, int pos)
    {
        if (l == r)
            return Tree[node];
        else
        {
            int mid = (l + r) / 2;
            if (pos <= mid)
                return query(2 * node, l, mid, pos);
            else
                return query(2 * node + 1, mid + 1, r, pos);
        }
    }
    int query(int node, int l, int r, int L, int R)
    {
        if (l > R || r < L)
            return 0;
        if (L == l && R == r)
            return Tree[node];
        else
        {
            int mid = (l + r) / 2;
            int q1 = query(2 * node, l, mid, L, min(R, mid));
            int q2 = query(2 * node + 1, mid + 1, r, max(mid + 1, L), R);
            return q1 + q2;
        }
    }
};
//S2 is for with Lazy updates
class S2
{
    vector<int>Tree, Lazy;
    //do operations only on indices [0,n-1]
public:
    S2(int n)
    {
        Tree.resize(4 * n + 1);
        Lazy.resize(4 * n + 1);
        // memset(Lazy, val, 4 * n + 4);

        //might give TLE 
    }
    void build(int node, int l, int r, vector<int>& A)
    {
        if (l == r)
            Tree[node] = A[l];
        else
        {
            int mid = (l + r) / 2;
            build(2 * node, l, mid, A);
            build(2 * node + 1, mid + 1, r, A);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    void update(int node, int l, int r, int L, int R, int val)
    {
        lazy(node, l, r);
        if (l > R || r < L)
            return;
        if (L == l && r == R)
        {
            Tree[node] += val * (r - l + 1);
            if (l != r)
                Lazy[2 * node] += val,
                Lazy[2 * node + 1] += val;
        }
        else
        {
            int mid = (l + r) / 2;
            update(2 * node, l, mid, L, min(mid, R), val);
            update(2 * node + 1, mid + 1, r, max(mid + 1, L), R, val);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    void update(int node, int l, int r, int pos, int val)
    {
        lazy(node, l, r);
        if (l == r)
            Tree[node] = val;
        else
        {
            int mid = (l + r) / 2;
            if (pos <= mid)
                update(2 * node, l, mid, pos, val);
            else
                update(2 * node + 1, mid + 1, r, pos, val);
            Tree[node] = Tree[2 * node] + Tree[2 * node + 1];
        }
    }
    int query(int node, int l, int r, int pos)
    {
        lazy(node, l, r);
        if (l == r)
            return Tree[node];
        else
        {
            int mid = (l + r) / 2;
            if (pos <= mid)
                return query(2 * node, l, mid, pos);
            else
                return query(2 * node + 1, mid + 1, r, pos);
        }
    }
    int query(int node, int l, int r, int L, int R)
    {
        lazy(node, l, r);
        if (l > R || r < L)
            return 0;
        if (L == l && R == r)
            return Tree[node];
        else
        {
            int mid = (l + r) / 2;
            return query(2 * node, l, mid, L, min(R, mid)) + query(2 * node + 1, mid + 1, r, max(mid + 1, L), R);
        }
    }
    void lazy(int node, int l, int r)
    {
        if (Lazy[node] == 0)
            return;
        Tree[node] += Lazy[node] * (r - l + 1);
        if (l != r)
            Lazy[2 * node] += Lazy[node],
            Lazy[2 * node + 1] += Lazy[node];
        Lazy[node] = 0;
    }
};
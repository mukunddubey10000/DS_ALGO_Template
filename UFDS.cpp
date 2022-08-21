//basic DSU without rollback
class DSU
{
public:
    vector<int>parent, rank;
    DSU(int n)
    {
        parent.resize(n + 1);
        rank.resize(n + 1);
        for (int i = 0;i <= n;i++)
            parent[i] = i, rank[i] = 1;
    }
    int Find(int node)
    {
        //path compression
        if (node == parent[node])
            return node;
        return parent[node] = Find(parent[node]);
    }
    void Join(int a, int b)
    {
        //rank compression
        if (rank[a] < rank[b])
            rank[b] += rank[a],
            parent[a] = b;
        else
            rank[a] += rank[b],
            parent[b] = a;
    }
};
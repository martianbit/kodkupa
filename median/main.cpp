#include <cstdint>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <algorithm>

#define MAX_N 50000

#define EXPLORE(cn, ln, paths) \
    initial_path.node = edge.ln; \
    paths.reserve(N - 1); \
    explore(edge.cn, edge.weight, edge.weight, initial_path, &paths); \
    paths.shrink_to_fit();

using namespace std;

struct Node
{
    uint16_t parent;
    uint32_t parent_weight;
    vector<uint16_t> children;
    vector<uint32_t> child_weights;
};

struct Edge
{
    uint32_t weight;
    uint16_t n;
    uint16_t m;
};

struct Path
{
    uint16_t node;
    uint16_t smaller_count;
    uint16_t exact_count;
    uint16_t bigger_count;
};

uint32_t k;
unordered_set<uint32_t> done;
struct Path initial_path;
struct Node tree[MAX_N + 1];

bool shall_step(uint16_t last_node, uint16_t next_node)
{
    return (last_node != next_node && next_node);
}

void explore(uint16_t current_node, uint32_t weight, uint32_t exact_value, struct Path last_path, vector<struct Path>* paths)
{
    uint16_t i;
    struct Path path;

    path = last_path;
    path.node = current_node;

    if(weight < exact_value)
        path.smaller_count++;
    else if(weight > exact_value)
        path.bigger_count++;
    else
        path.exact_count++;

    paths->push_back(path);

    if(shall_step(last_path.node, tree[current_node].parent))
        explore(tree[current_node].parent, tree[current_node].parent_weight, exact_value, path, paths);

    for(i = 0U; i < tree[current_node].children.size(); i++)
    {
        if(shall_step(last_path.node, tree[current_node].children[i]))
            explore(tree[current_node].children[i], tree[current_node].child_weights[i], exact_value, path, paths);
    }
}

void rec(struct Edge edge, uint16_t N)
{
    uint16_t diff, sc, ec, bc;
    uint32_t done_key;
    vector<struct Path> paths_1;
    vector<struct Path> paths_2;

    EXPLORE(n, m, paths_1);
    EXPLORE(m, n, paths_2);

    for(auto x : paths_1)
    {
        for(auto y : paths_2)
        {
            if(x.node == y.node)
                continue;

            done_key = (((uint32_t) min(x.node, y.node)) << 16) |
                       ((uint32_t) max(x.node, y.node));

            if(done.find(done_key) == done.end())
            {
                sc = x.smaller_count + y.smaller_count;
                ec = x.exact_count + y.exact_count - 1U;
                bc = x.bigger_count + y.bigger_count;

                diff = (uint16_t) abs(((int32_t) sc) - ((int32_t) bc));

                if(ec > diff || (bc > sc && ec == diff))
                {
                    k++;
                    done.insert(done_key);
                }
            }
        }
    }
}

int main()
{
    uint16_t N;
    uint32_t i, K;
    struct Edge edge;
    vector<struct Edge> edges;

    scanf("%hu %u", &N, &K);

    edges.reserve(N - 1);

    for(i = 1U; i < N; i++)
    {
        scanf("%hu %hu %u", &edge.n, &edge.m, &edge.weight);

        edges.push_back(edge);

        tree[edge.n].children.push_back(edge.m);
        tree[edge.n].child_weights.push_back(edge.weight);

        tree[edge.m].parent = edge.n;
        tree[edge.m].parent_weight = edge.weight;
    }

    sort(edges.begin(), edges.end(), [](auto x, auto y) {
        return x.weight < y.weight;
    });

    for(auto x : edges)
    {
        rec(x, N);

        if(k >= K)
        {
            printf("%u\n", x.weight);
            break;
        }
    }

    return 0;
}


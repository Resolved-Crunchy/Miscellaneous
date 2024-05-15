/* 
    Coursera C++ coding for everyone part 3, kruskal's algorithm based on implementation found here: https://www.geeksforgeeks.org/kruskals-algorithm-simple-implementation-for-adjacency-matrix/ 
    Dijkstra's algorithm based on the implentation found here: https://www.tutorialspoint.com/cplusplus-program-for-dijkstra-s-shortest-path-algorithm
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map> 

#define INFINITY 99999; //Inifinity value

class dstra //object wherein we will be creating our matrices and executing our algorithm (Dijkstra's algo)
{
public:
    dstra(){} //default constructor
    void driver_func() //driver function, like a main function away from main
    {
        bool check;
        init_mats();
        fill_randmat();
        fill_adjmat();
        std::cout << "Print adjacency matrix? (Type 0 for no, and 1 for yes) ";
        std::cin >> check;
        if (check == true)
        {
            print_int_mat(distmat);
        }
        fill_costmat();
        std::cout << "Use Dijkstra's algorithm to calculate the average path length for all vertices? (Type 0 for no, and 1 for yes) ";
        std::cin >> check;
        if (check == true)
        {
            ds_algo(0); //Get average of shortest path for all vertices starting from our scource as per assignment parameters
            double avg = average_pathlen();
            std::cout << "Average path length at density: " << dens << " = " << avg << std::endl;
        }
        std::cout << "Thank you!" << std::endl;
        std::cout << std::endl;
    }

    void set_densities(int ds, int sz) //set size and density from main
    {
        dens = ds;
        siz = sz;
    }

    void fill_randmat() //Fill a random 2D matrix for scoring
    {
        for (int i = 0; i < siz; i++)
        {
            for (int j = 0; j < siz; j++)
            {
                int r = rand() % 100;
                randmat[i][j] = r;
            }
        }
    }

    void print_vec(std::vector<int> vec) //debugging function to check if our distance vector filled correctly
    {
        for (unsigned int i = 0; i < vec.size(); i++)
        {
            std::cout << vec[i] << "  ";
        }
        std::cout << std::endl;
    }

    double average_pathlen() //returns our average path length as a double
    {
        double tot = dstradists.size();
        int sum = 0;
        for (unsigned int i = 0; i < dstradists.size(); i++)
        {
            sum += dstradists[i];
        }
        double dubsum = sum;
        return dubsum / tot;
    }

    void fill_adjmat() //Fill adjacency matrix with random values
    {
        for (int i = 0; i < siz; i++)
        {
            for (int j = 0; j < siz; j++)
            {
                if (randmat[i][j] <= dens && i != j && adjmat[i][j] == false) //i != j added here to avoid self loops, adjmat check added to avoid overwriting
                {
                    adjmat[i][j] = true;
                    adjmat[j][i] = true;
                    int r = rand() % 10 + 1;
                    distmat[i][j] = r;
                    distmat[j][i] = r;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    void ds_algo(int src) //Shortest path algorithm, takes a source node as an int argument and returns the summed min distance
    {
        int visited[siz], distance[siz];
        int count, mindist, nextnode;
        for (int i = 0; i < siz; i++)
        {
            distance[i] = costmat[src][i];
            visited[i] = 0;
        }
        distance[src] = 0;
        visited[src] = 1;
        count = 1;
        while (count < siz - 1)
        {
            mindist = INFINITY;
            for (int j = 0; j < siz; j++)
            {
                if (distance[j] < mindist && !visited[j])
                {
                    mindist = distance[j];
                    nextnode = j;
                }
            }
            visited[nextnode] = 1;
            for (int i = 0; i < siz; i++)
            {
                if (!visited[i])
                {
                    if (mindist + costmat[nextnode][i] < distance[i])
                    {
                        distance[i] = mindist + costmat[nextnode][i];
                    }
                    count++;
                }
            }
        }
        for (int i = 0; i < siz; i++) //Push back all valid distance values into our dstradists vector
        {
            if (i != src && distance[i] < 99999) //Idk why but it gave me an error when I used infinity here
            {
                dstradists.push_back(distance[i]);
            }
        }
    }

    void fill_costmat() //Fill cost matrix based on distance matrix
    {
        for (int i = 0; i < siz; i++)
        {
            for (int j = 0; j < siz; j++)
            {
                if (distmat[i][j] == 0)
                {
                    costmat[i][j] = INFINITY;
                }
                else
                {
                    costmat[i][j] = distmat[i][j];
                }
            }
        }
    }

    void init_mats() //Initialize all matrices to be filled with 0's
    {
        std::vector<int> ev;
        std::vector<bool> bv;
        for (int i = 0; i < siz; i++)
        {
            adjmat.push_back(bv);
            distmat.push_back(ev);
            randmat.push_back(ev);
            costmat.push_back(ev);
            for (int j = 0; j < siz; j++)
            {
                adjmat[i].push_back(false);
                randmat[i].push_back(0);
                distmat[i].push_back(0);
                costmat[i].push_back(0);
            }
        }
    }

    void print_int_mat(std::vector<std::vector<int>> vec) //function to print a vector of vector ints
    {
        for (unsigned int i = 0; i < vec.size(); i++)
        {
            for (unsigned int j = 0; j < vec[i].size(); j++)
            {
                std::cout << vec[i][j] << ", ";
            }
            std::cout << std::endl;
        }
    }

private:
    int dens, siz; //density and size (Number of nodes) respectively 
    std::vector<int> dstradists; //Vector to store shortest path scores for node 1-50. Scores to be averaged
    std::vector<std::vector<bool>> adjmat; //Adjacency matrix implemented as a 2D vector of bools, just tells us connectivity vs non
    std::vector<std::vector<int>> randmat, distmat, costmat, dstramat; //2D vector of random integers to be used to generate our adjacency matrix, distance matrix
};

class kruska //object wherein we will be creating an adjacency matrix and performing kruskal's algorithm
{
public:
    kruska() {} //default constructor
    void setup_vec(std::vector<int> ivec) //send input from main into a vector of vectors, each containing adjacency data
    {
        siz = ivec[0]; //vector size (number of nodes)
        ivec.erase(ivec.begin()); //erase first (size) int
        for (unsigned int i = 0; i < ivec.size() - 2; i += 3) //reads in sections of three and pushes them into a vector, before then pushing that vector into another vector
        {
            std::vector<int> vc;
            vc.push_back(ivec[i]);
            vc.push_back(ivec[i + 1]);
            vc.push_back(ivec[i + 2]);
            buildvec.push_back(vc);
        }
    }

    void driver_func() //like a main function away from main. Prevents clutter in main and can be easily indexed for debugging purposes
    {
        build_bmap();
        build_conmat();
        std::cout << "Ok, printing minimum spanning tree: " << std::endl; 
        kruskal_algo();
    }

    void build_bmap() //we need to account for values not existing in our input file, therefore we will use a map structure to store real coordinates and edges as key/value pair
    {
        for (unsigned int i = 0; i < buildvec.size(); i++)
        {
            std::pair<int, int> coordpair(buildvec[i][0], buildvec[i][1]); //make int pair object out of coordinate pair for each adjacency pair in the input file to use as key
            buildmap.insert(std::make_pair(coordpair, buildvec[i][2])); //insert coordpair object and the cost (buildvec[i][2]) as a key/value pair 
        }
    }

    void build_conmat() //Build a connectivity matrix of siz x siz, pushing cost from the map when found and 99999 (INFINITY value) when not
    {
        for (int i = 0; i < siz; i++)
        {
            std::vector<int> cvec;
            conmat.push_back(cvec);
            for (int j = 0; j < siz; j++)
            {
                std::pair<int, int> cpair(i, j);
                if (buildmap.find(cpair) == buildmap.end())
                {
                    conmat[i].push_back(99999);
                }
                else
                {
                    int val = buildmap[cpair];
                    conmat[i].push_back(val);
                }
            }
        }
    }

    int find(int i) //Find set of vertex i
    {
        while (parent[i] != i)
        {
            i = parent[i];
        }
        return i;
    }

    void union1(int i, int j) //Does union of i and j
    {
        int a = find(i);
        int b = find(j);
        parent[a] = b;
    }

    void kruskal_algo() //uses costmat in order to implement kruskal's algorithm
    {
        int mincost = 0;
        for (int i = 0; i < siz; i++) //Initialize sets of disjoint sets
        {
            parent.push_back(i);
        }
        int edge_count = 0;
        while (edge_count < siz - 1)
        {
            int min = 99999, a = -1, b = -1;
            for (int i = 0; i < siz; i++)
            {
                for (int j = 0; j < siz; j++)
                {
                    if (find(i) != find(j) && conmat[i][j] < min)
                    {
                        min = conmat[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
            union1(a, b);
            printf("Edge %d: (%d, %d) cost:%d \n", edge_count++, a, b, min);
            mincost += min;
        }
        std::cout << "Minimum cost is " << mincost << std::endl;
    }

    void print_vecovec(std::vector<std::vector<int>> vec) //debugging function, prints contents of 2D vector of ints 
    {
        for (unsigned int i = 0; i < vec.size(); i++)
        {
            for (unsigned int j = 0; j < vec[i].size(); j++)
            {
                std::cout << vec[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<int>> conmat, buildvec; //connectivity matrix, vector used to build conmat from input file data
    std::vector<int> parent;
    std::map<std::pair<int, int>, int> buildmap; //map structure to hold items from our buildvec as key/value pairs
    int siz;
};

int main()
{
    bool check;
    std::cout << "Running program for Dijkstra (0) or Kruskal's algorithm? (1): ";
    std::cin >> check;
    if (check == false)
    {
        srand(time(NULL)); //seed our pseudorandom number generator
        dstra twenty, forty;
        twenty.set_densities(20, 50);
        twenty.driver_func();
        forty.set_densities(40, 50);
        forty.driver_func(); //driver_func() member function is where each of our steps of the algorithm are organized
    }
    else 
    {
        std::string fstr;
        std::cout << "Ok, please provide a file name (.txt format): ";
        std::cin >> fstr;
        std::ifstream ifile(fstr); 
        if (ifile.is_open())
        {
            int value;
            std::vector<int> intvec;
            while (ifile >> value)
            {
                intvec.push_back(value);
            }
            kruska kska;
            kska.setup_vec(intvec);
            kska.driver_func();
        }
        else
        {
            std::cout << "Error with the file, terminating";
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
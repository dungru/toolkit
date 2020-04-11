#include <stdio.h>
#include <stdbool.h>
#include <iostream>

class Quick_Union
{
    private:
        int *id;
        int *sz;
        int element_num;
        int num_components;
    public:

        Quick_Union(int num) {
            element_num = num;
            id = new int(element_num);
            sz = new int(element_num);
            num_components = element_num;
            for (int i = 0; i < element_num; ++i) {
                id[i] = i;
                sz[i] = 1;
            }
        };
        ~Quick_Union() {

        };

        int find_root(int i) {
            int root = i;
            while (root != id[root]) {
                root = id[root];
            }
            //Path compress
            while (i != root) {
                int next = id[i];
                id[i] = root;
                i = next;
            }

            return root;
        };

        int components() {
            return num_components;
        }

        bool connected(int num1, int num2) {
            return find_root(num1) == find_root(num2);
        };

        void unify(int pp, int qq) {
            int root1 = find_root(pp);
            int root2 = find_root(qq);
            if(root1 == root2)
                return;
            if(sz[root1] < sz[root2]) {
                sz[root2] += sz[root1];
                id[root1] = root2;
            } else {
                sz[root1] += sz[root2];
                id[root2] = root1;
            }
            num_components--;
        };

        void dump_info() {
            int ii;
            for (ii = 0; ii < element_num; ++ii) {
                printf("[%d]=%d\n", ii, id[ii]);
            }
        };
    
};

int main(int argc, char const *argv[])
{
    Quick_Union union_find(9);
    printf("%d \n", union_find.components());
    union_find.unify(1,2);
    union_find.unify(3,2);
    union_find.unify(8,2);
    union_find.unify(7,6);
    union_find.unify(4,3);
    union_find.unify(5,9);
    printf("%d \n", union_find.components());
    union_find.dump_info();
    return 0;
}
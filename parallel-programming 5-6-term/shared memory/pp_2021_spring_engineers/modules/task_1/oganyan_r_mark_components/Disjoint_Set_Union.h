// Copyright 2021 Oganyan Robert

#ifndef MODULES_TASK_2_OGANYAN_R_MARK_COMPONENTS_OMP_DISJOINT_SET_UNION_H_
#define MODULES_TASK_2_OGANYAN_R_MARK_COMPONENTS_OMP_DISJOINT_SET_UNION_H_


#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range2d.h>
#include <tbb/task_scheduler_init.h>
#include "../../3rdparty/unapproved/unapproved.h"
#include <omp.h>

template<typename T>
class Disjoint_Set_Union {
private:
    std::vector<T> rank;
    std::vector<T> parent;

public:
    explicit Disjoint_Set_Union(int size) : rank(size), parent(size) {
    }

    void make_set(int vertex) {
        parent[vertex] = vertex;
        rank[vertex] = 0;
    }


    void Init() {
        for (std::size_t vertex = 0; vertex < rank.size(); ++vertex) {
            make_set(vertex);
        }
    }

    void InitOmp(int num_proc) {
        omp_set_num_threads(num_proc);
#pragma omp parallel default(none) shared(parent, rank)
        {
#pragma omp for schedule(static)
            for (std::size_t vertex = 0; vertex < rank.size(); ++vertex) {
                make_set(vertex);
            }
        }
        return;
    }

    void InitTbb(int num_proc) {
        tbb::task_scheduler_init init(num_proc);
        tbb::parallel_for(tbb::blocked_range<int>(0, rank.size(), rank.size() / num_proc + 1),
                          [&](tbb::blocked_range<int> block) {
                              for (auto vertex = block.begin(); vertex != block.end(); ++vertex) {
                                  make_set(vertex);
                              }
                          });

        init.terminate();
    }

    void InitStd(int num_proc) {
        std::vector<std::thread> threads;
        threads.reserve(num_proc);
        int div = rank.size() / num_proc;
        int mod = rank.size() % num_proc;

        int last = 0;


        for (int proc = 0; proc < num_proc; ++proc) {
            threads.emplace_back([&, proc, div, mod, last]() {
                int balance = (proc < mod) ? 1 : 0;
                for (int vertex = last; vertex < last + div + balance; ++vertex) {
                    make_set(vertex);
                }
            });
            last += div + ((proc < mod) ? 1 : 0);
        }

        for (auto &thread : threads) {
            thread.join();
        }

    }

    /*
    void Init(int size) {
        rank.resize(size);
        parent.resize(size);
        for (int vertex = 0; vertex < size; ++vertex) {
            make_set(vertex);
        }
    }
    */

    int find_set(int vertex) {
        //  Bug here (parent[vertex] = [parent[paren[vertex]])
        if (vertex == parent[vertex]) {
            return vertex;
        }

        return parent[vertex] = find_set(parent[vertex]);
    }

    int find_set(int vertex, int last) {
        if (vertex == parent[vertex]) {
            return vertex;
        }

        if (last == parent[vertex]) {
            return std::min(last, vertex);
        }

        return parent[vertex] = find_set(parent[vertex], vertex);
    }

    void union_sets(int fi_union, int se_union) {
        fi_union = find_set(fi_union, fi_union);
        se_union = find_set(se_union, se_union);
        if (fi_union != se_union) {
            if (rank[fi_union] < rank[se_union])
                std::swap(fi_union, se_union);
            parent[se_union] = fi_union;
            if (rank[fi_union] == rank[se_union])
                ++rank[fi_union];
        }
    }

    const std::vector<T> get_rank() {
        return this->rank;
    }

    const std::vector<T> get_parent() {
        return this->parent;
    }
};

#endif  //  MODULES_TASK_2_OGANYAN_R_MARK_COMPONENTS_OMP_DISJOINT_SET_UNION_H_

#ifndef MERGED_HEAP
#define MERGED_HEAP

#pragma once
#include<cmath>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<exception>
#include<stdexcept>
#include<deque>
//#include<vld.h>

template<class T>
class MergedHeap
{
public:
    virtual void merge(MergedHeap<T> &h) = 0;
    virtual void insert(T &obj) = 0;
    virtual T extractMin() = 0;
    virtual ~MergedHeap()
    {}
};

template<class T>
class UncommonHeap : public MergedHeap<T>
{
public:
    UncommonHeap()
    {
        node_ = NULL;
    }

    UncommonHeap(T &obj)
    {
        node_ = new TNode(obj);
    }

    void merge(MergedHeap<T> &mHeap)
    {
        UncommonHeap<T> &lHeap = dynamic_cast<UncommonHeap<T>&>(mHeap);
        if (node_ == NULL)
        {
            node_ = lHeap.node_;
            lHeap.node_ = NULL;
            return;
        }
        if (lHeap.node_ == NULL)
        {
            return;
        }
        node_ = mergeTree(node_, lHeap.node_);
        lHeap.node_ = NULL;
    }

    virtual void insert(T &obj) = 0;

    T extractMin()
    {
        if (node_ == NULL)
            throw std::runtime_error("Can't extract from empty heap");
        T ans = node_->key;
        TNode* left = node_->left;
        TNode* right = node_->right;
        delete node_;
        node_ = mergeTree(left, right);
        return ans;
    }

    ~UncommonHeap()
    {
        if (node_ == NULL)
            return;
        std::deque<TNode*> bfs;
        bfs.push_back(node_);
        while (bfs.size() != 0) {
            TNode* tmp = bfs.front();
            bfs.pop_front();
            if (tmp->right != NULL)
                bfs.push_back(tmp->right);
            if (tmp->left != NULL)
                bfs.push_back(tmp->left);
            delete tmp;
        }
    }

protected:
    class TNode
    {
    public:
        TNode()
        {
            left = right = NULL;
            dist = 0;
        }
        TNode(T &obj)
        {
            key = obj;
            dist = 0;
            left = right = NULL;
        }
        T key;
        int dist;
        TNode *left, *right;
    };
    virtual TNode* mergeTree(TNode *left, TNode *right) = 0;
    TNode* node_;
};

template<class T>
class LeftHeap : public UncommonHeap<T>
{
public:
    LeftHeap()
        : UncommonHeap<T>()
    { }

    LeftHeap(T &obj)
        : UncommonHeap<T>(obj)
    { }

    void insert(T &obj)
    {
        LeftHeap<T> tmp = LeftHeap<T>(obj);
        UncommonHeap<T>::merge(tmp);
    }
private:
    int dist(typename UncommonHeap<T>::TNode *node)
    {
        if (node == NULL)
            return -1;
        else
            return node->dist;
    }

    typename UncommonHeap<T>::TNode* mergeTree(typename UncommonHeap<T>::TNode *one, typename UncommonHeap<T>::TNode *two)
    {
        if (one == NULL)
            return two;
        if (two == NULL)
            return one;
        if (one->key > two->key)
            std::swap(one, two);
        one->right = mergeTree(one->right, two);
        if (dist(one->right) > dist(one->left))
        {
            std::swap(one->right, one->left);
        }
        one->dist = std::min(dist(one->left), dist(one->right)) + 1;
        return one;
    }
};

template<class T>
class SkewHeap : public UncommonHeap<T>
{
public:
    SkewHeap()
        : UncommonHeap<T>()
    { }

    SkewHeap(T &obj)
        : UncommonHeap<T>(obj)
    { }

    void insert(T &obj)
    {
        SkewHeap<T> tmp = SkewHeap<T>(obj);
        UncommonHeap<T>::merge(tmp);
    }
private:
    typename UncommonHeap<T>::TNode* mergeTree(typename UncommonHeap<T>::TNode *one, typename UncommonHeap<T>::TNode *two)
    {
        if (one == NULL)
            return two;
        if (two == NULL)
            return one;
        if (one->key > two->key)
            std::swap(one, two);
        one->right = mergeTree(one->right, two);
        std::swap(one->right, one->left);
        return one;
    }
};

template<class T>
class BinominalHeap : public MergedHeap<T>
{
public:
    BinominalHeap()
    {
        heap_.clear();
    }

    BinominalHeap(T &obj)
    {
        heap_.clear();
        TNode *tmp = new TNode(obj);
        heap_.push_back(tmp);
    }

    void merge(MergedHeap<T> &mHeap)
    {
        BinominalHeap<T> &secondHeap = dynamic_cast<BinominalHeap<T>&>(mHeap);
        if (secondHeap.heap_.size() == 0)
            return;
        if (heap_.size() == 0)
        {
            heap_ = secondHeap.heap_;
            secondHeap.heap_.clear();
            return;
        }
        TNode *p = NULL;
        for (size_t i = 0; i < std::max(heap_.size(), secondHeap.heap_.size()); ++i) {
            if (heap_.size() <= i)
            {
                heap_.push_back(NULL);
            }
            if (secondHeap.heap_.size() <= i)
            {
                secondHeap.heap_.push_back(NULL);
            }
            TNode *tmp = mergeTree_(heap_[i], secondHeap.heap_[i]);
            if (degree_(tmp) > degree_(p))
            {
                if (degree_(tmp) == i)
                {
                    heap_[i] = tmp;
                }
                else
                {
                    heap_[i] = p;
                    p = tmp;
                }
            }
            else
            {
                if (degree_(tmp) < degree_(p))
                {
                    heap_[i] = p;
                    p = NULL;
                }
                else
                {
                    heap_[i] = NULL;
                    p = mergeTree_(p, tmp);
                }
            }
        }
        if (p != NULL)
        {
            heap_.push_back(p);
        }
        secondHeap.heap_.clear();
    }

    void insert(T &obj)
    {
        BinominalHeap<T> tmp = BinominalHeap<T>(obj);
        merge(tmp);
    }

    T extractMin()
    {
        if(heap_.size() == 0)
            throw std::runtime_error("Can't extract from empty heap");
        size_t i = 0;
        while (heap_[i] == NULL) {
            ++i;
        }
        T min = heap_[i]->key_;
        size_t position = i;
        for (i = position; i < heap_.size(); ++i) {
            if (heap_[i] != NULL && heap_[i]->key_ < min)
            {
                min = heap_[i]->key_;
                position = i;
            }
        }
        TNode *tmp = heap_[position];
        heap_[position] = NULL;
        BinominalHeap<T> sons = BinominalHeap<T>();
        sons.heap_.resize(tmp->degree_);
        TNode *son = tmp->son_;
        for (i = 0; i < tmp->degree_; ++i)
        {
            sons.heap_[son->degree_] = son;
            son = son->brother_;
        }
        delete tmp;
        merge(sons);
        if (heap_.size() != 0 && heap_[heap_.size() - 1] == NULL)
            heap_.pop_back();
        return min;
    }

    ~BinominalHeap()
    {
        if (heap_.size() == 0)
            return;
        for (int i = 0; i < heap_.size(); ++i) {
            if (heap_[i] == NULL)
                continue;
            std::deque<TNode*> bfs;
            bfs.push_back(heap_[i]);
            while (bfs.size() != 0) {
                TNode* tmp = bfs.front();
                bfs.pop_front();
                TNode* son = tmp->son_;
                for (size_t j = 0; j < tmp->degree_; ++j) {
                    bfs.push_back(son);
                    son = son->brother_;
                }
                delete tmp;
            }
        }
        heap_.clear();
    }
private:
    class TNode
    {
    public:
        TNode()
        {
            parent_ = brother_ = son_ = NULL;
            degree_ = 0;
        }
        TNode(T &obj)
        {
            key_ = obj;
            degree_ = 0;
            parent_ = brother_ = son_ = NULL;
        }
        T key_;
        size_t degree_;
        TNode *parent_, *brother_, *son_;
    };
    TNode* mergeTree_(TNode *left, TNode *right)
    {
        if (right == NULL)
            return left;
        if (left == NULL)
            return right;
        if (right->key_ < left->key_)
        {
            std::swap(left, right);
        }
        right->parent_ = left;
        right->brother_ = left->son_;
        left->son_ = right;
        ++left->degree_;
        return left;
    }
    int degree_(TNode *node)
    {
        if (node == NULL)
            return -1;
        else
            return node->degree_;
    }
    std::vector<TNode*> heap_;
};

#endif // MERGED_HEAP


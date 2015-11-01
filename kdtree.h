/*
 * kdtree.h
 *
 *  Created on: 2015. okt. 26.
 *      Author: csabi
 */

#ifndef KDTREE_H_
#define KDTREE_H_
#include <vector>
#include <iostream>
#include <iomanip>
#include <utility>
#include <stack>
#include <utility>
#include <queue>
#include <vector>
#include "exceptions.hpp"
using namespace std;


template <size_t K, class KeyType, class ValType>
class kdtree
{
	size_t dimension = K;
	struct node
	{
		node* parent= NULL;
		node* left 	= NULL;
		node* right = NULL;
		size_t split_index;
		vector<KeyType> key;
		ValType val;
		bool operator < (const node& r_val) const;
		bool is_leaf() {return !(left || right);}
		node(const vector<KeyType>& coords): split_index(0), key(coords) {}
		node(const size_t& ind, const vector<KeyType>& coords, const ValType& val):
			split_index(ind), key(coords), val(val){};
	};

	const KeyType sq_distance(const node& x, const node& y) const;
	ostream& _postorder(ostream& o, node* p, int indent=0) const;

	node* root = NULL;
public:

	mutable size_t height = 0;

	class iterator
	{
		node * n;
	public:
		iterator(node* n): n(n) {}
		iterator(const iterator & it) : n(it.n) {}
		const vector<KeyType>& key() const{return n->key;} //modification is not available
		ValType val()	{return n->val;}	//modification doesn't yield invalid tree structure
	};


	void insert(const vector<KeyType>& key, const ValType& val);
	ostream& print(ostream& o = std::cout) const {_postorder(o, root); return o;}
	iterator nearest_neighbour(const vector<KeyType>& test_point, ValType val)const;
	vector<iterator> n_nearest_neighbour( const size_t& n, const vector<KeyType>& test_point, ValType val)const;


};

template <size_t K, class KeyType, class ValType>
const KeyType kdtree<K,KeyType,ValType>::sq_distance(const node& X, const node& Y) const
{
	/*
	 * This pulls KeyType must be 'scalars' to span over a normal vector space
	 */
	if(X.key.size()!=Y.key.size()) throw invalid_dimension();
	KeyType d=0;
	for (size_t i = 0; i < X.key.size(); i++){
		//cout<<X.val<<'\t'<<X.key[i]<<'\t'<<Y.val<<'\t'<<Y.key[i]<<'\n';
		d+=(X.key[i]-Y.key[i])*(X.key[i]-Y.key[i]);
	}

	//cout<<X.val<<'\t'<<Y.val<<'\t'<<d<<"\n\n";

	return d;
}

template <size_t K, class KeyType, class ValType>
ostream& kdtree<K,KeyType,ValType>::_postorder (ostream& o, node* p, int indent) const
{
	if(p != NULL) {
		if(p->right) {
			_postorder(o, p->right, indent+3*K);
		}
		if (indent) {
			o << std::setw(indent) << ' ';
		}
		if (p->right) o<<" /\n" << std::setw(indent) << ' ';
		o << '(';
		for(auto& i : p->key) o << i <<";";
		o << "\b \b)" << "->" << p->val << "\n";

		if(p->left) {
			o << std::setw(indent) << ' ' <<" \\\n";
			_postorder(o, p->left, indent+3*K);
		}
	}
	return o;
}

template <size_t K, class KeyType, class ValType>
bool kdtree<K,KeyType,ValType>::node::operator < (const node& r) const
{
	return key[r.split_index]<r.key[r.split_index];
}


template <size_t K, class KeyType, class ValType>
void kdtree<K,KeyType,ValType>::insert(const vector<KeyType>& key, const ValType& val)
{
	if(key.size() != K)
		throw invalid_dimension();

	auto x 	= root;
	node* par= NULL;
	auto i= new node(0, key, val);

	size_t depth=1;

	while (x != NULL)
	{
		par = x;
		if ((*i)<(*x)) {
			x = x->left;
		} else {
			x = x->right;
		}

		i->split_index = (i->split_index + 1) % K;

		
		depth++;

	}

	if(height<depth) height=depth;

	if (par != NULL)
	{
		if ((*i)<(*par)) par->left = i;
		else par->right = i;
	} else {
		root = i;
	}
	i->parent = par;
}



template <size_t K, class KeyType, class ValType>
typename kdtree<K,KeyType,ValType>::iterator
	kdtree<K,KeyType,ValType>::nearest_neighbour(const vector<KeyType>& test_point, ValType val) const
{
	return n_nearest_neighbour(1, test_point, val)[0];
}


template <size_t K, class KeyType, class ValType>
vector<typename kdtree<K,KeyType,ValType>::iterator>
kdtree<K,KeyType,ValType>::n_nearest_neighbour(const size_t& n, const vector<KeyType>& test_point, ValType val)const
{
	using dist_node=pair<KeyType, node*>;
	priority_queue<dist_node> q;

	if(test_point.size() != K) 	throw invalid_dimension();
	if(root == NULL) 			throw empty_tree();

	const node& query{0, test_point, val};

	std::stack<node*> s;
	s.push(root);
	node* curr = NULL;
	//direct dual of recursive implementation:
	//can't do bottom up
	enum {calling, unwinding} state = calling;
	while(!s.empty())
	{
		if(state == calling)
		{
			curr=s.top();
			if (curr == NULL)
			{
				//found query point's place in tree
				s.pop();
				state = unwinding;
				continue;
			}

			if(query < (*curr) )
				s.push(curr->left);
			else
				s.push(curr->right);


			if(curr->val == query.val)
				continue;

			auto sq_d = sq_distance(query, *curr);
			if(q.size()<n)
				q.push(dist_node(sq_d, curr));
			else
				if(sq_d < q.top().first){
					q.pop();
					q.push(dist_node(sq_d, curr));
				}

		}

		if (state == unwinding)
		{
			curr=s.top();
			s.pop();
			if(curr == NULL) continue;

			auto hyper_rad = curr->key[curr->split_index]-query.key[curr->split_index];
			if ( q.size() && hyper_rad * hyper_rad < q.top().first)
			{

				if(query < *curr )
					s.push(curr->right);
				else
					s.push(curr->left);

				state=calling;
			}
		}
	}
	//if !(n << number of keys) than this is the bottleneck
	vector<iterator> v;
	while(q.size())
	{
		v.push_back(q.top().second);
		q.pop();
	}


	return v;
}

#endif /* KDTREE_H_ */

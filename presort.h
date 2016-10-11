/*
 * presort.h
 *
 *  Created on: 2015. okt. 31.
 *      Author: csabi
 */

#ifndef PRESORT_H_
#define PRESORT_H_

#include "kdtree.h"
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

using c = vector<double>;
using p = pair<c, string>;
// BALANCED INSERTING in O ( kn logn ) time
// implementation based on the following article:
// Russell A. Brown, Building a Balanced k-d Tree in O(kn log n) Time,
// Journal of Computer Graphics Techniques (JCGT), vol. 4, no. 1, 50-68, 2015

vector<vector<p *>> presort(vector<p> &coords) {

  vector<p *> xyz, yzx, zxy;

  for (auto &i : coords)
    xyz.push_back(&i);

  // for a lot of query it is advised to use the best balanced tree
  std::sort(xyz.begin(), xyz.end(), [](const p *a, const p *b) {
    if (a->first[0] != b->first[0])
      return a->first[0] < b->first[0];
    if (a->first[1] != b->first[1])
      return a->first[1] < b->first[1];
    return a->first[2] < b->first[2];
  });

  yzx = xyz;

  std::sort(yzx.begin(), yzx.end(), [](const p *a, const p *b) {

    if (a->first[1] != b->first[1])
      return a->first[1] < b->first[1];
    if (a->first[2] != b->first[2])
      return a->first[2] < b->first[2];
    return a->first[0] < b->first[0];
  });

  zxy = yzx;

  std::sort(zxy.begin(), zxy.end(), [](const p *a, const p *b) {
    if (a->first[2] != b->first[2])
      return a->first[2] < b->first[2];
    if (a->first[0] != b->first[0])
      return a->first[0] < b->first[0];
    return a->first[1] < b->first[1];
  });

  return vector<vector<p *>>{xyz, yzx, zxy};
}

void ins(kdtree<3, double, string> &KDT, vector<p *> &x, vector<p *> &y,
         vector<p *> &z, size_t split = 0) {

  if (x.empty() || y.empty() || z.empty())
    return;

  vector<p *> left_x, left_y, left_z, right_x, right_y, right_z;
  p *median = NULL;

  if (split % 3 == 0) {
    if (x.size() > 3) {
      median = x[x.size() / 2];
    } else if (x.size() > 2) {
      KDT.insert(x[1]->first, x[1]->second);
      KDT.insert(x[0]->first, x[0]->second);
      KDT.insert(x[2]->first, x[2]->second);
      return;
    } else if (x.size() > 1) {
      KDT.insert(x[1]->first, x[1]->second);
      KDT.insert(x[0]->first, x[0]->second);
      return;
    } else {
      KDT.insert(x[0]->first, x[0]->second);
      return;
    }

    for (auto i : x) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_x.push_back(i);
      else
        right_x.push_back(i);
    }
    for (auto i : y) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_y.push_back(i);
      else
        right_y.push_back(i);
    }
    for (auto i : z) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_z.push_back(i);
      else
        right_z.push_back(i);
    }

  } else if (split % 3 == 1) {
    if (y.size() > 3) {
      median = y[y.size() / 2];
    } else if (y.size() > 2) {
      KDT.insert(y[1]->first, y[1]->second);
      KDT.insert(y[0]->first, y[0]->second);
      KDT.insert(y[2]->first, y[2]->second);
      return;
    } else if (y.size() > 1) {
      KDT.insert(y[1]->first, y[1]->second);
      KDT.insert(y[0]->first, y[0]->second);
      return;
    } else {
      KDT.insert(y[0]->first, y[0]->second);
      return;
    }

    for (auto i : y) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_y.push_back(i);
      else
        right_y.push_back(i);
    }
    for (auto i : x) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_x.push_back(i);
      else
        right_x.push_back(i);
    }
    for (auto i : z) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_z.push_back(i);
      else
        right_z.push_back(i);
    }

  } else {

    if (z.size() > 3) {
      median = z[z.size() / 2];
    } else if (z.size() > 2) {
      KDT.insert(z[1]->first, z[1]->second);
      KDT.insert(z[0]->first, z[0]->second);
      KDT.insert(z[2]->first, z[2]->second);
      return;
    } else if (z.size() > 1) {
      KDT.insert(z[1]->first, z[1]->second);
      KDT.insert(z[0]->first, z[0]->second);
      return;
    } else {
      KDT.insert(z[0]->first, z[0]->second);
      return;
    }

    for (auto i : z) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_z.push_back(i);
      else
        right_z.push_back(i);
    }
    for (auto i : x) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_x.push_back(i);
      else
        right_x.push_back(i);
    }
    for (auto i : y) {
      if (i->first == median->first)
        continue;
      if (i->first[split % 3] < median->first[split % 3])
        left_y.push_back(i);
      else
        right_y.push_back(i);
    }
  }

  if (median)
    KDT.insert(median->first, median->second);
  else
    throw internal_error();

  ins(KDT, left_x, left_y, left_z, split + 1);
  ins(KDT, right_x, right_y, right_z, split + 1);

  return;
}

#endif

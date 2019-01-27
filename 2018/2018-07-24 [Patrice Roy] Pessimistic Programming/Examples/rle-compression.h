#ifndef RLE_COMPRESSION_H
#define RLE_COMPRESSION_H

#include <vector>
#include <cassert>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <cstdint>

template <class D, class S> // Dstination, Source
   std::vector<D> rle_compression(const std::vector<S> &v) {
      if (v.empty()) return {};
      using namespace std;
      static_assert(is_convertible_v<S,D>);
      static_assert(is_convertible_v<
         typename iterator_traits<vector<S>::iterator>::difference_type, D
      >);
      vector<Dest> rle;
      typename vector<Dest>::size_type pos = 0, cur = 1;
      S tgt = v.front();
      for (; cur != v.size(); ++cur) {
         auto n = cur - pos;
         if (v[cur] != tgt || n == numeric_limits<D>::max()) {
            rle.emplace_back(static_cast<D>(n));
            rle.emplace_back(static_cast<D>(tgt));
            tgt = v[cur];
            pos = cur;
         }
      }
      if (auto n = cur - pos; n) {
         rle.emplace_back(static_cast<D>(n));
         rle.emplace_back(static_cast<D>(tgt));
      }
      return rle;
   }

#endif

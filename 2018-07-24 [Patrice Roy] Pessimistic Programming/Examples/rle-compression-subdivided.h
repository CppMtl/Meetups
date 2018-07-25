#ifndef RLE_COMPRESSION_H
#define RLE_COMPRESSION_H

#include <cstdint>
#include <vector>
#include <cassert>
#include <limits>
#include <type_traits>
#include <iterator>
#include <utility>

template <class dest_type, class OI, class SI, class Pred> // OI : output iterator. SI : source iterator
   std::pair<SI, OI> rle_compression(SI bi, SI ei, OI dest, Pred pred) {
      using namespace std;
      using src_type = typename iterator_traits<SI>::value_type;
      static_assert(is_convertible_v<src_type, dest_type>);
      if (bi == ei || !pred())
         return pair(bi, dest);

      src_type tgt = *bi;
      auto pos = next(bi);
      for (; pos != ei; ++pos) {
         auto dist = static_cast<dest_type>(distance(bi, pos)); // can be costly
         if (*pos != tgt || dist == numeric_limits<dest_type>::max()) {
            *dest++ = dist;
            *dest++ = tgt;
            tgt = *pos;
            bi = pos;
            if (!pred()) // we could be more granular than this
               return pair(bi, dest);
         }
      }
      if (auto dist = static_cast<dest_type>(distance(bi, pos)); dist) {
         *dest++ = dist;
         *dest++ = tgt;
      }
      return pair(ei, dest);
   }

#endif

//
//  utils.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "utils.hpp"

namespace utils {
    std::vector<std::weak_ptr<unsigned char[]>> convertToWeakPtrVector(const std::vector<std::shared_ptr<unsigned char[]>>& bitMask) {
        std::vector<std::weak_ptr<unsigned char[]>> result;
        result.reserve(bitMask.size());  // Reserve memory to avoid reallocations

        std::transform(bitMask.begin(), bitMask.end(), std::back_inserter(result),
                       [](const std::shared_ptr<unsigned char[]>& ptr) {
                           return std::weak_ptr<unsigned char[]>(ptr);  // Efficiently convert and move
                       });

        return result;
    }
}

//
//  utils.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "utils.hpp"

/* definition of convertToWeakPtrVector to convert shared_ptr vectors into weak_ptr vectors */
namespace utils {
    std::vector<std::weak_ptr<unsigned char[]>> convertToWeakPtrVector(const std::vector<std::shared_ptr<unsigned char[]>>& bitMask) {
        std::vector<std::weak_ptr<unsigned char[]>> result;
        result.reserve(bitMask.size());
        for (const auto& ptr : bitMask) {
            result.emplace_back(std::weak_ptr<unsigned char[]>(ptr));
        }
         return result;
    }
}


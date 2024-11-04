//
//  utils.hpp
//  sfml game template
//
//

#pragma once

#include <vector>
#include <memory>

/* utils namespace includes a convertToWeakPtrVector to convert shared_ptr vectors into weak_ptr vectors */
namespace utils {
    std::vector<std::weak_ptr<unsigned char[]>> convertToWeakPtrVector(const std::vector<std::shared_ptr<unsigned char[]>>& bitMask);

}



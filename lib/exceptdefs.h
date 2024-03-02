#pragma once

#include <stdexcept>
#include <utility>

namespace segvcatch {
struct hardware_exception_info {
    int signo;
    void *addr;
    // void *context;
};

class hardware_exception : public std::runtime_error {
  public:
    hardware_exception_info info;

    hardware_exception(const std::string &what_arg,
                       hardware_exception_info info) noexcept
        : std::runtime_error(what_arg), info(std::move(info)) {}

    hardware_exception(const std::string &what_arg)
        : std::runtime_error(what_arg) {}
};

class segmentation_fault : public hardware_exception {
  public:
    hardware_exception_info info;

    segmentation_fault(const std::string &what_arg)
        : hardware_exception(what_arg) {}

    segmentation_fault(const std::string &what_arg,
                       hardware_exception_info info) noexcept
        : hardware_exception(what_arg, std::move(info)) {}
};

class floating_point_error : public hardware_exception {
  public:
    floating_point_error(const std::string &what_arg)
        : hardware_exception(what_arg) {}

    floating_point_error(const std::string &what_arg,
                         hardware_exception_info info) noexcept
        : hardware_exception(what_arg, std::move(info)) {}
};

} // namespace segvcatch

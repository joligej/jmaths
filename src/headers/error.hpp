#pragma once

#include <exception>

// definitions of error and associated types
namespace jmaths {

class error : public std::exception {
   private:
    const char * const message_;
    static const char default_message_[];

   public:
    class division_by_zero;
    class invalid_base;

    error();
    explicit error(const char * message);
    virtual const char * what() const noexcept;
};

class error::division_by_zero : public error {
   private:
    static const char default_message_[];

   public:
    division_by_zero();
    explicit division_by_zero(const char * message);
};

class error::invalid_base : public error {
   private:
    static const char default_message_[];

   public:
    invalid_base();
    explicit invalid_base(const char * message);
};

}  // namespace jmaths

#ifndef EXCEPTIONS_HPP_INCLUDED
#define EXCEPTIONS_HPP_INCLUDED
#include <exception>

// LANG: HUN

class internal_error : public std::exception {
  const char *message;

public:
  internal_error(const char *msg = "Hiba van az implementacioban!")
      : message(msg) {}

  const char *what() const throw() { return message; }
};

class invalid_binary_search_tree : public std::exception {
public:
  const char *what() const throw() {
    return "Serulnek a binaris keresofa kriteriumok!";
  }
};

class invalid_dimension : public std::exception {
public:
  const char *what() const throw() { return "Rossz kulcs dimenzio!"; }
};

class invalid_iterator : public std::exception {
public:
  const char *what() const throw() { return "NULL elemre mutat az iterator!"; }
};

class empty_tree : public std::exception {
public:
  const char *what() const throw() {
    return "Ures a fa, nem ertelmezheto a kereses!";
  }
};

class duplicate_element : public std::exception {
public:
  const char *what() const throw() { return "Ket egyezo kulcs!"; }
};

class copy_not_implemented : public std::exception {
public:
  const char *what() const throw() { return "Masolas nincs implementalva!"; }
};

#endif // EXCEPTIONS_HPP_INCLUDED

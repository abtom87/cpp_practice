#include <algorithm>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <sstream>
#include <string>
#include <vector>

/**
 * Helper function to calculate and return SHA256 digest.
 */

std::string sha256(const std::string str) {
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int len;

  EVP_MD_CTX *sha256;
  sha256 = EVP_MD_CTX_new();
  EVP_DigestInit_ex(sha256, EVP_sha256(), NULL);
  EVP_DigestUpdate(sha256, str.c_str(), str.size());
  EVP_DigestFinal_ex(sha256, hash, &len);
  EVP_MD_CTX_free(sha256);

  std::stringstream ss;

  for (int i = 0; i < len; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(hash[i]);
  }
  return ss.str();
}

class Book {
private:
  std::string mAuthor;
  std::string mTitle;
  uint32_t mID;

public:
  Book(std::string auth, std::string title, uint32_t id) {
    mAuthor = auth;
    mTitle = title;
    mID = id;
  }

  std::string getAuthor() const { return mAuthor; }

  std::string getTitle() const { return mTitle; }

  uint32_t getID() const { return mID; }
};

class Library {

private:
  std::vector<Book> mBooks;

public:
  void append_book(Book book) { mBooks.push_back(book); }
  void print_list() {

    std::cout << "Current list of books: " << std::endl;

    for (auto book : mBooks)

      std::cout << "Author: " << book.getAuthor()
                << " Title: " << book.getTitle() << " ID: " << book.getID()
                << std::endl;
  }

  void remove_book(uint32_t id) {
    auto match_id = [id](const Book &book) { return book.getID() == id; };
    auto it = std::find_if(mBooks.begin(), mBooks.end(), match_id);

    if (it != mBooks.end()) {
      mBooks.erase(it); // Remove the book found at iterator `it`
      std::cout << "Book with ID " << id << " and title " << it->getTitle()
                << " removed." << std::endl;
    } else {
      std::cout << "No book with ID " << id << " found.";
    }
  }

  void remove_book_swap(const uint32_t id) {
    for (auto &book : mBooks) {
      if (book.getID() == id) {
        std::swap(book, mBooks.back());
        mBooks.pop_back();
        break;
      }
    }
  }

  void find_book_by_hashed_title(std::string hashed_title) {
    auto matched_hash = [&](const Book &book) {
      return sha256(book.getTitle()) == hashed_title;
    };
    auto it = std::find_if(mBooks.begin(), mBooks.end(), matched_hash);

    if (it != mBooks.end())
      std::cout << "Book found: " << it->getTitle() << std::endl;
  };

  void find_books_by_hashed_author(std::string hashed_auth) {
    auto matched_hash = [&](const Book &book) {
      return sha256(book.getAuthor()) == hashed_auth;
    };
    auto it = std::find_if(mBooks.begin(), mBooks.end(), matched_hash);

    auto match_hash_auth = it;

    for (auto iter = mBooks.begin(); iter != mBooks.end(); ++iter) {
      if (iter->getAuthor() == match_hash_auth->getAuthor() &&
          iter != mBooks.end()) {
        std::cout << "Book Title: " << iter->getTitle() << std::endl;
      }
    }
  }
};

int main() {

  Library lib;

  Book book1("Tovia Singer", "Let's get biblical", 124);
  Book book2("Jon Erikson", "Art of exploitation", 11124);
  Book book3("Tom Clancy", "Military book", 1234567);
  Book book4("Tom Clancy", "Military book 2", 26324567);

  lib.append_book(book1);
  lib.append_book(book2);
  lib.append_book(book3);
  lib.append_book(book4);

  lib.print_list();
  std::cout << sha256(book3.getTitle()) << std::endl;

  lib.find_book_by_hashed_title(sha256(book4.getTitle()));
  lib.find_books_by_hashed_author(sha256("Tovia Singer"));

  return 0;
}
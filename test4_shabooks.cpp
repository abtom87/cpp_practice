#include <algorithm>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>

/**
 * Helper function to calculate and return SHA256 digest.
 */

std::string sha256(const std::string str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);

  std::stringstream ss;

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::hex << static_cast<int>(hash[i]);
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
    bool found = false;
    for (auto it = mBooks.begin(); it != mBooks.end(); ++it) {
      std::string hash_bk_title = sha256(it->getTitle());

      // Compare hashes
      if (hash_bk_title == hashed_title) {
        std::cout << "Book found: " << it->getTitle() << std::endl;
        found = true;
        break; // Exit loop early once a match is found
      }
    }

    if (!found) {
      std::cout << "No book found with the given hashed title." << std::endl;
    }
  }
};

int main() {

  Library lib;

  Book book1("Tovia Singer", "Let's get biblical", 124);
  Book book2("Jon Erikson", "Art of exploitation", 11124);
  Book book3("Tom Clancy", "Military book", 1234567);

  lib.append_book(book1);
  lib.append_book(book2);
  lib.append_book(book3);

  lib.print_list();

  std::string hashed_title = sha256(book1.getTitle());

  lib.find_book_by_hashed_title(hashed_title);

  return 0;
}
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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
  lib.remove_book(124);
  lib.print_list();
  return 0;
}
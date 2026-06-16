#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <sstream>

// ============================================================
//  FORWARD DECLARATIONS
// ============================================================
class Book;
class Member;
class BorrowRecord;

// ============================================================
//  DATE UTILITY
// ============================================================
std::string currentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::setfill('0')
        << (tm->tm_year + 1900) << "-"
        << std::setw(2) << (tm->tm_mon + 1) << "-"
        << std::setw(2) << tm->tm_mday;
    return oss.str();
}

// ============================================================
//  LINKED LIST  –  stores BorrowRecords per member
// ============================================================
template<typename T>
class LinkedList {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* head;
    int   sz;
public:
    LinkedList() : head(nullptr), sz(0) {}
    ~LinkedList() { clear(); }

    void pushBack(const T& val) {
        Node* n = new Node(val);
        if (!head) { head = n; }
        else {
            Node* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = n;
        }
        ++sz;
    }

    // remove first match (by predicate)
    template<typename Pred>
    bool removeIf(Pred pred) {
        Node* prev = nullptr;
        Node* cur  = head;
        while (cur) {
            if (pred(cur->data)) {
                if (prev) prev->next = cur->next;
                else       head      = cur->next;
                delete cur;
                --sz;
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    template<typename Pred>
    T* findIf(Pred pred) {
        for (Node* cur = head; cur; cur = cur->next)
            if (pred(cur->data)) return &cur->data;
        return nullptr;
    }

    template<typename Fn>
    void forEach(Fn fn) const {
        for (Node* cur = head; cur; cur = cur->next) fn(cur->data);
    }

    int  size()  const { return sz; }
    bool empty() const { return sz == 0; }

    void clear() {
        while (head) { Node* t = head->next; delete head; head = t; }
        sz = 0;
    }
};

// ============================================================
//  QUEUE  –  pending borrow requests / return queue
// ============================================================
template<typename T>
class Queue {
    struct Node { T data; Node* next; Node(const T& d):data(d),next(nullptr){} };
    Node* frontNode;
    Node* backNode;
    int   sz;
public:
    Queue() : frontNode(nullptr), backNode(nullptr), sz(0) {}
    ~Queue() { while (!empty()) dequeue(); }

    void enqueue(const T& val) {
        Node* n = new Node(val);
        if (!backNode) { frontNode = backNode = n; }
        else           { backNode->next = n; backNode = n; }
        ++sz;
    }

    T dequeue() {
        if (empty()) throw std::underflow_error("Queue is empty");
        Node* t = frontNode;
        T val   = t->data;
        frontNode = frontNode->next;
        if (!frontNode) backNode = nullptr;
        delete t; --sz;
        return val;
    }

    const T& peek() const {
        if (empty()) throw std::underflow_error("Queue is empty");
        return frontNode->data;
    }

    bool empty() const { return sz == 0; }
    int  size()  const { return sz; }
};

// ============================================================
//  HASH TABLE  –  fast lookup for books (by ISBN) and members (by ID)
// ============================================================
template<typename K, typename V>
class HashTable {
    static const int BUCKETS = 64;
    struct Entry { K key; V* value; Entry* next;
        Entry(const K& k, V* v) : key(k), value(v), next(nullptr) {} };
    Entry* table[BUCKETS];

    int hash(const std::string& k) const {
        int h = 0;
        for (char c : k) h = (h * 31 + c) % BUCKETS;
        return (h + BUCKETS) % BUCKETS;
    }
    int hash(int k) const { return k % BUCKETS; }

public:
    HashTable() { for (int i = 0; i < BUCKETS; ++i) table[i] = nullptr; }
    ~HashTable() {
        for (int i = 0; i < BUCKETS; ++i) {
            Entry* cur = table[i];
            while (cur) { Entry* t = cur->next; delete cur; cur = t; }
        }
    }

    void insert(const K& key, V* val) {
        int idx = hash(key);
        Entry* cur = table[idx];
        while (cur) { if (cur->key == key) { cur->value = val; return; } cur = cur->next; }
        Entry* e = new Entry(key, val);
        e->next  = table[idx];
        table[idx] = e;
    }

    V* get(const K& key) const {
        int idx = hash(key);
        for (Entry* cur = table[idx]; cur; cur = cur->next)
            if (cur->key == key) return cur->value;
        return nullptr;
    }

    bool remove(const K& key) {
        int idx = hash(key);
        Entry* prev = nullptr; Entry* cur = table[idx];
        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next; else table[idx] = cur->next;
                delete cur; return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    template<typename Fn>
    void forEach(Fn fn) const {
        for (int i = 0; i < BUCKETS; ++i)
            for (Entry* cur = table[i]; cur; cur = cur->next)
                fn(cur->key, cur->value);
    }
};

// ============================================================
//  BORROW RECORD  –  Composition inside Member
// ============================================================
struct BorrowRecord {
    std::string isbn;
    std::string title;
    std::string borrowDate;
    std::string dueDate;        // 14 days from borrow
    bool        returned;

    BorrowRecord() : returned(false) {}
    BorrowRecord(const std::string& i, const std::string& t,
                 const std::string& bd, const std::string& dd)
        : isbn(i), title(t), borrowDate(bd), dueDate(dd), returned(false) {}

    void display() const {
        std::cout << "  ISBN: " << isbn
                  << "  |  Title: " << std::left << std::setw(30) << title
                  << "  |  Borrowed: " << borrowDate
                  << "  |  Due: " << dueDate
                  << "  |  Status: " << (returned ? "Returned" : "Active")
                  << "\n";
    }
};

// ============================================================
//  BOOK CLASS  –  Encapsulation
// ============================================================
class Book {
    std::string isbn;
    std::string title;
    std::string author;
    std::string genre;
    int         totalCopies;
    int         availableCopies;

public:
    Book() : totalCopies(0), availableCopies(0) {}
    Book(const std::string& i, const std::string& t,
         const std::string& a, const std::string& g, int copies = 1)
        : isbn(i), title(t), author(a), genre(g),
          totalCopies(copies), availableCopies(copies) {}

    // Getters
    std::string getIsbn()      const { return isbn; }
    std::string getTitle()     const { return title; }
    std::string getAuthor()    const { return author; }
    std::string getGenre()     const { return genre; }
    int getTotalCopies()       const { return totalCopies; }
    int getAvailableCopies()   const { return availableCopies; }
    bool isAvailable()         const { return availableCopies > 0; }

    // Setters
    void setTitle(const std::string& t)  { title = t; }
    void setAuthor(const std::string& a) { author = a; }

    bool checkOut() {
        if (availableCopies <= 0) return false;
        --availableCopies; return true;
    }
    bool checkIn() {
        if (availableCopies >= totalCopies) return false;
        ++availableCopies; return true;
    }
    void addCopies(int n) { totalCopies += n; availableCopies += n; }

    void display() const {
        std::cout << std::left
                  << "  ISBN: "    << std::setw(14) << isbn
                  << "  Title: "   << std::setw(30) << title
                  << "  Author: "  << std::setw(22) << author
                  << "  Genre: "   << std::setw(12) << genre
                  << "  Copies: "  << availableCopies << "/" << totalCopies
                  << (isAvailable() ? "" : "  [UNAVAILABLE]")
                  << "\n";
    }
};

// ============================================================
//  PERSON (BASE CLASS)  –  Inheritance
// ============================================================
class Person {
protected:
    int         id;
    std::string name;
    std::string email;
    std::string phone;

public:
    Person() : id(0) {}
    Person(int id, const std::string& n,
           const std::string& e, const std::string& p)
        : id(id), name(n), email(e), phone(p) {}

    virtual ~Person() {}

    int         getId()    const { return id; }
    std::string getName()  const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }

    void setName(const std::string& n)  { name = n; }
    void setEmail(const std::string& e) { email = e; }
    void setPhone(const std::string& p) { phone = p; }

    virtual void displayInfo() const = 0;
};

// ============================================================
//  MEMBER CLASS  –  Inherits Person, Composes LinkedList<BorrowRecord>
// ============================================================
class Member : public Person {
    std::string          membershipType;  // Regular / Premium
    std::string          joinDate;
    int                  maxBorrowLimit;
    LinkedList<BorrowRecord> borrowHistory;  // Composition

public:
    Member() : maxBorrowLimit(3) {}
    Member(int id, const std::string& n,
           const std::string& e, const std::string& p,
           const std::string& type = "Regular")
        : Person(id, n, e, p),
          membershipType(type),
          joinDate(currentDate()),
          maxBorrowLimit(type == "Premium" ? 6 : 3) {}

    std::string getMembershipType() const { return membershipType; }
    int  getMaxBorrowLimit()        const { return maxBorrowLimit; }
    int  getActiveBorrows()         const {
        int cnt = 0;
        borrowHistory.forEach([&](const BorrowRecord& r){ if (!r.returned) ++cnt; });
        return cnt;
    }
    bool canBorrow() const { return getActiveBorrows() < maxBorrowLimit; }

    void addBorrowRecord(const BorrowRecord& r) { borrowHistory.pushBack(r); }

    bool markReturned(const std::string& isbn) {
        BorrowRecord* rec = borrowHistory.findIf(
            [&](const BorrowRecord& r){ return r.isbn == isbn && !r.returned; });
        if (!rec) return false;
        rec->returned = true;
        return true;
    }

    void upgradeMembership() {
        membershipType = "Premium";
        maxBorrowLimit = 6;
    }

    void displayInfo() const override {
        std::cout << std::left
                  << "  ID: "         << std::setw(6)  << id
                  << "  Name: "       << std::setw(20) << name
                  << "  Email: "      << std::setw(26) << email
                  << "  Type: "       << std::setw(10) << membershipType
                  << "  Active Borrows: " << getActiveBorrows() << "/" << maxBorrowLimit
                  << "\n";
    }

    void displayBorrowHistory() const {
        std::cout << "  Borrow history for " << name << ":\n";
        if (borrowHistory.empty()) { std::cout << "    (none)\n"; return; }
        borrowHistory.forEach([](const BorrowRecord& r){ r.display(); });
    }
};

// ============================================================
//  LIBRARIAN CLASS  –  Inherits Person
// ============================================================
class Librarian : public Person {
    std::string employeeCode;
    std::string department;

public:
    Librarian() {}
    Librarian(int id, const std::string& n,
              const std::string& e, const std::string& p,
              const std::string& code, const std::string& dept)
        : Person(id, n, e, p), employeeCode(code), department(dept) {}

    std::string getEmployeeCode() const { return employeeCode; }
    std::string getDepartment()   const { return department; }

    void displayInfo() const override {
        std::cout << std::left
                  << "  ID: "     << std::setw(6)  << id
                  << "  Name: "   << std::setw(20) << name
                  << "  Code: "   << std::setw(10) << employeeCode
                  << "  Dept: "   << department << "\n";
    }
};

// ============================================================
//  LIBRARY CLASS  –  Composition of all structures
// ============================================================
class Library {
    std::string           name;
    HashTable<std::string, Book>   bookCatalog;   // ISBN → Book
    HashTable<int,        Member>  memberRegistry;// ID   → Member
    Queue<std::pair<int,std::string>> borrowQueue;// pending (memberId, isbn)
    int nextMemberId;

    static std::string dueDateFrom(const std::string& /*today*/) {
        // Add 14 days
        std::time_t t = std::time(nullptr) + 14 * 86400;
        std::tm* tm   = std::localtime(&t);
        std::ostringstream oss;
        oss << std::setfill('0')
            << (tm->tm_year + 1900) << "-"
            << std::setw(2) << (tm->tm_mon + 1) << "-"
            << std::setw(2) << tm->tm_mday;
        return oss.str();
    }

    void divider(char c = '-', int w = 72) const {
        std::cout << std::string(w, c) << "\n";
    }

public:
    Library(const std::string& n) : name(n), nextMemberId(1001) {}

    // ---- BOOK MANAGEMENT ----
    bool addBook(const Book& b) {
        if (bookCatalog.get(b.getIsbn())) return false; // duplicate
        bookCatalog.insert(b.getIsbn(), new Book(b));
        return true;
    }

    bool removeBook(const std::string& isbn) {
        Book* b = bookCatalog.get(isbn);
        if (!b) return false;
        delete b;
        return bookCatalog.remove(isbn);
    }

    Book* findBook(const std::string& isbn) { return bookCatalog.get(isbn); }

    void listBooks() const {
        divider('=');
        std::cout << "  " << name << " — BOOK CATALOG\n";
        divider();
        bookCatalog.forEach([](const std::string&, Book* b){ b->display(); });
        divider('=');
    }

    // ---- MEMBER MANAGEMENT ----
    int registerMember(const std::string& n, const std::string& e,
                       const std::string& p, const std::string& type = "Regular") {
        int id = nextMemberId++;
        memberRegistry.insert(id, new Member(id, n, e, p, type));
        return id;
    }

    Member* findMember(int id) { return memberRegistry.get(id); }

    void listMembers() const {
        divider('=');
        std::cout << "  " << name << " — MEMBER REGISTRY\n";
        divider();
        memberRegistry.forEach([](int, Member* m){ m->displayInfo(); });
        divider('=');
    }

    // ---- BORROW ----
    bool borrowBook(int memberId, const std::string& isbn) {
        Member* m = memberRegistry.get(memberId);
        Book*   b = bookCatalog.get(isbn);

        if (!m) { std::cout << "[ERROR] Member " << memberId << " not found.\n"; return false; }
        if (!b) { std::cout << "[ERROR] Book "   << isbn     << " not found.\n"; return false; }

        if (!m->canBorrow()) {
            std::cout << "[INFO] " << m->getName()
                      << " has reached borrow limit. Request queued.\n";
            borrowQueue.enqueue({memberId, isbn});
            return false;
        }
        if (!b->isAvailable()) {
            std::cout << "[INFO] \"" << b->getTitle()
                      << "\" unavailable. Request queued.\n";
            borrowQueue.enqueue({memberId, isbn});
            return false;
        }

        std::string today = currentDate();
        b->checkOut();
        m->addBorrowRecord(BorrowRecord(isbn, b->getTitle(), today, dueDateFrom(today)));
        std::cout << "[OK] \"" << b->getTitle() << "\" borrowed by "
                  << m->getName() << "  (due " << dueDateFrom(today) << ")\n";
        return true;
    }

    // ---- RETURN ----
    bool returnBook(int memberId, const std::string& isbn) {
        Member* m = memberRegistry.get(memberId);
        Book*   b = bookCatalog.get(isbn);

        if (!m || !b) { std::cout << "[ERROR] Invalid member or ISBN.\n"; return false; }

        if (!m->markReturned(isbn)) {
            std::cout << "[ERROR] No active borrow record for this book.\n";
            return false;
        }
        b->checkIn();
        std::cout << "[OK] \"" << b->getTitle() << "\" returned by "
                  << m->getName() << "\n";

        // Process pending requests from queue
        processQueue();
        return true;
    }

    // ---- PROCESS QUEUE ----
    void processQueue() {
        while (!borrowQueue.empty()) {
            auto [mid, isbn] = borrowQueue.peek();
            Member* m = memberRegistry.get(mid);
            Book*   b = bookCatalog.get(isbn);
            if (m && b && m->canBorrow() && b->isAvailable()) {
                borrowQueue.dequeue();
                borrowBook(mid, isbn);
            } else break;
        }
    }

    void showQueue() const {
        std::cout << "  Pending requests in queue: " << borrowQueue.size() << "\n";
    }

    // ---- REPORTS ----
    void memberHistory(int memberId) {
        Member* m = memberRegistry.get(memberId);
        if (!m) { std::cout << "[ERROR] Member not found.\n"; return; }
        divider();
        m->displayBorrowHistory();
        divider();
    }
};

// ============================================================
//  DEMO / MAIN
// ============================================================
void section(const std::string& title) {
    std::cout << "\n" << std::string(72,'#') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(72,'#') << "\n";
}

int main() {
    Library lib("Sunrise Public Library");

    section("1. Adding Books to Catalog");
    lib.addBook(Book("978-0-06-112008-4", "To Kill a Mockingbird", "Harper Lee",       "Fiction",   3));
    lib.addBook(Book("978-0-7432-7356-5", "1984",                  "George Orwell",    "Dystopia",  2));
    lib.addBook(Book("978-0-14-028329-7", "The Great Gatsby",      "F. Scott Fitzgerald","Classic", 2));
    lib.addBook(Book("978-0-06-093546-9", "To Kill a Mockingbird", "Harper Lee",       "Fiction",   1)); // dup
    lib.addBook(Book("978-0-7432-7023-6", "The Catcher in the Rye","J.D. Salinger",   "Fiction",   1));
    lib.addBook(Book("978-0-316-76948-0", "Brave New World",       "Aldous Huxley",   "Sci-Fi",    1));
    lib.listBooks();

    section("2. Registering Members");
    int aliceId = lib.registerMember("Alice Johnson", "alice@mail.com", "555-1001", "Premium");
    int bobId   = lib.registerMember("Bob Smith",    "bob@mail.com",   "555-1002");
    int carolId = lib.registerMember("Carol White",  "carol@mail.com", "555-1003");
    lib.listMembers();

    section("3. Borrowing Books");
    lib.borrowBook(aliceId, "978-0-06-112008-4");
    lib.borrowBook(aliceId, "978-0-7432-7356-5");
    lib.borrowBook(bobId,   "978-0-14-028329-7");
    lib.borrowBook(bobId,   "978-0-7432-7023-6");
    lib.borrowBook(carolId, "978-0-06-112008-4");
    lib.borrowBook(carolId, "978-0-316-76948-0");

    section("4. Attempting Over-Limit / Unavailable Borrow (Queue Demo)");
    // Bob has 2 borrows (limit=3), try one more
    lib.borrowBook(bobId, "978-0-316-76948-0"); // unavailable → queue
    lib.showQueue();

    section("5. Returning a Book");
    lib.returnBook(carolId, "978-0-316-76948-0"); // returns → triggers queue
    lib.showQueue();

    section("6. Member Borrow History");
    lib.memberHistory(aliceId);
    lib.memberHistory(bobId);

    section("7. Book Catalog after Transactions");
    lib.listBooks();

    section("8. Remove a Book");
    lib.removeBook("978-0-06-093546-9"); // was duplicate; doesn't exist
    bool ok = lib.removeBook("978-0-316-76948-0");
    std::cout << "  Remove 'Brave New World': " << (ok ? "success" : "not found") << "\n";
    lib.listBooks();

    std::cout << "\nDone.\n";
    return 0;
}
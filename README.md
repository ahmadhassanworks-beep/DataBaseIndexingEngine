Database Indexing Engine

A desktop application built in C++ and Qt Framework , demonstrating core database indexing concepts using a B+ Trees data structure.
 . It simulates a real-world database indexing engine where data is stored, retrieved, and managed efficiently using a B+ Tree.

Features
- Create custom databases with user-defined column names
- Insert records indexed and sorted automatically by B+ Tree
- View all records ,always returned in sorted order via leaf traversal
- Search a record by key in O(log n) time
- Update existing records by key
- Delete records with confirmation
- Data persists to file and reloads on next launch
- Support for multiple databases

DSA & DBMS Concepts Applied
- B+ Tree ,core indexing structure with node splitting and leaf linking
- Primary Key Constraint ,duplicate keys rejected at index level
- File-based Persistence ,records saved to .dat files
- Dynamic Schema— user defines table structure at runtime
- CRUD Operations —Create, Read, Update, Delete
- Sorted Traversal— leaf linked list enables efficient ordered retrieval

 Tech Stack
- C++ 
- Qt 6.11 Framework
- CMake Build System

How to Run
1. Install Qt Creator and Qt 6.x
2. Clone or download this repository
3. Open CMakeLists.txt in Qt Creator
4. Build and run the project


Developer
Ahmad— BS Computer Science, 4th Semester

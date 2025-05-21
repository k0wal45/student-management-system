# Student Management System – C++ Project

## Authors:
- ### Daniel Kowalski
  - https://daniel-kowalski.com/
  - https://www.linkedin.com/in/daniel-kowalski-com/
- ### Krzysztof Kantorski

## Features

1. Role-based system
2. Add, browse, and edit students (admin/employee only)
3. Add grades (admin/employee only)
4. Browse grades
5. Post global announcements (admin/employee only)
6. View global announcements
7. User registration and authentication

---

## Technologies

- **C++** (Object-Oriented Programming)
- **MongoDB** (NoSQL database)
- **wxWidgets** (GUI library)
- C++ STL libraries:
  - `<vector>`, `<map>`, `<string>`, `<fstream>`, `<chrono>`, `<iomanip>`
- **bcrypt** – password hashing
- **Git** + **GitHub** – version control

---

## 1. Login & Role Management

- Users log in via a graphical user interface
- Passwords are hashed (e.g., using bcrypt)
- Roles available:
  - **Student**
  - **Employee**
  - **Admin**
- Permissions depend on assigned role

---

## 2. User Management (Admin/Employee)

- Add new users (students or employees)
- Edit user information
- Reset user password
- Delete user accounts

---

## 3. Student Management

- Add new students
- Browse all students
- Filter students by major, name, average grade
- Edit student data
- Delete student records

---

## 4. Grade Management

- Add grades to students
- Edit/delete grades (employee only)
- Calculate and display average grade
- Students can view their own grades

---

## 5. Global Announcements (Message Board)

- Employees can post announcements
- All users can view announcements
- Each message includes: content, author, and date

---

## 6. Security

- Passwords are stored securely using hashes
- Role-based authentication
- GUI action restrictions based on user role

---

## 7. Graphical User Interface

- Login screen
- Main dashboard depends on the user's role
- Forms for managing students and grades
- List of announcements
- Student list and filtering options

---

## 8. MongoDB Integration

- MongoDB stores all data: students, grades, users, and announcements
- Full CRUD operations supported
- Filtering and sorting data directly from the database

---

## 9. Data Handling

- Data is loaded at app startup
- Changes are saved automatically
- Database connection error handling is implemented

---

# MongoDB Database Design

## Collections

### 1. `users`
```json
{
  "_id": ObjectId("..."),
  "login": "jankowalski",
  "password_hash": "$2b$12$Xk...klZ",   // bcrypt hashed password
  "role": "student",                   // or "employee"
  "first_name": "Jan",
  "last_name": "Kowalski",
  "last_login": "2025-05-14T10:23:00Z"
}
```

### 2. `students`
```json
{
  "_id": ObjectId("..."),
  "first_name": "Anna",
  "last_name": "Nowak",
  "student_id": "s123456",
  "email": "anna.nowak@example.com",
  "major": "Computer Science",
  "year": 2,
  "group": "2B",
  "grades": [                      // optional if stored separately
    ObjectId("..."),
    ObjectId("...")
  ]
}

```

### 3. `grades`

```json
{
  "_id": ObjectId("..."),
  "student_id": ObjectId("..."),
  "subject": "C++ Programming",
  "grade": 4.5,
  "comment": "Excellent project",
  "date_issued": "2025-05-12T00:00:00Z",
  "instructor": "Dr. Kowalski"
}
```

### 4. `announcements`

```json
{
  "_id": ObjectId("..."),
  "title": "New exam policy",
  "content": "Starting next week, a new policy will apply...",
  "author": "admin",
  "date_posted": "2025-05-10T15:30:00Z"
}
```
## Project Summary

The **Student Management System** is a C++ desktop application designed for managing student records, grades, users, and announcements within an academic environment. The system uses:

- **C++ (Object-Oriented Programming)** for core logic,
- **MongoDB** as the backend database for storing all persistent data,
- **wxWidgets** for a graphical user interface (GUI),
- **bcrypt** for secure password hashing,
- **Git and GitHub** for version control and collaboration.

The application supports multiple user roles — **Student**, **Employee**, and **Admin** — each with different permissions. Admins and employees can manage users, students, and grades, while students can log in and view their own academic data.

Core features include:
- Role-based authentication and login system
- Student and user account management
- Grade tracking with average calculations
- A global announcement board
- Secure password storage
- Realtime MongoDB data integration
- GUI built with wxWidgets

This project serves as a practical academic system simulation and can be expanded further with REST APIs, PDF export, or cloud-based deployment.


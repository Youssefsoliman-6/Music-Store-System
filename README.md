# Music Store System - Console Application

# Description
This C++ console application is a music store management system that allows users to browse, purchase, and manage music albums. The system features:

User Authentication: Secure login with username and password verification

# Album Management:

1-Browse albums by name, artist, or genre

2-Add new albums to the store inventory

3-View detailed album information (name, artist, genre, price)

# Shopping Cart Functionality:

1-Add/remove albums from cart

2-View current cart contents

3-Calculate order total

# Order Processing:

Collect delivery address information

Confirm orders with customer and order IDs

Data Persistence:

Reads album data from a text file on startup

Writes updates back to the file

Supports up to 100 albums in inventory

Key Features
Structured Data Handling: Uses structs for albums, addresses, and orders

Genre Enumeration: Supports five music genres (Rock, Pop, Progressive, HipHop, Metal)

Text Formatting: Properly formats output with setw() for alignment

Input Validation: Includes basic input checking and error handling

Case Handling: Automatically capitalizes names and genres for consistency

Technical Details
File I/O operations for persistent storage

Array-based data structures for album storage

Modular design with separate functions for each feature

Menu-driven interface with clear navigation

Usage
Compile and run the program

Login with default credentials (user/password123)

Use the menu system to browse albums, manage your cart, or add new albums

Place orders by confirming your delivery address

This project demonstrates core C++ concepts including file handling, data structures, control flow, and basic UI design in a console environment.
